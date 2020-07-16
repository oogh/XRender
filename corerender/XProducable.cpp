//
//  XProducable.cpp
//  XExporter
//
//  Created by Oogh on 2020/3/19.
//  Copyright © 2020 Oogh. All rights reserved.
//

#include "XProducable.h"

XProducable::XProducable()
        : mDisableVideo(false), mDisableAudio(true), mProduceMode(PRODUCE_MODE_SOFTWARE) {

}

XProducable::~XProducable() {

}

void XProducable::setDisableVideo(bool disabled) {
    mDisableVideo = disabled;
}

void XProducable::setDisableAudio(bool disabled) {
    mDisableAudio = disabled;
}

void XProducable::setInput(const std::string &filename) {
    mFilename = filename;
}

void XProducable::setProduceMode(ProduceMode mode) {
    mProduceMode = mode;
}

void XProducable::start() {

}

std::shared_ptr<XImage> XProducable::peekImage(long clock) {
    return nullptr;
}

void XProducable::endCurrentImageUse() {

}

std::shared_ptr<XSample> XProducable::getSample() {
    return nullptr;
}

void XProducable::stop() {

}

long XProducable::getOriginalDuration() const {
    return 0;
}

int XProducable::getOriginalWidth() const {
    return 0;
}

int XProducable::getOriginalHeight() const {
    return 0;
}
