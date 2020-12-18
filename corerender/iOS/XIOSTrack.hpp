//
//  XIOSTrack.hpp
//  XRender
//
//  Created by Oogh on 2020/12/18.
//  Copyright © 2020 Oogh. All rights reserved.
//

#ifndef XIOSTrack_hpp
#define XIOSTrack_hpp

#import <Foundation/Foundation.h>

@interface XIOSTrack : NSObject

- (void)prepare;

- (void)start;

- (void)seekTo:(long)targetPos;

- (void)pause;

- (void)stop;

@end

#endif /* XIOSTrack_hpp */
