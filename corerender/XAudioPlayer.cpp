//
// Created by Andy on 2020/8/30.
//

#include "XAudioPlayer.h"
#include "XLogger.h"
#include <cstdlib>

XAudioPlayer::XAudioPlayer()
: mDevice(nullptr), mContext(nullptr) {

}

XAudioPlayer::~XAudioPlayer() {

}

void XAudioPlayer::start() {
    // Rewind the source position and clear the buffer queue
    alSourceRewind(mSource00);
    alSourcei(mSource00, AL_BUFFER, 0);

    // 2. fill the buffer queue
    for (int i = 0; i < NUM_BUFFERS; ++i) {
        uint8_t* data = reinterpret_cast<uint8_t*>(malloc(BYTES_PRE_BUFFER));
        alBufferData(mBuffers[i], AL_FORMAT_STEREO16, data, BYTES_PRE_BUFFER, 44100);
        if (data) {
            free(data);
            data = nullptr;
        }
    }

    ALenum error = alGetError();
    if (error != AL_NO_ERROR) {
        LOGE("[XAudioPlayer] alBufferData() failed: %s\n", alError2Str(error));
        return;
    }

    // 3. queue buffer and start to playing
    alSourceQueueBuffers(mSource00, NUM_BUFFERS, mBuffers);
    alSourcePlay(mSource00);
    error = alGetError();
    if (error != AL_NO_ERROR) {
        LOGE("[XAudioPlayer] alBufferData() failed: %s\n", alError2Str(error));
        return;
    }
}

void XAudioPlayer::pause() {
    alSourcePause(mSource00);
}

void XAudioPlayer::stop() {
    alSourceStop(mSource00);
}

int XAudioPlayer::updateAudio(uint8_t *data, int size) {
    ALenum error = AL_NO_ERROR;
    ALint processed, state;
    alGetSourcei(mSource00, AL_SOURCE_STATE, &state);
    alGetSourcei(mSource00, AL_BUFFERS_PROCESSED, &processed);

    while (processed > 0) {
        ALuint bufferId;
        alSourceUnqueueBuffers(mSource00, 1, &bufferId);
        if ((error = alGetError()) != AL_NO_ERROR) {
            LOGE("[XAudioPlayer] alSourceUnqueueBuffers() failed: %s\n", alError2Str(error));
            return -1;
        }
        processed--;

        /* Read the next chunk of data, refill the buffer, and queue it
         * back on the source */
        alBufferData(bufferId, AL_FORMAT_STEREO16, data, size, 44100);
        if ((error = alGetError()) != AL_NO_ERROR) {
            LOGE("[XAudioPlayer] alBufferData() failed: %s\n", alError2Str(error));
            return -1;
        }

        alSourceQueueBuffers(mSource00, 1, &bufferId);
        if ((error = alGetError()) != AL_NO_ERROR) {
            LOGE("[XAudioPlayer] alSourceQueueBuffers() failed: %s\n", alError2Str(error));
            return -1;
        }
    }

    /* Make sure the source hasn't underrun */
    if (state != AL_PLAYING && state != AL_PAUSED) {
        ALint queued;

        /* If no buffers are queued, playback is finished */
        alGetSourcei(mSource00, AL_BUFFERS_QUEUED, &queued);
        if (queued == 0) {
            return 0;
        }

        alSourcePlay(mSource00);
        if ((error = alGetError()) != AL_NO_ERROR) {
            LOGE("[XAudioPlayer] alSourcePlay() failed: %s\n", alError2Str(error));
            return -1;
        }
    }

    return size;
}

bool XAudioPlayer::init() {
    // 1. 打开默认音频设备
    mDevice = alcOpenDevice(nullptr);
    if (!mDevice) {
        LOGE("[XALPlayer] alcOpenDevice failed: %s\n", alError2Str(alGetError()));
        return false;
    }

    // 2. 创建音频上下文环境
    mContext = alcCreateContext(mDevice, nullptr);
    if (!mContext) {
        LOGE("[XALPlayer] alcCreateContext failed: %s\n", alError2Str(alGetError()));
        return false;
    }

    // 3. 激活音频上下文环境
    if (!alcMakeContextCurrent(mContext)) {
        LOGE("[XALPlayer] alcMakeContextCurrent failed: %s\n", alError2Str(alGetError()));
        return false;
    }

    // 4. 创建音频Buffers
    alGenBuffers(NUM_BUFFERS, mBuffers);
    ALenum error = alGetError();
    if (error != AL_NO_ERROR) {
        LOGE("[XALPlayer] alGenBuffers failed: %s\n", alError2Str(alGetError()));
        return false;
    }

    // 5. 创建音频源
    alGenSources(1, &mSource00);
    error = alGetError();
    if (error != AL_NO_ERROR) {
        LOGE("[XALPlayer] alGenSources failed: %s\n", alError2Str(alGetError()));
        return false;
    }

    // 6. 配置音源参数
    alSourcei(mSource00, AL_LOOPING, AL_FALSE);
    alSourcef(mSource00, AL_SOURCE_TYPE, AL_STREAMING);
    alSourcef(mSource00, AL_GAIN, 1.0f);
    alSpeedOfSound(1.0);
    alDopplerVelocity(1.0);
    alDopplerFactor(1.0);
    alSourcef(mSource00, AL_PITCH, 1);
    alSourcef(mSource00, AL_GAIN, 1);
    alSource3f(mSource00, AL_POSITION, 0, 0, 0);
    alSource3f(mSource00, AL_VELOCITY, 0, 0, 0);

    return true;
}

void XAudioPlayer::deinit() {
    alDeleteSources(1, &mSource00);

    alDeleteBuffers(NUM_BUFFERS, mBuffers);

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