//
// Created by Andy on 2020/8/14.
//

#ifndef XRENDER_XSTHELPER_H
#define XRENDER_XSTHELPER_H

#include "XSTHeader.h"

class XSTHelper : public soundtouch::SoundTouch {

public:
    XSTHelper(int sampleRate, int channels);

    ~XSTHelper();

    /**
     * @brief 执行音频特效处理
     * @param dst 输出参数，指向输出数据缓冲区的指针
     * @param src 采样源数据
     * @param srcLen 采样个数
     * @return 实际输出的采样个数
     */
    int process(uint8_t* dst, uint8_t* src, int srcLen);
};


#endif //XRENDER_XSTHELPER_H
