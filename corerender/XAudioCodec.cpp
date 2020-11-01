//
// Created by Oogh on 2020/9/18.
//

#include "XAudioCodec.hpp"
#include "XMediaHandle.hpp"
#include "XLogger.hpp"
#include "XSampleQueue.hpp"
#include "XThreadUtils.hpp"


XAudioCodec::XAudioCodec(std::shared_ptr<AVFormatContext> ic, int index)
: mFormatCtx(ic), mIndex(index), mStatus(0), mSampleData(nullptr), mSampleDataIndex(0), mSampleBufferSize(0), mSampleBufferSizeMax(0),
  mDstSampleCountMax(0) {

}

XAudioCodec::~XAudioCodec() {

}

int XAudioCodec::open() {
    if (!mFormatCtx || mIndex < 0) {
        return AVERROR(EINVAL);
    }

    if (!mCodecCtx) {
        AVStream* stream = mFormatCtx->streams[mIndex];
        AVCodec* codec = avcodec_find_decoder(stream->codecpar->codec_id);
        if (!codec) {
            LOGE("[XAudioCodec] avcodec_find_decoder(%s) failed!\n", avcodec_get_name(stream->codecpar->codec_id));
            return AVERROR_DECODER_NOT_FOUND;
        }

        AVCodecContext* avctx = avcodec_alloc_context3(codec);
        if (!avctx) {
            LOGE("[XAudioCodec] avcodec_alloc_context3 failed!\n");
            return AVERROR(ENOMEM);
        }
        mCodecCtx = std::unique_ptr<AVCodecContext, CodecDeleter>(avctx);

        int ret = avcodec_parameters_to_context(avctx, stream->codecpar);
        if (ret < 0) {
            LOGE("[XAudioCodec] avcodec_parameters_to_context failed: %s\n",
                 av_err2str(ret));
            return ret;
        }

        AVDictionary* opts = nullptr;
        if (!av_dict_get(opts, "threads", nullptr, 0)) {
            av_dict_set(&opts, "threads", "auto", 0);
        }

        ret = avcodec_open2(avctx, codec, &opts);

        if (opts) {
            av_opt_free(opts);
        }

        if (ret < 0) {
            LOGE("[XAudioCodec] avcodec_open2 failed: %s\n", av_err2str(ret));
            return ret;
        }

        // 重采样环境配置
        if (mSwrContext) {
            mSwrContext.reset();
        }

        if (!mSwrContext) {
            SwrContext* swr = swr_alloc();
            if (!swr) {
                return AVERROR(ENOMEM);
            }
            mSwrContext = std::unique_ptr<SwrContext,SwrContextDeleter>(swr);
        }

        uint64_t channelLayout = (avctx->channel_layout && avctx->channels == av_get_channel_layout_nb_channels(avctx->channel_layout)) ? avctx->channel_layout : av_get_default_channel_layout(avctx->channels);

        av_opt_set_int(mSwrContext.get(), "in_channel_layout", channelLayout, 0);
        av_opt_set_int(mSwrContext.get(), "in_sample_rate", avctx->sample_rate, 0);
        av_opt_set_sample_fmt(mSwrContext.get(), "in_sample_fmt", avctx->sample_fmt, 0);

        av_opt_set_int(mSwrContext.get(), "out_channel_layout", DST_CHANNEL_LAYOUT, 0);
        av_opt_set_int(mSwrContext.get(), "out_sample_rate", DST_SAMPLE_RATE, 0);
        av_opt_set_sample_fmt(mSwrContext.get(), "out_sample_fmt", DST_SAMPLE_FMT, 0);

        ret = swr_init(mSwrContext.get());
        if (ret < 0) {
            LOGE("[XFFProducer] swr_init failed: %s\n", av_err2str(ret));
            return ret;
        }

        mDstSampleCountMax = 0;
        if (mSampleData) {
            av_freep(&mSampleData);
        }

        // 申请目标缓冲区内存空间
        int srcSampleCount = avctx->frame_size > 0 ? avctx->frame_size : 1024;
        int dstSampleCount = static_cast<int>(av_rescale_rnd(srcSampleCount, DST_SAMPLE_RATE, avctx->sample_rate, AV_ROUND_UP));
        mDstSampleCountMax = dstSampleCount;

        int dstChannels = av_get_channel_layout_nb_channels(DST_CHANNEL_LAYOUT);
        ret = av_samples_alloc(&mSampleData, nullptr, dstChannels, dstSampleCount, DST_SAMPLE_FMT, 0);
        if (ret < 0) {
            LOGE("[XFFProducer] av_samples_alloc_array_and_samples failed: %s\n", av_err2str(ret));
            return AVERROR(ENOMEM);
        }
    }

    return 0;
}

