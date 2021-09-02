//
//  NMCSocketConnect.h
//  LSMediaCapture
//
//  Created by taojinliang on 2017/6/27.
//  Copyright © 2017年 NetEase. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface NMCSocketConnect : NSObject

/**
 连接服务器
 
 @param url 服务器地址
 @return 是否接连成功
 */
-(BOOL)connectToServer:(NSString *)url;
@end
