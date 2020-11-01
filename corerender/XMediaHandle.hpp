//
// Created by Oogh on 2020/9/18.
//

#ifndef XRENDER_XMEDIAHANDLE_HPP
#define XRENDER_XMEDIAHANDLE_HPP

#include <memory>
#include <string>
#include "XFFHeader.hpp"

class XVideoCodec;
class XAudioCodec;

class XMediaHandle {
public:
    XMediaHandle();

    ~XMediaHandle();

    int open();

    void close();

public:
    void setInputFile(const std::string& filename);

    void setVideoDisabled(bool disable);

    void setAudioDisabled(bool disable);

    bool isVideoEnabled() const;

    bool isAudioEnabled() const;

public:
    int getOriginalWidth();

    int getOriginalHeight();

    long getOriginalDuration();

    long getOriginalAudioDuration();

    long getOriginalVideoDuration();

    int getOriginalRotation();

    int getOriginalFrameRate();

private:
    int openVideo();

    int openAudio();

private:
    std::string mFilename;

    bool mDisableVideo;
    std::shared_ptr<AVFormatContext> mVideoFormatCtx;
    std::shared_ptr<XVideoCodec> mVideoCodec;
    int mVideoIndex;

    bool mDisableAudio;
    std::shared_ptr<AVFormatContext> mAudioFormatCtx;
    std::shared_ptr<XAudioCodec> mAudioCodec;
    int mAudioIndex;

};


#endif //XRENDER_XMEDIAHANDLE_HPP
