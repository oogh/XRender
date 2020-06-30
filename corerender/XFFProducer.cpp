//
//  XFFProducer.cpp
//  XExporter
//
//  Created by Oogh on 2020/3/19.
//  Copyright © 2020 Oogh. All rights reserved.
//

#include "XFFProducer.h"
#include "XException.h"
#include "XThreadUtils.h"
#include "XPacketQueue.h"
#include "XFrameQueue.h"
#include "XImageQueue.h"
#include "libyuv.h"
#include "XTimeCounter.h"

XFFProducer::XFFProducer()
        : mVideoIndex(-1), mAudioIndex(-1), mAbortReq(false), mSeekReq(false), mSeekTargetPos(-1), mLastReqClock(INT64_MAX), mAVFormatSeeked(false), mPauseReq(false) {
}

XFFProducer::~XFFProducer() {
    if (!mAbortReq) {
        stop();
    }
}

void XFFProducer::setInput(const std::string &filename) {
    XProducable::setInput(filename);
    int ret = openInFile();
    if (ret < 0) {
        throw XException(av_err2str(ret));
    }
    closeInFile();
}

void XFFProducer::start() {

    if (!mFormatCtx) {
        int ret = openInFile();
        if (ret < 0) {
            return;
        }
    }
    
    if (!mReadTid) {
        mReadTid = std::make_unique<std::thread>([this] { readWorkThread(this); });
    }
}

void XFFProducer::seekTo(long targetPos) {
    
    mPauseReq = true;
    if (mVideoPacketQueue) {
        mVideoPacketQueue->flush();
    }
    if (mAudioPacketQueue) {
        mAudioPacketQueue->flush();
    }
    
    if (mImageQueue) {
        mImageQueue->flush();
    }
    
    mStatus &= ~(S_READ_END | S_VIDEO_END | S_AUDIO_END);
    {
        std::lock_guard<std::mutex> lock(mMutex);
        mContinueReadCond.notify_one();
        mContinueVideoCond.notify_one();
        mContinueAudioCond.notify_one();
    }
    
    mAVFormatSeeked = false;
    mSeekReq = true;
    mSeekTargetPos = targetPos;
    
    mPauseReq = false;
    {
        std::lock_guard<std::mutex> lock(mMutex);
        mContinueReadCond.notify_one();
        mContinueVideoCond.notify_one();
        mContinueAudioCond.notify_one();
    }
    
}

std::shared_ptr<XImage> XFFProducer::peekImage(long clock) {
    if (!mImageQueue) {
        return nullptr;
    }
    
    if (mLastReqClock > clock || (mLastReqClock == INT64_MAX && clock > 0)) {
//        seekTo(clock);
    }
    mLastReqClock = clock;
    
    for (;;) {
        auto image = mImageQueue->peekReadable();
        if (mSeekReq) {
            mSeekReq = false;
        }
        if (image->pts > clock) {
            return nullptr;
        } else if (image->pts <= clock && clock <= (image->pts + image->duration)) {
            return image;
        } else {
            mImageQueue->next();
            continue;
        }
    }

    return nullptr;
}

void XFFProducer::endCurrentImageUse() {
    if (!mImageQueue) {
        return;
    }

    mImageQueue->next();
}

std::shared_ptr<XSample> XFFProducer::getSample() {
    return XProducable::getSample();
}

void XFFProducer::stop() {
    XProducable::stop();

    {
        std::lock_guard<std::mutex> lock(mMutex);
        mAbortReq = true;
        mContinueReadCond.notify_one();
    }

    if (mReadTid && mReadTid->joinable()) {
        mReadTid->join();
    }
}

