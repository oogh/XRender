//
// Created by Andy on 2020/9/18.
//

#include "XMediaHandle.hpp"
#include "XVideoCodec.hpp"
#include "XAudioCodec.hpp"
#include "XLogger.hpp"

XMediaHandle::XMediaHandle()
: mDisableVideo(false), mDisableAudio(false), mVideoIndex(-1), mAudioIndex(-1) {

}

XMediaHandle::~XMediaHandle() {

}

int XMediaHandle::open() {
    int ret;

    if (!mDisableVideo) {
        ret = openVideo();
        if (ret < 0 ) {
            return ret;
        }
    }

    if (!mDisableAudio) {
        ret = openAudio();
        if (ret < 0) {
            return ret;
        }
    }
    return 0;
}

void XMediaHandle::close() {
    if (mVideoCodec) {
        mVideoCodec->close();
    }

    if (mAudioCodec) {
        mAudioCodec->close();
    }
}

void XMediaHandle::setInputFile(const std::string &filename) {
    mFilename = filename;
}

void XMediaHandle::setVideoDisabled(bool disable) {
    mDisableVideo = disable;
}

void XMediaHandle::setAudioDisabled(bool disable) {
    mDisableAudio = disable;
}

bool XMediaHandle::isAudioEnabled() const {
    return mAudioIndex >= 0;
}

bool XMediaHandle::isVideoEnabled() const {
    return mVideoIndex >= 0;
}

int XMediaHandle::openVideo() {
    AVFormatContext *ic = avformat_alloc_context();
    if (!ic) {
        LOGE("[XMediaHandle] avformat_alloc_context failed!\n");
        return AVERROR(ENOMEM);
    }
    mVideoFormatCtx = std::shared_ptr<AVFormatContext>(ic, InputFormatDeleter());

    int ret = avformat_open_input(&ic, mFilename.data(), nullptr, nullptr);
    if (ret < 0) {
        LOGE("[XMediaHandle] avformat_open_input failed: %s\n",
             av_err2str(ret));
        return ret;
    }

    ret = avformat_find_stream_info(ic, nullptr);
    if (ret < 0) {
        LOGE("[XMediaHandle] avformat_find_stream_info failed: %s\n", av_err2str(ret));
        return ret;
    }
    
    int index = av_find_best_stream(ic, AVMEDIA_TYPE_VIDEO, -1, -1, nullptr, 0);
    if (index < 0) {
        LOGE("[XMediaHandle] av_find_best_stream failed: cannot find video stream!\n");
        return AVERROR_STREAM_NOT_FOUND;
    }
    mVideoIndex = index;

    mVideoCodec = std::make_shared<XVideoCodec>(mVideoFormatCtx, index);
    ret = mVideoCodec->open();
    if (ret < 0) {
        return ret;
    }

    return 0;
}

int XMediaHandle::openAudio() {
    AVFormatContext *ic = avformat_alloc_context();
    if (!ic) {
        LOGE("[XMediaHandle] avformat_alloc_context failed!\n");
        return AVERROR(ENOMEM);
    }
    mAudioFormatCtx = std::shared_ptr<AVFormatContext>(ic, InputFormatDeleter());

    int ret = avformat_open_input(&ic, mFilename.data(), nullptr, nullptr);
    if (ret < 0) {
        LOGE("[XMediaHandle] avformat_open_input failed: %s\n",
             av_err2str(ret));
        return ret;
    }

    ret = avformat_find_stream_info(ic, nullptr);
    if (ret < 0) {
        LOGE("[XMediaHandle] avformat_find_stream_info failed: %s\n", av_err2str(ret));
        return ret;
    }

    int index = av_find_best_stream(ic, AVMEDIA_TYPE_AUDIO, -1, -1, nullptr, 0);
    if (index < 0) {
        LOGE("[XMediaHandle] av_find_best_stream failed: cannot find audio stream!\n");
        return AVERROR_STREAM_NOT_FOUND;
    }
    mAudioIndex = index;

    mAudioCodec = std::make_shared<XAudioCodec>(mAudioFormatCtx, index);
    ret = mAudioCodec->open();
    if (ret < 0) {
        return ret;
    }

    return 0;
}

int XMediaHandle::getOriginalWidth() {
    return 0;
}

int XMediaHandle::getOriginalHeight() {
    return 0;
}

long XMediaHandle::getOriginalDuration() {
    return 0;
}

long XMediaHandle::getOriginalAudioDuration() {
    return 0;
}

long XMediaHandle::getOriginalVideoDuration() {
    return 0;
}

int XMediaHandle::getOriginalRotation() {
    return 0;
}

int XMediaHandle::getOriginalFrameRate() {
    return 0;
}
