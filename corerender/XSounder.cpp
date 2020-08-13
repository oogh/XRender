//
//  XSounder.cpp
//  XPlayer
//
//  Created by Oogh on 2020/3/19.
//  Copyright © 2020 Oogh. All rights reserved.
//

#include "XSounder.h"
#include "XLogger.h"

XSounder::XSounder() {

}

XSounder::~XSounder() {
}

bool XSounder::init() {
    // 1. 打开音频设备
    if (!mAudioDevice) {
        mAudioDevice = alcOpenDevice(nullptr);
        if (!mAudioDevice) {
            return false;
        }
    }

    // 2. 初始化音频设备上下文环境
    if (!mAudioContext) {
        mAudioContext = alcCreateContext(mAudioDevice, nullptr);
        if (!mAudioContext) {
            return false;
        }
        alcMakeContextCurrent(mAudioContext);
    }

    // 3. 初始化音源
    alGenSources(1, &mSourceId);
    alSourcei(mSourceId, AL_LOOPING, AL_FALSE);
    alSourcef(mSourceId, AL_SOURCE_TYPE, AL_STREAMING);
    alSourcef(mSourceId, AL_GAIN, 1.0f);
    alSpeedOfSound(1.0);

    return true;
}

void XSounder::open(ALubyte* data, ALuint size) {
    if (!data || size <= 0) {
        return;
    }

    {
        std::lock_guard<std::mutex> lock(mMutex);

        ALenum error = alGetError();
        if (error != AL_NO_ERROR) {
            clearBuffer();
            return;
        }

        // 建立缓存区
        ALuint bufferID = 0;
        alGenBuffers(1, &bufferID);
        error = alGetError();
        if (error != AL_NO_ERROR) {
            LOGE("[XSounder] alGenBuffers failed: %d\n", error);
            return;
        }

        // 将数据添加到缓存区
        alBufferData(bufferID, AL_FORMAT_STEREO16, data, size, 44100);
        error = alGetError();
        if (error != AL_NO_ERROR) {
            LOGE("[XSounder] alBufferData failed: %d\n", error);
            return;
        }

        // 清空缓存
        clearBuffer();

        // 读取队列信息，获取音源的缓冲队列，以便舰艇控制播放的延迟
        int processed, queued;
        alGetSourcei(mSourceId, AL_BUFFERS_PROCESSED, &processed);
        alGetSourcei(mSourceId, AL_BUFFERS_QUEUED, &queued);

        if (queued <= BUFF_NUM) {
            alSourceQueueBuffers(mSourceId, 1, &bufferID);
            error = alGetError();
            if (error != AL_NO_ERROR) {
                return;
            }
        }

        error = alGetError();
        if (error != AL_NO_ERROR) {
            alDeleteBuffers(1, &bufferID);
            return;
        }
    }

    // 播放声音
    start();

}

void XSounder::start() {
    ALint state;
    alGetSourcei(mSourceId, AL_SOURCE_STATE, &state);
    if (state != AL_PLAYING) {
        alSourcePlay(mSourceId);
    }
}

void XSounder::stop() {
    start();

    ALint state;
    alGetSourcei(mSourceId, AL_SOURCE_STATE, &state);
    if (state != AL_STOPPED) {
        alSourceStop(mSourceId);
    }
}

ALint XSounder::getProcessedCount() {
    ALint processed;
    alGetSourcei(mSourceId, AL_BUFFERS_PROCESSED, &processed);
    return processed;
}

void XSounder::clearBuffer() {
    ALint processed;

    // 1. 获取音源的缓冲队列
    alGetSourcei(mSourceId, AL_BUFFERS_PROCESSED, &processed);

    // 2. 遍历清空缓冲区
    while (processed--) {
        ALuint buffer;
        alSourceUnqueueBuffers(mSourceId, 1, &buffer);
        alDeleteBuffers(1, &buffer);
    }
}

void XSounder::deinit() {
    alDeleteBuffers(1, &mSourceId);

    if (mAudioContext) {
        alcDestroyContext(mAudioContext);
        mAudioContext = nullptr;
    }

    if (mAudioDevice) {
        alcCloseDevice(mAudioDevice);
        mAudioDevice = nullptr;
    }
}
