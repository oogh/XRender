//
// Created by Andy on 2020/11/4.
//

#ifndef XRENDER_XTRACK_HPP
#define XRENDER_XTRACK_HPP

#include <string>

class XTimeline;

class XTrack {
public:
    explicit XTrack(const std::string& filename);

    ~XTrack();

    void setTimeline(std::shared_ptr<XTimeline> timeline);

    void replaceFilename(const std::string& filename);

    void setDelay(long delay);

    void setClipStartTime(long startTime);

    void setClipEndTime(long endTime);

    int getId() const;

    std::string getFilename() const;

    long getDelay() const;

    long getClipStartTime() const;

    long getClipEndTime() const;

    long getClipDuration() const;

private:
    static int ID_GENERATOR;

private:
    std::weak_ptr<XTimeline> mTimeline;
    std::string mFilename;
    int mId;
    long mDelay;
    long mClipStartTime;
    long mClipEndTime;
};


#endif //XRENDER_XTRACK_HPP
