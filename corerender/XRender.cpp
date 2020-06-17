//
// Created by Oogh on 2020/3/19.
//

#include "XRender.h"
#include "XTexture.h"
#include "XException.h"
#include "XImageUtil.h"

XRender::XRender(): mTextureWidth(720), mTextureHeight(1280) {
    
}

XRender::~XRender() {
}

void XRender::setInput(const std::string& filename) {
    mProducer = std::make_unique<XFileProducer>();
    mProducer->setInput(filename);
}

void XRender::start() {
    auto image = mProducer->getImage(0);
    mTexture->update(image->pixels, mTextureWidth, mTextureHeight);
}

void XRender::onSurfaceCreated() {
    
}

void XRender::onSurfaceChanged(int width, int height) {
    // 居中显示
    glViewport(width / 4, height / 4, width / 2, height / 2);
}

void XRender::onDrawFrame() {

    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    if (!mTexture) {
        mTexture = std::make_unique<XTexture>(mTextureWidth, mTextureHeight);
    }
    
    if (mTexture) {
        mTexture->draw();
    }
}
