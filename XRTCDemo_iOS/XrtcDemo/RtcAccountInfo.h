//
//  RtcAccountInfo.h
//  xrtc
//
//  Created by  xrtc on 21-07-10.
//  Copyright (c) 2021年 xrtc. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface RtcAccountInfo : NSObject

+ (id) sharedInstance;


- (NSString *) getAgoraAppId;
- (NSString *) getHuaWeiAppId;
- (NSString *) getTengXunAppId;
- (NSString *) getWangYiAppId;
- (NSString *) getToken;
- (NSString *) getDomain;
- (NSString *) getSecretKey;

@end
