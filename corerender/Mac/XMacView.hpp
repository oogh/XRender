//
// Created by Oogh on 2020/10/20.
//

#ifndef XMACVIEW_HPP
#define XMACVIEW_HPP

#include <memory>
#include "XGLHeader.hpp"
#include "XViewObserver.hpp"

class XRender;
class XImage;

class XMacView : public XViewObserver {
public:
    XMacView(int width, int height);

    ~XMacView();

    std::shared_ptr<XRender> getRender();

    void create();

public:
    void update(std::vector<std::shared_ptr<XImage>> images) override;

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
