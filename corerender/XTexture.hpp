//
// Created by Oogh on 2020/3/19.
//

#ifndef MAC

#ifndef ANDROIDDEMO_XTEXTURE_H
#define ANDROIDDEMO_XTEXTURE_H

#include "XGLHeader.hpp"

class XTexture {
public:
    XTexture(int width, int height);

    ~XTexture();

    void draw();

    void update(uint8_t* pixels, int width, int height);

private:
    const int TEXTURE_VERTEX_COUNT = 4;

private:
    GLuint mProgram;
    GLuint aPosition;
    GLuint aTextureCoord;

    GLuint mTextureId;

    float* mVertexCoords;
    float* mTextureCoords;

    uint8_t* mPixels;
    int mWidth;
    int mHeight;
};
#endif //ANDROIDDEMO_XTEXTURE_H

#endif
