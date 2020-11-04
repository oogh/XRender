//
// Created by Andy on 2020/11/4.
//

#include "XPlayer.hpp"
#include "XRender.hpp"
#include "XSounder.hpp"

XPlayer::XPlayer() {
    mRender = std::make_unique<XRender>();
    mSounder = std::make_unique<XSounder>();
}

XPlayer::~XPlayer() {

}

void XPlayer::setTimeline(std::shared_ptr<XTimeline> timeline) {
    mTimeline = timeline;
}

int XPlayer::prepared() {
    if (!mAudioTid) {
        mAudioTid = std::make_unique<std::thread>([this]{ audioWorkThread(this); });
    }


    return 0;
}

void XPlayer::audioWorkThread(void* opaque) {
    auto player = reinterpret_cast<XPlayer*>(opaque);

    player->mSounder->start();

    for (;;) {
        if (player->mAborted) {
            break;
        }


    }

    player->mSounder->stop();
}

