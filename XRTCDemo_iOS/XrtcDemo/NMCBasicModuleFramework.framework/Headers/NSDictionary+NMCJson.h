//
//  NSDictionary+NMCJson.h
//  NMCBasicModule
//
//  Created by taojinliang on 2018/10/17.
//  Copyright © 2018年 taojinliang. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface NSDictionary (NMCJson)
- (NSString *)nmc_JsonString: (NSString *)key;

- (NSDictionary *)nmc_JsonDict: (NSString *)key;
- (NSArray *)nmc_JsonArray: (NSString *)key;
- (NSArray *)nmc_JsonStringArray: (NSString *)key;

- (BOOL)nmc_JsonBool: (NSString *)key;
- (BOOL)nmc_jsonBool:(id)key
        defaultValue:(BOOL)value;

- (NSInteger)nmc_JsonInteger: (NSString *)key;
- (long long)nmc_JsonLongLong: (NSString *)key;
- (unsigned long long)nmc_JsonUnsignedLongLong:(NSString *)key;

- (double)nmc_JsonDouble: (NSString *)key;
@end

NS_ASSUME_NONNULL_END
