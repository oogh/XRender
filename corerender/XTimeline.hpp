//
// Created by Andy on 2020/11/4.
//

#ifndef XRENDER_XTIMELINE_HPP
#define XRENDER_XTIMELINE_HPP

#include <list>
#include <mutex>

class XTrack;

class XTimeline : public std::enable_shared_from_this<XTimeline> {
public:
    XTimeline();

    ~XTimeline();

    int addTrack(std::shared_ptr<XTrack> track);

    int removeTrack(int id);

private:
    std::list<std::shared_ptr<XTrack>> mTrackList;

    std::mutex mMutex;
};


#endif //XRENDER_XTIMELINE_HPP
