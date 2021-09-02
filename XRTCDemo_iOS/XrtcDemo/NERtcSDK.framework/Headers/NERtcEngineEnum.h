/*
 * Copyright (c) 2021 NetEase, Inc.  All rights reserved.
 */

#import <Foundation/Foundation.h>

#if TARGET_OS_IPHONE
#import <UIKit/UIKit.h>
typedef UIView VIEW_CLASS;
typedef UIColor COLOR_CLASS;
#elif TARGET_OS_MAC
#import <AppKit/AppKit.h>
typedef NSView VIEW_CLASS;
typedef NSColor COLOR_CLASS;
#endif


NS_ASSUME_NONNULL_BEGIN

#pragma mark -- ENGINE ENUM


/**
 当前 engine 的连接状态
 */
typedef NS_ENUM(NSUInteger, NERtcConnectionStateType) {
    /** 引擎尚未初始化。*/
    kNERtcConnectionStateUnknown = 0,
    /** 尚未加入房间。*/
    kNERtcConnectionStateDisconnected = 1,
    /** 正在加入房间。*/
    kNERtcConnectionStateConnecting = 2,
    /** 加入房间成功。*/
    kNERtcConnectionStateConnected = 3,
    /** 正在尝试重新加入房间。*/
    kNERtcConnectionStateReconnecting = 4,
    /** 加入房间失败。*/
    kNERtcConnectionStateFailed = 5,
};


/**
 用户离开房间的原因
 */
typedef NS_ENUM(NSInteger, NERtcSessionLeaveReason) {
    /** 正常离开。*/
    kNERtcSessionLeaveNormal = 0,
    /** 用户断线导致离开房间。*/
    kNERtcSessionLeaveForFailOver = 1,
    /** 用户因 Failover 导致离开房间 ，仅 SDK 内部使用。*/
    kNERTCSessionLeaveForUpdate = 2,
    /** 用户被踢导致离开房间。*/
    kNERtcSessionLeaveForKick = 3,
    /** 用户超时退出房间。*/
    kNERtcSessionLeaveTimeout = 4,
};

/**
 用户角色
 */
typedef NS_ENUM(NSInteger, NERtcClientRole) {
    /**（默认）直播模式中的主播，可以操作摄像头等音视频设备、发布流、配置互动直播推流任务、上下线对房间内其他用户可见。 */
    kNERtcClientRoleBroadcaster = 0,
    /** 直播模式中的观众，观众只能接收音视频流，不支持操作音视频设备、配置互动直播推流任务、上下线不通知其他用户。 */
    kNERtcClientRoleAudience = 1,
};

/**
 当前engine连接状态变更的原因
 */
typedef NS_ENUM(NSInteger, NERtcReasonConnectionChangedType) {
    /**离开房间。*/
    kNERtcReasonConnectionChangedLeaveChannel = 1,
    /**房间被关闭。*/
    kNERtcReasonConnectionChangedChannelClosed = 2,
    /**用户被踢出。*/
    kNERtcReasonConnectionChangedBeKicked = 3,
    /**连接超时。*/
    kNERtcReasonConnectionChangedTimeOut = 4,
    /**加入房间。*/
    kNERtcReasonConnectionChangedJoinChannel = 5,
    /**加入房间成功。*/
    kNERtcReasonConnectionChangedJoinSucceed = 6,
    /**重新加入房间成功。*/
    kNERtcReasonConnectionChangedReJoinSucceed = 7,
    /**媒体连接断开。*/
    kNERtcReasonConnectionChangedMediaConnectionDisconnected = 8,
    /**信令连接失败。*/
    kNERtcReasonConnectionChangedSignalDisconnected = 9,
    /**请求房间失败。*/
    kNERtcReasonConnectionChangedRequestChannelFailed = 10,
    /**加入房间失败。*/
    kNERtcReasonConnectionChangedJoinChannelFailed = 11,
    /**重新分配了服务端IP。*/
    kNERtcReasonConnectionChangedReDispatch = 12,
};

#pragma mark -- VIDEO ENUM


/**
 视频 profile 类型
 */
