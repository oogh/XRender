//
// Created by Oogh on 2020/3/19.
//

#ifndef ANDROIDDEMO_XRENDER_H
#define ANDROIDDEMO_XRENDER_H

#include <memory>
#include <string>
#include "XGLHeader.h"

//#define USE_FILE_PRODUCER
#define USE_FFMPEG_PRODUCER

#ifdef USE_FILE_PRODUCER
#include "XFileProducer.h"
#endif

#ifdef USE_FFMPEG_PRODUCER
#include "XFFProducer.h"
#endif

class XTexture;

class XRender {
public:
    XRender();

    ~XRender();
    
    void setInput(const std::string& filename);

    void start();

    void onSurfaceCreated();

    void onSurfaceChanged(int width, int height);

    void onDrawFrame();

private:
    std::unique_ptr<XTexture> mTexture;

    int mTextureWidth;
    int mTextureHeight;

#ifdef USE_FILE_PRODUCER
    std::unique_ptr<XFileProducer> mProducer;
#endif

#ifdef USE_FFMPEG_PRODUCER
    std::unique_ptr<XFFProducer> mProducer;
#endif
    
};
#endif //ANDROIDDEMO_XRENDER_H
