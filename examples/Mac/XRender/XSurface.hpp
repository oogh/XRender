//
// Created by Andy on 2020/10/20.
//

#ifndef XRENDER_XSURFACE_HPP
#define XRENDER_XSURFACE_HPP

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <memory>
#include <thread>

class XSurface {
public:
    XSurface(int width, int height);

    ~XSurface();

protected:
    virtual void onSurfaceCreated();

    virtual void onSurfaceSizeChanged(int width, int height);

    virtual void onDrawFrame();

private:
    void init();

    void deinit();

private:
    void renderWorkThread(void* opaque);

private:
    GLFWwindow* mWindow;
    unsigned int VBO, VAO, EBO;
    int mShaderProgram;
    std::unique_ptr<std::thread> mRenderTid;

    int mWidth;
    int mHeight;
};


#endif //XRENDER_XSURFACE_HPP
