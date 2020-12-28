//
//  XIOSVideoCodec.hpp
//  XRender
//
//  Created by Oogh on 2020/12/18.
//  Copyright © 2020 Oogh. All rights reserved.
//

#ifndef XIOSVideoCodec_hpp
#define XIOSVideoCodec_hpp

#import <AVFoundation/AVFoundation.h>

#include <memory>
#include <string>

struct XImage;

class XIOSVideoCodec {
public:
    XIOSVideoCodec();
    
    ~XIOSVideoCodec();
    
    void setFilename(const std::string& filename);
    
    std::shared_ptr<XImage> getImage(long clock);
    
private:
    int open();
    
    void close();
    
private:
    std::string mFilename;
    AVAssetReader* mReader;
    AVAssetReaderTrackOutput* mVideoReaderOutput;
};

#endif /* XIOSVideoCodec_hpp */
