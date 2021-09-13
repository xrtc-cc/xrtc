//
//  XRTCConstants.h
//  XRTC
//
//  Created by xrtc on 2021/6/25.
//  Copyright © 2021 xrtc. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

//----------------------日志打印--------------------
#define XRTC_DATE_STRING \
({NSDateFormatter *fmt = [[NSDateFormatter alloc] init];\
[fmt setDateFormat:@"YYYY-MM-dd hh:mm:ss:sss"];\
[fmt stringFromDate:[NSDate date]];})

#ifdef DEBUG
#   define XRTCNSLog(format, ...) printf("%s:Dev: [Line %d]\n%s\n\n", [XRTC_DATE_STRING UTF8String], __LINE__, [[NSString stringWithFormat:format, ##__VA_ARGS__] UTF8String])
//#   define XRTCNSLog(...)
#else
#   define XRTCNSLog(...)
#endif

//----------------------枚举类型--------------------
typedef NS_ENUM(NSInteger,XRTCRtcType) {
    /* 声网Rtc */
    XRTCRtcTypeAgora = 0,
    /* 华为Rtc */
    XRTCRtcTypeHuawei,
    /* 腾讯Rtc */
    XRTCRtcTypeTengXun,
    /* 网易Rtc */
    XRTCRtcTypeWangYi
};

typedef NS_ENUM(NSInteger,XRTCRole) {
    /* 双向流角色，例如主播加入 */
    XRTCRoleBroadcaster = 0,
    /* 发布流角色，例如广播,无法在房间中切换角色 */
    XRTCRolePublisher,
    /* 接收流角色，例如观众 */
    XRTCRoleAudience
};

typedef NS_ENUM(NSInteger,XRTCVideoDisplayMode) {
    /* 优先保证视频内容全部显示。视频尺寸等比缩放，直至视频窗口的一边与视窗边框对齐。如果视频长宽与显示窗口不同，视窗上未被填满的区域将被涂黑 */
    XRTCVideoDisplayModeFit = 0,
    /* 优先保证视窗被填满。视频尺寸等比缩放，直至整个视窗被视频填满。如果视频长宽与显示窗口不同，多出的视频将被截掉 */
    XRTCVideoDisplayModeHidden,
    /* 视频尺寸进行缩放和拉伸以充满显示视窗 */
    XRTCVideoDisplayModeFill,
    /* 自适应模式,显示图像和设备的横竖屏不同使用黑边模式,横竖屏相同使用裁剪模式 */
    XRTCVideoDisplayModeAdapt
};

typedef NS_ENUM(NSInteger,XRTCConnStateType) {
    /* 连接断开 */
    XRTCConnStateTypeDisconnected = 0,
    /* 建立网络连接中 */
    XRTCConnStateTypeConnecting,
    /* 网络连接成功 */
    XRTCConnStateTypeConnected,
    /* 重新建立网络连接中 */
    XRTCConnStateTypeReconnecting,
    /* 网络连接失败 */
    XRTCConnStateTypeFailed
};

typedef NS_ENUM(NSInteger, XRTCOrientationMode) {

    /* 横屏分辨率，例如：640 × 360 */
    XRTCOrientationModeLandscape     = 0,

    /* 竖屏分辨率，例如：360 × 640 */
    XRTCOrientationModePortrait     = 1,

};

/**
 房间场景
*/
typedef NS_ENUM(NSInteger, XRTCChannelProfileType) {
    /* 通信场景。该场景下，房间内所有用户都可以发布和接收音、视频流。适用于语音通话、视频群聊等应用场景 */
    XRTCChannelProfileCommunication = 0,
    /* 直播场景。该场景有主播和观众两种用户角色，主播可以发布和接收音视频流，观众直接接收流。适用于语聊房、视频直播、互动大班课等应用场景 */
    XRTCChannelProfileLiveBroadcasting,
 };

/**
 音频场景
*/
typedef NS_ENUM(NSInteger, XRTCAudioScenarioType) {
    /* 语音场景：默认的音频应用场景 */
    XRTCAudioScenarioTypeDefault = 0,
    /* 音乐场景 */
    XRTCAudioScenarioTypeMusic,
    /* 语音聊天室场景 */
    XRTCAudioScenarioTypeChatRoom,
};

