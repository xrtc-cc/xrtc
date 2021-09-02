//
//  NMCDDCallbackLogger.h
//  NMCBasicModule
//
//  Created by Netease on 2018/12/24.
//  Copyright © 2018年 taojinliang. All rights reserved.
//

#import "NMCDDLog.h"

NS_ASSUME_NONNULL_BEGIN

typedef void(^NMCDDLogCallback)(NSString *messageString);

@interface NMCDDCallbackLogger : NMCDDAbstractLogger <NMCDDLogger>

//zgn sdk前缀，默认是 "log_ios_default_"，不同的SDK务必设置不同的文件名，防止出错
@property(nonatomic, copy) NSString *sdkName;

@property (nonatomic, copy) NMCDDLogCallback loggerCallBack;

@property (nonatomic, readwrite, assign) BOOL automaticallyAppendNewlineForCustomFormatters;

- (instancetype)initWithSdkName:(NSString *)sdkName;

@end

NS_ASSUME_NONNULL_END
