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
class XRectangle;

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

    std::unique_ptr<XRectangle> mRectangle;
    
};
#endif //ANDROIDDEMO_XRENDER_H