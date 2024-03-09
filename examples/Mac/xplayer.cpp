//
// Created by Andy on 2020/11/4.
//

#include "XPlayer.hpp"
#include "XMacView.hpp"
#include "XTimeline.hpp"
#include "XTrack.hpp"
#include "XMediaCore.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
    XMediaCore::getInstance().setShaderPath("/Users/andy/Workspace/Oogh/XRender/Resources/shaders");

    auto timeline = std::make_shared<XTimeline>();
    auto track0 = std::make_shared<XTrack>();
    track0->setFilename("/Users/andy/Workspace/Resources/jieqian_720x1280.mp4");
    track0->setDelay(0);
    track0->setClipStartTime(0);
    track0->setClipEndTime(10000);
    timeline->addTrack(track0);

    auto player = std::make_shared<XPlayer>();

    player->setTimeline(std::move(timeline));
    player->start();

    auto view = std::make_shared<XMacView>(800, 600);
    player->attachObserverRender(view->getRender());
    view->create();

    return 0;
}
