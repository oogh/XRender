//
// Created by Andy on 2020/11/4.
//

#ifndef XRENDER_XPLAYER_HPP
#define XRENDER_XPLAYER_HPP

#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>

class XRender;
class XSounder;

class XTimeline;

class XPlayer {
public:
    XPlayer();

    ~XPlayer();

    void setTimeline(std::shared_ptr<XTimeline> timeline);

    int start();

private:
    void audioWorkThread(void* opaque);

    void videoWorkThread(void* opaque);

private:
    std::unique_ptr<XRender> mRender;
    std::unique_ptr<XSounder> mSounder;
    std::shared_ptr<XTimeline> mTimeline;

    std::unique_ptr<std::thread> mAudioTid;
    std::unique_ptr<std::thread> mVideoTid;
    std::mutex mMutex;
    std::condition_variable mContinueAudioWorkCond;
    std::condition_variable mContinueVideoWorkCond;

    bool mAborted;
};


#endif //XRENDER_XPLAYER_HPP
