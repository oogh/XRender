//
// Created by Oogh on 2020/10/20.
//

#include "XMacView.hpp"
#include "XLogger.hpp"
#include "XRender.hpp"

XMacView::XMacView(int width, int height)
        : mWidth(width), mHeight(height) {
}

XMacView::~XMacView() {

}

void XMacView::create() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#if PLATFORM_MAC
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(mWidth, mHeight, "LearnOpenGL", nullptr, nullptr);
    if (window == nullptr) {
        LOGE("[XMacView] Failed to create GLFW window\n");
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        LOGE("[XMacView] Failed to initialize GLAD\n");
        glfwTerminate();
        return;
    }

    mRender = std::make_shared<XRender>();

    onSurfaceCreated(window);

    glViewport(0, 0, mWidth, mHeight);
    while (true) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        onDrawFrame(window);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
}

std::shared_ptr<XRender> XMacView::getRender() {
    return mRender;
}

void XMacView::onSurfaceCreated(GLFWwindow* window) {
    mRender->onSurfaceCreated();
}

void XMacView::onSurfaceSizeChanged(GLFWwindow* window, int width, int height) {
    mRender->onSurfaceChanged(width, height);
}

void XMacView::onDrawFrame(GLFWwindow* window) {
    mRender->onDrawFrame();
}

void XMacView::update(std::vector<std::shared_ptr<XImage>> images) {
    if (mRender) {
        mRender->update(images);
    }
}
