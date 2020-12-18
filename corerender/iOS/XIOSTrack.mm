//
//  XIOSTrack.mm
//  XRender
//
//  Created by Oogh on 2020/12/18.
//  Copyright © 2020 Oogh. All rights reserved.
//

#import "XIOSTrack.hpp"
#include "XTrack.hpp"

@interface XIOSTrack()
{
    XTrack* _track;
}
@end

@implementation XIOSTrack

#pragma mark - Life Cycle
- (instancetype)init {
    if (self = [super init]) {
        [self setup];
    }
    return self;
}

- (void)dealloc {
    
}

#pragma mark - Public
- (void)prepare {
    
}

- (void)start {
    
}

- (void)seekTo:(long)targetPos {
    
}

- (void)pause {
    
}

- (void)stop {
    
}

#pragma mark - Private
- (void)setup {
    
}

@end
