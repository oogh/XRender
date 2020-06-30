//
// Created by Oogh on 2020/3/19.
//

#ifndef ANDROIDDEMO_XRENDER_H
#define ANDROIDDEMO_XRENDER_H

#include <memory>
#include <string>
#include <mutex>
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
    
    void seekTo(long targetPos);
    
    void pause();
    
    void onSurfaceCreated();

    void onSurfaceChanged(int width, int height);

    void onDrawFrame();
    
    void stop();
    
private:
    void refreshWorkThread(void* opaque);

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
    
    long mTargetPos;
    
    std::unique_ptr<std::thread> mRefreshTid;
    
    bool mPauseReq;
    
    bool mAbortReq;
    
    std::mutex mMutex;
    std::condition_variable mContinueRefreshCond;
    
};
#endif //ANDROIDDEMO_XRENDER_H
