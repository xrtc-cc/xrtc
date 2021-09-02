//
//  Common.h
//  xrtc
//
//  Created by 陈飞同 on 2019/8/22.
//  Copyright © 2019 陈飞同. All rights reserved.
//

#import <Foundation/Foundation.h>

#define UI_NAVIGATION_BAR_HEIGHT        44.0f
#define UI_STATUS_BAR_HEIGHT            [UIApplication sharedApplication].statusBarFrame.size.height

#define screen_amplify_x (([[UIScreen mainScreen] bounds].size.width)/375.0)//横向放大

NS_ASSUME_NONNULL_BEGIN

@interface XRTCCommon : NSObject

+ (void)applyMicOrcameraWithConfirm:(void(^)(id data))confirmMessage;

+ (BOOL)isIpad;

@end

NS_ASSUME_NONNULL_END
