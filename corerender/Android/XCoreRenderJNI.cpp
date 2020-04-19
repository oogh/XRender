//
// Created by Oogh on 2020/3/19.
//

#include "XCoreRenderJNI.h"
#include "XViewJNI.h"

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env = NULL;
    jint result = -1;
    if (vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6) != JNI_OK) {
        return -1;
    }

    JavaVM *javaVm;
    env->GetJavaVM(&javaVm);

    if (!viewRegisterNativeMethods(env)) {
        return -1;
    }

    result = JNI_VERSION_1_6;

    return result;
}

JNIEXPORT void JNICALL JNI_OnUnload(JavaVM *vm, void *reserved) {

}

