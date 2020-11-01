//
// Created by Oogh on 2020/9/18.
//

#include "XVideoCodec.hpp"
#include "XLogger.hpp"
#include "XPlatform.hpp"
#include "XThreadUtils.hpp"
#include "XImage.hpp"
#include "XImageQueue.hpp"
#include "libyuv.h"

AVPixelFormat XVideoCodec::mHWPixelFormat = AV_PIX_FMT_NONE;

XVideoCodec::XVideoCodec(std::shared_ptr<AVFormatContext> ic, int index)
: mFormatCtx(ic), mIndex(index), mBFrameIndex(0) {

}

XVideoCodec::~XVideoCodec() {

}

int XVideoCodec::open() {
    if (!mFormatCtx || mIndex < 0) {
        return AVERROR(EINVAL);
    }

    if (!mCodecCtx) {
        AVStream* stream = mFormatCtx->streams[mIndex];
        AVCodec* codec = avcodec_find_decoder(stream->codecpar->codec_id);
        if (!codec) {
            LOGE("[XVideoCodec] avcodec_find_decoder(%s) failed!\n", avcodec_get_name(stream->codecpar->codec_id));
            return AVERROR_DECODER_NOT_FOUND;
        }

        bool decodeByHardware = false;
        AVHWDeviceType type = AV_HWDEVICE_TYPE_NONE;
#if PLATFORM_ANDROID
        if (codec->id == AV_CODEC_ID_H264) {
            codec = avcodec_find_decoder_by_name("h264_mediacodec");
        } else if (codec->id == AV_CODEC_ID_HEVC) {
            codec = avcodec_find_decoder_by_name("hevc_mediacodec");
        }
        if (!codec) {
            codec = avcodec_find_decoder(stream->codecpar->codec_id);
            if (!codec) {
                LOGE("[XVideoCodec] avcodec_find_decoder failed: cannot find decoder %s\n",
                     avcodec_get_name(codec->id));
                return AVERROR_DECODER_NOT_FOUND;
            }
        }
        type = av_hwdevice_find_type_by_name("mediacodec");
#elif PLATFORM_IOS || PLATFORM_MAC
        type = av_hwdevice_find_type_by_name("videotoolbox");
        if (type == AV_HWDEVICE_TYPE_NONE) {
            LOGW("[XVideoCodec] av_hwdevice_find_type_by_name(videotoolbox) failed!\n");
        }  else {
            for (int i = 0;; ++i) {
                const AVCodecHWConfig *config = avcodec_get_hw_config(codec, i);
                if (!config) {
                    LOGW("[XVideoCodec] Decoder %s does not support device type %s.\n", codec->name,
                         av_hwdevice_get_type_name(type));
                } else if (config->methods & AV_CODEC_HW_CONFIG_METHOD_HW_DEVICE_CTX &&
                    config->device_type == type) {
                    mHWPixelFormat = config->pix_fmt;
                    decodeByHardware = true;
                    break;
                }
            }
        }
#endif

        AVCodecContext* avctx = avcodec_alloc_context3(codec);
        if (!avctx) {
            LOGE("[XVideoCodec] avcodec_alloc_context3 failed!\n");
            return AVERROR(ENOMEM);
        }
        mCodecCtx = std::unique_ptr<AVCodecContext, CodecDeleter>(avctx);

        int ret = avcodec_parameters_to_context(avctx, stream->codecpar);
        if (ret < 0) {
            LOGE("[XVideoCodec] avcodec_parameters_to_context failed: %s\n",
                 av_err2str(ret));
            return ret;
        }

        if (decodeByHardware) {
            avctx->get_format = [](AVCodecContext* ctx, const enum AVPixelFormat* format) -> AVPixelFormat {
                return mHWPixelFormat;
            };

            AVBufferRef *deviceCtx = nullptr;
            ret = av_hwdevice_ctx_create(&deviceCtx, type, nullptr, nullptr, 0);
            if (ret < 0) {
                LOGE("[XVideoCodec] av_hwdevice_ctx_create failed: %s\n", av_err2str(ret));
                return ret;
            }
            // TODO(oogh): 2020/09/22 需要通过 av_buffer_unref 释放
            avctx->hw_device_ctx = av_buffer_ref(deviceCtx);
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
            LOGE("[XVideoCodec] avcodec_open2 failed: %s\n", av_err2str(ret));
            return ret;
        }
    }

    return 0;
}