typedef NS_ENUM(NSUInteger, NERtcVideoProfileType) {
    /**普清（160×120/90 @15fps）*/
    kNERtcVideoProfileLowest = 0,          // 160x90/120
    /**标清（320x180/240 @15fps）*/
    kNERtcVideoProfileLow = 1,          // 320x180/240
    /**高清（640x360/480 @30fps）*/
    kNERtcVideoProfileStandard = 2,     // 640x360/480
    /**超清（1280x720 @30fps）*/
    kNERtcVideoProfileHD720P = 3,       // 1280x720
    /**1080P（1920x1080 @30fps）*/
    kNERtcVideoProfileHD1080P = 4,      // 1920x1080
    /**普清（160×120/90 @15fps）*/
    kNERtcVideoProfileMAX = kNERtcVideoProfileHD1080P,
    /**FakeVideo标识，仅在回调中显示。请勿主动设置，否则SDK会按照kNERtcVideoProfileStandard处理。
     当远端在纯音频状态发送 SEI 时，本端将会收到远端的onNERtcEngineUserVideoDidStartWithUserID 回调，其中 maxProfile 参数为FAKE_PROFILE， 表示对端发送 16*16 的FakeVideo，此时如果本端需要接收远端的SEI信息，只需要订阅一下远端的视频即可，无须设置远端画布。*/
    kNERtcVideoProfileFake = 5,
};

/**
 视频帧率
  - kNERtcVideoFrameRateFpsDefault：主流默认根据设置的maxProfile选择 ， 辅流默认kNERtcVideoFrameRateFps7
 */
typedef NS_ENUM(NSUInteger, NERtcVideoFrameRate) {
    /**0 fps。*/
    kNERtcVideoFrameRateFpsDefault = 0,
    /**7 fps。*/
    kNERtcVideoFrameRateFps7  = 7,
    /**10 fps。*/
    kNERtcVideoFrameRateFps10 = 10,
    /**15 fps。*/
    kNERtcVideoFrameRateFps15 = 15,
    /**24 fps。*/
    kNERtcVideoFrameRateFps24 = 24,
    /**30 fps。*/
    kNERtcVideoFrameRateFps30 = 30,
    /**60 fps。*/
    kNERtcVideoFrameRateFps60 = 60,
};

/**
 带宽受限时视频编码适应性偏好
 
 - kNERtcDegradationDefault:
 通信场景(kNERtcChannelProfileCommunication),使用Balanced；
 直播场景(kNERtcChannelProfileLiveBroadcasting),使用MaintainQuality
 - kNERtcDegradationMaintainFramerate: 降低视频分辨率以保证编码帧率
 - kNERtcDegradationMaintainQuality: 降低编码帧率以保证视频分辨率
 - kNERtcDegradationBalanced: 平衡调节
 */
typedef NS_ENUM(NSUInteger, NERtcDegradationPreference) {
    /**默认）根据场景模式调整适应性偏好。

     - 通信场景中，选择DEGRADATION_BALANCED模式，在编码帧率和视频质量之间保持平衡。
     - 直播场景中，选择DEGRADATION_MAINTAIN_QUALITY模式，降低编码帧率以保证视频质量。
     */
    kNERtcDegradationDefault = 0,
    /**流畅优先，降低视频质量以保证编码帧率。在弱网环境下，降低视频清晰度以保证视频流畅，此时画质降低，画面会变得模糊，但可以保持视频流畅。*/
    kNERtcDegradationMaintainFramerate,
    /**清晰优先，降低编码帧率以保证视频质量。在弱网环境下，降低视频帧率以保证视频清晰，此时可能会出现一定卡顿。*/
    kNERtcDegradationMaintainQuality,
    /**在编码帧率和视频质量之间保持平衡。*/
    kNERtcDegradationBalanced,
};

/**
 屏幕共享功能的编码策略倾向
 */
typedef NS_ENUM(NSUInteger, NERtcSubStreamContentPrefer) {
    /**
     （默认）内容类型为动画。

     - 当共享的内容是视频、电影或游戏时，推荐选择该内容类型。
     - 当用户设置内容类型为动画时，按用户设置的帧率处理。
     */
    kNERtcSubStreamContentPreferMotion = 0,
    /**
     内容类型为细节。

     - 当共享的内容是图片或文字时，推荐选择该内容类型。
     - 当用户设置内容类型为细节时，最高允许用户设置到 10 帧，设置超过10帧时，按10帧处理。
     */
    kNERtcSubStreamContentPreferDetails,
};

/**
 远端视频流类型
 */
typedef NS_ENUM(NSUInteger, NERtcRemoteVideoStreamType) {
    /**大流，高清画质。*/
    kNERtcRemoteVideoStreamTypeHigh,
    /**小流，低清画质。*/
    kNERtcRemoteVideoStreamTypeLow
};

/**
 视频发布模式
 */
typedef NS_ENUM(NSUInteger, NERtcVideoPubMode) {
    /**按对端订阅格式发流。*/
    kNERtcVideoSendOnPubWithNone = 0,
    /**初始发送大流。*/
    kNERtcVideoSendOnPubWithHigh,
    /**初始发布小流。*/
    kNERtcVideoSendOnPubWithLow,
    /**初始大小流同时发送。*/
    kNERtcVideoSendOnPubWithAll,
};

