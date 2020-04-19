//
// Created by Oogh on 2020/3/19.
//

#include "XTexture.h"
#include "XShader.h"
#include "XLogger.h"
#include <cstring>

#define TO_STR(x) #x

const char* gVertexShader = TO_STR(
        attribute vec4 aPosition;
        attribute vec2 aTextureCoord;
        varying vec2 vTextureCoord;
        void main() {
            vTextureCoord = vec2(aTextureCoord.x, 1.0 - aTextureCoord.y);
            gl_Position = aPosition;
        }
);

const char *gFragRGBA = TO_STR(
        precision mediump float;
        varying vec2 vTextureCoord;
        uniform sampler2D uTexture;
        void main() {
            gl_FragColor = texture2D(uTexture, vTextureCoord);
        }
);

const char *gFragYUV420P = TO_STR(
        precision mediump float;
        varying vec2 vTextureCoord;
        uniform sampler2D yTexture;
        uniform sampler2D uTexture;
        uniform sampler2D vTexture;
        void main() {
            vec3 yuv;
            vec3 rgb;
            yuv.r = texture2D(yTexture, vTextureCoord).r;
            yuv.g = texture2D(uTexture, vTextureCoord).r - 0.5;
            yuv.b = texture2D(vTexture, vTextureCoord).r - 0.5;
            rgb = mat3(1.0, 1.0, 1.0,
                       0.0, -0.39465, 2.03211,
                       1.13983, -0.58060, 0.0) * yuv;
            gl_FragColor = vec4(rgb, 1.0);
        }
);

XTexture::XTexture(int width, int height)
: mWidth(width), mHeight(height) {
    mVertexCoords = new float[TEXTURE_VERTEX_COUNT * 3] {
            1.0f, -1.0f, 0.0f,
            -1.0f, -1.0f, 0.0f,
            1.0f, 1.0f, 0.0f,
            -1.0f, 1.0f, 0.0f,
    };

    mTextureCoords = new float[TEXTURE_VERTEX_COUNT * 2] {
            1.0f, 0.0f, //右下
            0.0f, 0.0f,
            1.0f, 1.0f,
            0.0, 1.0
    };

    // 1. 创建着色器程序
    mProgram = createProgram(gVertexShader, gFragRGBA);

    // 2. 获取着色器中的属性饮用
    aPosition = static_cast<GLuint>(glGetAttribLocation(mProgram, "aPosition"));
    aTextureCoord = static_cast<GLuint>(glGetAttribLocation(mProgram, "aTextureCoord"));

    // 3. 使用着色器程序
    glUseProgram(mProgram);

    // 4. 向着色器程序中传递顶点坐标数据
    glVertexAttribPointer(aPosition, 3, GL_FLOAT, GL_FALSE, 0, mVertexCoords);
    glEnableVertexAttribArray(aPosition);

    // 5. 向着色器程序中传递纹理坐标数据
    glVertexAttribPointer(aTextureCoord, 2, GL_FLOAT, GL_FALSE, 0, mTextureCoords);
    glEnableVertexAttribArray(aTextureCoord);

    // 以上是着色器程序部分
    //---------------------------------------
    // 以下是纹理部分

    // 1. 设置纹理层
    glUniform1i(glGetUniformLocation(mProgram, "uTexture"), 0);

    // 2. 创建纹理
    glGenTextures(1, &mTextureId);
    glBindTexture(GL_TEXTURE_2D, mTextureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    mPixels = new uint8_t[mWidth * mHeight * 4];
}

XTexture::~XTexture() {
    if (mVertexCoords) {
        delete[] mVertexCoords;
        mVertexCoords = nullptr;
    }

    if (mTextureCoords) {
        delete[] mTextureCoords;
        mTextureCoords = nullptr;
    }
}

void XTexture::draw() {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mTextureId);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, mPixels);
    
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void XTexture::update(uint8_t* pixels, int width, int height) {
    LOGD("[XTexture] update width: %d, height: %d\n", width, height);

    memcpy(mPixels, pixels, width * height * 4);
}
