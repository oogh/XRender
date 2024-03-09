//
// Created by Oogh on 2020/11/10.
//

#include <string>
#include <memory>
#include "XTimelineJNI.hpp"
#include "XTimeline.hpp"

static const std::string CLASS_NAME = "com/core/render/XTimeline";

jlong timelineCreate(JNIEnv*, jobject) {
    auto* timeline = new std::shared_ptr<XTimeline>(new XTimeline());
    return reinterpret_cast<jlong>(timeline);
}

jint timelineAddTrack(JNIEnv* env, jobject, jlong timelinePtr, jlong trackPtr) {
    auto* timeline = reinterpret_cast<std::shared_ptr<XTimeline> *>(timelinePtr);
    auto* track = reinterpret_cast<std::shared_ptr<XTrack> *>(trackPtr);
    return static_cast<jint>((*timeline)->addTrack(*track));
}

jint timelineRemoveTrack(JNIEnv*, jobject, jlong timelinePtr, jlong trackPtr) {
    auto* timeline = reinterpret_cast<std::shared_ptr<XTimeline> *>(timelinePtr);
    auto* track = reinterpret_cast<std::shared_ptr<XTrack> *>(trackPtr);
    return static_cast<jint>((*timeline)->removeTrack((*track)->getId()));
}

void timelineDestroy(JNIEnv*, jobject, jlong timelinePtr) {
    auto* timeline = reinterpret_cast<std::shared_ptr<XTimeline> *>(timelinePtr);
    delete timeline;
}

static JNINativeMethod gMethods[] = {
        {"nativeCreateTimeline", "()J", reinterpret_cast<void*>(timelineCreate)},
        {"nativeAddTrack", "(JJ)I", reinterpret_cast<void*>(timelineAddTrack)},
        {"nativeRemoveTrack", "(JJ)I", reinterpret_cast<void*>(timelineRemoveTrack)},
        {"nativeDestroyTimeline", "(J)V", reinterpret_cast<void*>(timelineDestroy)},
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