/**
 视频渲染缩放模式
 */
typedef NS_ENUM(NSUInteger, NERtcVideoRenderScaleMode) {
    /**< 适应视频，视频尺寸等比缩放。优先保证视频内容全部显示。若视频尺寸与显示视窗尺寸不一致，视窗未被填满的区域填充背景色。*/
    kNERtcVideoRenderScaleFit      = 0,
    /**视频尺寸非等比缩放。保证视频内容全部显示，且填满视窗。*/
    kNERtcVideoRenderScaleFullFill = 1,
    /**适应区域，视频尺寸等比缩放。保证所有区域被填满，视频超出部分会被裁剪。*/
    kNERtcVideoRenderScaleCropFill = 2,
};

/**
 视频旋转模式。
 */
typedef NS_ENUM(NSUInteger, NERtcVideoRotationMode) {
    /** （默认）通过系统设备方向来判断视频旋转模式。如果本地系统设备横向放置，本地画面在本地和远端将展示为横屏模式。
    */
    NERtcVideoRotationModeBySystem = 0,
    /** 通过 App 方向来判断视频旋转模式。如果本地 App UI 切换到横屏，本地画面在本地和远端也将展示为横屏模式。
    */
    NERtcVideoRotationModeByApp = 1,
};


/**
 视频镜像模式。
 */
typedef NS_ENUM(NSUInteger, NERtcVideoMirrorMode) {
    /**（默认）由 SDK 决定镜像模式。*/
    kNERtcVideoMirrorModeAuto    = 0,
    /** 启用镜像模式。*/
    kNERtcVideoMirrorModeEnabled = 1,
    /** 关闭镜像模式。*/
    kNERtcVideoMirrorModeDisabled = 2,
};

/**
 视频旋转方向模式。
 */
typedef NS_ENUM(NSUInteger, NERtcVideoOutputOrientationMode) {
    /**（默认）该模式下 SDK 输出的视频方向与采集到的视频方向一致。接收端会根据收到的视频旋转信息对视频进行旋转。
    * 
    * 该模式适用于接收端可以调整视频方向的场景。
    * - 如果采集的视频是横屏模式，则输出的视频也是横屏模式。
    * - 如果采集的视频是竖屏模式，则输出的视频也是竖屏模式。 
    */
    kNERtcVideoOutputOrientationModeAdaptative    = 0,
    /** 该模式下 SDK 固定输出横屏模式的视频。如果采集到的视频是竖屏模式，则视频编码器会对其进行裁剪。
     * 
     * 该模式适用于接收端无法调整视频方向的场景，例如旁路推流。
    */
    kNERtcVideoOutputOrientationModeFixedLandscape = 1,
    /** 该模式下 SDK 固定输出竖屏模式的视频，如果采集到的视频是横屏模式，则视频编码器会对其进行裁剪。
     * 
     * 该模式适用于接收端无法调整视频方向的场景，例如旁路推流。
    */
    kNERtcVideoOutputOrientationModeFixedPortrait = 2,
};

/**
 视频裁剪模式
*/
typedef NS_ENUM(NSUInteger, NERtcVideoCropMode) {
    /**不裁剪。*/
    kNERtcVideoCropDefault,
    /**16:9*/
    kNERtcVideoCropMode16_9,
    /**4:3*/
    kNERtcVideoCropMode4_3,
    /**1:1*/
    kNERtcVideoCropMode1_1,
};

/**
 视频渲染类型。
 */
typedef NS_ENUM(NSUInteger, NERtcRenderType) {
    /**默认视频渲染类型。*/
    kNERtcRenderDefault = 0,
    /**OpenGLES 渲染。*/
    kNERtcRenderOpenGL,
    /**Metal 渲染。*/
    kNERtcRenderMetal,
};

/**
 视频数据的旋转方向
 */
typedef NS_ENUM(NSUInteger, NERtcVideoRotationType) {
    /**0 度。*/
    kNERtcVideoRotation_0 = 0,
    /**90 度。*/
    kNERtcVideoRotation_90 = 90,
    /**180 度。*/
    kNERtcVideoRotation_180 = 180,
    /**270 度。*/
    kNERtcVideoRotation_270 = 270,
};

/**
 视频格式类型
 */
typedef NS_ENUM(NSUInteger, NERtcVideoFormatType) {
    /**I420 视频格式。*/
    kNERtcVideoFormatI420 = 0,
    /**NV12 视频格式。*/
    kNERtcVideoFormatNV12,
    /**BGRA 视频格式。*/
    kNERtcVideoFormatBGRA,
};

