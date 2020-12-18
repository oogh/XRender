package com.core.render;

import java.util.ArrayList;
import java.util.List;

public class XTimeline {
    private long mNativePtr = -1L;

    private List<XTrack> mTrackList;

    public XTimeline() {
        mNativePtr = nativeCreateTimeline();
        mTrackList = new ArrayList<>();
    }

    public long getNativePtr() {
        return mNativePtr;
    }

    public int addTrack(XTrack track) {
        track.setTimeline(this);
        mTrackList.add(track);
        nativeAddTrack(mNativePtr, track.getNativePtr());
        return mTrackList.size();
    }

    public int removeTrack(XTrack track) {
        mTrackList.remove(track);
        nativeRemoveTrack(mNativePtr, track.getNativePtr());
        return mTrackList.size();
    }

    private native long nativeCreateTimeline();
    private native int nativeAddTrack(long nativePtr, long trackPtr);
    private native int nativeRemoveTrack(long nativePtr, long trackPtr);
    private native void nativeDestroyTimeline(long nativePtr);
}
