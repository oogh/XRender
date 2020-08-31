//
// Created by Andy on 2020/8/14.
//

#ifndef XRENDER_XALHEADER_H
#define XRENDER_XALHEADER_H

#if __APPLE__
#include <OpenAL/OpenAL.h>
#elif __ANDROID__
#include <AL/al.h>
#include <AL/alc.h>
#endif

const char* alError2Str(ALenum error) {
    switch (error) {
        case AL_INVALID_NAME: return "AL_INVALID_NAME : Invalid Name parameter passed to AL call";
        case AL_INVALID_ENUM: return "AL_INVALID_ENUM : Invalid parameter passed to AL call";
        case AL_INVALID_VALUE: return "AL_INVALID_VALUE : Invalid enum parameter value";
        case AL_INVALID_OPERATION: return "AL_INVALID_OPERATION : Illegal call";
        case AL_OUT_OF_MEMORY: return "AL_OUT_OF_MEMORY : No mojo";
        default: return "Unknown error code";
    }
    return "";
}


#endif //XRENDER_XALHEADER_H