/**
 SEI发送的流通道类型
 */
typedef NS_ENUM(NSUInteger, NERtcStreamChannelType) {
    /** 主流通道。*/
    kNERtcStreamChannelTypeMainStream = 0,
    /** 辅流通道。*/
    kNERtcStreamChannelTypeSubStream,
};

#pragma mark -- AUDIO ENUM

/**
 音频路由
*/
typedef NS_ENUM(NSInteger, NERtcAudioOutputRouting) {
    /**系统默认*/
    kNERtcAudioOutputRoutingDefault = 0,
    /**耳机*/
    kNERtcAudioOutputRoutingHeadset,
    /**听筒*/
    kNERtcAudioOutputRoutingEarpiece,
    /**扬声器*/
    kNERtcAudioOutputRoutingLoudspeaker,
    /**蓝牙外设*/
    kNERtcAudioOutputRoutingBluetooth,
};

/**
音频质量能力概要
音频属性:设置采样率，码率，编码模式和声道数
*/
typedef NS_ENUM(NSInteger, NERtcAudioProfileType) {
    /** 默认设置。语音场景下为 STANDARD，音乐场景下为 HIGH_QUALITY。*/
    kNERtcAudioProfileDefault = 0,
    /** 标准音质模式。采样率为 16 kHz、语音编码、单声道、编码码率最大值为 20 Kbps。*/
    kNERtcAudioProfileStandard,
    /** 标准扩展模式。采样率为 16 kHz、语音编码、单声道、编码码率最大值为 32 Kbps。*/
    kNERtcAudioProfileStandardExtend,
    /** 中等音质模式。采样率为 48 kHz、音乐编码、单声道、编码码率最大值为 32 Kbps。*/
    kNERtcAudioProfileMiddleQuality,
    /** 中等音质模式（立体音）。采样率为 48 kHz、音乐编码、双声道、编码码率最大值为 64 Kbps。*/
    kNERtcAudioProfileMiddleQualityStereo,
    /** 高音质模式。采样率为 48 kHz、音乐编码、单声道、编码码率最大值为 64 Kbps。*/
    kNERtcAudioProfileHighQuality,
    /** 高音质模式（立体音）。采样率为 48 kHz、音乐编码、双声道、编码码率最大值为 128 Kbps。*/
    kNERtcAudioProfileHighQualityStereo,
};

/**
音频应用场景
不同的场景设置对应不同的音频采集模式（移动平台）、播放模式
 
 - kNERtcAudioScenarioDefault: 默认设置为 kNERtcAudioScenarioSpeech
 - kNERtcAudioScenarioSpeech: 语音场景. NERtcAudioProfileType 推荐使用 kNERtcAudioProfileMiddleQuality 及以下
 - kNERtcAudioScenarioMusic: 音乐场景。NERtcAudioProfileType 推荐使用 kNERtcAudioProfileMiddleQualityStereo 及以上
 - kNERtcAudioScenarioChatRoom: 语聊房场景。 NERtcAudioProfileType 推荐使用 kNERtcAudioProfileMiddleQualityStereo 及以上
*/
typedef NS_ENUM(NSInteger, NERtcAudioScenarioType) {
    /**默认的音频应用场景（语音）。*/
    kNERtcAudioScenarioDefault = 0,
    /**语音场景。*/
    kNERtcAudioScenarioSpeech,
    /**音乐场景。*/
    kNERtcAudioScenarioMusic,
    /**语音聊天室场景。*/
    kNERtcAudioScenarioChatRoom,
};

/**
 房间场景
*/
typedef NS_ENUM(NSInteger, NERtcChannelProfileType) {
    /**（默认）通信场景。该场景下，房间内所有用户都可以发布和接收音、视频流。适用于语音通话、视频群聊等应用场景。 */
    kNERtcChannelProfileCommunication = 0,
    /**直播场景。该场景有主播和观众两种用户角色，可以通过setClientRole设置。主播可以发布和接收音视频流，观众直接接收流。适用于语聊房、视频直播、互动大班课等应用场景。*/
    kNERtcChannelProfileLiveBroadcasting,
};


/**
音频会话控制权限
SDK 对 Audio Session 的控制权限
*/
typedef NS_ENUM(NSInteger, NERtcAudioSessionOperationRestriction) {
    /**没有限制，SDK 可以完全控制 Audio Session 操作*/
    kNERtcAudioSessionOperationRestrictionNone = 0,
    /**限制 SDK 对 Audio Session 进行任何操作，SDK 将不能再对 Audio Session 进行任何配置*/
    kNERtcAudioSessionOperationRestrictionAll,
    /**离开房间时，SDK 会保持 Audio Session 处于活动状态*/
    kNERtcAudioSessionOperationRestrictionDeactivateSession,
};

