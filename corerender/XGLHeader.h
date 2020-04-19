//
// Created by Oogh on 2020/3/19.
//

#ifndef ANDROIDDEMO_XGLHEADER_H
#define ANDROIDDEMO_XGLHEADER_H

#if __ANDROID__
    #include <GLES2/gl2.h>
#elif __APPLE__
    #import <OpenGLES/ES2/gl.h>
#endif

#endif //ANDROIDDEMO_XGLHEADER_H
