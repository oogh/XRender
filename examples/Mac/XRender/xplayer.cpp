//
// Created by Andy on 2020/11/4.
//

#include <iostream>
#include "XPlayer.hpp"
#include "XTimeline.hpp"
#include "XTrack.hpp"
#include "XSurface.hpp"

#include <thread>

int main(int argc, char* argv[]) {

    auto timeline = std::make_shared<XTimeline>();
    auto track0 = std::make_shared<XTrack>();
    track0->setFilename("/Users/andy/Workspace/Resources/jieqian_720x1280.mp4");
    track0->setDelay(1000);
    track0->setClipStartTime(0);
    track0->setClipEndTime(10000);
    timeline->addTrack(track0);

    auto player = std::make_shared<XPlayer>();
    player->setTimeline(timeline);

    auto surface = std::make_shared<XSurface>(800, 600);
    surface->setPlayer(player);
    surface->create();

    getchar();

    return 0;
}