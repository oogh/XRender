//
//  XImage.h
//  XExporter
//
//  Created by Oogh on 2020/3/19.
//  Copyright © 2020 Oogh. All rights reserved.
//

#ifndef XEXPORTER_XIMAGE_H
#define XEXPORTER_XIMAGE_H

#include <memory>
#include "XFFHeader.hpp"

enum ImageType {
    IMG_TYPE_UNKNOWN = -1,
    IMG_TYPE_YUV420P = 0, // AV_PIX_FMT_YUV420P
    IMG_TYPE_RGB24 = 2, // AV_PIX_FMT_RGB24
    IMG_TYPE_RGBA = 26, // AV_PIX_FMT_RGBA
};

struct XImage {
    uint8_t* pixels[4] = {nullptr};

    int linesize[4] = {0};

    int width = 0;

    int height = 0;

    long pts = -1;

    long duration = -1;

    int format = -1;

    XImage(): width(0), height(0), pts(-1), duration(-1) {
    }

    ~XImage() {
        free();
    }

    void free() {
        this->width = 0;
        this->height = 0;
        this->pts = -1;
        this->duration = -1;
        if (this->pixels[0] != nullptr) {
            av_freep(&this->pixels[0]);
            av_freep(&this->pixels);
        }

        for (int i = 0; i < 4; ++i) {
            this->linesize[i] = 0;
        }
    }
};

#endif //XEXPORTER_XIMAGE_H
