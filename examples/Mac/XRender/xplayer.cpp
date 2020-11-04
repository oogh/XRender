//
// Created by Andy on 2020/11/4.
//

#include <iostream>
#include "XPlayer.hpp"
#include "XTimeline.hpp"
#include "XTrack.hpp"

int main(int argc, char* argv[]) {

    auto timeline = std::make_shared<XTimeline>();
    auto track0 = std::make_shared<XTrack>();
    track0->setFilename("/Users/oogh/Workspace/Resources/jieqian_720x1280.mp4");
    track0->setDelay(1000);
    track0->setClipStartTime(0);
    track0->setClipEndTime(10000);
    timeline->addTrack(track0);
    auto track1 = std::make_shared<XTrack>();
    track1->setFilename("/Users/oogh/Workspace/Resources/douyin.mp4");
    track1->setDelay(0);
    track1->setClipStartTime(0);
    track1->setClipEndTime(20000);
    timeline->addTrack(track1);

    auto player = std::make_unique<XPlayer>();
    player->setTimeline(timeline);
    player->start();

    getchar();

    return 0;
}