int XFFProducer::openInFile() {
    AVFormatContext *ic = avformat_alloc_context();
    if (!ic) {
        av_log(nullptr, AV_LOG_FATAL, "[XFFProducer] avformat_alloc_context failed!\n");
        return AVERROR(ENOMEM);
    }
    mFormatCtx = std::unique_ptr<AVFormatContext, InputFormatDeleter>(ic);

    int ret = avformat_open_input(&ic, mFilename.data(), nullptr, nullptr);
    if (ret < 0) {
        av_log(nullptr, AV_LOG_FATAL, "[XFFProducer] avformat_open_input failed: %s\n",
               av_err2str(ret));
        return ret;
    }

    ret = avformat_find_stream_info(ic, nullptr);
    if (ret < 0) {
        av_log(nullptr, AV_LOG_FATAL, "[XFFProducer] avformat_find_stream_info failed: %s\n",
               av_err2str(ret));
        return ret;
    }

    if (!mDisableVideo) {
        int videoIndex = av_find_best_stream(ic, AVMEDIA_TYPE_VIDEO, -1, -1, nullptr, 0);
        if (videoIndex >= 0) {
            mVideoIndex = videoIndex;
            ret = openVideoCodec();
            if (ret < 0) {
                return ret;
            }
            closeVideoCodec();
        }
    }

    if (!mDisableAudio) {
        int audioIndex = av_find_best_stream(ic, AVMEDIA_TYPE_AUDIO, -1, -1, nullptr, 0);
        if (audioIndex >= 0) {
            mAudioIndex = audioIndex;
            ret = openAudioCodec();
            if (ret < 0) {
                return ret;
            }
            closeAudioCodec();
        }
    }

    return 0;
}

int XFFProducer::openVideoCodec() {
    if (mDisableVideo || mVideoIndex < 0 || !mFormatCtx) {
        return -1;
    }

    AVCodecContext *avctx = avcodec_alloc_context3(nullptr);
    if (!avctx) {
        av_log(nullptr, AV_LOG_FATAL, "[XFFProducer] avcodec_alloc_context3 failed!\n");
        return AVERROR(ENOMEM);
    }
    mVideoCodecCtx = std::unique_ptr<AVCodecContext, CodecDeleter>(avctx);

    AVStream *stream = mFormatCtx->streams[mVideoIndex];
    int ret = avcodec_parameters_to_context(avctx, stream->codecpar);
    if (ret < 0) {
        av_log(nullptr, AV_LOG_FATAL, "[XFFProducer] avcodec_parameters_to_context failed: %s\n",
               av_err2str(ret));
        return ret;
    }

    AVCodec* codec = nullptr;
    if (avctx->codec_id == AV_CODEC_ID_H264) {
        codec = avcodec_find_decoder_by_name("h264_mediacodec");
    } else if (avctx->codec_id == AV_CODEC_ID_HEVC) {
        codec = avcodec_find_decoder_by_name("hevc_mediacodec");
    }

    if (!codec) {
        codec = avcodec_find_decoder(avctx->codec_id);
        if (!codec) {
            av_log(nullptr, AV_LOG_FATAL,
                   "[XFFProducer] avcodec_find_decoder failed: cannot find decoder %s\n",
                   avcodec_get_name(avctx->codec_id));
            return AVERROR_DECODER_NOT_FOUND;
        }
    }

    ret = avcodec_open2(avctx, codec, nullptr);
    if (ret < 0) {
        av_log(nullptr, AV_LOG_FATAL, "[XFFProducer] avcodec_open2 failed: %s\n", av_err2str(ret));
        return ret;
    }
    return 0;
}

