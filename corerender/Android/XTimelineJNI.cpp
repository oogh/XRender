//
// Created by Oogh on 2020/11/10.
//

#include "XTimelineJNI.hpp"

#include <string>
#include <memory>
#include "XTimeline.hpp"

static const std::string CLASS_NAME = "com/core/render/XTimeline";

jlong timelineCreate(JNIEnv*, jobject) {
    std::shared_ptr<XTimeline>* timeline = new std::shared_ptr<XTimeline>(new XTimeline());
    return reinterpret_cast<jlong>(timeline);
}

void timelineDestroy(JNIEnv*, jobject, jlong timelinePtr) {
    std::shared_ptr<XTimeline>* timeline = reinterpret_cast<std::shared_ptr<XTimeline> *>(timelinePtr);
    delete timeline;
}

void timelineSetInput(JNIEnv* env, jobject, jlong timelinePtr, jstring filename) {
    std::shared_ptr<XTimeline>* timeline = reinterpret_cast<std::shared_ptr<XTimeline> *>(timelinePtr);
    jboolean copy;
    const char* path = env->GetStringUTFChars(filename, &copy);
//    (*timeline)->setInput(path);
    env->ReleaseStringUTFChars(filename, path);
}

void timelinePrepare(JNIEnv*, jobject, jlong timelinePtr, jlong timestamp) {
    std::shared_ptr<XTimeline>* timeline = reinterpret_cast<std::shared_ptr<XTimeline> *>(timelinePtr);
//    (*timeline)->prepare(timestamp);
}

void timelineStart(JNIEnv*, jobject, jlong timelinePtr) {
    std::shared_ptr<XTimeline>* timeline = reinterpret_cast<std::shared_ptr<XTimeline> *>(timelinePtr);
//    (*timeline)->start();
}

void timelinePause(JNIEnv*, jobject, jlong timelinePtr) {
    std::shared_ptr<XTimeline>* timeline = reinterpret_cast<std::shared_ptr<XTimeline> *>(timelinePtr);
//    (*timeline)->pause();
}

void timelineStop(JNIEnv*, jobject, jlong timelinePtr) {
    std::shared_ptr<XTimeline>* timeline = reinterpret_cast<std::shared_ptr<XTimeline> *>(timelinePtr);
//    (*timeline)->stop();
}

static JNINativeMethod gMethods[] = {
        {"nativeCreateTimeline", "()J", reinterpret_cast<void*>(timelineCreate)},
        {"nativeDestroyTimeline", "(J)V", reinterpret_cast<void*>(timelineDestroy)},
        {"nativeSetInput", "(JLjava/lang/String;)V", reinterpret_cast<void*>(timelineSetInput)},
        {"nativePrepare", "(JJ)V", reinterpret_cast<void*>(timelinePrepare)},
        {"nativeStart", "(J)V", reinterpret_cast<void*>(timelineStart)},
        {"nativePause", "(J)V", reinterpret_cast<void*>(timelinePause)},
        {"nativeStop", "(J)V", reinterpret_cast<void*>(timelineStop)},
};

int timelineRegisterNativeMethods(JNIEnv* env) {
    jclass clazz = env->FindClass(CLASS_NAME.c_str());
    if (nullptr == clazz) {
        return -1;
    }
    if (env->RegisterNatives(clazz, gMethods, sizeof(gMethods) / sizeof(gMethods[0])) < 0) {
        return JNI_FALSE;
    }
    return JNI_TRUE;
}