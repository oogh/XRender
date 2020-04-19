//
// Created by Oogh on 2020/3/19.
//

#include "XViewJNI.h"
#include <string>
#include <memory>
#include "XRender.h"

static const std::string CLASS_NAME = "com/core/render/XView";

jlong viewCreate(JNIEnv*, jobject) {
    std::shared_ptr<XRender>* render = new std::shared_ptr<XRender>(new XRender());
    return reinterpret_cast<jlong>(render);
}

void viewSurfaceCreated(JNIEnv*, jobject, jlong viewPtr) {
    std::shared_ptr<XRender>* render = reinterpret_cast<std::shared_ptr<XRender> *>(viewPtr);
    (*render)->onSurfaceCreated();
}

void viewSurfaceChanged(JNIEnv*, jobject, jlong viewPtr, jint width, jint height) {
    std::shared_ptr<XRender>* render = reinterpret_cast<std::shared_ptr<XRender> *>(viewPtr);
    (*render)->onSurfaceChanged(width, height);
}

void viewDrawFrame(JNIEnv*, jobject, jlong viewPtr) {
    std::shared_ptr<XRender>* render = reinterpret_cast<std::shared_ptr<XRender> *>(viewPtr);
    (*render)->onDrawFrame();
}

void viewDestroy(JNIEnv*, jobject, jlong viewPtr) {
    std::shared_ptr<XRender>* render = reinterpret_cast<std::shared_ptr<XRender> *>(viewPtr);
    delete render;
}

void viewSetInput(JNIEnv* env, jobject, jlong viewPtr, jstring filename) {
    std::shared_ptr<XRender>* render = reinterpret_cast<std::shared_ptr<XRender> *>(viewPtr);
    jboolean copy;
    const char* path = env->GetStringUTFChars(filename, &copy);
    (*render)->setInput(path);
    env->ReleaseStringUTFChars(filename, path);
}

void viewStart(JNIEnv*, jobject, jlong viewPtr) {
    std::shared_ptr<XRender>* render = reinterpret_cast<std::shared_ptr<XRender> *>(viewPtr);
    (*render)->start();
}

static JNINativeMethod gMethods[] = {
        {"nativeCreateView", "()J", reinterpret_cast<void*>(viewCreate)},
        {"nativeSurfaceCreated", "(J)V", reinterpret_cast<void*>(viewSurfaceCreated)},
        {"nativeSurfaceChanged", "(JII)V", reinterpret_cast<void*>(viewSurfaceChanged)},
        {"nativeDrawFrame", "(J)V", reinterpret_cast<void*>(viewDrawFrame)},
        {"nativeDestroyView", "(J)V", reinterpret_cast<void*>(viewDestroy)},
        {"nativeSetInput", "(JLjava/lang/String;)V", reinterpret_cast<void*>(viewSetInput)},
        {"nativeStart", "(J)V", reinterpret_cast<void*>(viewStart)},
};

int viewRegisterNativeMethods(JNIEnv* env) {
    jclass clazz = env->FindClass(CLASS_NAME.c_str());
    if (nullptr == clazz) {
        return -1;
    }
    if (env->RegisterNatives(clazz, gMethods, sizeof(gMethods) / sizeof(gMethods[0])) < 0) {
        return JNI_FALSE;
    }
    return JNI_TRUE;
}


