#include <iostream>
#include "XFFProducer.h"
#include "XImageUitls.h"
#include "XImage.h"
#include "XTimeCounter.h"
#include <vector>
#include <string>
#include "XALPlayer.h"
#include "XLogger.h"

void testRender() {
    std::vector<std::string> filenames = {
//        "/Users/andy/Movies/ok.gif",
//        "/Users/andy/Movies/feiqi.gif",
//        "/Users/andy/Movies/kazhu.gif",
//        "/Users/andy/Downloads/jieqian.gif",
//        "/Users/andy/Downloads/Andy.jpg",
//        "/Users/andy/Downloads/logo.png",
//        "/Users/andy/Downloads/spring.jpg",
//        "/Users/andy/Downloads/exportTest.gif",
//        "/Users/andy/Downloads/IMG_0383.GIF",
//        "/Users/andy/Workspace/Gaoding/resources/videoEditTest/testData/gif0.gif",
//        "/Users/andy/Workspace/Gaoding/resources/videoEditTest/testData/gif1.gif",
//        "/Users/andy/Workspace/Gaoding/resources/videoEditTest/testData/gif2.gif",
//        "/Users/andy/Workspace/Gaoding/resources/videoEditTest/testData/apng0.png",
//        "/Users/andy/Workspace/Gaoding/resources/videoEditTest/testData/apng1.png",
//        "/Users/andy/Workspace/Gaoding/resources/videoEditTest/testData/apng2.png",
//        "/Users/andy/Movies/jieqian_720x1280.mp4",
        "/Users/andy/Movies/xinwenlianbo.mp4",
//        "/Users/andy/Movies/lianche.mp4",
//        "/Users/andy/Movies/douyin.mp4",
//        "/Users/andy/Movies/720.mp4",
//        "/Users/andy/Movies/jieqian_720x1280.mp4",
//        "/Users/andy/Movies/lianche.mp4",
//        "/Users/andy/Movies/douyin.mp4",
//        "/Users/andy/Movies/720.mp4"
    };
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
    std::string filename = "/Users/andy/Movies/xinwenlianbo.mp4";
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
    std::string filename = "/Users/oogh/Movies/xinwenlianbo.mp4";
    auto producer = std::make_unique<XFFProducer>();
    producer->setInput(filename);
    producer->setDisableVideo(true);
    producer->start();

    auto sounder = std::make_unique<XALPlayer>();
    sounder->init();

    int len;
    int size = 4096;
    uint8_t* data = reinterpret_cast<uint8_t*>(malloc(size));
    do {
        memset(data, '\0', size);
        len = producer->readSamples(data, size);
        if (len > 0) {
            //LOGE("[main] update data: %p, len: %d\n", data, len);
            sounder->updateData(data, len);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds (10));
    } while (len >= 0);


    sounder->deinit();
}

int main(int argc, char* argv[]) {
    
    testOpenAL();

    return 0;
}

