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

- (IBAction)onStartClick:(id)sender {
    NSString *filename = [[NSBundle mainBundle] pathForResource:@"jieqian_720x1280" ofType:@"mp4"];
    [self.displayView setInput:filename];
    [self.displayView start];
    
}

- (IBAction)onProgressBegin:(UISlider *)sender {
}

- (IBAction)onProgressEnd:(UISlider *)sender {
}

- (IBAction)onProgressChanged:(UISlider *)sender {
    [self.displayView seekTo:(long)sender.value];
}

@end
