//
//  NMCNetAddress.h
//  LSMediaCapture
//
//  Created by taojinliang on 2017/5/15.
//  Copyright © 2017年 NetEase. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface NMCNetAddress : NSObject

/*
 * 获取当前设备ip地址
 */
+ (NSString *)deviceIPAdress;


/*
 * 获取当前设备网关地址
 */
+ (NSString *)getGatewayIPAddress;


/*
 * 通过域名获取服务器DNS地址
 */
+ (NSArray *)getDNSsWithDormain:(NSString *)hostName;


/*
 * 获取本地网络的DNS地址
 */
+ (NSArray *)getDNSServers;

/**
 * 格式化IPV6地址
 */
+(NSString *)formatIPV6Address:(struct in6_addr)ipv6Addr;

/**
 判断是否是IP地址
 */
+ (BOOL)isIPAddress:(NSString *)host;

/**
 判断是否是ipv6网络
 */
+ (BOOL)isIPV6:(NSString *)host;
@end
