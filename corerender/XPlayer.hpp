//
// Created by Andy on 2020/11/4.
//

#ifndef XRENDER_XPLAYER_HPP
#define XRENDER_XPLAYER_HPP

#include <memory>
#include <thread>

class XRender;
class XSounder;

class XTimeline;

class XPlayer {
public:
    XPlayer();

    ~XPlayer();

    void setTimeline(std::shared_ptr<XTimeline> timeline);

    int prepared();

private:
    void audioWorkThread(void* opaque);

private:
    std::unique_ptr<XRender> mRender;
    std::unique_ptr<XSounder> mSounder;
    std::shared_ptr<XTimeline> mTimeline;

    std::unique_ptr<std::thread> mAudioTid;

    bool mAborted;
};


#endif //XRENDER_XPLAYER_HPP
