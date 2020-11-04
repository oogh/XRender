//
// Created by Andy on 2020/11/4.
//

#ifndef XRENDER_XSAMPLE_HPP
#define XRENDER_XSAMPLE_HPP

#include <memory>
#include "XFFHeader.hpp"

struct XSample {
    uint8_t* pixels = nullptr;

    int length = 0;

    int format = -1;

    XSample() {
    }

    ~XSample() {
        free();
    }

    void free() {
        this->length = 0;
        this->format = -1;
        if (this->pixels != nullptr) {
            av_freep(&this->pixels);
        }
    }
};


#endif //XRENDER_XSAMPLE_HPP
