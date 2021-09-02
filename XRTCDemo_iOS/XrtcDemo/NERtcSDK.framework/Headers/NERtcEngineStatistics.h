/*
 * Copyright (c) 2021 NetEase, Inc.  All rights reserved.
 */

#import <Foundation/Foundation.h>
#import "NERtcEngineEnum.h"

NS_ASSUME_NONNULL_BEGIN

/**
 通话相关的统计信息
 */
@interface NERtcStats : NSObject

/**
 发送字节数（bytes），累计值
 */
@property (nonatomic, assign) int64_t txBytes;

/**
 接收字节数（bytes），累计值
 */
@property (nonatomic, assign) int64_t rxBytes;

/**
 当前 App 的 CPU 使用率 (%)
 */
@property (nonatomic, assign) uint32_t cpuAppUsage;

/**
 当前系统的 CPU 使用率 (%)
 */
@property (nonatomic, assign) uint32_t cpuTotalUsage;

 /**
 * 当前 App 的内存占比 (%) , 占最大可用内存
 */
@property (nonatomic, assign) uint32_t memoryAppUsageRatio;
 
/**
 * 当前系统的内存占比 (%)
 */
@property (nonatomic, assign) uint32_t memoryTotalUsageRatio;
 
/**
 * 当前 App 的内存大小 (KB)
 */
@property (nonatomic, assign) uint32_t memoryAppUsageInKBytes;
 
/**
 * 自加入房间的通话时长 ， 退出后再加入重新计时 ( 单位：S)
 */
@property (nonatomic, assign) uint64_t totalDuration;
 
/**
 * 自加入房间后累计的发送的音频字节数（Byte）
 */
@property (nonatomic, assign) uint64_t txAudioBytes;
 
/**
 * 自加入房间后累计的发送的视频字节数（Byte）
 */
@property (nonatomic, assign) uint64_t txVideoBytes;
 
 
/**
 * 自加入房间后累计的接收的音频字节数（Byte）
 */
@property (nonatomic, assign) uint64_t rxAudioBytes;
 
 
/**
 * 自加入房间后累计的接收的视频字节数（Byte）
 */
@property (nonatomic, assign) uint64_t rxVideoBytes;
 
/**
 * 音频接收码率（kbps）
 */
@property (nonatomic, assign) uint64_t rxAudioKBitRate;
 
/**
 * 视频接收码率（kbps）
 */
@property (nonatomic, assign) uint64_t rxVideoKBitRate;
 
/**
 * 音频发送码率（kbps）
 */
@property (nonatomic, assign) uint64_t txAudioKBitRate;
 
/**
 * 视频发送码率（kbps）
 */
@property (nonatomic, assign) uint64_t txVideoKBitRate;
 
/**
 * 上行平均往返时延(ms)
 */
@property (nonatomic, assign) uint64_t upRtt;
 
/**
 * 下行平均往返时延(ms)
 */
@property (nonatomic, assign) uint64_t downRtt;

/**
 * 本地上行音频丢包率(%)
 */
@property (nonatomic, assign) uint32_t txAudioPacketLossRate;
 
/**
 * 本地上行视频实际丢包率(%)
 */
@property (nonatomic, assign) uint32_t txVideoPacketLossRate;
 
/**
 * 本地上行音频丢包数
 */
@property (nonatomic, assign) uint32_t txAudioPacketLossSum;
 
/**
 * 本地上行视频丢包数
 */
@property (nonatomic, assign) uint32_t txVideoPacketLossSum;
 
/**
 * 本地上行音频抖动 (ms)
 */
@property (nonatomic, assign) uint32_t txAudioJitter;
 
/**
 * 本地上行视频抖动 (ms)
 */
@property (nonatomic, assign) uint32_t txVideoJitter;
 
/**
 * 本地下行音频丢包率(%)
 */
@property (nonatomic, assign) uint32_t rxAudioPacketLossRate;
 
/**
 * 本地下行视频丢包率(%)
 */
@property (nonatomic, assign) uint32_t rxVideoPacketLossRate;
 
/**
 * 本地下行音频丢包数
 */