int XFFProducer::openAudioCodec() {
    if (mDisableAudio || mAudioIndex < 0 || !mFormatCtx) {
        return -1;
    }

    AVCodecContext *avctx = avcodec_alloc_context3(nullptr);
    if (!avctx) {
        av_log(nullptr, AV_LOG_FATAL, "[XFFProducer] avcodec_alloc_context3 failed!\n");
        return AVERROR(ENOMEM);
    }
    mAudioCodecCtx = std::unique_ptr<AVCodecContext, CodecDeleter>(avctx);

    AVStream *stream = mFormatCtx->streams[mAudioIndex];
    int ret = avcodec_parameters_to_context(avctx, stream->codecpar);
    if (ret < 0) {
        av_log(nullptr, AV_LOG_FATAL, "[XFFProducer] avcodec_parameters_to_context failed: %s\n",
               av_err2str(ret));
        return ret;
    }

    AVCodec *codec = avcodec_find_decoder(avctx->codec_id);
    if (!codec) {
        av_log(nullptr, AV_LOG_FATAL,
               "[XFFProducer] avcodec_find_decoder failed: cannot find decoder %s\n",
               avcodec_get_name(avctx->codec_id));
        return AVERROR_DECODER_NOT_FOUND;
    }

    ret = avcodec_open2(avctx, codec, nullptr);
    if (ret < 0) {
        av_log(nullptr, AV_LOG_FATAL, "[XFFProducer] avcodec_open2 failed: %s\n", av_err2str(ret));
        return ret;
    }

    return 0;
}

void XFFProducer::closeVideoCodec() {
    if (mVideoCodecCtx) {
        mVideoCodecCtx.reset();
    }
}

void XFFProducer::closeAudioCodec() {
    if (mAudioCodecCtx) {
        mAudioCodecCtx.reset();
    }
}

void XFFProducer::closeInFile() {
    closeVideoCodec();
    closeAudioCodec();
    if (mFormatCtx) {
        mFormatCtx.reset();
    }
}

void XFFProducer::readWorkThread(void *opaque) {
    XThreadUtils::configThreadName("readWorkThread");
    av_log(nullptr, AV_LOG_INFO, "[XFFProducer] readWorkThread ++++\n");
    XFFProducer *producer = reinterpret_cast<XFFProducer *>(opaque);
    if (!producer || !mFormatCtx) {
        return;
    }

    AVFormatContext *ic = producer->mFormatCtx.get();
    XPacketQueue *videoQ = nullptr;
    XPacketQueue *audioQ = nullptr;

    // video work thread
    if (!producer->mDisableVideo && producer->mVideoIndex >= 0) {
        if (!producer->mVideoPacketQueue) {
            producer->mVideoPacketQueue = std::make_unique<XPacketQueue>();
            videoQ = producer->mVideoPacketQueue.get();
        }

        if (!producer->mVideoTid) {
            producer->mVideoTid = std::make_unique<std::thread>(
                    [this, producer] { videoWorkThread(producer); });
        }
    }

    // audio work thread
    if (!producer->mDisableAudio && producer->mAudioIndex >= 0) {
        if (!producer->mAudioPacketQueue) {
            producer->mAudioPacketQueue = std::make_unique<XPacketQueue>();
            audioQ = producer->mAudioPacketQueue.get();
        }

        if (!producer->mAudioTid) {
            producer->mAudioTid = std::make_unique<std::thread>(
                    [this, producer] { audioWorkThread(producer); });
        }
    }

    int ret;
    long pts = 0;
    long duration = 0;
    for (;;) {
        if (producer->mAbortReq) {
            break;
        }
        
        if (producer->mPauseReq) {
            std::unique_lock<std::mutex> lock(mMutex);
            producer->mContinueReadCond.wait(lock);
            continue;
        }
        
        if (producer->mSeekReq && !producer->mAVFormatSeeked) {
            long ts = static_cast<long>(av_rescale(producer->mSeekTargetPos, AV_TIME_BASE, 1000));
            ret = avformat_seek_file(ic, -1, INT64_MIN, ts, INT64_MAX, 0);
            if (ret < 0) {
                av_log(nullptr, AV_LOG_FATAL, "[XFFProducer] avformat_seek_file failed: %s\n", av_err2str(ret));
                break;
            }
            producer->mAVFormatSeeked = true;
        }

        auto pkt = std::make_shared<Packet>();
        ret = av_read_frame(ic, pkt->avpkt);
        if (ret < 0) {
            if (ret == AVERROR_EOF && (producer->mStatus & S_READ_END) != S_READ_END) {
                if (videoQ) {
                    videoQ->putNullPacket(producer->mVideoIndex);
                }
                if (audioQ) {
                    audioQ->putNullPacket(producer->mAudioIndex);
                }
                producer->mStatus |= S_READ_END;
            }
            std::unique_lock<std::mutex> lock(producer->mMutex);
            mContinueReadCond.wait(lock);
            continue;
        }

        pts = static_cast<long>(pkt->avpkt->pts *
                                av_q2d(ic->streams[pkt->avpkt->stream_index]->time_base) * 1000);
        duration = static_cast<long>(pkt->avpkt->duration *
                                     av_q2d(ic->streams[pkt->avpkt->stream_index]->time_base) *
                                     1000);
        if (pkt->avpkt->stream_index == producer->mVideoIndex) {
            if (videoQ) {
                videoQ->put(pkt);
            }
        } else if (pkt->avpkt->stream_index == producer->mAudioIndex) {
            if (audioQ) {
                audioQ->put(pkt);
            }
        }
    }

    if (producer->mVideoTid && producer->mVideoTid->joinable()) {
        producer->mVideoTid->join();
    }

    if (producer->mAudioTid && producer->mAudioTid->joinable()) {
        producer->mAudioTid->join();
    }

    av_log(nullptr, AV_LOG_INFO, "[XFFProducer] readWorkThread ----\n");
}

