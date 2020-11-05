//
// Created by Andy on 2020/11/4.
//

#include "XPlayer.hpp"
#include "XRender.hpp"
#include "XSounder.hpp"
#include "XTimeline.hpp"
#include "XThreadUtils.hpp"
#include "XLogger.hpp"

XPlayer::XPlayer() {
    mRender = std::make_shared<XRender>();
    mSounder = std::make_unique<XSounder>();
}

XPlayer::~XPlayer() {

}

void XPlayer::setTimeline(std::shared_ptr<XTimeline> timeline) {
    mTimeline = timeline;
}

void XPlayer::setRender(std::shared_ptr<XRender> render) {
    mRender = render;
}

int XPlayer::start() {
    if (!mAudioTid) {
        mAudioTid = std::make_unique<std::thread>([this]{ audioWorkThread(this); });
    }

    if (!mVideoTid) {
        mVideoTid = std::make_unique<std::thread>([this]{ videoWorkThread(this); });
    }

    return 0;
}

void XPlayer::audioWorkThread(void* opaque) {
    XThreadUtils::configThreadName("audioWorkThread");
    LOGD("[XPlayer] audioWorkThread ++++\n");
    auto player = reinterpret_cast<XPlayer*>(opaque);

    player->mSounder->start();

    for (;;) {
        if (player->mAborted) {
            break;
        }

        if (player->mTimeline->isCompleted()) {
            std::unique_lock<std::mutex> lock(player->mMutex);
            player->mContinueAudioWorkCond.wait(lock);
        }

        auto sample = player->mTimeline->getSample(4096);
        player->mSounder->updateAudio(sample->data, sample->length);

    }

    player->mSounder->stop();
    LOGD("[XPlayer] audioWorkThread ----\n");
}

void XPlayer::videoWorkThread(void* opaque) {
    XThreadUtils::configThreadName("videoWorkThread");
    LOGD("[XPlayer] videoWorkThread ++++\n");
    auto player = reinterpret_cast<XPlayer*>(opaque);

    player->mRender->start();

    for (;;) {
        if (player->mAborted) {
            break;
        }

        if (player->mTimeline->isCompleted()) {
            std::unique_lock<std::mutex> lock(player->mMutex);
            player->mContinueVideoWorkCond.wait(lock);
        }

        auto image = player->mTimeline->getImage(player->mTimeline->getClock());
        player->mRender->updatePixel(image->pixels[0], image->width, image->height);
    }

    player->mRender->stop();
    LOGD("[XPlayer] videoWorkThread ----\n");
}

