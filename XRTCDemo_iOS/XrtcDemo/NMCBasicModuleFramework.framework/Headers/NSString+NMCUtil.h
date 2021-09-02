//
//  NSString+NMCUtil.h
//  NMCBasicModule
//
//  Created by taojinliang on 2018/10/16.
//  Copyright © 2018年 taojinliang. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface NSString (NMCUtil)
- (NSString *)nmc_MD5;

- (NSString *)nmc_stringByAppendingExt:(NSString *)ext;

//判断是否为纯整形
- (BOOL)nmc_isPureInteger;

//判断是否为浮点形：
- (BOOL)nmc_isPureFloat;

- (NSString *)nmc_urlEncodeString;

- (NSString *)nmc_urlDecodeString;

- (NSString *)nmc_asOptional;    //如果长度为0，就返回nil

@end

NS_ASSUME_NONNULL_END
