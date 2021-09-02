//
//  XRTCInitParam.h
//  XRTC
//
//  Created by xrtc on 2021/6/30.
//  Copyright © 2021 xrtc. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface XRTCInitParam : NSObject

@property (nonatomic, strong) NSString* appId;
@property (nonatomic, strong) NSString* token;
@property (nonatomic, strong) NSString* domain;
@property (nonatomic, strong) NSString* secretKey;

@end

