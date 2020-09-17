#include <iostream>
#include "XFFProducer.h"
#include "XImageUitls.h"
#include "XImage.h"
#include "XTimeCounter.h"
#include <vector>
#include <string>
#include "XAudioPlayer.h"
#include "XLogger.h"

std::vector<std::string> filenames = {
//        "/Users/andy/Workspace/Resources/ok.gif",
//        "/Users/andy/Workspace/Resources/feiqi.gif",
//        "/Users/andy/Workspace/Resources/kazhu.gif",
//        "/Users/andy/Workspace/Resources/jieqian.gif",
//        "/Users/andy/Workspace/Resources/Andy.jpg",
//        "/Users/andy/Workspace/Resources/logo.png",
//        "/Users/andy/Workspace/Resources/spring.jpg",
//        "/Users/andy/Workspace/Resources/exportTest.gif",
//        "/Users/andy/Workspace/Resources/IMG_0383.GIF",
//        "/Users/andy/Workspace/Resources/jieqian_720x1280.mp4",
//        "/Users/andy/Workspace/Resources/xinwenlianbo.mp4",
//        "/Users/andy/Workspace/Resources/lianche.mp4",
//        "/Users/andy/Workspace/Resources/douyin.mp4",
//        "/Users/andy/Workspace/Resources/720.mp4",
//        "/Users/andy/Workspace/Resources/jieqian_720x1280.mp4",
//        "/Users/andy/Workspace/Resources/lianche.mp4",
//        "/Users/andy/Workspace/Resources/douyin.mp4",
//        "/Users/andy/Workspace/Resources/720.mp4",
//        "/Users/andy/Workspace/Resources/money1.png",
//        "/Users/andy/Workspace/Resources/jpg.jpg",
//        "/Users/andy/Workspace/Resources/bbb-palettegen.png",
//        "/Users/andy/Workspace/Resources/logo.png",
//        "/Users/andy/Workspace/Resources/png3.png",
        "/Users/andy/Workspace/Resources/image.png",
};

void testRender() {
    std::vector<std::shared_ptr<XFFProducer>> producerList;
    int size = filenames.size();
    for (int i = 0; i < size; ++i) {
        auto producer = std::make_shared<XFFProducer>();
        producer->setInput(filenames[i]);
        producerList.emplace_back(producer);

        producer->start();
    }


    long duration = 2000;
    int fps = 25;
    long offset = static_cast<long>(1000.0 / fps);
    int width, height;
    XTimeCounter saveCounter;
    for (long clock = 0; clock < duration; clock += offset) {
        for (int i = 0; i < size; ++i) {
            auto producer = producerList.at(i);
            auto image = producer->peekImage(clock);
            if (image && image->pixels) {
                saveCounter.markStart();
                char filename[128];
                sprintf(filename, "/Users/andy/images/%ld.bmp", clock);
                rgb2bmp(filename, image->pixels[0], image->width, image->height);
                saveCounter.markEnd();
                std::cout << "[" << saveCounter.getRunDuration() << "] save: " << filename << std::endl;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(offset));
    }

    for (int i = 0; i < size; ++i) {
        auto producer = producerList.at(i);
        producer->stop();
    }

    std::vector<std::shared_ptr<XFFProducer>>().swap(producerList);
}

void testSounder() {
    std::string filename = filenames.at(0);
    auto producer = std::make_unique<XFFProducer>();
    producer->setInput(filename);
    producer->setDisableVideo(true);
    producer->start();

    int len;

//    FILE *outFile = fopen("/Users/andy/output.pcm", "wb+");
//    if (!outFile) {
//        LOGE("[main] open file failed: /Users/andy/output.pcm\n");
//        return;
//    }

    int total = 0;
    do {
        int bufferSize = 4096;
        uint8_t* buffer = reinterpret_cast<uint8_t*>(malloc(bufferSize));
        memset(buffer, '\0', bufferSize);
        len = producer->readSamples(buffer, bufferSize);
        if (len > 0) {
            total += len;
            LOGI("[main] write length: %d total: %d\n", len, total);
//            fwrite(buffer, 1, len, outFile);
        }
        free(buffer);
    } while (len >= 0);

//    fclose(outFile);
    LOGI("[main] close\n");

//    auto sounder = std::make_unique<XSounder>();
//    bool success = sounder->init();
//    if (!success) {
//        LOGE("[main] sounder init failed!\n");
//        return;
//    }
//
//    int bufferSize = 4096 * 4;
//    uint8_t* buffer = reinterpret_cast<uint8_t*>(malloc(bufferSize));
//    int len = producer->readSamples(&buffer, bufferSize);
//
//    sounder->open(buffer, len);
//
//    while (len > 0) {
//        int processd = sounder->getProcessedCount();
//        if (processd <= 0) {
//            continue;
//        }
//
//        while (processd--) {
//            len = producer->readSamples(&buffer, bufferSize);
//            sounder->open(buffer, len);
//        }
//    }
//
//    sounder->deinit();

//    getchar();
}

void testOpenAL() {
    std::string filename = filenames.at(0);
    auto producer = std::make_unique<XFFProducer>();
    producer->setInput(filename);
    producer->setDisableVideo(true);
    producer->start();

    auto sounder = std::make_unique<XAudioPlayer>();
    sounder->start();

    int len, ret;
    int size = 4096;
    uint8_t* data = reinterpret_cast<uint8_t*>(malloc(size));
    do {
        memset(data, '\0', size);
        len = producer->readSamples(data, size);
        if (len > 0) {
            sounder->updateAudio(data, len);
        }
    } while (len >= 0);
}

int main(int argc, char* argv[]) {
    
    testRender();

    return 0;
}