#pragma mark - media

typedef NS_ENUM(NSInteger, NERtcMediaPriorityType){
    /** 高优先级 */
    kNERtcMediaPriorityHigh = 50,
    /** （默认）普通优先级 */
    kNERtcMediaPriorityNormal = 100,
};

typedef NS_ENUM(NSInteger, NERtcStreamFallbackOptions){
    /** 
     * 上行或下行网络较弱时，不对音视频流作回退处理，但不能保证音视频流的质量。
     * 
     * @note 该选项只对 setLocalPublishFallbackOption 方法有效，对 setRemoteSubscribeFallbackOption 方法无效。
    */
    kNERtcStreamFallbackOptionDisabled = 0,
    /** 
     * 在下行网络条件较差的情况下，SDK 将只接收视频小流，即低分辨率、低码率视频流。
     * 
     * @note 该选项只对 setRemoteSubscribeFallbackOption 方法有效，对 setLocalPublishFallbackOption 方法无效。 */
    kNERtcStreamFallbackOptionVideoStreamLow = 1,
    /** 
     * - 上行网络较弱时，只发布音频流。
     * - 下行网络较弱时，先尝试只接收视频小流，即低分辨率、低码率视频流。如果网络环境无法显示视频，则再回退到只接收音频流。 */
    kNERtcStreamFallbackOptionAudioOnly = 2,
};

/**
  录音音质
 */
typedef NS_ENUM(NSInteger, NERtcAudioRecordingQuality) {
    /**低音质*/
    kNERtcAudioRecordingQualityLow = 0,
    /**（默认）中音质*/
    kNERtcAudioRecordingQualityMedium = 1,
    /**高音质*/
    kNERtcAudioRecordingQualityHigh = 2,
};

/**
  录音回调状态码
 */
typedef NS_ENUM(NSInteger, NERtcAudioRecordingCode) {
    /**不支持的录音文件格式。*/
    NERtcAudioRecordingCodeErrorSuffix = 1,
    /**无法创建录音文件，原因通常包括：
     * - 应用没有磁盘写入权限。
     * - 文件路径不存在。
     * 
    */
    kNERtcAudioRecordingOpenFileFailed = 2,
    /**开始录制。*/
    kNERtcAudioRecordingCodeStart = 3,
    /**录制错误。原因通常为磁盘空间已满，无法写入。*/
    kNERtcAudioRecordingRecordError = 4,
    /**完成录制。*/
    kNERtcAudioRecordingCodeFinish = 5,
};

#pragma mark - 变声/美声

/**
频谱子带索引 预设值。
 */
typedef NS_ENUM(NSInteger, NERtcAudioEqualizationBandFrequency ) {
    /** 0: 31 Hz。*/
    NERtcAudioEqualizationBand31 = 0,
    /** 1: 62 Hz。*/
    NERtcAudioEqualizationBand62 = 1,
    /** 2: 125 Hz。*/
    NERtcAudioEqualizationBand125 = 2,
    /** 3: 250 Hz。*/
    NERtcAudioEqualizationBand250 = 3,
    /** 4: 500 Hz。*/
    NERtcAudioEqualizationBand500 = 4,
    /** 5: 1 kHz。*/
    NERtcAudioEqualizationBand1K = 5,
    /** 6: 2 kHz。*/
    NERtcAudioEqualizationBand2K = 6,
    /** 7: 4 kHz。*/
    NERtcAudioEqualizationBand4K = 7,
    /** 8: 8 kHz。*/
    NERtcAudioEqualizationBand8K = 8,
    /** 9: 16 kHz。*/
    NERtcAudioEqualizationBand16K = 9,
};

/**
变声 预设值
 */
typedef NS_ENUM(NSInteger, NERtcVoiceChangerType) {
    /**（默认）关闭变声音效。*/
    kNERtcVoiceChangerOff = 0,
    /** 机器人。 */
    kNERtcVoiceChangerRobot,
    /** 巨人。 */
    kNERtcVoiceChangerGaint,
    /** 恐怖。*/
    kNERtcVoiceChangerHorror,
    /** 成熟。 */
    kNERtcVoiceChangerMature,
    /** 男变女。 */
    kNERtcVoiceChangerManToWoman,
    /** 女变男。*/
    kNERtcVoiceChangerWomanToMan,
    /** 男变萝莉。 */
    kNERtcVoiceChangerManToLoli,
    /** 女变萝莉。*/
    kNERtcVoiceChangerWomanToLoli
};

