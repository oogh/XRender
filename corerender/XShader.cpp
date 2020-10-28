//
// Created by Oogh on 2020/3/19.
//

#include <fstream>
#include <sstream>
#include "XLogger.hpp"
#include "XShader.hpp"
#include "XLogger.hpp"

const char* vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 position;\n"
                                 "void main()\n"
                                 "{\n"
                                 "gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
                                 "}\0";
const char* fragmentShaderSource = "#version 330 core\n"
                                   "out vec4 color;\n"
                                   "void main()\n"
                                   "{\n"
                                   "color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                   "}\n\0";

XShader::XShader(const char* vertexFilePath, const char* fragmentFilePath) {
//    std::string vertexCode;
//    std::string fragmentCode;
//    std::ifstream vShaderFile;
//    std::ifstream fShaderFile;
//
//    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
//    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
//    try {
//        vShaderFile.open(vertexFilePath);
//        fShaderFile.open(fragmentFilePath);
//        std::stringstream vShaderStream, fShaderStream;
//        vShaderStream << vShaderFile.rdbuf();
//        fShaderStream << fShaderFile.rdbuf();
//        vShaderFile.close();
//        fShaderFile.close();
//        vertexCode = vShaderStream.str();
//        fragmentCode = fShaderStream.str();
//    }
//    catch (std::ifstream::failure &e) {
//        LOGE("SHADER::FILE_NOT_SUCCEESFULLY_READ\n");
//    }
//    const char* vShaderCode = vertexCode.data();
//    const char* fShaderCode = fragmentCode.data();

    GLuint vertex;
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertex);
    checkCompileError(vertex, "VERTEX");

    GLuint fragment;
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentShaderSource, nullptr);
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
            LOGE("SHADER_COMPILATION_ERROR of type: %s error: %s\n", type.data(), infoLog);
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
            LOGE("PROGRAM_LINKING_ERROR of type: %s error: %s\n", type.data(), infoLog);
        }
    }
}