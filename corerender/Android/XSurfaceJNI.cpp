//
// Created by Oogh on 2020/3/19.
//

#include "XSurfaceJNI.hpp"
#include <string>
#include <memory>
#include "XRender.hpp"

static const std::string CLASS_NAME = "com/core/render/XSurface";

jlong surfaceCreate(JNIEnv*, jobject) {
    std::shared_ptr<XRender>* render = new std::shared_ptr<XRender>(new XRender());
    return reinterpret_cast<jlong>(render);
}

void surfaceSurfaceCreated(JNIEnv*, jobject, jlong surfacePtr) {
    std::shared_ptr<XRender>* render = reinterpret_cast<std::shared_ptr<XRender> *>(surfacePtr);
    (*render)->onSurfaceCreated();
}

void surfaceSurfaceChanged(JNIEnv*, jobject, jlong surfacePtr, jint width, jint height) {
    std::shared_ptr<XRender>* render = reinterpret_cast<std::shared_ptr<XRender> *>(surfacePtr);
    (*render)->onSurfaceChanged(width, height);
}

void surfaceDrawFrame(JNIEnv*, jobject, jlong surfacePtr) {
    std::shared_ptr<XRender>* render = reinterpret_cast<std::shared_ptr<XRender> *>(surfacePtr);
    (*render)->onDrawFrame();
}

void surfaceDestroy(JNIEnv*, jobject, jlong surfacePtr) {
    std::shared_ptr<XRender>* render = reinterpret_cast<std::shared_ptr<XRender> *>(surfacePtr);
    delete render;
}

void surfaceSetInput(JNIEnv* env, jobject, jlong surfacePtr, jstring filename) {
    std::shared_ptr<XRender>* render = reinterpret_cast<std::shared_ptr<XRender> *>(surfacePtr);
    jboolean copy;
    const char* path = env->GetStringUTFChars(filename, &copy);
//    (*render)->setInput(path);
    env->ReleaseStringUTFChars(filename, path);
}

void surfacePrepare(JNIEnv*, jobject, jlong surfacePtr, jlong timestamp) {
    std::shared_ptr<XRender>* render = reinterpret_cast<std::shared_ptr<XRender> *>(surfacePtr);
//    (*render)->prepare(timestamp);
}

void surfaceStart(JNIEnv*, jobject, jlong surfacePtr) {
    std::shared_ptr<XRender>* render = reinterpret_cast<std::shared_ptr<XRender> *>(surfacePtr);
//    (*render)->start();
}

void surfacePause(JNIEnv*, jobject, jlong surfacePtr) {
    std::shared_ptr<XRender>* render = reinterpret_cast<std::shared_ptr<XRender> *>(surfacePtr);
//    (*render)->pause();
}

void surfaceStop(JNIEnv*, jobject, jlong surfacePtr) {
    std::shared_ptr<XRender>* render = reinterpret_cast<std::shared_ptr<XRender> *>(surfacePtr);
//    (*render)->stop();
}

static JNINativeMethod gMethods[] = {
        {"nativeCreateSurface", "()J", reinterpret_cast<void*>(surfaceCreate)},
        {"nativeSurfaceCreated", "(J)V", reinterpret_cast<void*>(surfaceSurfaceCreated)},
        {"nativeSurfaceChanged", "(JII)V", reinterpret_cast<void*>(surfaceSurfaceChanged)},
        {"nativeDrawFrame", "(J)V", reinterpret_cast<void*>(surfaceDrawFrame)},
        {"nativeDestroySurface", "(J)V", reinterpret_cast<void*>(surfaceDestroy)},
        {"nativeSetInput", "(JLjava/lang/String;)V", reinterpret_cast<void*>(surfaceSetInput)},
        {"nativePrepare", "(JJ)V", reinterpret_cast<void*>(surfacePrepare)},
        {"nativeStart", "(J)V", reinterpret_cast<void*>(surfaceStart)},
        {"nativePause", "(J)V", reinterpret_cast<void*>(surfacePause)},
        {"nativeStop", "(J)V", reinterpret_cast<void*>(surfaceStop)},
};

int surfaceRegisterNativeMethods(JNIEnv* env) {
    jclass clazz = env->FindClass(CLASS_NAME.c_str());
    if (nullptr == clazz) {
        return -1;
    }
    if (env->RegisterNatives(clazz, gMethods, sizeof(gMethods) / sizeof(gMethods[0])) < 0) {
        return JNI_FALSE;
    }
    return JNI_TRUE;
}


