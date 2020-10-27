//
// Created by Oogh on 2020/3/19.
//

#ifndef ANDROIDDEMO_XSHADER_H
#define ANDROIDDEMO_XSHADER_H

#include <string>
#include "XGLHeader.hpp"

class XShader {
public:
    XShader(const char* vertex, const char* fragment);

    ~XShader();

    void use();

    void setBool(const std::string& name, bool value) const;

    void setInt(const std::string& name, int value) const;

    void setFloat(const std::string& name, float value) const;

private:
    void checkCompileError(GLuint shader, std::string type);

private:
    GLuint mProgram;

};

#endif //ANDROIDDEMO_XSHADER_H
