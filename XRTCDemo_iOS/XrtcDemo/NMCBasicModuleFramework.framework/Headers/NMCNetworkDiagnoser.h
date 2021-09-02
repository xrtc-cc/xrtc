//
//  NMCNetworkDiagnoser.h
//  LSMediaCapture
//
//  Created by taojinliang on 2017/5/12.
//  Copyright © 2017年 NetEase. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef void(^NetworkCallBack)(NSArray *array);
typedef void(^NetworkContinousPingCallBack)(NSArray *array);
typedef void(^NetworkContinousPingResultCallBack)(NSDictionary *dict);

@interface NMCNetworkDiagnoser : NSObject
@property (nonatomic, readonly) BOOL isRunning;

+(instancetype)shareInstance;
+(void)destoryInstance;
-(void)startNetworkDiagnoser:(NSDictionary *)dict callback:(NetworkCallBack)callback;//原先方法暂时保留，但不实现
-(void)startNetworkDiagnoser:(NSDictionary *)dict conPingCallBack:(NetworkContinousPingCallBack)conPingCallBack callback:(NetworkCallBack)callback;
-(void)stopNetworkDiagnoser;

-(void)startContinuousPingDiagnoser:(NSArray *)array callback:(NetworkContinousPingResultCallBack)callback;
-(void)stopContinuousPingDiagnoser;
@end
