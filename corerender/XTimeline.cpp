//
// Created by Andy on 2020/11/4.
//

#include "XTimeline.hpp"
#include "XTrack.hpp"

XTimeline::XTimeline() {

}

XTimeline::~XTimeline() {

}

int XTimeline::addTrack(std::shared_ptr<XTrack> track) {
    std::lock_guard<std::mutex> lock(mMutex);
    track->setTimeline(shared_from_this());
    mTrackList.emplace_back(track);
    return mTrackList.size();
}

int XTimeline::removeTrack(int id) {
    std::lock_guard<std::mutex> lock(mMutex);
    auto iter = std::find_if(mTrackList.begin(), mTrackList.end(), [id](std::shared_ptr<XTrack> track) {
        return track->getId() == id;
    });
    return mTrackList.size();
}