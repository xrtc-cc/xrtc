//
//  XRTCStatsInfo.h
//  XRTC
//
//  Created by xrtc on 2021/6/25.
//  Copyright © 2021 xrtc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

/**
 本地统计数据
 */
@interface XRTCLocalRtcStats: NSObject
@property (assign, nonatomic) NSUInteger uid;
@property (assign, nonatomic) NSUInteger audioSendBytes;
@property (assign, nonatomic) NSUInteger videoSendBytes;
@property (assign, nonatomic) NSUInteger audioSendBytesNow;
@property (assign, nonatomic) NSUInteger videoSendBytesNow;
@end

@interface XRTCLocalAudioStats: NSObject
@property (assign, nonatomic) NSUInteger uid;
@property (assign, nonatomic) NSUInteger audioSendPackets;
@property (assign, nonatomic) NSUInteger audioSendPacketsLost;
@property (assign, nonatomic) NSUInteger audioSentBitrate;
@property (assign, nonatomic) NSUInteger audiosentSampleRate;
@property (assign, nonatomic) NSUInteger audioSendBytes;
@end

@interface XRTCLocalVideoStats: NSObject
@property (assign, nonatomic) NSUInteger uid;
@property (assign, nonatomic) NSUInteger videoSendPackets;
@property (assign, nonatomic) NSUInteger videoSendPacketsLost;
@property (assign, nonatomic) NSUInteger videoSentBitrate;
@property (assign, nonatomic) NSUInteger videoSendFrameRate;
@property (assign, nonatomic) NSUInteger videoSendResolutionWidth;
@property (assign, nonatomic) NSUInteger videoSendResolutionHeight;
@property (assign, nonatomic) NSUInteger videoSendBytes;
@end

/**
 远端统计数据
 */
@interface XRTCRemoteRtcStats: NSObject
@property (assign, nonatomic) NSUInteger uid;
@property (assign, nonatomic) NSUInteger audioReceiveBytes;
@property (assign, nonatomic) NSUInteger videoReceiveBytes;
@property (assign, nonatomic) NSUInteger audioReceiveBytesNow;
@property (assign, nonatomic) NSUInteger videoReceiveBytesNow;
@end

@interface XRTCRemoteAudioStats: NSObject
@property (assign, nonatomic) NSUInteger uid;
@property (assign, nonatomic) NSUInteger audioReceiveBitrate;
@property (assign, nonatomic) NSUInteger audioReceiveDelay;
@property (assign, nonatomic) NSUInteger endToEndDelay;
@property (assign, nonatomic) NSUInteger audioReceivePackets;
@property (assign, nonatomic) NSUInteger audioReceivePacketsLost;
@property (assign, nonatomic) NSUInteger audioReceivePacketsLostNow;
@property (assign, nonatomic) NSUInteger audioReceiveBytes;
@end

@interface XRTCRemoteVideoStats: NSObject
@property (assign, nonatomic) NSUInteger uid;
@property (assign, nonatomic) NSUInteger videoReceiveDelay;
@property (assign, nonatomic) NSUInteger videoReceivePacketsLost;
@property (assign, nonatomic) NSUInteger videoReceivePacketsLostNow;
@property (assign, nonatomic) NSUInteger videoReceivePackets;
@property (assign, nonatomic) NSUInteger videoReceiveFrameRate;
@property (assign, nonatomic) NSUInteger videoReceiveDecodeFrameRate;
@property (assign, nonatomic) NSUInteger videoReceiveResolutionWidth;
@property (assign, nonatomic) NSUInteger videoReceiveResolutionHeight;
@property (assign, nonatomic) NSUInteger videoReceiveBytes;
@end

NS_ASSUME_NONNULL_END
