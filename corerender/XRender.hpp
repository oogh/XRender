//
// Created by Oogh on 2020/3/19.
//

#ifndef ANDROIDDEMO_XRENDER_H
#define ANDROIDDEMO_XRENDER_H

#include <memory>
#include <string>
#include <mutex>
#include "XGLHeader.hpp"

class XFFProducer;
class XTexture;

class XRender {
    using OnProgressChangeCallback = std::function<void(long current, long duration)>;
public:
    XRender();

    ~XRender();
    
    void start();

    void updatePixel(uint8_t* pixel, int width, int height);
    
    void onSurfaceCreated();

    void onSurfaceChanged(int width, int height);

    void onDrawFrame();
    
    void stop();

private:
    int mTextureWidth;
    int mTextureHeight;

    std::unique_ptr<XFFProducer> mProducer;

    long mTargetPos;
    
    std::unique_ptr<std::thread> mRefreshTid;
    
    bool mPauseReq;
    
    bool mAbortReq;
    
    std::mutex mMutex;
    std::condition_variable mContinueRefreshCond;
    
    OnProgressChangeCallback mProgressChangeCallback;

    std::unique_ptr<XTexture> mTexture;
    
};
#endif //ANDROIDDEMO_XRENDER_H