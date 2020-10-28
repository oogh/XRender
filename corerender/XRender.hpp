//
// Created by Oogh on 2020/3/19.
//

#ifndef ANDROIDDEMO_XRENDER_H
#define ANDROIDDEMO_XRENDER_H

#include <memory>
#include <string>
#include <mutex>
#include "XGLHeader.hpp"

//#define USE_FILE_PRODUCER
#define USE_FFMPEG_PRODUCER

#ifdef USE_FILE_PRODUCER
#include "XFileProducer.h"
#endif

#ifdef USE_FFMPEG_PRODUCER
#include "XFFProducer.hpp"
#endif

//class XTexture;

class XTriangle;

class XRender {
    using OnProgressChangeCallback = std::function<void(long current, long duration)>;
public:
    XRender();

    ~XRender();
    
    void setOnProgressChangeCallback(OnProgressChangeCallback callback = nullptr);
    
    void setInput(const std::string& filename);

    void prepare(long timestamp);

    void start();
    
    void seekTo(long targetPos);
    
    void pause();
    
    void onSurfaceCreated();

    void onSurfaceChanged(int width, int height);

    void onDrawFrame();
    
    void stop();
    
private:
    void refreshWorkThread(void* opaque);

private:
//    std::unique_ptr<XTexture> mTexture;
    std::unique_ptr<XTriangle> mTriangle;

    int mTextureWidth;
    int mTextureHeight;

#ifdef USE_FILE_PRODUCER
    std::unique_ptr<XFileProducer> mProducer;
#endif

#ifdef USE_FFMPEG_PRODUCER
    std::unique_ptr<XFFProducer> mProducer;
#endif
    
    long mTargetPos;
    
    std::unique_ptr<std::thread> mRefreshTid;
    
    bool mPauseReq;
    
    bool mAbortReq;
    
    std::mutex mMutex;
    std::condition_variable mContinueRefreshCond;
    
    OnProgressChangeCallback mProgressChangeCallback;
    
};
#endif //ANDROIDDEMO_XRENDER_H