//
// Created by Oogh on 2020/3/19.
//

#include "XRender.h"
#include "XTexture.h"
#include "XException.h"
#include "XImageUtil.h"
#include "XImage.h"

XRender::XRender(): mTextureWidth(720), mTextureHeight(1280) {
    
}

XRender::~XRender() {
}

void XRender::setInput(const std::string& filename) {
#ifdef USE_FILE_PRODUCER
    mProducer = std::make_unique<XFileProducer>();
#endif

#ifdef USE_FFMPEG_PRODUCER
    mProducer = std::make_unique<XFFProducer>();
#endif

    mProducer->setInput(filename);
}

void XRender::start() {
    mProducer->start();

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

    if (mProducer) {
        auto image = mProducer->getImage(0);
        if (image && image->pixels[0]) {
            mTexture->update(image->pixels[0], mTextureWidth, mTextureHeight);
        }
    }

    
    if (mTexture) {
        mTexture->draw();
    }
}
