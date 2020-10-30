//
// Created by Andy on 2020/10/20.
//

#include "XSurface.hpp"
#include "XLogger.hpp"
#include "XTriangle.hpp"

XSurface::XSurface(int width, int height)
        : mWidth(width), mHeight(height) {
    init();
}

XSurface::~XSurface() {
    deinit();
}

void XSurface::init() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(mWidth, mHeight, "LearnOpenGL", nullptr, nullptr);
    if (window == nullptr) {
        LOGE("[XSurface] Failed to create GLFW window\n");
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        LOGE("[XSurface] Failed to initialize GLAD\n");
        glfwTerminate();
        return;
    }

    mTriangle = std::make_unique<XTriangle>();

    onSurfaceCreated(window);

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        onDrawFrame(window);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
}

void XSurface::deinit() {

}

void XSurface::onSurfaceCreated(GLFWwindow* window) {
}

void XSurface::onSurfaceSizeChanged(GLFWwindow* window, int width, int height) {

}

void XSurface::onDrawFrame(GLFWwindow* window) {
    mTriangle->draw();
}
