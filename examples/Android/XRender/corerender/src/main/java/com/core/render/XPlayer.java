package com.core.render;

public class XPlayer {
    private long mNativePtr = -1L;
    private XTimeline mTimeline;
    private XMacView mSurface;

    public XPlayer() {
        mNativePtr = nativeCreatePlayer();
    }

    public void setSurface(XMacView surface) {
        mSurface = surface;
        if (mNativePtr != -1L) {
            nativeSetSurface(mNativePtr, surface.getNativePtr());
        }
    }

    public void setTimeline(XTimeline timeline) {
        mTimeline = timeline;
        if (mNativePtr != -1L) {
            nativeSetTimeline(mNativePtr, timeline.getNativePtr());
        }
    }

    public void prepare() {
        if (mNativePtr != -1L) {
            nativePlayerPrepare(mNativePtr);
        }
    }

    public void start() {
        if (mNativePtr != -1L) {
            nativePlayerStart(mNativePtr);
        }
    }

    public void destroy() {
        if (mNativePtr != -1L) {
            nativeDestroyPlayer(mNativePtr);
        }
    }

    private native long nativeCreatePlayer();
    private native void nativeSetSurface(long nativePtr, long surfacePtr);
    private native void nativeSetTimeline(long nativePtr, long timelinePtr);
    private native void nativePlayerPrepare(long nativePtr);
    private native void nativePlayerStart(long nativePtr);
    private native void nativeDestroyPlayer(long nativePtr);
}
