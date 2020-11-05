//
// Created by Oogh on 2020/10/20.
//

#ifndef XRENDER_XSURFACE_HPP
#define XRENDER_XSURFACE_HPP

#include <memory>
#include "XGLHeader.hpp"

class XRender;
class XPlayer;

class XSurface {
public:
    XSurface(int width, int height);

    ~XSurface();

    void setPlayer(std::shared_ptr<XPlayer> player);

    void create();

protected:
    void onSurfaceCreated(GLFWwindow* window);

    void onSurfaceSizeChanged(GLFWwindow* window, int width, int height);

    void onDrawFrame(GLFWwindow* window);

private:
    int mWidth;
    int mHeight;

    std::shared_ptr<XRender> mRender;
    std::shared_ptr<XPlayer> mPlayer;
};


#endif //XRENDER_XSURFACE_HPP