void XFFProducer::videoWorkThread(void *opaque) {
    XThreadUtils::configThreadName("videoWorkThread");
    av_log(nullptr, AV_LOG_INFO, "[XFFProducer] videoWorkThread ++++\n");
    XFFProducer *producer = reinterpret_cast<XFFProducer *>(opaque);
    if (!producer) {
        return;
    }

    if (!producer->mVideoCodecCtx) {
        producer->openVideoCodec();
    }

    if (!producer->mImageQueue) {
        producer->mImageQueue = std::make_unique<XImageQueue>();
    }

    int ret;
    for (;;) {
        if (producer->mAbortReq) {
            break;
        }
        
        if (producer->mPauseReq) {
            std::unique_lock<std::mutex> lock(mMutex);
            producer->mContinueVideoCond.wait(lock);
            continue;
        }
        
        ret = decodeVideoFrame();
        if (ret < 0) {
            if (ret == AVERROR_EOF) {
                if ((producer->mStatus & S_READ_END) && !(producer->mStatus & S_VIDEO_END)) {
                    producer->mStatus |= S_VIDEO_END;
                }
                std::unique_lock<std::mutex> lock(producer->mMutex);
                mContinueVideoCond.wait(lock);
                continue;
            }
            break;
        }
    }

    av_log(nullptr, AV_LOG_INFO, "[XFFProducer] videoWorkThread ----\n");
}

void XFFProducer::audioWorkThread(void *opaque) {
    XThreadUtils::configThreadName("audioWorkThread");
    av_log(nullptr, AV_LOG_INFO, "[XFFProducer] audioWorkThread ++++\n");
    XFFProducer *producer = reinterpret_cast<XFFProducer *>(opaque);
    if (!producer) {
        return;
    }

    av_log(nullptr, AV_LOG_INFO, "[XFFProducer] audioWorkThread ----\n");
}