/**
美声 预设值
 */
typedef NS_ENUM(NSInteger, NERtcVoiceBeautifierType) {
    /**（默认）关闭美声效果。*/
    kNERtcVoiceBeautifierOff = 0,
    /**低沉。*/
    kNERtcVoiceBeautifierMuffled,
    /**圆润。*/
    kNERtcVoiceBeautifierMellow,
    /**清澈。*/
    kNERtcVoiceBeautifierClear,
    /**磁性。*/
    kNERtcVoiceBeautifierMagnetic,
    /**录音棚。*/
    kNERtcVoiceBeautifierRecordingStudio,
    /**天籁。*/
    kNERtcVoiceBeautifierNature,
    /**KTV。*/
    kNERtcVoiceBeautifierKTV,
    /**悠远。*/
    kNERtcVoiceBeautifierRemote,
    /**教堂。*/
    kNERtcVoiceBeautifierChurch,
    /**卧室。*/
    kNERtcVoiceBeautifierBedroom,
    /**Live*/
    kNERtcVoiceBeautifierLive
};

#pragma mark -- RECORD ENUM

/**
 本端参与的云端录制模式。
 */
typedef NS_ENUM(NSUInteger, NERtcRecordType) {
    /**本端参与合流+单流录制。同时录制合流内容和单流内容。*/
    kNERtcRecordTypeAll = 0,
    /**本端参与合流录制模式。房间内参与合流录制的成员，其音视频流混合录制为一个音视频文件。*/
    kNERtcRecordTypeMix = 1,
    /**本端参与单流录制模式。房间内参与单流录制的成员，分开录制其音视频流，每位成员均有其对应的音视频文件。*/
    kNERtcRecordTypeSingle = 2,
};

#pragma mark -- AUDIO MIXING ENUM

/**
 伴音任务状态
 */
typedef NS_ENUM(NSUInteger, NERtcAudioMixingState) {
    /** 音乐文件正常结束播放。*/
    kNERtcAudioMixingStateFinished = 0,
    /** 音乐文件播放失败。*/
    kNERtcAudioMixingStateFailed,
};

/**
 伴音错误码
 */
typedef NS_ENUM(NSUInteger, NERtcAudioMixingErrorCode) {
    /** 伴音正常*/
    kNERtcAudioMixingErrorOK        = 0 ,
    /** 伴音出现严重错误*/
    kNERtcAudioMixingErrorFatal         ,
    /** 伴音不能正常打开*/
    kNERtcAudioMixingErrorCanNotOpen    ,
    /** 音频解码错误*/
    kNERtcAudioMixingErrorDecode        ,
    /** 操作中断码*/
    kNERtcAudioMixingErrorInterrupt     ,
    /** 404 file not found，only for http/https*/
    kNERtcAudioMixingErrorHttpNotFound  ,
    /** 打开流/文件失败*/
    kNERtcAudioMixingErrorOpen          ,
    /** 获取解码信息失败/超时*/
    kNERtcAudioMixingErrorNInfo         ,
    /** 无音频流*/
    kNERtcAudioMixingErrorNStream       ,
    /** 无解码器*/
    kNERtcAudioMixingErrorNCodec        ,
    /** 无内存*/
    kNERtcAudioMixingErrorNMem          ,
    /** 解码器打开失败/超时*/
    kNERtcAudioMixingErrorCodecOpen     ,
    /** 无效音频参数（声道、采样率）*/
    kNERtcAudioMixingErrorInvalidInfo   ,
    /** 打开流/文件超时*/
    kNERtcAudioMixingErrorOpenTimeout   ,
    /** 网络io超时*/
    kNERtcAudioMixingErrorIoTimeout     ,
    /** 网络io错误*/
    kNERtcAudioMixingErrorIo            ,
};


#pragma mark -- Network Status ENUM
/**
 网络连接的类型
*/
typedef NS_ENUM(NSInteger, NERtcNetworkConnectionType) {
    /**未检测到。*/
    kNERtcNetworkConnectionTypeNone,
    /**无法确定。*/
    kNERtcNetworkConnectionTypeUnknown,
    /**2G。*/
    kNERtcNetworkConnectionType2G,
    /**3G。*/
    kNERtcNetworkConnectionType3G,
    /**4G。*/
    kNERtcNetworkConnectionType4G,
    /**5G。*/
    kNERtcNetworkConnectionType5G,
    /**WiFi。*/
    kNERtcNetworkConnectionTypeWiFi,
    /**运营商网络，但是无法确定运营商。*/
    kNERtcNetworkConnectionTypeWWAN,
};


#pragma mark -- Live Stream
/**
 直播推流音频采样率
 */
