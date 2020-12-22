//
//  XIOSPlayer.hpp
//  XRender
//
//  Created by Oogh on 2020/12/18.
//  Copyright © 2020 Oogh. All rights reserved.
//

#ifndef XIOSPlayer_hpp
#define XIOSPlayer_hpp

#import <Foundation/Foundation.h>

@class XIOSTimeline;

@interface XIOSPlayer : NSObject

- (void)setTimeline:(XIOSTimeline*)timeline;

- (void)prepare;

- (void)start;

- (void)seekTo:(long)targetPos;

- (void)pause;

- (void)stop;

@end

#endif /* XIOSPlayer_hpp */
