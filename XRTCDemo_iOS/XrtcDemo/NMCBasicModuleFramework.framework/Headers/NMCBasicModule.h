//
//  NMCHeader.h
//  NMCBasicModule
//
//  Created by taojinliang on 2017/7/4.
//  Copyright © 2017年 Netease. All rights reserved.
//

#ifndef NMCHeader_h
#define NMCHeader_h

//版本号
#import "NMCBasicModuleGitVersion.h"

//NMC类别 category
#import "NMCCategoryHeader.h"

//日志模块
#import "NMCCocoaLumberjack.h"

//录制模块
#import "NMCAVAssetMovWriter.h"
#import "NMCMp4Recorder.h"

//dispatch
#import "NMCDispatch.h"

//zip压缩
#import "NMCZipArchive.h"

#if !TARGET_OS_OSX
//数据搜集启动模块
#import "NMCDTR.h"

//定位模块
#import "NMCLocationManager.h"

//测速模块
#import "NMCNetwork.h"

//网络模块
#import "NMC_Reachability.h"
#import "NMCNetWorkManager.h"

//设备模块
#import "NMCDeviceManager.h"
#import "NMCNetAddress.h"
#import "NMCHardwareUsage.h"
#import "NMCHardwareUsageMonitor.h"

#endif



#endif /* NMCHeader_h */
