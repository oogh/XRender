//
//  XSounder.h
//  XPlayer
//
//  Created by Oogh on 2020/3/19.
//  Copyright © 2020 Oogh. All rights reserved.
//

#ifndef XSOUNDER_H
#define XSOUNDER_H

#if __APPLE__
#include <OpenAL/OpenAL.h>
#elif __ANDROID__
#include <AL/al.h>
#include <AL/alc.h>
#endif

#include <string>
#include <mutex>

class XSounder {
public:
    XSounder();

    ~XSounder();

    bool init();

    void open(ALubyte* data, ALuint size);

    void deinit();

    ALint getProcessedCount();

    void start();

    void stop();

private:

    void clearBuffer();

private:
    static const int BUFF_NUM = 4;    // 缓冲个数
    static const int BUFF_SIZE = 4096;  // 缓冲大小

private:
    ALCdevice *mAudioDevice;
    ALCcontext *mAudioContext;
    ALuint mSourceId;

    std::mutex mMutex;
};

#endif //XSOUNDER_H
