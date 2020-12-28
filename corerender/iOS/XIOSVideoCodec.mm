//
//  XIOSVideoCodec.mm
//  XRender
//
//  Created by Oogh on 2020/12/18.
//  Copyright © 2020 Oogh. All rights reserved.
//

#import <AVFoundation/AVFoundation.h>
#include "XIOSVideoCodec.hpp"
#include "XLogger.hpp"
#include "XImage.hpp"

XIOSVideoCodec::XIOSVideoCodec() {
    
}

XIOSVideoCodec::~XIOSVideoCodec() {
    
}

void XIOSVideoCodec::setFilename(const std::string& filename) {
    mFilename = filename;
}

std::shared_ptr<XImage> XIOSVideoCodec::getImage(long clock) {
    CMSampleBufferRef buffer = [mVideoReaderOutput copyNextSampleBuffer];
    auto image = std::make_shared<XImage>();
    image->pixels[3] = reinterpret_cast<uint8_t*>(buffer);
    
    CMTime pts = CMSampleBufferGetPresentationTimeStamp(buffer);
    image->pts = static_cast<long>(CMTimeGetSeconds(pts) * 1000);
    
    CMTime duration = CMSampleBufferGetDuration(buffer);
    image->duration = static_cast<long>(CMTimeGetSeconds(duration) * 1000);
    
//    CMVideoFormatDescription desc = CMSampleBufferGetFormatFormatDescription(buffer);
    return nullptr;
}

int XIOSVideoCodec::open() {

    NSString* path = [NSString stringWithCString:mFilename.data()
                                        encoding:[NSString defaultCStringEncoding]];
    AVAsset* asset = [AVAsset assetWithURL:[NSURL fileURLWithPath:path]];
    
    NSError* err;
    mReader = [AVAssetReader assetReaderWithAsset:asset error:&err];
    if (err) {
        LOGE("[XIOSVideoCodec] assetReaderWithAsset failed: %@", err.localizedDescription);
        return -1;
    }
    AVAssetTrack* videoTrack = [[mReader.asset tracksWithMediaType:AVMediaTypeVideo] firstObject];
    
    NSDictionary* readSettings = @{
        (id)kCVPixelBufferPixelFormatTypeKey: @(kCVPixelFormatType_32BGRA),
        (id)kCVPixelBufferIOSurfacePropertiesKey: [NSDictionary dictionary]
    };
    mVideoReaderOutput = [AVAssetReaderTrackOutput assetReaderTrackOutputWithTrack:videoTrack
                                                                    outputSettings:readSettings];
    
    if ([mReader canAddOutput:mVideoReaderOutput]) {
        [mReader addOutput:mVideoReaderOutput];
    }
    
    [mReader startReading];
    
    return 0;
}

void XIOSVideoCodec::close() {
    
}
