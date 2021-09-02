//
//  NERtcEngine.h
//  NERtcSDK
//
//  Created by Sampson on 2019/4/25.
//  Copyright © 2019 Netease. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "INERtcEngineEx.h"
#import "NERtcEngineErrorCode.h"

NS_ASSUME_NONNULL_BEGIN


/**
 NERtcEngine 类
 */
@interface NERtcEngine : NSObject <INERtcEngineEx>

/**
 获取 NERtcEngine 实例
 获取之后一直到调用destroyEngine之前，返回的实例都将是同一个

 @return NERtcEngine 实例
 */
+ (instancetype)sharedEngine;

/**
 释放当前的 NERtcEngine 实例
 建议在 App 确定不再需要使用 NERtcEngine 实例时，通过该接口释放 NERtcEngine 实例的对象资源；
 
 1. 该接口的工作方式为同步调用方式，必须在子线程中才能调用，否则会调用失败；如：
 dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
     [NERtcEngine destroyEngine];
 });
 2. 该接口不得在 SDK 的回调中调用，在接口返回前也不允许调用 SDK 的其他任何接口；
 3. 接口调用返回之后，如果需要再次使用 SDK，可以重新调用 sharedEngine 来获取一个新的 NERtcEngine 实例。

 */
+ (int)destroyEngine;

@end

NS_ASSUME_NONNULL_END
