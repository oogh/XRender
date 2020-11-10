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

    public int addTrack(XTrack track) {
        track.setTimeline(this);
        mTrackList.add(track);
        return mTrackList.size();
    }

    public int removeTrack(XTrack track) {
        mTrackList.remove(track);
        return mTrackList.size();
    }

    private native long nativeCreateTimeline();
}
