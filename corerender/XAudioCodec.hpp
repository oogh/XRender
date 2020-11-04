//
// Created by Oogh on 2020/9/18.
//

#ifndef XRENDER_XAUDIOCODEC_HPP
#define XRENDER_XAUDIOCODEC_HPP

#include <memory>
#include <thread>
#include "XFFHeader.hpp"
#include "XSample.hpp"
#include "XPacketQueue.hpp"

class XSampleQueue;

class XAudioCodec {
public:
    XAudioCodec();

    ~XAudioCodec();

    int setFilename(std::string filename);

    void setLoop(bool loop);

    int open();

    int seekFileTo(long target);

    std::shared_ptr<Packet> getPacket();

    std::shared_ptr<XSample> getSample(int length);

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
    std::string mFilename;
    std::unique_ptr<AVFormatContext, InputFormatDeleter> mFormatCtx;

    int mIndex;
    std::unique_ptr<AVCodecContext, CodecDeleter> mCodecCtx;
    std::unique_ptr<std::thread> mAudioTid;
    std::unique_ptr<XSampleQueue> mSampleQueue;
    std::unique_ptr<SwrContext, SwrContextDeleter> mSwrContext;
    bool mLoop;

    ///< 采样缓存区
    uint8_t* mAudioBuffer;

    ///< 记录当前采样缓存区中已被读取的采样数据的大小
    int mAudioBufferIndex;

    ///< 记录当前采样缓存区中剩余采样数据的大小，其值等于 mAudioBufferSizeMax - mAudioBufferIndex
    int mAudioBufferSize;

    ///< 记录上一次重采样之后缓存区中存放的采样数据的大小
    int mAudioBufferSizeMax;

    ///< 理论上执行重采样过后，得到的采样数。这里记录最大值，确保重采样输出缓存区的内存空间足够大
    int mDstSampleCountMax;
};

#endif //XRENDER_XAUDIOCODEC_HPP
