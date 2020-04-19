package com.core.render;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class XView extends GLSurfaceView implements GLSurfaceView.Renderer {

    private long mNativePtr = -1L;

    static {
        System.loadLibrary("corerender");
    }

    public XView(Context context) {
        super(context);
        init();
    }

    public XView(Context context, AttributeSet attrs) {
        super(context, attrs);
        init();
    }

    private void init() {
        mNativePtr = nativeCreateView();
        setEGLContextClientVersion(2);
        setRenderer(this);
    }

    public void setInput(String filename) {
        nativeSetInput(mNativePtr, filename);
    }

    public void start() {
        nativeStart(mNativePtr);
    }

    @Override
    public void onSurfaceCreated(GL10 gl10, EGLConfig eglConfig) {
        if (mNativePtr != -1L) {
            nativeSurfaceCreated(mNativePtr);
        }
    }

    @Override
    public void onSurfaceChanged(GL10 gl10, int width, int height) {
        if (mNativePtr != -1L) {
            nativeSurfaceChanged(mNativePtr, width, height);
        }
    }

    @Override
    public void onDrawFrame(GL10 gl10) {
        if (mNativePtr != -1L) {
            nativeDrawFrame(mNativePtr);
        }
    }

    public void destroy() {
        if (mNativePtr != -1L) {
            nativeDestroyView(mNativePtr);
            mNativePtr = -1L;
        }
    }

    private native long nativeCreateView();

    private native void nativeSurfaceCreated(long nativePtr);

    private native void nativeSurfaceChanged(long nativePtr, int width, int height);

    private native void nativeDrawFrame(long nativePtr);

    private native void nativeDestroyView(long nativePtr);

    private native void nativeSetInput(long nativePtr, String filename);

    private native void nativeStart(long nativePtr);
}
