package com.core.render;

public class XTrack {
    private long mNativePtr = -1L;

    private XTimeline mTimeline;
    private int mId;
    private String mFilename;
    private long mDelay;
    private long mClipStartTime;
    private long mClipEndTime;

    public XTrack() {
        mNativePtr = nativeCreateTrack();
        if (mNativePtr != -1L) {
            mId = nativeGetTrackId(mNativePtr);
        }
    }

    public long getNativePtr() {
        return mNativePtr;
    }

    protected void setTimeline(XTimeline timeline) {
        mTimeline = timeline;
    }

    public void setFilename(String filename) {
        mFilename = filename;
    }

    public void setDelay(long delay) {
        mDelay = delay;
    }

    public void setClipStartTime(long clipStartTime) {
        mClipStartTime = clipStartTime;
    }

    public void setClipEndTime(long clipEndTime) {
        mClipEndTime = clipEndTime;
    }

    public int getId() {
        return mId;
    }

    public XTimeline getTimeline() {
        return mTimeline;
    }

    public String getFilename() {
        return mFilename;
    }

    public long getDelay() {
        return mDelay;
    }

    public long getClipStartTime() {
        return mClipStartTime;
    }

    public long getClipEndTime() {
        return mClipEndTime;
    }

    public void destroy() {
        if (mNativePtr != -1L) {
            nativeDestroyTrack(mNativePtr);
            mNativePtr = -1L;
        }
    }

    private native long nativeCreateTrack();
    private native void nativeSetFilename(long nativePtr, String filename);
    private native void nativeSetDelay(long nativePtr, long delay);
    private native void nativeSetClipStartTime(long nativePtr, long clipStartTime);
    private native void nativeSetClipEndTime(long nativePtr, long clipEndTime);
    private native int nativeGetTrackId(long nativePtr);
    private native void nativeDestroyTrack(long nativePtr);

}
