//
// Created by Oogh on 2020/10/26.
//

#ifndef XRENDER_XTRIANGLE_HPP
#define XRENDER_XTRIANGLE_HPP

#include "XShader.hpp"

class XTriangle {

public:
    XTriangle();

    ~XTriangle();

    void draw();

private:
    std::unique_ptr<XShader> mShader;
    GLuint VBO;
    GLuint VAO;
};


#endif //XRENDER_XTRIANGLE_HPP
