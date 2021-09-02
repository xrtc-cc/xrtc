//
//  RtcAccountInfo.m
//  xrtc
//
//  Created by  xrtc on 21-07-10.
//  Copyright (c) 2021年 xrtc. All rights reserved.
//

#import "RtcAccountInfo.h"

static RtcAccountInfo *theRtcAccountInfo=nil;

@implementation RtcAccountInfo

+ (id)sharedInstance
{
    @synchronized(self)
    {
        if (theRtcAccountInfo==nil)
        {
            theRtcAccountInfo = [[RtcAccountInfo alloc] init];
        }
    }
    
    return theRtcAccountInfo;
}

+(id)allocWithZone:(NSZone *)zone
{
    if (theRtcAccountInfo==nil)
    {
        theRtcAccountInfo = [super allocWithZone:zone];
        
        return theRtcAccountInfo;
    }
    
    return nil;
}

/**
 重写这个方法
 */
- (id) init
{
    self = [super init];
    if (self)
    {
        [self initUserInfo];
    }
    
    return self;
}

#pragma mark -
#pragma mark 初始化
///////////////////////////////////////////////////////////////////////////////////////////////////
/**
 初始化用户信息
 */
- (void)initUserInfo
{
    NSDictionary* dic = [NSDictionary dictionaryWithObjectsAndKeys:
                         @"3f2e8611c431457ba2ab836f57cbfbf5",                                 @"agora_appId",
                         @"6120b750fa163e4cc9a01da073e34592",                                 @"hrtc_appId",
                         @"1400564619",                                                       @"trtc_appId",
                         @"08d76687725460110105849dce92845d",                                 @"yunxinrtc_appId",
                         @"",                                                                 @"token",
                         @"6120b750fa163e4cc9a01da073e34592.cloudrtc.myhuaweicloud.com",      @"domain",
                         @"5f4186fd12befa64f3d7db112ac4854b75eaf535d393fc289c923a7582dfe13f", @"secretKey",
                         nil];
    
    for (id key in dic)
    {
        [[NSUserDefaults standardUserDefaults] setObject:[dic objectForKey:key] forKey:key];
    }
}

#pragma mark -
#pragma mark Rtc账号相关

///////////////////////////////////////////////////////////////////////////////////////////////////
/**
 获取声网appId
 */
- (NSString *) getAgoraAppId
{
    return [[NSUserDefaults standardUserDefaults] stringForKey:@"agora_appId"];
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/**
 获取华为appId
 */
- (NSString *) getHuaWeiAppId
{
    return [[NSUserDefaults standardUserDefaults] stringForKey:@"hrtc_appId"];
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/**
 获取腾讯appId
 */
- (NSString *) getTengXunAppId
{
    return [[NSUserDefaults standardUserDefaults] stringForKey:@"trtc_appId"];
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/**
 获取云信appId
 */
- (NSString *) getWangYiAppId
{
    return [[NSUserDefaults standardUserDefaults] stringForKey:@"yunxinrtc_appId"];
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/**
 获取token
 */
- (NSString *) getToken
{
	return [[NSUserDefaults standardUserDefaults] stringForKey:@"token"];
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/**
 获取domain
 */
- (NSString *) getDomain
{
    return [[NSUserDefaults standardUserDefaults] stringForKey:@"domain"];
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/**
 获取secretKey
 */
- (NSString *) getSecretKey
{
    return [[NSUserDefaults standardUserDefaults] stringForKey:@"secretKey"];
}


@end
