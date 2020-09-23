//
// Created by Andy on 2020/8/14.
//

#ifndef XRENDER_XALHEADER_HPP
#define XRENDER_XALHEADER_HPP

#if __APPLE__
#include <OpenAL/OpenAL.h>
#elif __ANDROID__
#include <AL/al.h>
#include <AL/alc.h>
#endif

extern const char* alError2Str(ALenum error);


#endif //XRENDER_XALHEADER_HPP
