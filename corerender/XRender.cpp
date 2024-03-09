//
// Created by Oogh on 2020/3/19.
//

#include <chrono>
#include "XRender.hpp"
#include "XException.hpp"
#include "XImageUtil.hpp"
#include "XImage.hpp"
#include "XThreadUtils.hpp"
#include "XLogger.hpp"
#include "XTimeCounter.hpp"
#include "XFFProducer.hpp"
#include "XTexture.hpp"

XRender::XRender(): mTextureWidth(0), mTextureHeight(0), mTargetPos(0), mAbortReq(false), mPauseReq(true) {
    
}

XRender::~XRender() {
    if (mRefreshTid && mRefreshTid->joinable()) {
        mRefreshTid->join();
    }
    mRefreshTid.reset();
}

void XRender::setInput(std::string path) {

}

void XRender::prepare(int64_t timestamp) {

}

void XRender::start() {

}

void XRender::pause() {

}

void XRender::updatePixel(uint8_t* pixel, int width, int height) {
    std::lock_guard<std::mutex> lock(mMutex);
    if (mTexture) {
        mTexture->setPixels(pixel, width, height);
    }
    mContinueRefreshCond.notify_one();
}

void XRender::onSurfaceCreated() {
    mTexture = std::make_unique<XTexture>();
}

void XRender::onSurfaceChanged(int width, int height) {
    // 居中显示
    // glViewport(width / 4, height / 4, width / 2, height / 2);
    
    // 全画布显示
    glViewport(0, 0, width, height);
}

void XRender::onDrawFrame() {
    mTexture->draw();
}

void XRender::stop() {
    std::lock_guard<std::mutex> lock(mMutex);
    mAbortReq = true;
    mContinueRefreshCond.notify_one();
    
    if (mRefreshTid && mRefreshTid->joinable()) {
        mRefreshTid->join();
    }
}
