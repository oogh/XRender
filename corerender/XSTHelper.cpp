//
// Created by Andy on 2020/8/14.
//

#include "XSTHelper.h"
#include "XLogger.h"
XSTHelper::XSTHelper(int sampleRate, int channels) {
    setSampleRate(static_cast<uint>(sampleRate));
    setChannels(static_cast<uint>(channels));
        
//    setSetting(SETTING_SEQUENCE_MS, 40);
//    setSetting(SETTING_SEEKWINDOW_MS, 15);
//    setSetting(SETTING_OVERLAP_MS, 8);
//
//    setSetting(SETTING_USE_QUICKSEEK, 0);
//    setSetting(SETTING_USE_AA_FILTER, 1);
}

XSTHelper::~XSTHelper() {

}

int XSTHelper::process(uint8_t **dst, uint8_t *src, int srcLen) {
    // srcLen * channels * sizeof(S16)
    int srcSize = srcLen * 2 * 2;
    soundtouch::SAMPLETYPE* srcBuffer = reinterpret_cast<soundtouch::SAMPLETYPE*>(malloc(srcSize));
    
    for (int i = 0; i < srcSize / 2 + 1; i++) {
        srcBuffer[i] = (src[i * 2] | (src[i * 2 + 1] << 8));
    }
    
    putSamples((const soundtouch::SAMPLETYPE *)src, srcLen);
    
    *dst = reinterpret_cast<uint8_t*>(malloc(srcSize));
    short* dstBuffer = reinterpret_cast<short*>(*dst);
    

    int len, offset = 0, result = 0;
    do {
        len = receiveSamples(srcBuffer, srcLen);
        if (len > 0) {
            if (mOutFile) {
                fwrite(srcBuffer, 1, len * 4, mOutFile);
            }
            memcpy(dstBuffer + offset, srcBuffer, len * 4);
            offset += len * 4;
            result += len;
        }
    } while (len > 0);

    flush();

    do {
        len = receiveSamples(srcBuffer, srcLen);
        if (len > 0) {
            if (mOutFile) {
                fwrite(srcBuffer, 1, len * 4, mOutFile);
            }
            memcpy(dstBuffer + offset, srcBuffer + offset, len * 4);
            offset += len * 4;
            result += len;
        }
    } while (len > 0);
    
    clear();
    
    return result;
}

void XSTHelper::setOutputFile(FILE* fp) {
    if (!mOutFile) {
        mOutFile = fp;
    }
}
