//
//  ViewController.m
//  XRender
//
//  Created by Oogh on 2020/4/9.
//  Copyright © 2020 oogh. All rights reserved.
//

#import "ViewController.h"
#import "XView.h"

@interface ViewController ()
@property (weak, nonatomic) IBOutlet XView *displayView;
@property (weak, nonatomic) IBOutlet UISlider *slider;

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    self.slider.minimumValue = 0.0f;
    self.slider.maximumValue = 15000.0f;
    self.slider.value = 0.0f;
}

- (IBAction)onOncePressClick:(id)sender {
    NSString *filename = [[NSBundle mainBundle] pathForResource:@"douyin_700x1240" ofType:@"mp4"];
    [self.displayView setInput:filename];
    [self.displayView start];
    
}
- (IBAction)onAddClick:(UIButton *)sender {
    NSString *filename = [[NSBundle mainBundle] pathForResource:@"douyin_700x1240" ofType:@"mp4"];
    [self.displayView setInput:filename];
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

- (IBAction)onProgressEnd:(UISlider *)sender {
}

- (IBAction)onProgressChanged:(UISlider *)sender {
    [self.displayView seekTo:(long)sender.value];
}

@end
