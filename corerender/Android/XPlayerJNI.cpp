//
// Created by Andy on 2020/11/11.
//

#include <string>
#include <memory>
#include "XPlayerJNI.hpp"
#include "XPlayer.hpp"
#include "XTimeline.hpp"
#include "XRender.hpp"

static const std::string CLASS_NAME = "com/core/render/XPlayer";

jlong playerCreate(JNIEnv*, jobject) {
    auto* player = new std::shared_ptr<XPlayer>(new XPlayer());
    return reinterpret_cast<jlong>(player);
}

void playerSetSurface(JNIEnv*, jobject, jlong playerPtr, jlong surfacePtr) {
    auto* player = reinterpret_cast<std::shared_ptr<XPlayer> *>(playerPtr);
    auto* render = reinterpret_cast<std::shared_ptr<XRender> *>(surfacePtr);
    (*player)->setRender(*render);
}

void playerSetTimeline(JNIEnv* env, jobject, jlong playerPtr, jlong timelinePtr) {
    auto* player = reinterpret_cast<std::shared_ptr<XPlayer> *>(playerPtr);
    auto* timeline = reinterpret_cast<std::shared_ptr<XTimeline> *>(timelinePtr);
    (*player)->setTimeline(*timeline);
}

void playerPrepare(JNIEnv*, jobject, jlong playerPtr) {
    auto* player = reinterpret_cast<std::shared_ptr<XPlayer> *>(playerPtr);
    (*player)->prepare();
}

void playerStart(JNIEnv*, jobject, jlong playerPtr) {
    auto* player = reinterpret_cast<std::shared_ptr<XPlayer> *>(playerPtr);
    (*player)->start();
}

void playerDestroy(JNIEnv*, jobject, jlong playerPtr) {
    auto* player = reinterpret_cast<std::shared_ptr<XPlayer> *>(playerPtr);
    delete player;
}

static JNINativeMethod gMethods[] = {
        {"nativeCreatePlayer", "()J", reinterpret_cast<void*>(playerCreate)},
        {"nativeSetSurface", "(JJ)V", reinterpret_cast<void*>(playerSetSurface)},
        {"nativeSetTimeline", "(JJ)V", reinterpret_cast<void*>(playerSetTimeline)},
        {"nativePlayerPrepare", "(J)V", reinterpret_cast<void*>(playerPrepare)},
        {"nativePlayerStart", "(J)V", reinterpret_cast<void*>(playerStart)},
        {"nativeDestroyPlayer", "(J)V", reinterpret_cast<void*>(playerDestroy)},
};

int playerRegisterNativeMethods(JNIEnv* env) {
    jclass clazz = env->FindClass(CLASS_NAME.c_str());
    if (nullptr == clazz) {
        return -1;
    }
    if (env->RegisterNatives(clazz, gMethods, sizeof(gMethods) / sizeof(gMethods[0])) < 0) {
        return JNI_FALSE;
    }
    return JNI_TRUE;
}
