//
// Created by Oogh on 2020/11/1.
//

#include "XTexture.hpp"
#include "XImageUitls.hpp"

#if __APPLE__
std::string XTexture::sVertexFilePath = "/Users/oogh/Workspace/XRender/Resources/shaders/texture.vs";
std::string XTexture::sFragmentFilePath = "/Users/oogh/Workspace/XRender/Resources/shaders/texture.fs";
#elif __ANDROID__
std::string XTexture::sVertexFilePath = "/sdcard/Android/data/com.demo.render/files/shaders/texture.vs";
std::string XTexture::sFragmentFilePath = "/sdcard/Android/data/com.demo.render/files/shaders/texture.fs";
#endif

XTexture::XTexture(): mWidth(0), mHeight(0), mPixels(nullptr) {
    mShader = std::make_unique<XShader>(sVertexFilePath, sFragmentFilePath);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    float vertices[] = {
            // positions          // colors           // texture coords
            1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
            1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
            -1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f  // top left
    };
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    unsigned int indices[] = {
            0, 1, 3,
            1, 2, 3
    };
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glGenTextures(1, &mTexture);
    glBindTexture(GL_TEXTURE_2D, mTexture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

XTexture::~XTexture() {
    if (mPixels) {
        free(mPixels);
        mPixels = nullptr;
    }
}

void XTexture::setPixels(uint8_t* pixels, int width, int height) {
    if (!pixels || width <= 0 || height <= 0) {
        return;
    }
    
    if (mWidth != width || mHeight != height) {
        if (mPixels) {
            free(mPixels);
            mPixels = nullptr;
        }
        mWidth = width;
        mHeight = height;
    }
    
    size_t size = width * height * 4;
    if (!mPixels) {
        mPixels = reinterpret_cast<uint8_t*>(malloc(size));
    }
    
    memcpy(mPixels, pixels, size);

}

void XTexture::draw() {
    if (mPixels) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, mPixels);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    
    glBindTexture(GL_TEXTURE_2D, mTexture);

    mShader->use();
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
