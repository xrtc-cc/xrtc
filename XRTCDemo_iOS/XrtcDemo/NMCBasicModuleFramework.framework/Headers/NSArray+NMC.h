//
//  NSArray+NMC.h
//  nvs
//
//  Created by taojinliang on 2018/7/31.
//  Copyright © 2018年 Netease. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface NSArray (NMC)
- (NSArray *)nmc_https;
- (NSArray *)nmc_asOptional;    //如果长度为0，就返回nil
@end