@property (nonatomic, assign) uint64_t rxAudioPacketLossSum;
 
/**
 * 本地下行视频丢包数
 */
@property (nonatomic, assign) uint32_t rxVideoPacketLossSum;
 
/**
 * 本地下行音频抖动 (ms)
 */
@property (nonatomic, assign) uint32_t rxAudioJitter;
 
/**
 * 本地下行视频抖动 (ms)
 */
@property (nonatomic, assign) uint32_t rxVideoJitter;
@end

/**
 本地视频单条流统计信息
 */
@interface NERtcVideoLayerSendStats : NSObject

/**
 * 流类型： 主流（1） ， 辅流（2）
 */
@property (nonatomic, assign) int32_t layerType;

/**
 * 视频流宽
 */
@property (nonatomic, assign) int32_t width;
 
/**
 * 视频流高
 */
@property (nonatomic, assign) int32_t height;

/**
（上次统计后）发送码率(Kbps)
 */
@property (nonatomic, assign) int64_t sendBitrate;

/**
（上次统计后）发送帧率(fps)
 */
@property (nonatomic, assign) int32_t encoderOutputFrameRate;
  
/**
 * 视频采集帧率
 */
@property (nonatomic, assign) int32_t captureFrameRate;
  
/**
 * 编码器的目标码率(kbps)
 */
@property (nonatomic, assign) int32_t targetBitrate;
 
/**
 * 编码器的实际编码码率(kbps)
 */
@property (nonatomic, assign) int32_t encoderBitrate;

/**
 * 视频发送帧率
 */
@property (nonatomic, assign) int32_t sentFrameRate;

/**
 * 视频渲染帧率
 */
@property (nonatomic, assign) int32_t renderFrameRate;

/**
 * 编码器名字
 */
@property (nonatomic, copy) NSString *encoderName;

@end

/**
 本地视频流上传统计信息
 */
@interface NERtcVideoSendStats : NSObject

/**
 * 具体每条流的上行统计信息 详见 NERtcVideoLayerSendStats
 */
@property (nonatomic, strong)NSArray<NERtcVideoLayerSendStats *> *videoLayers;

@end

/**
 每条流的接收下行统计信息
 */
@interface NERtcVideoLayerRecvStats : NSObject

/**
 * 流类型： 主流（1） ， 辅流（2）
 */
@property (nonatomic, assign) int32_t layerType;

/**
 * 视频流宽
 */
@property (nonatomic, assign) int32_t width;
 
/**
 * 视频流高
 */
@property (nonatomic, assign) int32_t height;

/**
 （上次统计后）接收码率(Kbps)
 */
@property (nonatomic, assign) int64_t receivedBitrate;

/**
 * 接收到的帧率
 */
@property (nonatomic, assign) int32_t fps;

/**
 * 接收视频的丢包率
 */
@property (nonatomic, assign) int32_t packetLossRate;
  
/**
 * 解码器输出帧率
 */
@property (nonatomic, assign) int32_t decoderOutputFrameRate;
 
/**
 * 渲染帧率
 */
@property (nonatomic, assign) int32_t rendererOutputFrameRate;
 
/**
 * 接收视频卡顿累计时长（ms）， 从收到对应用户的视频算起
 */
@property (nonatomic, assign) int64_t totalFrozenTime;
 
/**
 * 接收视频的平均卡顿率， 从收到对应用户的视频算起
 */
@property (nonatomic, assign) int32_t frozenRate;

/**
 * 解码器名字
 */
@property (nonatomic, copy) NSString *decoderName;

@end


/**
 远端视频流的统计信息
 */
@interface NERtcVideoRecvStats : NSObject

/**
 用户 ID，指定是哪个用户的视频流
 */
@property (nonatomic, assign) uint64_t uid;

/**
 * 当前uid 每条流的接收下行统计信息 详见 NERtcVideoLayerRecvStats
 */
@property (nonatomic, strong) NSArray<NERtcVideoLayerRecvStats *> *videoLayers;

@end


/**
 本地音频流上传统计信息
 */
@interface NERtcAudioSendStats : NSObject

