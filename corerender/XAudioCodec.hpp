//
// Created by Andy on 2020/9/18.
//

#ifndef XRENDER_XAUDIOCODEC_HPP
#define XRENDER_XAUDIOCODEC_HPP

#include <memory>
#include <thread>
#include "XFFHeader.hpp"

class XSampleQueue;

class XAudioCodec {
public:
    XAudioCodec(std::shared_ptr<AVFormatContext> ic, int index);

    ~XAudioCodec();

    int open();

    int decodePacket(bool async = false);

    int getSamples(uint8_t* out, int size);

    void close();

private:
    int decodeAudioFrame();

    int sampleConvert(AVFrame* src);

private:
    void audioWorkThread(void* opaque);

private:
    const uint64_t DST_CHANNEL_LAYOUT = AV_CH_LAYOUT_STEREO;
    const int DST_SAMPLE_RATE = 44100;
    const int DST_CHANNELS = 2;
    const AVSampleFormat DST_SAMPLE_FMT = AV_SAMPLE_FMT_S16;

    int mStatus;
    const int S_READ_EOF = 1 << 0;
    const int S_AUDIO_DECODE_EOF = 1 << 1;

private:
    std::shared_ptr<AVFormatContext> mFormatCtx;

    int mIndex;
    std::unique_ptr<AVCodecContext, CodecDeleter> mCodecCtx;
    std::unique_ptr<std::thread> mAudioTid;
    std::unique_ptr<XSampleQueue> mSampleQueue;
    std::unique_ptr<SwrContext, SwrContextDeleter> mSwrContext;

    uint8_t* mSampleData;
    int mSampleDataIndex;
    int mSampleBufferSize;
    int mSampleBufferSizeMax;
    int mDstSampleCountMax;
};

#endif //XRENDER_XAUDIOCODEC_HPP
