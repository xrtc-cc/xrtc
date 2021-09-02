/*
 * Copyright (c) 2021 NetEase, Inc.  All rights reserved.
 */

#import <Foundation/Foundation.h>
#import "NERtcEngineBase.h"


NS_ASSUME_NONNULL_BEGIN

@protocol NERtcEngineDelegateEx;

/**
 初始化设置 NERtcEngine 的参数类
 */
@interface NERtcEngineContext : NSObject

/**
 应用的 App Key。在云信控制台创建应用后，可以查看对应的 App Key。
 */
@property (nonatomic, copy) NSString *appKey;

/**
 日志配置，详细信息请参考 {@link NERtcLogSetting}。
 */
@property (nonatomic, strong) NERtcLogSetting *logSetting;

/**
 NERtcEngine 的回调接口，由用户提供。
 */
@property (nonatomic, weak) id<NERtcEngineDelegateEx> engineDelegate;

/**
* 私有化服务器地址
* @note 如需启用私有化功能，请联系技术支持获取详情
*/
@property (nonatomic, strong, nullable) NERtcServerAddresses *serverAddress;

@end

NS_ASSUME_NONNULL_END
