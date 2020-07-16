#include <iostream>
#include "XFFProducer.h"
#include "XImageUitls.h"
#include "XImage.h"
#include "XTimeCounter.h"
#include <vector>
#include <string>

int main(int argc, char* argv[]) {
    
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
        "/Users/andy/Movies/jieqian_720x1280.mp4",
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

    return 0;
}

