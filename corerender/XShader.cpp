//
// Created by Oogh on 2020/3/19.
//

#include <fstream>
#include <sstream>
#include "XLogger.hpp"
#include "XShader.hpp"

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

XShader::XShader(std::string vertexFilePath, std::string fragmentFilePath) {
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        vShaderFile.open(vertexFilePath);
        fShaderFile.open(fragmentFilePath);
        std::stringstream vShaderStream, fShaderStream;
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        vShaderFile.close();
        fShaderFile.close();
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure &e) {
        LOGW("[XShader] open vs/fs file failed: %s\n", e.what());
    }
    const char* vShaderCode = vertexCode.data();
    const char* fShaderCode = fragmentCode.data();

    GLuint vertex;
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, nullptr);
    glCompileShader(vertex);
    checkCompileError(vertex, "VERTEX");

    GLuint fragment;
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, nullptr);
    glCompileShader(fragment);
    checkCompileError(fragment, "FRAGMENT");

    mProgram = glCreateProgram();
    glAttachShader(mProgram, vertex);
    glAttachShader(mProgram, fragment);
    glLinkProgram(mProgram);
    checkCompileError(mProgram, "PROGRAM");

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

XShader::~XShader() {
    glDeleteProgram(mProgram);
}

void XShader::use() {
    glUseProgram(mProgram);
}

void XShader::setBool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(mProgram, name.data()), (int) value);
}

void XShader::setInt(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(mProgram, name.data()), value);
}

void XShader::setFloat(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(mProgram, name.data()), value);
}

void XShader::checkCompileError(GLuint shader, std::string type) {
    int success;
    char infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
            LOGE("[XShader] SHADER_COMPILATION_ERROR of type: %s error: %s\n", type.data(), infoLog);
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
            LOGE("[XShader] PROGRAM_LINKING_ERROR of type: %s error: %s\n", type.data(), infoLog);
        }
    }
}