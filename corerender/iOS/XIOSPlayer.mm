//
//  XIOSPlayer.mm
//  XRender
//
//  Created by Oogh on 2020/12/18.
//  Copyright © 2020 Oogh. All rights reserved.
//

#import "XIOSPlayer.hpp"
#import "XIOSTimeline.hpp"
#include "XPlayer.hpp"

@interface XIOSPlayer()
{
    std::shared_ptr<XPlayer> _player;
}
@end

@implementation XIOSPlayer

#pragma mark - Life Cycle
- (instancetype)init {
    if (self = [super init]) {
        _player = std::make_shared<XPlayer>();
    }
    return self;
}

- (void)dealloc {
    if (_player) {
        _player.reset();
    }
}

#pragma mark - Public
- (void)setTimeline:(XIOSTimeline *)timeline {
    _player->setTimeline([timeline getNativeTimeline]);
}

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
