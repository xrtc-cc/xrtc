//
//  NSData+NMCGZIP.h
//  NMCBasicModule
//
//  Created by taojinliang on 2018/10/16.
//  Copyright © 2018年 taojinliang. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface NSData (NMCGZIP)

- (nullable NSData *)nmc_gzippedDataWithCompressionLevel:(float)level;
- (nullable NSData *)nmc_gzippedData;
- (nullable NSData *)nmc_gunzippedData;
- (BOOL)nmc_isGzippedData;

- (NSString *)nmc_md5;
- (BOOL)nmc_writeToFile:(NSString *)filepath
                dataMD5:(NSString *)dataMD5;
@end

NS_ASSUME_NONNULL_END
