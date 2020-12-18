//
//  ViewController.m
//  XRender
//
//  Created by Oogh on 2020/4/9.
//  Copyright © 2020 oogh. All rights reserved.
//

#import "ViewController.h"
#import "XView.hpp"
#import "XIOSMediaCore.hpp"

@interface ViewController ()
@property (weak, nonatomic) IBOutlet XView *displayView;
@property (weak, nonatomic) IBOutlet UISlider *slider;
@property (weak, nonatomic) IBOutlet UITextField *currentTextField;
@property (weak, nonatomic) IBOutlet UITextField *durationTextField;

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    self.slider.value = 0;
    self.currentTextField.placeholder = @"0";
    self.durationTextField.placeholder = @"0";
    
    InitParams params = {
        .resPath = @"",
        .shaderPath = @""
    };
    
    [XIOSMediaCore setup:params];
    
    __weak typeof(self) weakSelf = self;
    self.displayView.progressChangeCallback = ^(long current, long duration) {
        __strong typeof(weakSelf) strongSelf = weakSelf;
        dispatch_async(dispatch_get_main_queue(), ^{
            strongSelf.slider.maximumValue = duration;
            strongSelf.slider.value = current;
            
            strongSelf.currentTextField.text = [NSString stringWithFormat: @"%ld", current];
            strongSelf.durationTextField.text = [NSString stringWithFormat: @"%ld", duration];;
        });
    };
}

- (IBAction)onOncePressClick:(UIButton *)sender {
        NSString *filename = [[NSBundle mainBundle] pathForResource:@"douyin_700x1240" ofType:@"mp4"];
        [self.displayView setInput:filename];
        [self.displayView start];
}

- (IBAction)onSetInputClick:(UIButton *)sender {
    NSString *filename = [[NSBundle mainBundle] pathForResource:@"douyin_700x1240" ofType:@"mp4"];
        [self.displayView setInput:filename];
}


- (IBAction)onPrepareClick:(UIButton *)sender {
    
}

- (IBAction)onStartClick:(UIButton *)sender {
    [self.displayView start];
}

- (IBAction)onPauseClick:(UIButton *)sender {
    [self.displayView pause];
}

- (IBAction)onStopClick:(UIButton *)sender {
    [self.displayView stop];
}

- (IBAction)onProgressBegin:(UISlider *)sender {
}

- (IBAction)onProgressChanged:(UISlider *)sender {
    [self.displayView seekTo:(long)sender.value];
}

- (IBAction)onProgressEnd:(UISlider *)sender {
}

@end
