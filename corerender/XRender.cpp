//
// Created by Oogh on 2020/3/19.
//

#include "XRender.hpp"
//#include "XTexture.hpp"
#include "XException.hpp"
#include "XImageUtil.hpp"
#include "XTriangle.hpp"
#include "XImage.hpp"
#include "XThreadUtils.hpp"
#include "XLogger.hpp"
#include "XTimeCounter.hpp"
#include <chrono>

XRender::XRender(): mTextureWidth(0), mTextureHeight(0), mTargetPos(0), mAbortReq(false), mPauseReq(true) {
    
}

XRender::~XRender() {
    if (mRefreshTid && mRefreshTid->joinable()) {
        mRefreshTid->join();
    }
    mRefreshTid.reset();
}

void XRender::setInput(const std::string& filename) {
#ifdef USE_FILE_PRODUCER
    mProducer = std::make_unique<XFileProducer>();
#endif

#ifdef USE_FFMPEG_PRODUCER
    mProducer = std::make_unique<XFFProducer>();
#endif

    mProducer->setProduceMode(PRODUCE_MODE_HARDWARE);
//    mProducer->setInput(filename);
}

void XRender::setOnProgressChangeCallback(OnProgressChangeCallback callback) {
    mProgressChangeCallback = callback;
}

void XRender::prepare(long timestamp) {
    mTargetPos = timestamp;
}

void XRender::start() {
//    mProducer->start();
    
    mPauseReq = false;
    if (!mRefreshTid) {
        mRefreshTid = std::make_unique<std::thread>([this] { refreshWorkThread(this); });
    } else {
        std::lock_guard<std::mutex> lock(mMutex);
        mContinueRefreshCond.notify_one();
    }
}

void XRender::seekTo(long targetPos) {
    mTargetPos = targetPos;
}

void XRender::pause() {
    std::lock_guard<std::mutex> lock(mMutex);
    mPauseReq = true;
}

void XRender::onSurfaceCreated() {
    
}

void XRender::onSurfaceChanged(int width, int height) {
    // 居中显示
    // glViewport(width / 4, height / 4, width / 2, height / 2);
    
    // 全画布显示
    glViewport(0, 0, width, height);
}

void XRender::onDrawFrame() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    if (!mTriangle) {
        mTriangle = std::make_unique<XTriangle>();
    }

    if (mTriangle) {
        mTriangle->draw();
    }
}

void XRender::refreshWorkThread(void* opaque) {
    XThreadUtils::configThreadName("refreshWorkThread");
    LOGD("[XRender] refreshWorkThread ++++\n");
    XRender* render = reinterpret_cast<XRender*>(opaque);
    if (!render) {
        return;
    }
    
    XTimeCounter peekImageCounter;
    for (;;) {
        if (render->mAbortReq) {
            break;
        }
        
        if (render->mPauseReq) {
            std::unique_lock<std::mutex> lock(render->mMutex);
            render->mContinueRefreshCond.wait(lock);
            continue;
        }
        
        if (render->mProducer) {
            peekImageCounter.markStart();
            auto image = render->mProducer->peekImage(render->mTargetPos);
            if (image && image->pixels[0]) {
                peekImageCounter.markEnd();
//                if (render->mTexture) {
//                    render->mTexture->update(image->pixels[0], render->mTextureWidth, render->mTextureHeight);
//                }
                if (image->pts > render->mTargetPos) {
                    render->mProducer->endCurrentImageUse();
                }
                
                if (mProgressChangeCallback) {
                    mProgressChangeCallback(render->mTargetPos, render->mProducer->getOriginalDuration());
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(33));
            mTargetPos += 33;
        }
    }
    LOGD("[XRender] refreshWorkThread ----\n");
}

void XRender::stop() {
    std::lock_guard<std::mutex> lock(mMutex);
    mAbortReq = true;
    mContinueRefreshCond.notify_one();
    
    if (mRefreshTid && mRefreshTid->joinable()) {
        mRefreshTid->join();
    }
}
