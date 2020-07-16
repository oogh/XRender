//
//  XFFProducer.h
//  XExporter
//
//  Created by Oogh on 2020/3/19.
//  Copyright © 2020 Oogh. All rights reserved.
//

#ifndef XEXPORTER_XFFPRODUCER_H
#define XEXPORTER_XFFPRODUCER_H

#include <thread>
#include <mutex>
#include <condition_variable>
#include <string>
#include "XProducable.h"
#include "XFFHeader.h"
#include "XSampleQueue.h"

class XPacketQueue;
class XImageQueue;
class XSampleQueue;

class XFFProducer : public XProducable {
public:
    XFFProducer();

    ~XFFProducer() override;

    void setInput(const std::string& filename) override;

    void start() override;

    std::shared_ptr<XImage> peekImage(long clock) override;
    
    void endCurrentImageUse() override;

    std::shared_ptr<XSample> getSample() override;
    
    void stop() override;
    
public:
    long getOriginalDuration() const override;
    
    int getOriginalWidth() const override;
    
    int getOriginalHeight() const override;
    
private:
    void seekTo(long targetPos);

private:
    int openInFile();

    int openVideoCodec();

    int openAudioCodec();

    void closeVideoCodec();

    void closeAudioCodec();

    void closeInFile();

private:
    void readWorkThread(void* opaque);

    void videoWorkThread(void* opaque);

    void audioWorkThread(void* opaque);

private:
    int decodeVideoFrame();

    void queueFrame(AVFrame* frame, long pts, long duration);

    void frameConvert(std::shared_ptr<XImage> dst, AVFrame* src);

private:
    unsigned int mStatus;
    const int S_READ_END = 1 << 0;
    const int S_VIDEO_END = 1 << 1;
    const int S_AUDIO_END = 1 << 2;

    const AVPixelFormat DST_PIX_FMT = AV_PIX_FMT_RGBA;

private:
    std::unique_ptr<AVFormatContext, InputFormatDeleter> mFormatCtx;

    int mVideoIndex;
    int mAudioIndex;

    std::unique_ptr<AVCodecContext, CodecDeleter> mVideoCodecCtx;
    std::unique_ptr<AVCodecContext, CodecDeleter> mAudioCodecCtx;

    std::unique_ptr<std::thread> mReadTid;
    std::unique_ptr<std::thread> mVideoTid;
    std::unique_ptr<std::thread> mAudioTid;

    std::unique_ptr<XPacketQueue> mVideoPacketQueue;
    std::unique_ptr<XPacketQueue> mAudioPacketQueue;

    bool mSeekReq;
    bool mAVFormatSeeked;
    
    long mSeekTargetPos;
    
    bool mPauseReq;
    
    bool mAbortReq;

    std::mutex mMutex;
    std::condition_variable mContinueReadCond;
    std::condition_variable mContinueVideoCond;
    std::condition_variable mContinueAudioCond;

    std::unique_ptr<XImageQueue> mImageQueue;
    rbuf_t* mSampleQueue;

    std::unique_ptr<SwsContext, SwsContextDeleter> mSwsContext;
    std::unique_ptr<SwrContext, SwrContextDeleter> mSwrContext;

    int mPutVideoPacket = 0;
    int mPutAudioPacket = 0;
    int mGetVideoPacket = 0;
    int mGetAudioPacket = 0;
    
    long mLastReqClock;
};


#endif //XEXPORTER_XFFPRODUCER_H
