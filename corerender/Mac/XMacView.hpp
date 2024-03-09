//
// Created by Oogh on 2020/10/20.
//

#ifndef XMACVIEW_HPP
#define XMACVIEW_HPP

#include <memory>
#include "XGLHeader.hpp"
#include "XRender.hpp"

class XImage;

class XMacView {
public:
    XMacView(int width, int height);

    ~XMacView();

    std::shared_ptr<XRender> getRender();

    void create();

protected:
    void onSurfaceCreated(GLFWwindow* window);

    void onSurfaceSizeChanged(GLFWwindow* window, int width, int height);

    void onDrawFrame(GLFWwindow* window);

private:
    int mWidth;
    int mHeight;

    std::shared_ptr<XRender> mRender;
};


#endif // XMACVIEW_HPP
