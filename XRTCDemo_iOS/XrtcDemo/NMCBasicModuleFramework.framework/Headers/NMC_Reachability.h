//
//  NMC_Reachability.h
//  NMCBasicModule
//
//  Created by taojinliang on 2018/4/25.
//  Copyright © 2018年 taojinliang. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <SystemConfiguration/SystemConfiguration.h>
#import <netinet/in.h>
#import <CoreTelephony/CTCarrier.h>


// Reachability is available for iOS 7 or higher, fully support IPv6.
typedef NS_ENUM(NSInteger, NMCNetworkStatus) {
    NMCNetworkStatusNotReachable,     // 未检测到
    NMCNetworkStatusNotSure,          // 无法确定
    NMCNetworkStatusReachableVia2G,   // 2G
    NMCNetworkStatusReachableVia3G,   // 3G
    NMCNetworkStatusReachableVia4G,   // 4G
    NMCNetworkStatusReachableVia5G,   // 5G
    NMCNetworkStatusReachableViaWiFi, // WiFi
    NMCNetworkStatusReachableViaWWAN, // WWAN not sure, should not use
};

typedef NS_ENUM(NSInteger, NMCMAMCarrier) {
    NMCMAMOperatorChinaMobile,    // 中国移动
    NMCMAMOperatorChinaUnicom,    // 中国联通
    NMCMAMOperatorChinaTelecom,   // 中国电信
    NMCMAMOperatorOther,          // 其他
    NMCMAMOperatorNone,           // None，无 sim 卡
};



extern NSString *nmc_kReachabilityChangedNotification;


@interface NMC_Reachability : NSObject
// 当前网络类型
@property (nonatomic, readonly) NMCNetworkStatus currentNetworkStatus;
// 变化之前网络类型（当网络没有变化时，值为 NMCNetworkStatusNotSure；变化一次后，值为变化之前的网络类型）
@property (nonatomic, readonly) NMCNetworkStatus oldNetworkStatus;
// 当前运营商
@property (nonatomic, readonly) NMCMAMCarrier currentCarrier;
@property (nonatomic, readonly) CTCarrier *currentCTCarrier;

/*!
 * Use to check the reachability of a given host name.
 */
+ (instancetype)nmc_reachabilityWithHostName:(NSString *)hostName;

/*!
 * Use to check the reachability of a given IP address.
 */
+ (instancetype)nmc_reachabilityWithAddress:(const struct sockaddr_in *)hostAddress;

/*!
 * Checks whether the default route is available. Should be used by applications that do not connect to a particular host.
 */
+ (instancetype)nmc_reachabilityForInternetConnection;

/*!
 * Checks whether a local WiFi connection is available.
 */
+ (instancetype)nmc_reachabilityForLocalWiFi;

/*!
 * Start listening for reachability notifications on the current run loop.
 */
- (BOOL)nmc_startNotifier;
- (void)nmc_stopNotifier;

- (NMCNetworkStatus)nmc_currentReachabilityStatus;

/*!
 * WWAN may be available, but not active until a connection has been established. WiFi may require a connection for VPN on Demand.
 */
- (BOOL)nmc_connectionRequired;

+ (BOOL)isWiFi;

@end
