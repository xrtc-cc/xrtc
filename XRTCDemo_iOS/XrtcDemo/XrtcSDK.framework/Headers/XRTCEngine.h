//
//  XRTCEngine.h
//  XRTC
//
//  Created by xrtc on 21-02-10.
//
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "XRTCConstants.h"
#import "XRTCInitParam.h"
#import "XRTCStatsInfo.h"

@protocol XRTCEngineDelegate;
@interface XRTCEngine: NSObject

@property (nonatomic, strong) id xdyRtc; //Rtc对象，id类型为OC的通用对象类型，属于万能指针，可用于动态绑定
@property (nonatomic, assign) id <XRTCEngineDelegate> delegate;

/**
 * 创建XRTC实例（单例模式）
 */
+ (id)shareInstance;

/**
 * 获取XRTC SDK版本号
 */
+ (NSString *)getSdkVersion;

/**
 * 获取XRTC当前支持的RTC及其版本号
 */
+ (NSMutableDictionary *)getSupportRtc;

/**
 * 获取当前的RTC类型
 */
+ (XRTCRtcType)getCurrentRtcType;

/**
 * 获取使用XRTC的授权
 */
- (void)getAuthorization:(NSString *)xrtcId xrtcKey:(NSString *)xrtcKey authorizedResult:(void (^)(int code, NSString *msg))authorizedResultBlock;

/**
 * 切换RTC类型
 */
- (int)switchRtc:(XRTCRtcType)rtcType;

/**
 * 初始化XRTC引擎
 */
- (int)initRtcEngine:(XRTCInitParam *)engineInfo;

/**
 * 设置视频编码
 */
- (int)videoEncoderConfiguration:(XRTCVideoEncode)videoEncode;

/**
 * 设置房间场景
 */
- (void)setChannelProfile:(XRTCChannelProfileType)profile;

/**
 * 设置音频场景
 */
- (void)setAudioProfileScenario:(XRTCAudioScenarioType)scenario;

/**
 * 设置为扬声器模式，加入频道成功后调用
 */
- (int)setEnableSpeakerphone:(BOOL)isOpen;

/**
 * 设置本地播放的远端用户的音量
 */
- (int)adjustPlaybackSignalVolume:(NSInteger)volume;

/**
 * 本地加入频道
 */
- (int)joinRtcChannel:(NSString *)channelId
                  uid:(NSUInteger)uid;

/**
 * 本地离开频道
 */
- (int)leaveChannel;

/**
 * 绑定本地视频承载视图，并指定视频填充模式
 */
- (int)bindLocalVideo:(UIView *)videoParentView
                  uid:(NSUInteger)uid
            channelId:(NSString *)channelId
                 mode:(XRTCVideoDisplayMode)mode;

/**
 * 解绑本地视频承载视图
 */
- (int)unBindLocalVideo:(UIView *)videoParentView
                    uid:(NSUInteger)uid
              channelId:(NSString *)channelId;

/**
 * 绑定远端视频承载视图，并指定视频填充模式
 */
- (int)bindRemoteVideo:(UIView *)videoParentView
                   uid:(NSUInteger)uid
             channelId:(NSString *)channelId
                  mode:(XRTCVideoDisplayMode)mode;

/**
 * 解绑远端视频承载视图
 */
- (int)unBindRemoteVideo:(UIView *)videoParentView
                     uid:(NSUInteger)uid
               channelId:(NSString *)channelId;

/**
 * 切换RTC角色
 */
- (void)setRtcClientRole:(XRTCRole)clientRole;

/**
 * 取消或恢复发布本地音频流
 */
- (int)muteLocalAudioStream:(BOOL)mute;

/**
 * 取消或恢复发布本地视频流
 */
- (int)muteLocalVideoStream:(BOOL)mute;

/**
 * 设置是否订阅及播放远端音频流
 */
- (int)muteRemoteAudioStream:(NSUInteger)uid
                        mute:(BOOL)mute;

/**
 * 设置是否订阅及播放远端视频流
 */
- (int)muteRemoteVideoStream:(NSUInteger)uid
                        mute:(BOOL)mute;