typedef NS_ENUM(NSInteger, XRTCVideoEncode) {
    /* 1. 分辨率：160 * 120，帧率：15 fps，码率：65 Kbps */
    XRTCVideoEncode_120P = 0,
    /* 2. 分辨率：120 * 120，帧率：15 fps，码率：50 Kbps */
    XRTCVideoEncode_120P_3,
    /* 3. 分辨率：320 * 180，帧率：15 fps，码率：140 Kbps */
    XRTCVideoEncode_180P,
    /* 4. 分辨率：180 * 180，帧率：15 fps，码率：100 Kbps */
    XRTCVideoEncode_180P_3,
    /* 5. 分辨率：240 * 180，帧率：15 fps，码率：120 Kbps */
    XRTCVideoEncode_180P_4,
    /* 6. 分辨率：320 * 240，帧率：15 fps，码率：200 Kbps */
    XRTCVideoEncode_240P,
    /* 7. 分辨率：240 * 240，帧率：15 fps，码率：140 Kbps */
    XRTCVideoEncode_240P_3,
    /* 8. 分辨率：424 * 240，帧率：15 fps，码率：220 Kbps */
    XRTCVideoEncode_240P_4,
    /* 9. 分辨率：640 * 360，帧率：15 fps，码率：400 Kbps */
    XRTCVideoEncode_360P,
    /* 10. 分辨率：360 * 360，帧率：15 fps，码率：260 Kbps */
    XRTCVideoEncode_360P_3,
    /* 11. 分辨率：640 * 360，帧率：30 fps，码率：600 Kbps */
    XRTCVideoEncode_360P_4,
    /* 12. 分辨率：360 * 360，帧率：30 fps，码率：400 Kbps */
    XRTCVideoEncode_360P_6,
    /* 13. 分辨率：480 * 360，帧率：15 fps，码率：320 Kbps */
    XRTCVideoEncode_360P_7,
    /* 14. 分辨率：480 * 360，帧率：30 fps，码率：490 Kbps */
    XRTCVideoEncode_360P_8,
    /* 15. 分辨率：640 * 360，帧率：15 fps，码率：800 Kbps */
    XRTCVideoEncode_360P_9,
    /* 16. 分辨率：640 * 360，帧率：24 fps，码率：800 Kbps */
    XRTCVideoEncode_360P_10,
    /* 17. 分辨率：640 * 360，帧率：24 fps，码率：1000 Kbps */
    XRTCVideoEncode_360P_11,
    /* 18. 分辨率：640 * 480，帧率：15 fps，码率：500 Kbps */
    XRTCVideoEncode_480P,
    /* 19. 分辨率：640 * 480，帧率：30 fps，码率：1000 Kbps */
    XRTCVideoEncode_480P_2,
    /* 20. 分辨率：480 * 480，帧率：15 fps，码率：400 Kbps */
    XRTCVideoEncode_480P_3,
    /* 21. 分辨率：640 * 480，帧率：30 fps，码率：750 Kbps */
    XRTCVideoEncode_480P_4,
    /* 22. 分辨率：480 * 480，帧率：30 fps，码率：600 Kbps */
    XRTCVideoEncode_480P_6,
    /* 23. 分辨率：848 * 480，帧率：15 fps，码率：610 Kbps */
    XRTCVideoEncode_480P_8,
    /* 24. 分辨率：848 * 480，帧率：30 fps，码率：930 Kbps */
    XRTCVideoEncode_480P_9,
    /* 25. 分辨率：640 * 480，帧率：10 fps，码率：400 Kbps */
    XRTCVideoEncode_480P_10,
    /* 26. 分辨率：1280 * 720，帧率：15 fps，码率：1130 Kbps */
    XRTCVideoEncode_720P,
    /* 27. 分辨率：1280 * 720，帧率：30 fps，码率：2000 Kbps */
    XRTCVideoEncode_720P_2,
    /* 28. 分辨率：1280 * 720，帧率：30 fps，码率：1710 Kbps */
    XRTCVideoEncode_720P_3,
    /* 29. 分辨率：960 * 720，帧率：15 fps，码率：910 Kbps */
    XRTCVideoEncode_720P_5,
    /* 30. 分辨率：960 * 720，帧率：30 fps，码率：1380 Kbps */
    XRTCVideoEncode_720P_6,
    /* 31. 分辨率：1920 * 1080，帧率：15 fps，码率：2080 Kbps */
    XRTCVideoEncode_1080P,
    /* 32 .分辨率：1920 * 1080，帧率：30 fps，码率：3150 Kbps */
    XRTCVideoEncode_1080P_2
};

@interface XRTCConstants : NSObject

@end

NS_ASSUME_NONNULL_END