/**
 （上次统计后）发送码率(Kbps)
 */
@property (nonatomic, assign) int64_t sentBitrate;

/**
 特定时间内的音频丢包率
 */
@property (nonatomic, assign) int16_t lossRate;


@property (nonatomic, assign) int64_t rtt;

/**
 音量等级(0-100)
*/
@property (nonatomic, assign) int volume;

/**
 * 本地音频采集声道数
 */
@property (nonatomic, assign) int16_t numChannels;
 
/**
 * 本地音频采样率（Hz）
 */
@property (nonatomic, assign) int32_t sentSampleRate;
@end

/**
 远端用户的音频统计
 */
@interface NERtcAudioRecvStats : NSObject

/**
 用户 ID，指定是哪个用户的音频流
 */
@property (nonatomic, assign) uint64_t uid;

/**
 （上次统计后）接收码率(Kbps)
 */
@property (nonatomic, assign) int64_t receivedBitrate;


/**
 特定时间内的音频丢包率
 */
@property (nonatomic, assign) int16_t audioLossRate;

/**
 音量等级(0-100)
*/
@property (nonatomic, assign) int volume;

/**
 * 音频卡顿累计时长，从收到远端用户音频算起
 */
@property (nonatomic, assign) int64_t totalFrozenTime;
 
/**
 * 平均音频卡顿率，从收到远端用户音频算起
 */
@property (nonatomic, assign) int32_t frozenRate;
@end

/**
 用户的网络质量
*/
@interface NERtcNetworkQualityStats : NSObject

/**
 用户 ID
*/
@property (nonatomic, assign) uint64_t userId;

/**
 上行的网络质量
*/
@property (nonatomic, assign) NERtcNetworkQuality txQuality;

/**
 下行的网络质量
*/
@property (nonatomic, assign) NERtcNetworkQuality rxQuality;

@end

/**
 统计信息回调
 */
@protocol NERtcEngineMediaStatsObserver<NSObject>

@optional

/**
 当前通话统计回调
 SDK 定期向 App 报告当前通话的统计信息，每 2 秒触发一次。
 
 @param stat NERTC 引擎统计数据: NERtcStats
 */
-(void)onRtcStats:(NERtcStats *)stat;

/**
 本地音频流统计信息回调
 该回调描述本地设备发送音频流的统计信息，每 2 秒触发一次。
 
 @param stat 本地音频流统计信息。详见 NERtcAudioSendStats.
 */
- (void)onLocalAudioStat:(NERtcAudioSendStats *)stat;

/**
 通话中远端音频流的统计信息回调。
 该回调描述远端用户在通话中端到端的音频流统计信息，每 2 秒触发一次。
 
 @param stats 每个远端用户音频统计信息的数组。详见 NERtcAudioRecvStats.
 */
- (void)onRemoteAudioStats:(NSArray<NERtcAudioRecvStats*> *)stats;

/**
 本地视频流统计信息回调
 该回调描述本地设备发送视频流的统计信息，每 2 秒触发一次。
 
 @param stat 本地视频流统计信息。详见 NERtcVideoSendStats.
 @note 纯音频SDK禁用该接口，如需使用请前往云信官网下载并替换成视频SDK
 */
-(void)onLocalVideoStat:(NERtcVideoSendStats *)stat;

/**
 通话中远端视频流的统计信息回调
 该回调描述远端用户在通话中端到端的视频流统计信息，每 2 秒触发一次。
 
 @param stats 远端用户视频统计信息的数组。详见 NERtcVideoRecvStats.
 @note 纯音频SDK禁用该接口，如需使用请前往云信官网下载并替换成视频SDK
 */
- (void)onRemoteVideoStats:(NSArray<NERtcVideoRecvStats*> *)stats;

/**
 通话中每个用户的网络上下行质量报告回调
 该回调描述每个用户在通话中的网络状态，每 2 秒触发一次。
 
 @param stats 用户网络状态数组
 */
- (void)onNetworkQuality:(NSArray<NERtcNetworkQualityStats *> *)stats;

@end



NS_ASSUME_NONNULL_END
