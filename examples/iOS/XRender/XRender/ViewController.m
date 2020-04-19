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

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
}

- (IBAction)onStartClick:(id)sender {
    NSString *filename = [[NSBundle mainBundle] pathForResource:@"jieqian_720x1280" ofType:@"rgba"];
    [self.displayView setInput:filename];
    [self.displayView start];
}


@end
