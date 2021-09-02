//
//  NMCSpeedUploadManager.h
//  LSMediaCapture
//
//  Created by taojinliang on 2017/5/18.
//  Copyright © 2017年 NetEase. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "NMCNetworkDiagnoserCommon.h"

@interface NMCSpeedUploadManager : NSObject
@property(nonatomic, strong) NMCSpeedCaclUtil *speedUtil;
+(NMCSpeedUploadManager *)shareInstance;
+(void)destoryInstance;
-(void)startSpeedUpload:(NSMutableArray *)speeds callback:(NMCFileUploadManagerResultCallBack)callback;
-(void)stopSpeedUpload;
/**
 测速之前设置测速次数和上传数据大小
 
 @param count 测速次数
 @param capacity 上传数据大小(仅限于文件上传类型,经测试，帝联不能超过500k（含500k)),单位是字节，500k＝500*1024，默认为499k
 */
-(void)setSpeedCacl:(NSInteger)count Capacity:(unsigned long long)capacity;
@end
