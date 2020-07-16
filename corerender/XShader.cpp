//
// Created by Oogh on 2020/3/19.
//

#ifndef MAC

#include "XShader.h"
#include "XLogger.h"

void print(const char* name, GLenum type) {
    const char* value = reinterpret_cast<const char*>(glGetString(type));
    LOGI("[XShader] %s = %s \n", name, value);
}

void printOperationError(const char* op) {
    for (GLint error = glGetError(); error; error = glGetError()) {
        LOGE("[XShader] run %s(): glError (0x%x)\n", op, error);
    }
}

GLuint create(GLenum type, const char* code) {
    GLuint shader = glCreateShader(type);
    if (!shader) {
        printOperationError("glCreateShader");
        return 0;
    }
    glShaderSource(shader, 1, &code, nullptr);
    GLint compiled = GL_FALSE;
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
        GLint infoLogLen = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLen);
        if (infoLogLen > 0) {
            GLchar *infoLog = new GLchar[infoLogLen];
            if (infoLog) {
                glGetShaderInfoLog(shader, infoLogLen, nullptr, infoLog);
                LOGE("[XShader] compile failed： %s shader:\n%s\n",
                     type == GL_VERTEX_SHADER ? "vertex" : "fragment",
                     infoLog);
                delete[] infoLog;
            }
        }
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

GLuint createProgram(const char* vertex, const char* fragment) {
    GLuint vShader = 0;
    GLuint fShader = 0;
    GLuint program = 0;
    GLint linked = GL_FALSE;
    vShader = create(GL_VERTEX_SHADER, vertex);
    if (!vShader) {
        glDeleteShader(vShader);
        return 0;
    }
    fShader = create(GL_FRAGMENT_SHADER, fragment);
    if (!fShader) {
        glDeleteShader(vShader);
        glDeleteShader(fShader);
        return 0;
    }
    program = glCreateProgram();
    if (!program) {
        printOperationError("glCreateProgram");
        glDeleteShader(vShader);
        glDeleteShader(fShader);
        return 0;
    }
    glAttachShader(program, vShader);
    glAttachShader(program, fShader);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (!linked) {
        GLint len = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);
        if (len) {
            GLchar *error_msg = new GLchar[len];
            if (error_msg) {
                glGetProgramInfoLog(program, len, nullptr, error_msg);
                LOGE("[XShader] glLinkProgram failed:\n%s\n", error_msg);
                delete[] error_msg;
            }
        }
        glDeleteProgram(program);
        program = 0;
    }
    return program;
}

#endif