typedef NS_ENUM(NSInteger, NERtcLiveStreamAudioSampleRate) {
    /**32K*/
    kNERtcLiveStreamAudioSampleRate32000 = 32000,
    /**44.1K*/
    kNERtcLiveStreamAudioSampleRate44100 = 44100,
    /**48K*/
    kNERtcLiveStreamAudioSampleRate48000 = 48000,
};

/**
 直播推流音频编码规格
 */
typedef NS_ENUM(NSInteger, NERtcLiveStreamAudioCodecProfile) {
    /** NERtcLiveStreamAudioCodecProfileLCAAC: LC-AAC 规格，表示基本音频编码规格*/
    kNERtcLiveStreamAudioCodecProfileLCAAC = 0,
    /** NERtcLiveStreamAudioCodecProfileHEAAC: HE-AAC 规格，表示高效音频编码规格*/
    kNERtcLiveStreamAudioCodecProfileHEAAC = 1,
};

/**
 直播推流模式
 */
typedef NS_ENUM(NSInteger, NERtcLiveStreamMode) {
    /**
     （默认）推流音视频。
     */
    kNERtcLsModeVideo = 0,
    /**
     推流纯音频。
     */
    kNERtcLsModeAudio,
};

/**
 直播推流模式
 */
typedef NS_ENUM(NSInteger, NERtcLiveStreamVideoScaleMode) {
    /** 适应视频，视频尺寸等比缩放。优先保证视频内容全部显示。若视频尺寸与显示视窗尺寸不一致，视窗未被填满的区域填充背景色。 */
    kNERtcLsModeVideoScaleFit = 0,
    /** （默认）适应区域，视频尺寸等比缩放。保证所有区域被填满，视频超出部分会被裁剪。 */
    kNERtcLsModeVideoScaleCropFill ,
};

/**
 直播推流状态
 */
typedef NS_ENUM(NSInteger, NERtcLiveStreamStateCode) {
    /** 推流中 */
    kNERtcLsStatePushing = 505,
    /** 互动直播推流失败 */
    kNERtcLsStatePushFail = 506,
    /** 推流结束 */
    kNERtcLsStatePushStopped = 511,
};

#pragma mark -- Audio Frame Observer
/**
 音频类型

 - kNERtcAudioTypePCM16: PCM16
*/
typedef NS_ENUM(NSInteger, NERtcAudioType) {
    /** PCM 16。 */
    kNERtcAudioTypePCM16    = 0 ,
};

/**
 音频数据读写模式
*/
typedef NS_ENUM(NSInteger, NERtcAudioFrameOpMode) {
    /**只读模式。用户仅从 AudioFrame 获取原始音频数据。例如，如用户通过 SDK 采集数据，自己进行 RTMP/RTMPS 推流，则可以选择该模式。*/
    kNERtcAudioFrameOpModeReadOnly    = 0 ,
    /**读写模式。用户从 AudioFrame 获取并修改数据，并返回给 SDK 进行编码传输。例如，如用户自己有音效处理模块，且想要根据实际需要对数据进行前处理 （例如变声），则可以选择该模式。*/
    kNERtcAudioFrameOpModeReadWrite   = 1 ,
};

#pragma mark -- Device type
/**
 音频设备类型
*/
typedef NS_ENUM(int, NERtcAudioDeviceType) {
    /**未知设备。*/
    kNERtcAudioDeviceTypeUnknown = 0,
    /**音频采集设备。*/
    kNERtcAudioDeviceTypeRecord,
    /**音频播放设备。*/
    kNERtcAudioDeviceTypePlayout,
};

/**
 视频设备类型
*/
typedef NS_ENUM(int, NERtcVideoDeviceType) {
    /**未知视频设备。*/
    kNERtcVideoDeviceTypeUnknown = 0,
    /**视频采集设备。*/
    kNERtcVideoDeviceTypeCapture,
};

#pragma mark -- Device state
/**
 音频设备状态
*/
typedef NS_ENUM(int, NERtcAudioDeviceState) {
    /**已初始化。*/
    kNERtcAudioDeviceStateInitialized = 0,
    /**已启动。*/
    kNERtcAudioDeviceStateStarted,
    /**已停止。*/
    kNERtcAudioDeviceStateStoped,
    /**反初始化。*/
    kNERtcAudioDeviceStateUnInitialized,
};

/**
 视频设备状态无限制，SDK 可以完全控制 Audio Session。
*/
typedef NS_ENUM(int, NERtcVideoDeviceState) {
    /**已初始化。*/
    kNERtcVideoDeviceStateInitialized = 0,
    /**已启动。*/
    kNERtcVideoDeviceStateStarted,
    /**已停止。*/
    kNERtcVideoDeviceStateStoped,
    /**反初始化。*/
    kNERtcVideoDeviceStateUnInitialized,
};


