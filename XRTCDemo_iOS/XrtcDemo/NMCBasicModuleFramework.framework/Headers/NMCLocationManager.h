//
//  NMCLocationManager.h
//  LSMediaCapture
//
//  Created by taojinliang on 2017/6/23.
//  Copyright © 2017年 NetEase. All rights reserved.
//

#import <Foundation/Foundation.h>


#define nmcNotifyLocatationManager @"nmcNotifyLocatationManager"
#define nmcNotifyLocatationManagerResult @"nmcNotifyLocatationManagerResult"


typedef NS_ENUM(NSInteger, NMCLocatationManagerState) {
    NMCLocatationManagerState_ing = 1,
    NMCLocatationManagerState_Success,
    NMCLocatationManagerState_Fail,
    NMCLocatationManagerState_Denied,
    NMCLocatationManagerState_NotDetermined // 模拟器里遇到，一直处于notdetermined状态
};


@interface NMCLocatationManagerResult : NSObject
@property (nonatomic) NMCLocatationManagerState status;
@property (nonatomic) NSString *province;
@property (nonatomic) NSString *city;
@property (nonatomic) NSString *district;
@property (nonatomic) float    longitude;
@property (nonatomic) float    latitude;
@property (nonatomic) int isInChina; // 0, not a valid value, 1 in china  2 not in china
@end

@interface NMCLocationManager : NSObject

+(NMCLocationManager*)shareInstance;
+(void)destoryInstance;
- (void)needUseLocation;
@end