int XFFProducer::decodeVideoFrame() {
    int ret;

    if (!mVideoCodecCtx) {
        ret = openVideoCodec();
        if (ret < 0) {
            return ret;
        }
    }

    ret = AVERROR(EAGAIN);
    long pts = -1;
    long duration = -1;
    for (;;) {

        // receive frame
        do {
            auto frame = std::make_shared<Frame>();
            ret = avcodec_receive_frame(mVideoCodecCtx.get(), frame->avframe);
            if (ret < 0 && ret != AVERROR(EAGAIN) && ret != AVERROR_EOF) {
                return ret;
            }

            if (ret == AVERROR_EOF) {
                avcodec_flush_buffers(mVideoCodecCtx.get());
                return ret;
            }

            if (ret >= 0) {
                AVStream *stream = mFormatCtx->streams[mVideoIndex];
                pts = static_cast<long>(frame->avframe->pts * av_q2d(stream->time_base) * 1000);
                duration = static_cast<long>(frame->avframe->pkt_duration *
                                             av_q2d(stream->time_base) * 1000);
                if (mSeekReq) {
                    if (!(pts <= mSeekTargetPos && mSeekTargetPos < pts + duration)) {
                        continue;
                    }
                }
                queueFrame(frame->avframe, pts, duration);
                return 1;
            }

            if (ret < 0 && ret != AVERROR(EAGAIN)) {
                av_log(nullptr, AV_LOG_FATAL, "[XFFProducer] avcodec_receive_frame failed: %s\n",
                       av_err2str(ret));
                return ret;
            }

        } while (ret != AVERROR(EAGAIN));

        // get packet

        auto pkt = mVideoPacketQueue->get();
        if (!pkt) {
            return -1;
        }

        // send packet
        ret = avcodec_send_packet(mVideoCodecCtx.get(), pkt->avpkt);
        if (ret == AVERROR(EAGAIN)) {
            continue;
        }
    }
    return 0;
}

void XFFProducer::queueFrame(AVFrame *frame, long pts, long duration) {

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
//    av_log(nullptr, AV_LOG_INFO, "[XFFProducer] queue frame pts: %ld\n", pts);
}

void XFFProducer::frameConvert(std::shared_ptr<XImage> dst, AVFrame *src) {
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
        } break;

        case AV_PIX_FMT_NV21: {
            libyuv::NV21ToABGR(src->data[0], src->linesize[0],
                               src->data[1], src->linesize[1],
                               dst->pixels[0], dst->linesize[0],
                               src->width, src->height);
        } break;

        case AV_PIX_FMT_NV12: {
            libyuv::NV12ToABGR(src->data[0], src->linesize[0],
                               src->data[1], src->linesize[1],
                               dst->pixels[0], dst->linesize[0],
                               src->width, src->height);
        } break;

        case AV_PIX_FMT_YUV422P:
        case AV_PIX_FMT_YUVJ422P: {
            libyuv::I422ToABGR(src->data[0], src->linesize[0],
                               src->data[1], src->linesize[1],
                               src->data[2], src->linesize[2],
                               dst->pixels[0], dst->linesize[0],
                               src->width, src->height);
        } break;

        case AV_PIX_FMT_YUV444P:
        case AV_PIX_FMT_YUVJ444P: {
            libyuv::I444ToABGR(src->data[0], src->linesize[0],
                               src->data[1], src->linesize[1],
                               src->data[2], src->linesize[2],
                               dst->pixels[0], dst->linesize[0],
                               src->width, src->height);
        } break;

        default: {
            SwsContext *sws = nullptr;
            if (!mSwsContext) {
                sws = sws_getContext(src->width, src->height,
                                     static_cast<AVPixelFormat>(src->format),
                                     dst->width, dst->height, DST_PIX_FMT,
                                     SWS_FAST_BILINEAR,
                                     nullptr, nullptr, nullptr);
                if (!sws) {
                    av_log(nullptr, AV_LOG_FATAL, "[XFFProducer] sws_getContext failed!\n");
                    return;
                }
                mSwsContext = std::unique_ptr<SwsContext, SwsContextDeleter>(sws);
            } else {
                sws = mSwsContext.get();
            }

            sws_scale(sws, src->data, src->linesize, 0, src->height, dst->pixels, dst->linesize);
        } break;
    }
}
























