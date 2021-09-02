//
//  NMCNetWorkManager.h
//  NMCBasicModule
//
//  Created by taojinliang on 2018/4/25.
//  Copyright © 2018年 taojinliang. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "NMC_Reachability.h"

@interface NMCNetWorkManager : NSObject
+(instancetype)shareInstance;
//获取当前网络类型
-(NMCNetworkStatus)currentNetworkStatus;
//是否离线
+ (BOOL)isOffline;
//是否是运营商网络
+ (BOOL)isWWAN;
//是否是wifi网络
+ (BOOL)isWifi;

+ (NMC_Reachability *)reachability;

@end