int XVideoCodec::decodePacket(bool async) {
    if (async) {
        if (!mVideoTid) {
            mVideoTid = std::make_unique<std::thread>([this] { videoWorkThread(this); });
        }
        return 0;
    } else {
        return decodeVideoFrame();
    }
}

void XVideoCodec::videoWorkThread(void* opaque) {
    XThreadUtils::configThreadName("videoWorkThread");
    auto codec = reinterpret_cast<XVideoCodec*>(opaque);

    int ret;
    for (;;) {
        ret = codec->decodeVideoFrame();
        if (ret < 0) {
            break;
        }
    }
}

int XVideoCodec::decodeVideoFrame() {
    int ret = AVERROR(EAGAIN);
    for (;;) {
        auto frame = std::make_shared<Frame>();
        ret = avcodec_receive_frame(mCodecCtx.get(), frame->avframe);
        if (ret < 0 && ret != AVERROR(EAGAIN) && ret != AVERROR_EOF) {
            LOGE("[XVideoCodec] avcodec_receive_frame failed: %s\n", av_err2str(ret));
            return ret;
        }

        if (ret == AVERROR_EOF) {
            // TODO(oogh): 2020/09/22 视频解码完成
            mStatus |= S_VIDEO_DECODE_EOF;
            return ret;
        }

        if (ret >= 0) {
            if (frame->avframe->format == mHWPixelFormat) {
                auto cpuFrame = std::make_shared<Frame>();
                ret = av_hwframe_transfer_data(cpuFrame->avframe, frame->avframe, 0);
                if (ret < 0) {
                    LOGE("[XVideoCodec] av_hwframe_transfer_data failed: %s\n",
                         av_err2str(ret));
                    return ret;
                }
                queueFrame(cpuFrame->avframe, static_cast<long>(frame->avframe->pts), static_cast<long>(frame->avframe->pkt_duration));
            } else {
                queueFrame(frame->avframe, static_cast<long>(frame->avframe->pts), static_cast<long>(frame->avframe->pkt_duration));
            }
            return 1;
        }

        auto pkt = std::make_shared<Packet>();
        ret = av_read_frame(mFormatCtx.get(), pkt->avpkt);
        if (ret < 0) {
            if (ret == AVERROR_EOF && !(mStatus & S_READ_EOF)) {
                ret = avcodec_send_packet(mCodecCtx.get(), nullptr);
                if (ret < 0 && ret != AVERROR(EAGAIN) && ret != AVERROR_EOF) {
                    LOGE("[XVideoCodec] avcodec_send_packet failed: %s\n", av_err2str(ret));
                    return ret;
                }
                mStatus |= S_READ_EOF;
                continue;
            }
            return ret;
        }

        AVStream* stream = mFormatCtx->streams[mIndex];
        if (pkt->avpkt->stream_index == mIndex &&
            !(stream->disposition & AV_DISPOSITION_ATTACHED_PIC) &&
            checkIsValidPacket(pkt->avpkt)) {
            av_packet_rescale_ts(pkt->avpkt, stream->time_base, {1, 1000});
            ret = avcodec_send_packet(mCodecCtx.get(), pkt->avpkt);
            if (ret < 0 && ret != AVERROR(EAGAIN) && ret != AVERROR_EOF) {
                LOGE("[XVideoCodec] avcodec_send_packet failed: %s\n", av_err2str(ret));
                return ret;
            }
        }
    }
    return 0;
}

