//
//  Common.m
//  xrtc
//
//  Created by 陈飞同 on 2019/8/22.
//  Copyright © 2019 陈飞同. All rights reserved.
//

#import "XRTCCommon.h"
#import <sys/utsname.h>
#import <UIKit/UIKit.h>
#import <AVFoundation/AVFoundation.h>

@interface XRTCCommon()

@end

@implementation XRTCCommon


+ (void)applyMicOrcameraWithConfirm:(void(^)(id data))confirmMessage{
    AVAuthorizationStatus videoAuthorStatus = [AVCaptureDevice authorizationStatusForMediaType:AVMediaTypeVideo];
    AVAuthorizationStatus audioAuthorStatus = [AVCaptureDevice authorizationStatusForMediaType:AVMediaTypeAudio];
    
    static BOOL micAvailable = true;
    static BOOL cameraAvailable = true;
    
    if (AVAuthorizationStatusAuthorized == videoAuthorStatus && AVAuthorizationStatusAuthorized == audioAuthorStatus) {
        micAvailable = true;
        cameraAvailable = true;
        confirmMessage(@"mic_camera");
        return;
    }else{
        if (AVAuthorizationStatusRestricted == videoAuthorStatus || AVAuthorizationStatusDenied == videoAuthorStatus) {
            cameraAvailable = false;
        }
        if (AVAuthorizationStatusRestricted == audioAuthorStatus || AVAuthorizationStatusDenied == audioAuthorStatus) {
            micAvailable = false;
        }
    }
    
    if ((!micAvailable) && (!cameraAvailable)) {
        confirmMessage(@"noMic_nocamera");
        return;
    }
    
    if ((!micAvailable) && cameraAvailable) {
        confirmMessage(@"noMic_camera");
        return;
    }
    
    if ((!cameraAvailable) && micAvailable) {
        confirmMessage(@"mic_nocamera");
        return;
    }
    if (AVAuthorizationStatusNotDetermined == videoAuthorStatus && AVAuthorizationStatusNotDetermined == audioAuthorStatus){
        cameraAvailable = false;
        micAvailable = false;
        if (!micAvailable) {
            [AVCaptureDevice requestAccessForMediaType:AVMediaTypeAudio completionHandler:^(BOOL granted) {
                if (granted) {
                    micAvailable = true;
                }
                
                [AVCaptureDevice requestAccessForMediaType:AVMediaTypeVideo completionHandler:^(BOOL granted) {
                    if (granted) {
                        cameraAvailable = true;
                    }
                    dispatch_async(dispatch_get_main_queue(), ^{
                        NSString *message = [XRTCCommon validateMic:micAvailable andcamera:cameraAvailable];
                        confirmMessage(message);
                    });
                }];
            }];
        }
    }else if (AVAuthorizationStatusNotDetermined == videoAuthorStatus && AVAuthorizationStatusNotDetermined != audioAuthorStatus){
        cameraAvailable = false;
        micAvailable = true;
        [AVCaptureDevice requestAccessForMediaType:AVMediaTypeVideo completionHandler:^(BOOL granted) {
            if (granted) {
                cameraAvailable = true;
            }
            dispatch_async(dispatch_get_main_queue(), ^{
                NSString *message = [XRTCCommon validateMic:micAvailable andcamera:cameraAvailable];
                confirmMessage(message);
            });
        }];
    }else if (AVAuthorizationStatusNotDetermined != videoAuthorStatus && AVAuthorizationStatusNotDetermined == audioAuthorStatus) {
        cameraAvailable = true;
        micAvailable = false;
        [AVCaptureDevice requestAccessForMediaType:AVMediaTypeAudio completionHandler:^(BOOL granted) {
            if (granted) {
                micAvailable = true;
            }
            dispatch_async(dispatch_get_main_queue(), ^{
                NSString *message = [XRTCCommon validateMic:micAvailable andcamera:cameraAvailable];
                confirmMessage(message);
            });
        }];
    }
}

+ (NSString *)validateMic:(BOOL)micAvailable andcamera:(BOOL)cameraAvailable{
    if (micAvailable && cameraAvailable) {
        return @"mic_camera";
    }else if (!cameraAvailable && !micAvailable) {
        return @"noMic_nocamera";
    }else if (!cameraAvailable && micAvailable) {
        return @"mic_nocamera";
    }else if (cameraAvailable && !micAvailable) {
        return @"noMic_camera";
    }
    return @"error";
}

+ (BOOL)isIpad {
    
    BOOL b = NO;
    if ([[XRTCCommon getDeviceType] isEqualToString:@"iPad"]) {
        b = YES;
    }
    return b;
}

+ (NSString *)getDeviceType {
    NSString *deviceNameStr = @"";
    NSString *deviceType = [UIDevice currentDevice].model;

    if([deviceType isEqualToString:@"iPhone"]) {
        deviceNameStr = @"iPhone";
    }
    else if([deviceType isEqualToString:@"iPod touch"]) {
        deviceNameStr = @"iPod touch";
    }
    else {
        deviceNameStr = @"iPad";
    }
    return deviceNameStr;
}

@end
