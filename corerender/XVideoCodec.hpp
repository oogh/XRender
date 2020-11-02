//
// Created by Oogh on 2020/9/18.
//

#ifndef XRENDER_XVIDEOCODEC_HPP
#define XRENDER_XVIDEOCODEC_HPP

#include <memory>
#include <thread>
#include "XFFHeader.hpp"

struct XImage;
struct XImageQueue;

class XVideoCodec {
public:
    XVideoCodec(std::shared_ptr<AVFormatContext> ic, int index);

    ~XVideoCodec();

    int open();

    int decodePacket(bool async = true);

    std::shared_ptr<XImage> getImage(long clock);

    void close();

private:
    int decodeVideoFrame();

    void queueFrame(AVFrame* frame, long pts, long duration);

    void frameConvert(std::shared_ptr<XImage> dst, AVFrame* src);

private:
    bool checkIsValidPacket(AVPacket* pkt);

private:
    void videoWorkThread(void* opaque);

private:
    int mStatus;
    const int S_READ_EOF = 1 << 0;
    const int S_VIDEO_DECODE_EOF = 1 << 1;

    const AVPixelFormat DST_PIX_FMT = AV_PIX_FMT_RGBA;

private:
    std::shared_ptr<AVFormatContext> mFormatCtx;

    int mIndex;
    std::unique_ptr<AVCodecContext, CodecDeleter> mCodecCtx;
    std::unique_ptr<std::thread> mVideoTid;
    std::unique_ptr<XImageQueue> mImageQueue;
    std::unique_ptr<SwsContext, SwsContextDeleter> mSwsContext;

    ///< 在一个GOP中，两个P帧之间的B帧的数量 这里是一个丢B帧的逻辑。如果在两个P帧之间存在多个B帧，则丢弃第奇数个B帧
    int mBFrameIndex;

    ///< 上一次取到的帧
    std::shared_ptr<XImage> mLastImage;
    static AVPixelFormat mHWPixelFormat;
};


#endif //XRENDER_XVIDEOCODEC_HPP
