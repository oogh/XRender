//
//  XSounder.cpp
//  XPlayer
//
//  Created by Oogh on 2020/3/19.
//  Copyright © 2020 Oogh. All rights reserved.
//

#include "XSounder.h"
#include "XLogger.h"

void XSounder::refreshQueueBuffer() {
    ALint processedNum = NumOfBuffProcessed();
    while (processedNum--) {
        ALuint buffID;
        alSourceUnqueueBuffers(mSourceId, 1, &buffID);
        alDeleteBuffers(1, &buffID);
    }
}

ALenum XSounder::checkError() {
    ALenum errState = AL_NO_ERROR;
    errState = alGetError();
    switch (errState) {
        case AL_NO_ERROR:
            LOGE("AL_NO_ERROR");
            break;

        case AL_INVALID_NAME:
            LOGE("AL_INVALID_NAME : Invalid Name paramater passed to AL call");
            break;

        case AL_INVALID_ENUM:
            LOGE("AL_INVALID_ENUM : Invalid parameter passed to AL call");
            break;

        case AL_INVALID_VALUE:
            LOGE("AL_INVALID_VALUE : Invalid enum parameter value");
            break;

        case AL_INVALID_OPERATION:
            LOGE("AL_INVALID_OPERATION : Illegal call");
            break;

        case AL_OUT_OF_MEMORY:
            LOGE("AL_OUT_OF_MEMORY : No mojo");
            break;

        default:
            LOGE("Unknown error code");
            break;
    }
    return errState;
}

XSounder::XSounder() {
    m_fillBufLen = 0;
    m_totalBufLen = BUFF_NUM * BUFF_SIZE;
    m_initBuf = (ALubyte *) malloc(m_totalBufLen);

    init();
}

XSounder::~XSounder() {
    close();
}

bool XSounder::init() {
    mAudioDevice = alcOpenDevice(nullptr);     // 参数为 nullptr, 让 ALC 使用默认设备
    mAudioContext = alcCreateContext(mAudioDevice, nullptr);
    alcMakeContextCurrent(mAudioContext);   // 设置当前上下文

    alGenSources(1, &mSourceId);       // 初始化音源 ID

    return true;
}

bool XSounder::ConfigOpenAL(ALuint channels, ALuint bits, ALuint frequency) {
    ALenum format;
    if (8 == bits) {
        if (1 == channels) {
            format = AL_FORMAT_MONO8;   // 单声道 8 bit
        } else if (2 == channels) {
            format = AL_FORMAT_STEREO8; // 立体声（双声道） 8 bit
        } else {
            format = 0;
        }
    } else if (16 == bits) {
        if (1 == channels) {
            format = AL_FORMAT_MONO16;  // 单声道 16 bit
        } else if (2 == channels) {
            format = AL_FORMAT_STEREO16;    // 立体声（双声道） 16 bit
        } else {
            format = 0;
        }
    } else {
        format = 0;
    }
    if (0 == format) {
        LOGE("Incompatible format : channels = %d bits = %d\n", channels, bits);
        return false;
    }
    mFormat = format;
    mSampleRate = frequency;
    return true;
}


void XSounder::setLoop(ALint isLoop) {
    alSourcei(mSourceId, AL_LOOPING, isLoop);
}


void XSounder::setVolume(ALfloat volume) {
    alSourcef(mSourceId, AL_GAIN, volume);
}


void XSounder::setSpeed(ALfloat speed) {
    alSpeedOfSound(speed);
}


ALint XSounder::NumOfBuffProcessed() {
    ALint bufNum;
    alGetSourcei(mSourceId, AL_BUFFERS_PROCESSED, &bufNum);
    return bufNum;
}


ALSourceState XSounder::SourceState() {
    ALint val;
    ALSourceState state;
    alGetSourcei(mSourceId, AL_SOURCE_STATE, &val);
    switch (val) {
        case AL_INITIAL:
            state = alSource_initial;
            break;

        case AL_PLAYING:
            state = alSource_playing;
            break;

        case AL_PAUSED:
            state = alSource_paused;
            break;

        case AL_STOPPED:
            state = alSource_stopped;
            break;

        default:
            state = alSource_initial;
            break;
    }
    return state;
}


void XSounder::OpenAudio(ALubyte *buffer, ALuint length) {
    if (nullptr == buffer || 0 == length) {
        LOGE("Can not open audio !");
        return;
    }
    if (0 == m_fillBufLen || m_totalBufLen > m_fillBufLen) { // 先初始化完所有预备（开头部分）缓冲区
        ALint needLen = m_totalBufLen - m_fillBufLen;     // 需要初始化缓冲长度
        ALint cpyLen = length > needLen ? needLen : length;  // 复制数据长度
        ALint remainLen = length - cpyLen;  // 剩余数据长度

        memcpy(m_initBuf, buffer, cpyLen);
        m_fillBufLen += cpyLen;

        if (m_totalBufLen == m_fillBufLen) {  // 缓冲已满，可以开始播放
            alGenBuffers(BUFF_NUM, m_buffers);  // 初始化缓冲
            for (ALint i = 0; i < BUFF_NUM; i++)    // 开头数据初始化
            {
                LOGE("初始化数据到第 %d 个缓冲区\n", i);
                alBufferData(m_buffers[i], mFormat, m_initBuf + i * BUFF_SIZE, BUFF_SIZE,
                             mSampleRate);
            }
            LOGE("所有缓冲区数据初始化完毕，开始进入 AL 音源队列 !\n");
            alSourceQueueBuffers(mSourceId, BUFF_NUM, m_buffers);  // 将数据缓冲送人音源队列
            alDeleteSources(BUFF_NUM, m_buffers);   // 送入队列后的数据缓冲可以清理
            free(m_initBuf);
            m_initBuf = nullptr;

            PlaySound();
        }
        if (0 < remainLen) {  // 缓冲已满，数据还有剩余
            ALuint bufId;
            alGenBuffers(1, &bufId);
            alBufferData(bufId, mFormat, buffer + cpyLen, remainLen, mSampleRate);
            alSourceQueueBuffers(mSourceId, 1, &bufId);    // 将数据缓冲送人音源队列
            alDeleteBuffers(1, &bufId); // 送入队列后的数据缓冲可以清理
        }
    } else {
        refreshQueueBuffer();   // 更新缓存

        // 循环播放
        ALuint loopBufID;
        alGenBuffers(1, &loopBufID);
        alBufferData(loopBufID, mFormat, buffer, (ALsizei) length, mSampleRate);
        // 新替换缓冲区重新如队列等待 OpenAL 处理
        alSourceQueueBuffers(mSourceId, 1, &loopBufID);
        alDeleteBuffers(1, &loopBufID); // 送入队列后的数据缓冲可以清理

        PlaySound();
    }
}


void XSounder::PlaySound() {
    if (alSource_playing != SourceState()) {
        alSourcePlay(mSourceId);
    }
}


void XSounder::PauseSound() {
    if (alSource_paused != SourceState()) {
        alSourcePause(mSourceId);
    }
}

void XSounder::StopSound() {
    if (alSource_stopped != SourceState()) {
        alSourceStop(mSourceId);
    }
}

void XSounder::close() {
    alDeleteSources(1, &mSourceId);
    alcDestroyContext(mAudioContext);
    alcCloseDevice(mAudioDevice);
}

