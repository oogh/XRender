//
// Created by Oogh on 2020/11/10.
//

#include <string>
#include <memory>
#include "XTrackJNI.hpp"
#include "XTrack.hpp"

static const std::string CLASS_NAME = "com/core/render/XTrack";

jlong trackCreate(JNIEnv*, jobject) {
    std::shared_ptr<XTrack>* track = new std::shared_ptr<XTrack>(new XTrack());
    return reinterpret_cast<jlong>(track);
}

void trackSetFilename(JNIEnv* env, jobject, jlong trackPtr, jstring filename) {
    std::shared_ptr<XTrack>* track = reinterpret_cast<std::shared_ptr<XTrack> *>(trackPtr);
    jboolean copy;
    const char* path = env->GetStringUTFChars(filename, &copy);
    (*track)->setFilename(path);
    env->ReleaseStringUTFChars(filename, path);
}

void trackSetDelay(JNIEnv*, jobject, jlong trackPtr, jlong delay) {
    std::shared_ptr<XTrack>* track = reinterpret_cast<std::shared_ptr<XTrack> *>(trackPtr);
    (*track)->setDelay(static_cast<long>(delay));
}

void trackSetClipStartTime(JNIEnv*, jobject, jlong trackPtr, jlong clipStartTime) {
    std::shared_ptr<XTrack>* track = reinterpret_cast<std::shared_ptr<XTrack> *>(trackPtr);
    (*track)->setClipStartTime(static_cast<long>(clipStartTime));
}

void trackSetClipEndTime(JNIEnv*, jobject, jlong trackPtr, jlong clipEndTime) {
    std::shared_ptr<XTrack>* track = reinterpret_cast<std::shared_ptr<XTrack> *>(trackPtr);
    (*track)->setClipEndTime(clipEndTime);
}

jint trackGetId(JNIEnv*, jobject, jlong trackPtr) {
    std::shared_ptr<XTrack>* track = reinterpret_cast<std::shared_ptr<XTrack> *>(trackPtr);
    return static_cast<jint>((*track)->getId());
}

void trackDestroy(JNIEnv*, jobject, jlong trackPtr) {
    std::shared_ptr<XTrack>* track = reinterpret_cast<std::shared_ptr<XTrack> *>(trackPtr);
    delete track;
}

static JNINativeMethod gMethods[] = {
        {"nativeCreateTrack", "()J", reinterpret_cast<void*>(trackCreate)},
        {"nativeSetFilename", "(JLjava/lang/String;)V", reinterpret_cast<void*>(trackSetFilename)},
        {"nativeSetDelay", "(JJ)V", reinterpret_cast<void*>(trackSetDelay)},
        {"nativeSetClipStartTime", "(JJ)V", reinterpret_cast<void*>(trackSetClipStartTime)},
        {"nativeSetClipEndTime", "(JJ)V", reinterpret_cast<void*>(trackSetClipEndTime)},
        {"nativeGetTrackId", "(J)I", reinterpret_cast<void*>(trackGetId)},
        {"nativeDestroyTrack", "(J)V", reinterpret_cast<void*>(trackDestroy)},
};

int trackRegisterNativeMethods(JNIEnv* env) {
    jclass clazz = env->FindClass(CLASS_NAME.c_str());
    if (nullptr == clazz) {
        return -1;
    }
    if (env->RegisterNatives(clazz, gMethods, sizeof(gMethods) / sizeof(gMethods[0])) < 0) {
        return JNI_FALSE;
    }
    return JNI_TRUE;
}