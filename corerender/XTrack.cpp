//
// Created by Andy on 2020/11/4.
//

#include "XTrack.hpp"
#include "XTimeline.hpp"

int XTrack::ID_GENERATOR = 0;

XTrack::XTrack(const std::string& filename)
: mId(ID_GENERATOR++), mFilename(filename), mDelay(0), mClipStartTime(0), mClipEndTime(0) {

}

XTrack::~XTrack() {

}

void XTrack::setTimeline(std::shared_ptr<XTimeline> timeline) {
    mTimeline = timeline;
}

void XTrack::replaceFilename(const std::string& filename) {
    mFilename = filename;
}

int XTrack::getId() const {
    return mId;
}

void XTrack::setDelay(long delay) {
    mDelay = delay;
}

void XTrack::setClipStartTime(long startTime) {
    mClipStartTime = startTime;
}

void XTrack::setClipEndTime(long endTime) {
    mClipEndTime = endTime;
}

std::string XTrack::getFilename() const {
    return mFilename;
}

long XTrack::getDelay() const {
    return mDelay;
}

long XTrack::getClipStartTime() const {
    return mClipStartTime;
}

long XTrack::getClipEndTime() const {
    return mClipEndTime;
}

long XTrack::getClipDuration() const {
    return mClipEndTime - mClipStartTime;
}