/**
 网络质量状态
*/
typedef NS_ENUM(int, NERtcNetworkQuality) {
    /**网络质量未知*/
    kNERtcNetworkQualityUnknown = 0,
    /**网络质量极好*/
    kNERtcNetworkQualityExcellent,
    /**用户主观感觉和 excellent 差不多，但码率可能略低于 excellent*/
    kNERtcNetworkQualityGood,
    /**用户主观感受有瑕疵但不影响沟通*/
    kNERtcNetworkQualityPoor,
    /**勉强能沟通但不顺畅*/
    kNERtcNetworkQualityBad,
    /**网络质量非常差，基本不能沟通*/
    kNERtcNetworkQualityVeryBad,
    /**完全无法沟通*/
    kNERtcNetworkQualityDown,
};

/**
 媒体流转发状态
 */
typedef NS_ENUM(NSUInteger, NERtcChannelMediaRelayState) {
    /**初始状态。在成功调用 stopChannelMediaRelay 停止跨房间媒体流转发后， onNERtcEngineChannelMediaRelayStateDidChange 会回调该状态。*/
    NERtcChannelMediaRelayStateIdle = 0,
    /**SDK 尝试跨房间转发媒体流。*/
    NERtcChannelMediaRelayStateConnecting = 1,
    /**源房间主播角色成功加入目标房间。*/
    NERtcChannelMediaRelayStateRunning = 2,
    /**发生异常，详见 onNERtcEngineDidReceiveChannelMediaRelayEvent 的 error 中提示的错误信息。*/
    NERtcChannelMediaRelayStateFailure = 3,
};

/**
 媒体流转发回调事件
 */
typedef NS_ENUM(NSUInteger, NERtcChannelMediaRelayEvent) {
    /**媒体流转发停止。*/
    NERtcChannelMediaRelayEventDisconnect = 0,
    /**SDK 正在连接服务器，开始尝试转发媒体流。*/
    NERtcChannelMediaRelayEventConnecting = 1,
    /**连接服务器成功。*/
    NERtcChannelMediaRelayEventConnected = 2,
    /**视频媒体流成功转发到目标房间。*/
    NERtcChannelMediaRelayEventVideoSentToDestChannelSuccess = 3,
    /**音频媒体流成功转发到目标房间。*/
    NERtcChannelMediaRelayEventAudioSentToDestChannelSuccess = 4,
    /**屏幕共享等其他媒体流成功转发到目标房间。*/
    NERtcChannelMediaRelayEventOtherStreamSentToDestChannelSuccess = 5,
    /**媒体流转发失败。原因包括：
     * - kNERtcErrChannelReserveErrorParam(414)：请求参数错误。
     * - kNERtcErrChannelMediaRelayInvalidState(30110)：重复调用 startChannelMediaRelay。
     * - kNERtcErrChannelMediaRelayPermissionDenied(30111)：媒体流转发权限不足。例如调用 startChannelMediaRelay 的房间成员为主播角色、或房间为双人通话房间，不支持转发媒体流。
     * - kNERtcErrChannelMediaRelayStopFailed(30112)：调用 stopChannelMediaRelay 前，未调用 startChannelMediaRelay。
    */
    NERtcChannelMediaRelayEventFailure = 100,
};

/**
 加密模式
 */
typedef NS_ENUM(NSUInteger, NERtcEncryptionMode) {
    /**
     * 128 位 SM4 加密，ECB 模式。
     */
    NERtcEncryptionModeGMCryptoSM4ECB,
};


#pragma mark -- Log
/**
 日志级别
*/
typedef NS_ENUM(NSInteger, NERtcLogLevel) {
    /**Fatal 级别日志信息。*/
    kNERtcLogLevelFatal         = 0,
    /**Error 级别日志信息。*/
    kNERtcLogLevelError         = 1,
    /**Warning 级别日志信息。 */
    kNERtcLogLevelWarning       = 2,
    /**Info 级别日志信息。默认级别。 */
    kNERtcLogLevelInfo          = 3,
    /**Detail Info 级别日志信息。*/
    kNERtcLogLevelDetailInfo    = 4,
    /**Debug 级别日志信息。*/
    kNERtcLogLevelDebug         = 5,
    /**Verbose 级别日志信息。*/
    kNERtcLogLevelVerbose       = 6,
    /**关闭日志打印。*/
    kNERtcLogLevelOff           = 7,
};

NS_ASSUME_NONNULL_END
