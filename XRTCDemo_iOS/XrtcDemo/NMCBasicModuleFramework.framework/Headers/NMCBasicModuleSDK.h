//
//  NMCBasicModuleSDK.h
//  NMCBasicModule
//
//  Created by taojinliang on 2020/5/22.
//  Copyright © 2020 taojinliang. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface NMCBasicModuleSDK : NSObject

/// 单例
+ (instancetype)sharedInstance;

/// SDK版本
- (NSString *)sdkVersion;

/// SDK Git版本
- (NSString *)sdkGitVersion;
@end

NS_ASSUME_NONNULL_END
