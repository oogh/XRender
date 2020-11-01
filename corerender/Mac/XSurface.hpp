//
// Created by Andy on 2020/10/20.
//

#ifndef XRENDER_XSURFACE_HPP
#define XRENDER_XSURFACE_HPP

#include <memory>
#include "XGLHeader.hpp"

class XRectangle;

class XSurface {
public:
    XSurface(int width, int height);

    virtual ~XSurface();

protected:
    virtual void onSurfaceCreated(GLFWwindow* window);

    virtual void onSurfaceSizeChanged(GLFWwindow* window, int width, int height);

    virtual void onDrawFrame(GLFWwindow* window);

private:
    void init();

    void deinit();

private:
    GLFWwindow* mWindow;

    int mWidth;
    int mHeight;

    std::unique_ptr<XRectangle> mRectangle;

};


#endif //XRENDER_XSURFACE_HPP
