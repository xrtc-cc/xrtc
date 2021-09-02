//
//  NMCHardwareUsageMonitor.h
//  NMC
//
//  Created by Sampson on 2019/3/22.
//  Copyright © 2019 Netease. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "NMCHardwareUsage.h"

NS_ASSUME_NONNULL_BEGIN

@class NMCHardwareUsageInfo;

@protocol NMCHardwareUsageInfoHandler <NSObject>

- (void)onNMCHardwareUsageInfoDidReceive:(NMCHardwareUsageInfo *)info;

@end

@interface NMCHardwareUsageMonitor : NSObject

+ (instancetype)sharedInstance;

- (void)registerMonitorHandler:(id<NMCHardwareUsageInfoHandler>)handler duration:(NSTimeInterval)duration;
- (void)unregisterMonitorHandler:(id<NMCHardwareUsageInfoHandler>)handler;

- (void)unregisterAllMonitorHandlers;

@end

@interface NMCHardwareUsageInfo : NSObject

@property (nonatomic, assign) float appCpuUsage; // in %,
@property (nonatomic, assign) float deviceCpuUsage; // in %,
@property (nonatomic, assign) double usedMemory; // in KBs
@property (nonatomic, assign) double totalMembory; // in KBs
@property (nonatomic, readonly) float batteryLevel; // in %,

@end

NS_ASSUME_NONNULL_END
