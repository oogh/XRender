//
// Created by Oogh on 2020/3/19.
//

#ifndef ANDROIDDEMO_XRENDER_H
#define ANDROIDDEMO_XRENDER_H

#include "XGLHeader.h"
#include <memory>
#include <string>
#include "XFileProducer.h"

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

    std::unique_ptr<XFileProducer> mProducer;
};
#endif //ANDROIDDEMO_XRENDER_H
