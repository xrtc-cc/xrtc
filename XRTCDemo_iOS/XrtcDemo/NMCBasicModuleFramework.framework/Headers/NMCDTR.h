//
//  NMCDTR.h
//  NMCBasicModule
//
//  Created by fenric on 2017/12/27.
//  Copyright © 2017年 taojinliang. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef NS_ENUM(NSUInteger, NMCDTRSource) {
    NMCDTRSourceStreamer,
    NMCDTRSourcePlayer,
};

@interface NMCDTR : NSObject

+ (instancetype)sharedDTR;
+(void)destoryInstance;

- (void)start:(NMCDTRSource)source version:(NSString *)version;

@end