bool XVideoCodec::checkIsValidPacket(AVPacket *pkt) {
    AVStream *stream = mFormatCtx->streams[mIndex];
    AVPacket packet;
    av_parser_parse2(stream->parser, mCodecCtx.get(), &packet.data, &packet.size, pkt->data, pkt->size, pkt->pts, pkt->dts, pkt->pos);
    if (stream->parser->pict_type == AV_PICTURE_TYPE_B) {
        mBFrameIndex++;
        if (!(mBFrameIndex & 1)) {
            return false;
        }
    } else if (stream->parser->pict_type == AV_PICTURE_TYPE_I || stream->parser->pict_type == AV_PICTURE_TYPE_P) {
        mBFrameIndex = 0;
    }
    return true;
}

void XVideoCodec::queueFrame(AVFrame *frame, long pts, long duration) {

    auto image = mImageQueue->peekWritable();
    if (!image) {
        return;
    }

    image->width = frame->width;
    image->height = frame->height;
    image->pts = pts;
    image->duration = duration;
    image->format = DST_PIX_FMT;
    frameConvert(image, frame);

    mImageQueue->push();
}

void XVideoCodec::frameConvert(std::shared_ptr<XImage> dst, AVFrame *src) {
    if (!dst->pixels[0]) {
        av_image_alloc(dst->pixels, dst->linesize, dst->width, dst->height,
                       static_cast<AVPixelFormat>(dst->format), 1);
    }
    switch (src->format) {
        case AV_PIX_FMT_YUV420P:
        case AV_PIX_FMT_YUVJ420P: {
            libyuv::I420ToABGR(src->data[0], src->linesize[0],
                               src->data[1], src->linesize[1],
                               src->data[2], src->linesize[2],
                               dst->pixels[0], dst->linesize[0],
                               src->width, src->height);
        }
            break;

        case AV_PIX_FMT_NV21: {
            libyuv::NV21ToABGR(src->data[0], src->linesize[0],
                               src->data[1], src->linesize[1],
                               dst->pixels[0], dst->linesize[0],
                               src->width, src->height);
        }
            break;

        case AV_PIX_FMT_NV12: {
            libyuv::NV12ToABGR(src->data[0], src->linesize[0],
                               src->data[1], src->linesize[1],
                               dst->pixels[0], dst->linesize[0],
                               src->width, src->height);
        }
            break;

        case AV_PIX_FMT_YUV422P:
        case AV_PIX_FMT_YUVJ422P: {
            libyuv::I422ToABGR(src->data[0], src->linesize[0],
                               src->data[1], src->linesize[1],
                               src->data[2], src->linesize[2],
                               dst->pixels[0], dst->linesize[0],
                               src->width, src->height);
        }
            break;

        case AV_PIX_FMT_YUV444P:
        case AV_PIX_FMT_YUVJ444P: {
            libyuv::I444ToABGR(src->data[0], src->linesize[0],
                               src->data[1], src->linesize[1],
                               src->data[2], src->linesize[2],
                               dst->pixels[0], dst->linesize[0],
                               src->width, src->height);
        }
            break;

        default: {
            SwsContext *sws = nullptr;
            if (!mSwsContext) {
                sws = sws_getContext(src->width, src->height,
                                     static_cast<AVPixelFormat>(src->format),
                                     dst->width, dst->height, DST_PIX_FMT,
                                     SWS_FAST_BILINEAR,
                                     nullptr, nullptr, nullptr);
                if (!sws) {
                    LOGE("[XVideoCodec] sws_getContext failed!\n");
                    return;
                }
                mSwsContext = std::unique_ptr<SwsContext, SwsContextDeleter>(sws);
            } else {
                sws = mSwsContext.get();
            }

            sws_scale(sws, src->data, src->linesize, 0, src->height, dst->pixels, dst->linesize);
        }
            break;
    }
}

std::shared_ptr<XImage> XVideoCodec::getImage(long clock) {
    if (!mImageQueue) {
        return nullptr;
    }

    for (;;) {
        auto image = mImageQueue->peekReadable();
        if (image->pts > clock) {
            return mLastImage ? mLastImage : nullptr;
        } else if (image->pts <= clock && clock <= (image->pts + image->duration)) {
            mLastImage = image;
            return image;
        } else {
            mImageQueue->next();
            continue;
        }
    }

    return nullptr;
}

void XVideoCodec::close() {
    if (mCodecCtx) {
        mCodecCtx.reset();
    }
}