int XAudioCodec::decodePacket(bool async) {
    if (async) {
        if (!mAudioTid) {
            mAudioTid = std::make_unique<std::thread>([this] { audioWorkThread(this); });
        }
        return 0;
    } else {
        return decodeAudioFrame();
    }
    return 0;
}

void XAudioCodec::audioWorkThread(void *opaque) {
    XThreadUtils::configThreadName("audioWorkThread");
    auto codec = reinterpret_cast<XAudioCodec*>(opaque);

    int ret;
    for (;;) {
        ret = codec->decodeAudioFrame();
        if (ret < 0) {
            break;
        }
    }
}

int XAudioCodec::decodeAudioFrame() {
    int ret = AVERROR(EAGAIN);
    for (;;) {
        auto frame = std::make_shared<Frame>();
        ret = avcodec_receive_frame(mCodecCtx.get(), frame->avframe);
        if (ret < 0 && ret != AVERROR(EAGAIN) && ret != AVERROR_EOF) {
            LOGE("[XAudioCodec] avcodec_receive_frame failed: %s\n", av_err2str(ret));
            return ret;
        }

        if (ret == AVERROR_EOF) {
            // TODO(oogh): 2020/09/22 视频解码完成
            mStatus |= S_AUDIO_DECODE_EOF;
            return ret;
        }

        if (ret >= 0) {
            int len = sampleConvert(frame->avframe);
            return len;
        }

        auto pkt = std::make_shared<Packet>();
        ret = av_read_frame(mFormatCtx.get(), pkt->avpkt);
        if (ret < 0) {
            if (ret == AVERROR_EOF && !(mStatus & S_READ_EOF)) {
                ret = avcodec_send_packet(mCodecCtx.get(), nullptr);
                if (ret < 0 && ret != AVERROR(EAGAIN) && ret != AVERROR_EOF) {
                    LOGE("[XAudioCodec] avcodec_send_packet failed: %s\n", av_err2str(ret));
                    return ret;
                }
                mStatus |= S_READ_EOF;
                continue;
            }
            return ret;
        }

        if (pkt->avpkt->stream_index == mIndex) {
            av_packet_rescale_ts(pkt->avpkt, mFormatCtx->streams[mIndex]->time_base, {1, 1000});
            ret = avcodec_send_packet(mCodecCtx.get(), pkt->avpkt);
            if (ret < 0 && ret != AVERROR(EAGAIN) && ret != AVERROR_EOF) {
                LOGE("[XAudioCodec] avcodec_send_packet failed: %s\n", av_err2str(ret));
                return ret;
            }
        }
    }
    return 0;
}

int XAudioCodec::getSamples(uint8_t* out, int size) {
    if (!mSampleQueue) {
        return 0;
    }

    if (mStatus & S_AUDIO_DECODE_EOF && mSampleQueue->used() <= 0) {
        return -1;
    }

    int len = mSampleQueue->read(out, size);

    return len;
}

int XAudioCodec::sampleConvert(AVFrame *src) {
    if (mSampleData) {
        av_freep(&mSampleData);
    }

    // 1. 分配目标缓冲区内存空间
    int dstChannels = av_get_channel_layout_nb_channels(DST_CHANNEL_LAYOUT);
    int dstSampleCount = static_cast<int>(av_rescale_rnd(swr_get_delay(mSwrContext.get(), src->sample_rate) + src->nb_samples, DST_SAMPLE_RATE, src->sample_rate, AV_ROUND_UP));
    mSampleBufferSize = av_samples_alloc(&mSampleData, nullptr, dstChannels, dstSampleCount, DST_SAMPLE_FMT, 1);
    if (mSampleBufferSize < 0) {
        LOGE("[XFFProducer] av_samples_alloc failed!");
        return AVERROR(ENOMEM);
    }

    // 2. 执行重采样
    int count = swr_convert(mSwrContext.get(), &mSampleData, dstSampleCount, (const uint8_t **)src->data, src->nb_samples);
    if (count < 0) {
        LOGE("[XFFProducer] swr_convert failed: %s\n", av_err2str(count));
        return count;
    }

    // 3. 重采样完成后，计算重采样后的数据大小
    int size = av_samples_get_buffer_size(nullptr, dstChannels, count, DST_SAMPLE_FMT, 1);
    if (size < 0) {
        LOGE("[XFFProducer] av_samples_get_buffer_size failed: %s\n", av_err2str(size));
        return size;
    }

    mSampleBufferSize = size;
    mSampleBufferSizeMax = size;

    return mSampleBufferSize;
}

void XAudioCodec::close() {
    if (mCodecCtx) {
        mCodecCtx.reset();
    }
}




