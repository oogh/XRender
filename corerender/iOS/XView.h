//
//  XView.h
//  XRender
//
//  Created by Oogh on 2020/04/17.
//  Copyright © 2020 Oogh. All rights reserved.
//

#ifndef XView_h
#define XView_h

#import <UIKit/UIKit.h>

typedef void (^OnProgressChangeCallback)(long current, long duration);


@interface XView : UIView

@property (nonatomic, copy) OnProgressChangeCallback progressChangeCallback;

- (void)setInput:(NSString*)filename;

- (void)start;

- (void)seekTo:(long)targetPos;

- (void)pause;

- (void)stop;

@end

#endif /* XView_h */