/**
 * 销毁RTC
 */
- (void)destroyRtcEngine;

/**
 * 切换摄像头
 */
- (int)switchCamera;

///////////////////////////// 子频道相关API，当前仅声网支持，暂不推荐使用 /////////////////////////////
- (void)joinSubChannel:(NSString *)subChannelId token:(NSString *)token uid:(NSUInteger)uid;
- (int)leaveSubChannel;
- (int)destroySubChannel;

@end


@protocol XRTCEngineDelegate <NSObject>
@optional

///////////////////////////// 子频道相关回调，当前仅声网支持，暂不推荐使用 /////////////////////////////
- (void)XRTCEngine:(XRTCEngine *)xrtcEngine didOfflineOfUid:(NSUInteger)uid channelId:(NSString *)channelId;
- (void)XRTCEngine:(XRTCEngine *)xrtcEngine firstRemoteVideoFrameOfUid:(NSUInteger)uid channelId:(NSString *)channelId;
- (void)XRTCEngine:(XRTCEngine *)xrtcEngine firstRemoteAudioFrameOfUid:(NSUInteger)uid channelId:(NSString *)channelId;

/**
 * 加入频道成功回调
 */
- (void)XRTCEngine:(XRTCEngine *)xrtcEngine joinSuccess:(NSUInteger)uid;

/**
 * 离开频道回调
 */
- (void)XRTCEngine:(XRTCEngine *)xrtcEngine didLeaveChannel:(NSUInteger)uid;

/**
 * 远端用户加入当前频道回调
 */
- (void)XRTCEngine:(XRTCEngine *)xrtcEngine didJoinedOfUid:(NSUInteger)uid;

/**
 * 远端用户离开当前频道回调
 */
- (void)XRTCEngine:(XRTCEngine *)xrtcEngine didOfflineOfUid:(NSUInteger)uid;

/**
 * 已解码远端音频首帧回调
 */
- (void)XRTCEngine:(XRTCEngine *)xrtcEngine firstRemoteAudioFrameOfUid:(NSUInteger)uid;

/**
 * 已解码远端视频首帧回调
 */
- (void)XRTCEngine:(XRTCEngine *)xrtcEngine firstRemoteVideoDecodedOfUid:(NSUInteger)uid;

/**
 * 远端用户停止/恢复发送音频流回调
 */
- (void)XRTCEngine:(XRTCEngine *)xrtcEngine didVideoMuted:(BOOL)muted byUid:(NSUInteger)uid;

/**
 * 远端用户停止/恢复发送视频流回调
 */
- (void)XRTCEngine:(XRTCEngine *)xrtcEngine didAudioMuted:(BOOL)muted byUid:(NSUInteger)uid;

/**
 * 发生错误回调
 */
- (void)XRTCEngine:(XRTCEngine *)xrtcEngine onError:(NSInteger)errorCode errorMsg:(NSString *)errorMsg;

/**
 * 发生警告回调
 */
- (void)XRTCEngine:(XRTCEngine *)xrtcEngine onWarning:(NSInteger)warningCode warningMsg:(NSString *)warningMsg;

/**
 * 服务器连接状态回调
 */
- (void)XRTCEngine:(XRTCEngine *)xrtcEngine onConnStateChange:(XRTCConnStateType)state;

/**
 * 本地RTC音视频数据统计回调
 */
- (void)XRTCEngine:(XRTCEngine *)xrtcEngine
   onLocalRtcStats:(XRTCLocalRtcStats *)localRtcStats
        audioStats:(XRTCLocalAudioStats *)localAudioStats
        videoStats:(XRTCLocalVideoStats *)localVideoStats;

/**
 * 远端RTC音视频数据统计回调
 */
- (void)XRTCEngine:(XRTCEngine *)xrtcEngine
  onRemoteRtcStats:(XRTCRemoteRtcStats *)remoteRtcStats
        audioStats:(XRTCRemoteAudioStats *)remoteAudioStats
        videoStats:(XRTCRemoteVideoStats *)remoteVideoStats;

@end
