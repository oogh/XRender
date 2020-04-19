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
    if (mInput) {
        fclose(mInput);
        mInput = nullptr;
    }
}

void XRender::setInput(const std::string& filename) {
    mInput = fopen(filename.data(), "rb");
    if (!mInput) {
        throw XException("[XViewer] failed to open file");
    }
}

void XRender::start() {
    uint8_t* buf = new uint8_t[mTextureWidth * mTextureHeight * 4] { 0 };
    fread(buf, 1, mTextureWidth * mTextureHeight * 4, mInput);
    mTexture->update(buf, mTextureWidth, mTextureHeight);

    delete[] buf;

    fclose(mInput);
    mInput = nullptr;
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
