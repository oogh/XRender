//
//  XIOSMediaCore.mm
//  XRender
//
//  Created by Oogh on 2020/12/18.
//  Copyright © 2020 Oogh. All rights reserved.
//

#import "XIOSMediaCore.hpp"

static InitParams sParams;

@implementation XIOSMediaCore

#pragma mark - Public

+ (void)setup:(InitParams)params {
    sParams = params;
}

+ (NSString*)getResPath {
    return sParams.resPath;
}

@end
