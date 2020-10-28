//
// Created by Andy on 2020/10/20.
//

#ifndef XRENDER_XSURFACE_HPP
#define XRENDER_XSURFACE_HPP

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <memory>
#include <thread>
#include "XShader.hpp"

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
    void renderWorkThread(void* opaque);

private:
    GLFWwindow* mWindow;
    unsigned int VBO, VAO, EBO;
    int mShaderProgram;
    std::unique_ptr<std::thread> mRenderTid;

    int mWidth;
    int mHeight;

    std::unique_ptr<XShader> mShader;

};


#endif //XRENDER_XSURFACE_HPP
