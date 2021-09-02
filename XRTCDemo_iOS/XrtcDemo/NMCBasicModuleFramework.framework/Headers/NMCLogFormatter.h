//
//  NMCLogFormatter.h
//  LSMediaCapture
//
//  Created by emily on 2016/11/30.
//  Copyright © 2016年 NetEase. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "NMCDDLog.h"

@protocol NMCDDLogFormatter;
/**
 *  处理NMC的log日志格式
 */
@interface NMCLogFormatter : NSObject <NMCDDLogFormatter>

@end

