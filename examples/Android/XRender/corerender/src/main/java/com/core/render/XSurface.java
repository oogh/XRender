package com.core.render;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class XMacView extends GLSurfaceView implements GLSurfaceView.Renderer {

    private long mNativePtr = -1L;

    static {
        System.loadLibrary("corerender");
    }

    public XMacView(Context context) {
        super(context);
        init();
    }

    public XMacView(Context context, AttributeSet attrs) {
        super(context, attrs);
        init();
    }

    private void init() {
        mNativePtr = nativeCreateSurface();
        setEGLContextClientVersion(3);
        setRenderer(this);
    }

    public long getNativePtr() {
        return mNativePtr;
    }

    public void setInput(String filename) {
        if (mNativePtr != -1L) {
            nativeSetInput(mNativePtr, filename);
        }
    }

    public void prepare(long timestamp) {
        if (mNativePtr != -1L) {
            nativePrepare(mNativePtr, timestamp);
        }
    }

    public void start() {
        if (mNativePtr != -1L) {
            nativeStart(mNativePtr);
        }
    }

    public void pause() {
        if (mNativePtr != -1L) {
            nativePause(mNativePtr);
        }
    }

    public void stop() {
        if (mNativePtr != -1L) {
            nativeStop(mNativePtr);
        }
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
            nativeDestroySurface(mNativePtr);
            mNativePtr = -1L;
        }
    }

    private native long nativeCreateSurface();

    private native void nativeSurfaceCreated(long nativePtr);

    private native void nativeSurfaceChanged(long nativePtr, int width, int height);

    private native void nativeDrawFrame(long nativePtr);

    private native void nativeDestroySurface(long nativePtr);

    private native void nativeSetInput(long nativePtr, String filename);

    private native void nativePrepare(long nativePtr, long timestamp);

    private native void nativeStart(long nativePtr);

    private native void nativePause(long nativePtr);

    private native void nativeStop(long nativePtr);
}
