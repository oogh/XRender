//
// Created by Andy on 2020/11/4.
//

#ifndef XRENDER_XTIMELINE_HPP
#define XRENDER_XTIMELINE_HPP

#include <list>
#include <mutex>
#include "XTrack.hpp"

class XSample;

class XTimeline : public std::enable_shared_from_this<XTimeline> {
public:
    XTimeline();

    ~XTimeline();

    int addTrack(std::shared_ptr<XTrack> track);

    int removeTrack(int id);

    std::shared_ptr<XSample> getSample(int length);

    std::shared_ptr<XImage> getImage(long clock);

    long getClock();

    bool isCompleted();

private:
    void updateDuration();

private:
    std::list<std::shared_ptr<XTrack>> mTrackList;

    std::mutex mMutex;

    int mUsedSample;

    long mDuration;
};


#endif //XRENDER_XTIMELINE_HPP
