//
// Created by Andy on 2020/11/4.
//

#ifndef XRENDER_XTRACK_HPP
#define XRENDER_XTRACK_HPP

#include <string>
#include "XAudioCodec.hpp"

class XTimeline;

class XTrack {
public:
    XTrack();

    ~XTrack();

    void setTimeline(std::shared_ptr<XTimeline> timeline);

    void setFilename(const std::string& filename);

    void setDelay(long delay);

    void setClipStartTime(long startTime);

    void setClipEndTime(long endTime);

    int getId() const;

    std::string getFilename() const;

    long getDelay() const;

    long getClipStartTime() const;

    long getClipEndTime() const;

    long getClipDuration() const;

    std::shared_ptr<XSample> getSample(long clock, int length);

private:
    static int ID_GENERATOR;

private:
    std::weak_ptr<XTimeline> mTimeline;
    std::string mFilename;
    int mId;
    long mDelay;
    long mClipStartTime;
    long mClipEndTime;

    std::unique_ptr<XAudioCodec> mAudioCodec;
};


#endif //XRENDER_XTRACK_HPP
