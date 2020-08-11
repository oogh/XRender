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

/* 音源状态类型 */
enum ALSourceState {
    alSource_initial = 0x00,     // 初始
    alSource_playing = 0x01,     // 播放
    alSource_paused = 0x02,     // 暂停
    alSource_stopped = 0x03,     // 停止
};

class XSounder {
public:
    XSounder();

    ~XSounder();

    /**
     初始化 OpenAL
     @return 是否初始化成功（true：成功； false：失败）
     */
    bool init();

    /**
     配置 OpenAL
     @param channels 声道数
     @param bits 位深
     @param frequency 采样频率
     @return 是否配置成功（true：成功； false：失败）
     */
    bool ConfigOpenAL(ALuint channels, ALuint bits, ALuint frequency);

    /**
     设置是否循环播放
     @param isLoop 是否循环（1：循环；0：不循环）
     */
    void setLoop(ALint isLoop);

    /**
     设置音量大小
     @param volume 音量值（0.0 —— 1.0）
     */
    void setVolume(ALfloat volume);

    /**
     设置播放速度
     @param speed 播放速度（1.0：正常速度）
     */
    void setSpeed(ALfloat speed);

    /**
     获取 OpenAL 已经处理（播放）完毕的缓冲个数
     @return 缓冲个数
     */
    ALint NumOfBuffProcessed();

    /**
     获取音源状态
     @return 音源状态值，参见‘SourceState’
     */
    ALSourceState SourceState();

    /**
     打开音频缓存进行播放
     @param buffer 需要播放的音频缓存数据
     @param length 缓存长度
     */
    void OpenAudio(ALubyte *buffer, ALuint length);

    /**
     开启播放
     */
    void PlaySound();

    /**
     暂停播放
     */
    void PauseSound();

    /**
     停止播放
     */
    void StopSound();

    /**
     清理 OpenAL
     */
    void close();

private:
    /**
     更新 OpenAL 缓存
     */
    void refreshQueueBuffer();

    /**
     检出 OpenAL 状态
     @return OpenAL 状态值
     */
    ALenum checkError();

private:
    static const int BUFF_NUM = 3;    // 缓冲个数
    static const int BUFF_SIZE = 4096;  // 缓冲大小

private:
    ALCdevice *mAudioDevice;               // 硬件，获取设备音频硬件资源
    ALCcontext *mAudioContext;              // 内容，给播放器提供上下文环境描述
    ALuint mSourceId;             // 音源，标识每一个音源
    ALuint m_buffers[BUFF_NUM];    // 缓存
    ALenum mFormat;               // 格式
    ALsizei mSampleRate;            // 采样频率
    ALubyte *m_initBuf;              // 缓冲区
    ALint m_fillBufLen;           // 初始化缓冲区长度
    ALint m_totalBufLen;          // 缓冲区总长度

};

#endif //XSOUNDER_H
