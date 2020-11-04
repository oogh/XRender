//
// Created by Andy on 2020/11/4.
//

#include <iostream>
#include "XPlayer.hpp"
#include "XTimeline.hpp"
#include "XTrack.hpp"

int main(int argc, char* argv[]) {

    auto timeline = std::make_shared<XTimeline>();
    auto track0 = std::make_shared<XTrack>("/Users/andy/Workspace/Resources/jieqian_720x1280.mp4");
    timeline->addTrack(track0);
    auto track1 = std::make_shared<XTrack>("/Users/andy/Workspace/Resources/720.mp4");
    timeline->addTrack(track1);

    auto player = std::make_unique<XPlayer>();
    player->setTimeline(timeline);

    return 0;
}