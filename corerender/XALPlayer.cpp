//
// Created by Andy on 2020/8/14.
//

#include "XALPlayer.h"
#include "XLogger.h"

XALPlayer::XALPlayer()
:  mDevice(nullptr), mContext(nullptr) {
}

XALPlayer::~XALPlayer() {

}

bool XALPlayer::init() {
    // 1. 打开默认音频设备
    mDevice = alcOpenDevice(nullptr);
    if (!mDevice) {
        LOGE("[XALPlayer] alcOpenDevice failed: %s\n", alError2Str(alGetError()));
        return false;
    }
    if (alGetError() != AL_NO_ERROR) {
        LOGE("[XALPlayer] alcOpenDevice failed: %s\n", alError2Str(alGetError()));
    }

    // 2. 创建音频上下文环境
    mContext = alcCreateContext(mDevice, nullptr);
    if (!mContext) {
        LOGE("[XALPlayer] alcCreateContext failed: %s\n", alError2Str(alGetError()));
        return false;
    }
    if (alGetError() != AL_NO_ERROR) {
        LOGE("[XALPlayer] alcCreateContext failed: %s\n", alError2Str(alGetError()));
    }

    // 3. 激活音频上下文环境
    if (!alcMakeContextCurrent(mContext)) {
        LOGE("[XALPlayer] alcMakeContextCurrent failed: %s\n", alError2Str(alGetError()));
        return false;
    }
    if (alGetError() != AL_NO_ERROR) {
        LOGE("[XALPlayer] alcMakeContextCurrent failed: %s\n", alError2Str(alGetError()));
    }

    // 6. 创建并初始化Source
    alGenSources(1, &mSources);
    if (alGetError() != AL_NO_ERROR) {
        LOGE("[XALPlayer] alGenSources failed: %s\n", alError2Str(alGetError()));
        return false;
    }

    // 7. 配置声源参数
    alSourcei(mSources, AL_LOOPING, AL_FALSE);
    alSourcef(mSources, AL_SOURCE_TYPE, AL_STREAMING);
    alSourcef(mSources, AL_GAIN, 1.0f);
    alSpeedOfSound(1.0);
    alDopplerVelocity(1.0);
    alDopplerFactor(1.0);
    alSourcef(mSources, AL_PITCH, 1);
    alSourcef(mSources, AL_GAIN, 1);
    alSource3f(mSources, AL_POSITION, 0, 0, 0);
    alSource3f(mSources, AL_VELOCITY, 0, 0, 0);

    return true;
}

void XALPlayer::updateData(uint8_t* data, int size) {
    if (!data || size <= 0) {
        LOGE("[XALPlayer] there is no audio data to play!\n");
        return;
    }

    ALuint buffers[PLAYER_BUFFER_COUNT];
    for (int i = 0; i < PLAYER_BUFFER_COUNT; ++i) {
        alGenBuffers(1, &buffers[i]);
        alBufferData(buffers[i], AL_FORMAT_STEREO16, data + PLAYER_BUFFER_SIZE * i, PLAYER_BUFFER_SIZE, 44100);
        alSourceQueueBuffers(mSources, 1, &buffers[i]);
    }

    ALint processed, queued, state;
    alGetSourcei(mSources, AL_BUFFERS_PROCESSED, &processed);
    alGetSourcei(mSources, AL_BUFFERS_QUEUED, &queued);
    alGetSourcei(mSources, AL_SOURCE_STATE, &state);
    if (state == AL_STOPPED || state == AL_PAUSED || state == AL_INITIAL) {
        if (queued < processed || queued == 0 || (queued == 1 && processed == 1)) {
            stop();
            flushQueueBuffer();
            deinit();
            return;
        }

        start();
    }

    flushQueueBuffer();
}

void XALPlayer::start() {
    ALint state;
    alGetSourcei(mSources, AL_SOURCE_STATE, &state);
    if(state != AL_PLAYING) {
        alSourcePlay(mSources);
    }
}

void XALPlayer::pause() {
    alSourcePause(mSources);
}

void XALPlayer::stop() {
    alSourceStop(mSources);
}

void XALPlayer::flushQueueBuffer() {
    ALint processed;
    alGetSourcei(mSources, AL_BUFFERS_PROCESSED, &processed);
    while (processed--) {
        ALuint buffer;
        alSourceUnqueueBuffers(mSources, 1, &buffer);
        alDeleteBuffers(1, &buffer);
    }
}

void XALPlayer::deinit() {
    alDeleteSources(1, &mSources);

    alcMakeContextCurrent(nullptr);

    if (mContext) {
        alcDestroyContext(mContext);
        mContext = nullptr;
    }

    if (mDevice) {
        alcCloseDevice(mDevice);
        mDevice = nullptr;
    }
}

const char* XALPlayer::alError2Str(ALenum error) {
    switch (error) {
        case AL_INVALID_NAME: return "AL_INVALID_NAME : Invalid Name parameter passed to AL call";
        case AL_INVALID_ENUM: return "AL_INVALID_ENUM : Invalid parameter passed to AL call";
        case AL_INVALID_VALUE: return "AL_INVALID_VALUE : Invalid enum parameter value";
        case AL_INVALID_OPERATION: return "AL_INVALID_OPERATION : Illegal call";
        case AL_OUT_OF_MEMORY: return "AL_OUT_OF_MEMORY : No mojo";
        default: return "Unknown error code";
    }
    return "";
}
