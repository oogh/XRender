//
// Created by Oogh on 2020/3/19.
//

#ifndef ANDROIDDEMO_LOGGER_H
#define ANDROIDDEMO_LOGGER_H

#define TAG "XMedia"

#if __ANDROID__
#include <android/log.h>
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)
#elif __APPLE__
#include <cstdio>
#define LOGI(...) printf(__VA_ARGS__)
#define LOGD(...) printf(__VA_ARGS__)
#define LOGE(...) printf(__VA_ARGS__)
#endif

#endif //ANDROIDDEMO_LOGGER_H
