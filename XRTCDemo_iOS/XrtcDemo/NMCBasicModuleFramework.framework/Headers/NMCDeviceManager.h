//
//  NMCDeviceManager.h
//  LSMediaCapture
//
//  Created by taojinliang on 16/10/1.
//  Copyright © 2016年 NetEase. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CoreGraphics/CoreGraphics.h>

typedef enum NetWorkType{
    NetWorkTypeWWAN,
    NetWorkTypeWIFI,
    NetWorkType2G,
    NetWorkType3G,
    NetWorkType4G,
}NetWorkType;

@interface NMCDeviceManager : NSObject
/**
 * 获取SIM运营商信息
 */
+(NSString *) getOperators;
/**
 * 获取设备IP地址
 */
+(NSString *) getIPAddress;
/**
 * 获取设备ID
 */
+(NSString *)getDeviceID;
/**
 *获取设备类型
 */
+(NSString *)getDeviceMode;
/**
 *获取设备平台
 */
+(NSString *)getDevicePlatForm;

/**
获取当前设备的处理器和CPU主频信息
*/
+(NSString *)getDeviceScoInfo;

/**
 * 获取网络类型
 */
+(NetWorkType)getNetworkIntType;
/**
 * 获取网络类型
 */
+(NSString *) getNetworkType;
/**
 根据域名获取ip地址
 */
+(NSString*)getIPV4AddressByHostName:(NSString*)strHostName;
/**
 是否是ip地址
 */
+(BOOL)isIPAddress:(NSString *)host;

/**
 获取当前时间字符串
 
 @return 获取当前时间字符串
 */
+ (NSString *)currentTimestamp;

/**
 获取系统版本号
 
 @return 系统版本号字符串
 */
+ (NSString *)systemVersion;


/**
 获取设备电量
 
 @return 设备电量（0 - 100）
 */
+ (NSInteger)deviceBatteryLevel;

/**
 获取网络数据流量
 
 @return 网络数据流量 [@(接收数据流量), @(发送数据流量)]
 */
- (NSArray *)networkDataTraffic;

/**
 获取当前应用的CPU占用率
 */
+(CGFloat)getAppCpuUsage;

/**
 获取当前应用所有线程的CPU占用率
 */
+(NSArray*)getAppThreadCpuUsage;

/**
 获取整个设备的CPU占用率
 */
+(CGFloat)getDeviceCpuUsage;

/**
 CPU核数
 */
+ (NSUInteger)getCpuNumber;

/**
 CPU 频率
 */
+ (NSUInteger)getCpuFrequency;

/**
 CPU 最大频率
 */
+ (NSUInteger)getCpuMaxFrequency;

/**
 CPU 最小频率
 */
+ (NSUInteger)getCpuMinFrequency;

/**
 获取当前 App Memory 的使用情况(MB)
 */
+ (NSUInteger)getAppResidentMemory;

/**
 获取设备所有物理内存大小(MB)
 */
+(NSUInteger)getDevicePhysicalMemory;

/**
 获取当前设备的 Memory 使用情况(MB)
 */
+(int64_t)getDeviceUsedMemory;

/**
 获取当前设备可用的 Memory(MB)
 */
+ (uint64_t)getDeviceAvailableMemory;

//活跃内存
+ (NSUInteger)getActiveMemory;

//不活跃内存
+ (NSUInteger)getInactiveMemory;

//剩余内存
+ (NSUInteger)getFreeMemory;

//已用内存
+ (NSUInteger)getUsedMemory;

//存放内核和数据结构的内存
+ (NSUInteger)getWiredMemory;

//可释放的内存
+ (NSUInteger)getPurgeableMemory;
@end
