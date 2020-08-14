//
// Created by Andy on 2020/8/14.
//

#ifndef XRENDER_XALPLAYER_H
#define XRENDER_XALPLAYER_H

#include <cinttypes>
#include "XALHeader.h"

class XALPlayer {
public:
    XALPlayer();

    ~XALPlayer();

    bool init();

    void updateData(uint8_t* data, int size);

    void start();

    void pause();

    void stop();

    void deinit();

private:
    /**
     * @brief 清理已经使用完毕的Buffer
     */
    void flushQueueBuffer();

private:
    const char* alError2Str(ALenum error);

private:
    static const int PLAYER_BUFFER_COUNT = 1;
    static const int PLAYER_BUFFER_SIZE = 4096;

private:
    ALCdevice* mDevice;
    ALCcontext* mContext;
    ALuint mSources;
};


#endif //XRENDER_XALPLAYER_H
