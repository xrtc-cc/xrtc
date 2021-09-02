//
//  NMCNetworkDiagnoserCommon.h
//  LSMediaCapture
//
//  Created by taojinliang on 2017/5/15.
//  Copyright © 2017年 NetEase. All rights reserved.
//

#ifndef NMCNetworkDiagnoseCommon_h
#define NMCNetworkDiagnoseCommon_h

#import "NMCCocoaLumberjack.h"

#import <Foundation/Foundation.h>

#import <sys/socket.h>
#import <netinet/in.h>
#import <netinet/tcp.h>
#import <arpa/inet.h>

#define NMCSpeedStartNotification @"NMCSpeedStartNotification"
#define NMCSpeedStopNotification @"NMCSpeedStopNotification"


@interface NMCSpeed : NSObject
@property(nonatomic, strong) NSString *url;
@property(nonatomic, strong) NSString *method;
@property(nonatomic, strong) NSString *type;
@property(nonatomic, strong) NSString *region;
@end

@interface NMCSpeedUpload : NSObject
@property(nonatomic, strong) NMCSpeed *speed;
@property(nonatomic, assign) BOOL isSuccess;
@property(nonatomic, assign) unsigned long long uploadSize;
@property(nonatomic, assign) float uploadTime;
@property(nonatomic, assign) float uploadSpeed;//uploadSize/uploadTime
@end

/**
 测速辅助类
 */
@interface NMCSpeedCaclUtil:NSObject
@property(nonatomic, strong) NSString *speedUrl;//测速url
@property(nonatomic, strong) NSDate *speedDate;//测速时间
@property(nonatomic, strong) NSString *optimalUrl;//最优推流地址
@property(nonatomic, strong) NSString *speedCdnType;//测速结果获取的cdnType
@property(nonatomic, assign) BOOL isAddNetNotif;//添加网络监听通知
@property(nonatomic, strong) NSDate *netChangedDate;//网络变化时间
@property(nonatomic, assign) BOOL isNetChanged;//网络是否变化
-(void)unInitSpeedCaclUtil;
@end

typedef NS_ENUM( NSInteger, NMCCmdType){
    NMCCmdTypeHttpGet = 0,
    NMCCmdTypeHttpPost = 1,
    NMCCmdTypePing = 2,
    NMCCmdTypeTraceRoute = 3,
    NMCCmdTypeContinuousPing = 4
};


@interface NMCCmd : NSObject
@property(nonatomic, strong) NSString *cmdName;//httpget httppost ping traceroute
@property(nonatomic, strong) NSString *url;//httpget httppost
@property(nonatomic, assign) float timeout;//httpget httppost traceroute
@property(nonatomic, assign) NSInteger fileSize;//httppost
@property(nonatomic, strong) NSString *host;//ping traceroute
@property(nonatomic, assign) float waitTime;//ping
@property(nonatomic, assign) NSInteger count;//ping
@property(nonatomic, assign) NMCCmdType type;
@property(nonatomic, assign) BOOL isIPv6;
@property(nonatomic, strong) NSString *hostIP;//ping traceroute
@property(nonatomic, assign) NSInteger uploadPeriod;//continuous_ping
@end


@interface NMCDiagnoseRecord : NSObject
@property (nonatomic) BOOL success;
@property (nonatomic) NSString *ip;
@property (nonatomic) NSArray<NSNumber *> *recvDurations;
@property (nonatomic) NSInteger total;
@property (nonatomic) NSInteger ttl;
@property (nonatomic) NSString *result;
@property (nonatomic) NSDictionary *errorInfo;
@property (nonatomic) NMCCmd *cmd;
@property (nonatomic) NSInteger startTime;
@property (nonatomic) NSArray<NSDictionary *> *seqDurations;
@end

typedef void(^NMCNetworkDiagnoseResultCallback)(NMCDiagnoseRecord* record);
typedef void(^NMCTraceRouteDiagnoseResultCallback)(NSArray* records);
typedef void(^NMCFileUploadPostEmptyCallback)(NSInteger statusCode);
typedef void(^NMCFileUploadResultCallBack)(NMCSpeedUpload *upload);
typedef void(^NMCFileUploadManagerResultCallBack)(NSArray* uploads);

#pragma mark - Structures

// ICMP数据报结构
typedef struct NMCICMPPacket {
    uint8_t     type;
    uint8_t     code;
    uint16_t    checksum;
    uint16_t    identifier;
    uint16_t    sequenceNumber;
    // data...
} NMCICMPPacket;

// IP数据报结构
typedef struct NMCIPHeader {
    uint8_t versionAndHeaderLength;
    uint8_t differentiatedServices;
    uint16_t totalLength;
    uint16_t identification;
    uint16_t flagsAndFragmentOffset;
    uint8_t timeToLive;
    uint8_t protocol;
    uint16_t headerChecksum;
    uint8_t sourceAddress[4];
    uint8_t destinationAddress[4];
    // options...
    // data...
} NMCIPHeader;

// ICMPv4报文类型
typedef enum NMCICMPv4Type {
    kNMCICMPv4TypeEchoReply = 0, // 回显应答
    kNMCICMPv4TypeEchoRequest = 8, // 回显请求
    kNMCICMPv4TypeTimeOut = 11, // 超时
}ICMPv4Type;

// ICMPv6报文类型
typedef enum NMCICMPv6Type {
    kNMCICMPv6TypeEchoReply = 129, // 回显应答
    kNMCICMPv6TypeEchoRequest = 128, // 回显请求
    kNMCICMPv6TypeTimeOut = 3, // 超时
}ICMPv6Type;

#pragma mark - Functions

/**
 计算校验和
 */
uint16_t NMCNetDiagMakeChecksum(const void *buffer, size_t bufferLen);

/**
 创建ICMP数据报
 */
NSData *NMCNetDiagBuildPacket(uint16_t seq, uint16_t identifier, NSData *payload, BOOL isICMPv6);

/**
 解析ICMP数据报
 */
NMCICMPPacket *NMCNetDiagGetICMPFromPacket(char *packet, int len);

/**
 校验是否为有效的ICMP数据
 */
BOOL NMCNetDiagIsValidResponse(char *buffer, int len, int seq, int identifier);

/**
 创建sockaddr结构体
 */
struct sockaddr *NMCNetDiagBuildSockaddr(const void *host, int port, BOOL isIPv6);

/**
 创建定时器

 @param interval 每隔几秒执行一次
 @param queue 执行线程
 @param block 定时器回调
 @return 定时器
 */
dispatch_source_t NMCCreateDispatchStatisticTimer(double interval, dispatch_queue_t queue, dispatch_block_t block);

#endif /* NMCNetworkDiagnoseCommon_h */
