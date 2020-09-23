//
// Created by Oogh on 2020/3/19.
//

#ifndef ANDROIDDEMO_XGLHEADER_H
#define ANDROIDDEMO_XGLHEADER_H

#include "XPlatform.hpp"

#if PLATFORM_ANDROID
    #include <GLES2/gl2.h>
#elif PLATFORM_IOS
    #import <OpenGLES/ES2/gl.h>
#endif

#endif //ANDROIDDEMO_XGLHEADER_H
