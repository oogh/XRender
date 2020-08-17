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

int XSTHelper::process(uint8_t *dst, uint8_t *src, int srcLen) {
    
    int a = sizeof(uint8_t);
    int b = sizeof(short);

    short* dstBuffer = reinterpret_cast<short*>(dst);
    short* srcBuffer = reinterpret_cast<short*>(src);

    putSamples(srcBuffer, srcLen);
    
    int dstLen = srcLen;
    // nb_samples * channels * sizeof(short);
    int cacheSize = dstLen * 2 * sizeof(short);
    short* cache = reinterpret_cast<short*>(malloc(cacheSize));

    LOGE("[XSTHelper] srcLen: %d, cacheSize: %d\n", srcLen, cacheSize);
    int len, size, total = 0, offset = 0;
    do {
        len = receiveSamples(cache, dstLen);
        size = len * 2 * sizeof(short);
        memcpy(dstBuffer + offset, cache, len);
        offset += size;
        total += len;
    } while (len > 0);

    flush();

    do {
        len = receiveSamples(cache, dstLen);
        size = len * 2 * sizeof(short);
        memcpy(dstBuffer + offset, cache, len);
        offset += size;
        total += len;
    } while (len > 0);

    if (cache) {
        free(cache);
    }
    
    LOGE("[XSTHelper] total: %d, offset: %d\n", total, offset);

    return total;
}
