/*
 * Copyright (c) 2021 NetEase, Inc.  All rights reserved.
 */

#import <Foundation/Foundation.h>
#import "NERtcEngineErrorCode.h"
#import "NERtcEngineEnum.h"
#import "NERtcEngineBase.h"
#import <CoreVideo/CoreVideo.h>

NS_ASSUME_NONNULL_BEGIN

/**
 NERtcEngine 常用回调
 */
@protocol NERtcEngineDelegate <NSObject>

@optional

/**
 发生错误回调。

 该回调方法表示 SDK 运行时出现了网络或媒体相关的错误。
 
 通常情况下，SDK上报的错误意味着 SDK 无法自动恢复，需要 App 干预或提示用户。

 @param errCode 错误码。详细信息请参考 {@link NERtcEngineErrorCode.NERtcError}。
 */
- (void)onNERtcEngineDidError:(NERtcError)errCode;

/**
 发生警告回调。

 该回调方法表示 SDK 运行时出现了网络或媒体相关的警告。
 
 通常情况下，SDK上报的警告意味着 SDK 无法自动适配，需要 App 干预或提示用户。

 @since V4.3.0

 @param warnCode 错误码。详细信息请参考 {@link NERtcWarning}。
 @param msg 警告描述。
 */
- (void)onNERtcEngineDidWarning:(NERtcWarning)warnCode msg:(NSString *)msg;

/**
 SDK和服务端的连接状态已改变回调。
 
 该回调在 SDK 和服务端的连接状态发生改变时触发，并告知用户当前的连接状态和引起状态改变的原因。

 @param state 当前的连接状态。详细信息请参考  {@link NERtcConnectionStateType}。
 @param reason 引起当前连接状态改变的原因。详细信息请参考 {@link NERtcReasonConnectionChangedType}。
 */
- (void)onNERtcEngineConnectionStateChangeWithState:(NERtcConnectionStateType)state
                                             reason:(NERtcReasonConnectionChangedType)reason;

/**
退出房间回调。

App 调用 leaveChannel 方法后，SDK 提示 App 退出房间是否成功。 
 
 @param result 退出房间结果。详细信息请参考  {@link NERtcEngineErrorCode.NERtcError}。
 */
- (void)onNERtcEngineDidLeaveChannelWithResult:(NERtcError)result;

/**
 网络连接中断，且 SDK 连续 3 次重连服务器失败。

 @note
 - SDK 在调用 joinChannel 加入房间成功后，如果和服务器失去连接且连续 3 次重连失败，就会触发该回调。
 - 如果 SDK 在断开连接后，连续 3 次重连失败，SDK 会停止尝试重连。
 
 @param reason 网络连接中断原因。详细信息请查看  {@link NERtcEngineErrorCode.NERtcError}。
 */
- (void)onNERtcEngineDidDisconnectWithReason:(NERtcError)reason;

/**
 重新加入房间回调。
 在弱网环境下，若客户端和服务器失去连接，SDK会自动重连。自动重连成功后触发此回调方法。
 
 @param result 重连结果。详细信息请参考 {@link NERtcEngineErrorCode.NERtcError}。
*/
- (void)onNERtcEngineRejoinChannel:(NERtcError)result;

/**
 远端用户加入当前房间回调。

 该回调提示有远端用户加入了房间，并返回新加入用户的 ID；如果加入之前，已经有其他用户在房间中了，新加入的用户也会收到这些已有用户加入房间的回调。
 
 该回调在如下情况下会被触发：

 - 远端用户调用 joinChannelWithToken 方法加入房间。
 - 远端用户网络中断后重新加入房间。
 
 @param userID   用户 ID。
 @param userName 新加入房间的远端用户名。废弃字段，无需关注。
 */
- (void)onNERtcEngineUserDidJoinWithUserID:(uint64_t)userID userName:(NSString *)userName;

/**
 远端用户离开当前房间回调。

 提示有远端用户离开了房间（或掉线）。用户离开房间有两个原因，即正常离开和超时掉线：

 - 正常离开的时候，远端用户会收到相关消息消息，判断用户离开房间。
 - 超时掉线的依据是，在一定时间内（40~50s），用户没有收到对方的任何数据包，则判定为对方掉线。
 
 @param userID 离开房间的远端用户 ID。
 @param reason 远端用户离开的原因。详细信息请参考  {@link NERtcSessionLeaveReason}。
 */
- (void)onNERtcEngineUserDidLeaveWithUserID:(uint64_t)userID reason:(NERtcSessionLeaveReason)reason;

/**
 直播场景下用户角色已切换回调。

 用户加入房间后，通过 setClientRole 切换用户角色后会触发此回调。例如从主播切换为观众、从观众切换为主播。
 
 @note 直播场景下，如果您在加入房间后调用该方法切换用户角色，调用成功后，会触发以下回调：
 
 - 主播切观众，本端触发 onNERtcEngineDidClientRoleChanged 回调，远端触发 onNERtcEngineUserDidLeaveWithUserID 回调。
 - 观众切主播，本端触发 onNERtcEngineDidClientRoleChanged 回调，远端触发 onNERtcEngineUserDidJoinWithUserID 回调。
 
 @param oldRole 切换前的角色。
 @param newRole 切换后的角色。
 */
- (void)onNERtcEngineDidClientRoleChanged:(NERtcClientRole)oldRole newRole:(NERtcClientRole)newRole;

/**
 重连开始回调。

 客户端和服务器断开连接时，SDK 会进行重连，重连开始时触发此回调。重连结果请参考 onNERtcEngineRejoinChannel 、onNERtcEngineDidDisconnectWithReason。
 */
- (void)onNERtcEngineReconnectingStart;

/**
 远端用户开启音频回调。

 @param userID 远端用户 ID。
 */
- (void)onNERtcEngineUserAudioDidStart:(uint64_t)userID;

/**
 远端用户停用音频回调。

 @param userID 远端用户 ID。
 */
- (void)onNERtcEngineUserAudioDidStop:(uint64_t)userID;

/**
 远端用户开启视频回调。

 @note 纯音频 SDK 禁用该接口，如需使用请前往云信官网下载并替换成视频 SDK。

 @param userID 用户 ID，提示是哪个用户的视频流。
 @param profile 远端用户的视频 profile 档位。详细信息请参考  {@link NERtcVideoProfileType}。
 
 */
- (void)onNERtcEngineUserVideoDidStartWithUserID:(uint64_t)userID videoProfile:(NERtcVideoProfileType)profile;

/**
 远端用户停用视频回调。

 @note 纯音频 SDK 禁用该接口，如需使用请前往云信官网下载并替换成视频 SDK。

 @param userID 远端用户 ID。
 */
- (void)onNERtcEngineUserVideoDidStop:(uint64_t)userID;

/**
 远端用户开启屏幕共享辅流通道的回调。

 @note 纯音频 SDK 禁用该接口，如需使用请前往云信官网下载并替换成视频 SDK。

 @param userID 远端用户 ID。
 @param profile 远端视频分辨率等级。详细信息请参考  {@link NERtcVideoProfileType}。
 */
- (void)onNERtcEngineUserSubStreamDidStartWithUserID:(uint64_t)userID subStreamProfile:(NERtcVideoProfileType)profile;

/**
 远端用户停止屏幕共享辅流通道的回调。

 @note 纯音频 SDK 禁用该接口，如需使用请前往云信官网下载并替换成视频 SDK。

 @param userID 远端用户 ID。
 */
- (void)onNERtcEngineUserSubStreamDidStop:(uint64_t)userID;

@end

/**
直播推流状态回调
如果需要监听直播推流状态，需要实现这个 protocol
*/
@protocol NERtcEngineLiveStreamObserver <NSObject>

@optional
/**
 推流状态已改变回调。

 @param state 推流状态，详细信息请参考  {@link NERtcLiveStreamStateCode}。
 @param taskID 推流任务 ID。
 @param url 推流任务对应的 URL 地址。
*/
- (void)onNERTCEngineLiveStreamState:(NERtcLiveStreamStateCode)state taskID:(NSString *)taskID url:(NSString *)url;

@end

/**
 本地视频数据采集回调
 如果需要对采集数据做美颜等处理，需要实现这个 protocol。
 */
@protocol NERtcEngineVideoFrameObserver <NSObject>

@optional

/**
 设置视频采集数据回调，用于美颜等操作。

 同步返回，Engine 将会继续视频处理流程。

 @note 纯音频 SDK 禁用该接口，如需使用请前往云信官网下载并替换成视频 SDK。

 @param bufferRef CVPixelBufferRef，iOS 原生格式。
 @param rotation 视频方向，详细信息请参考  {@link NERtcVideoRotationType}。
 
 */
- (void)onNERtcEngineVideoFrameCaptured:(CVPixelBufferRef)bufferRef rotation:(NERtcVideoRotationType)rotation;

@end

/**
 音频会话回调
 */
@protocol NERtcEngineAudioSessionObserver <NSObject>

@optional
//
//- (void)onNERtcEngineAudioDeviceInterruptionBegan;
//
//- (void)onNERtcEngineAudioDeviceInterruptionEnded;

/**
 音频路由变化回调。

 @param routing 当前音频输出路由。
 */
- (void)onNERtcEngineAudioDeviceRoutingDidChange:(NERtcAudioOutputRouting)routing;

@end

/**
 外部视频渲染。
 */
@protocol NERtcEngineVideoRenderSink <NSObject>

@optional
/**
 外部渲染视频数据回调
 
 @note 纯音频 SDK 禁用该接口，如需使用请前往云信官网下载并替换成视频 SDK。
 @param frame 视频帧的数据信息。详细信息请参考 {@link NERtcVideoFrame}。
 */
- (void)onNERtcEngineRenderFrame:(NERtcVideoFrame *_Nonnull)frame;

@end


/**
 注册 SEI 观测者。
 
 注册观测者并拉流成功后，当远端流发送 SEI，本端会收到 onNERtcEngineRecvSEIMsg 回调。
 */
@protocol NERtcEngineVideoSEIObserver <NSObject>

@optional

/**
 收到远端流的 SEI 内容回调。
 
 当远端成功发送 SEI 后，本端会收到此回调。

 @note 纯音频 SDK 禁用该回调，如需使用请前往云信官网下载并替换成视频 SDK。

 @param userID 发送 SEI 的用户 ID。
 @param message 对应用户的 SEI 信息。
 */
- (void)onNERtcEngineRecvSEIMsg:(uint64_t)userID message:(NSData *)message;

@end

/**
 NERtcEngine 扩展回调。
 */
@protocol NERtcEngineDelegateEx <NERtcEngineDelegate, NERtcEngineVideoFrameObserver, NERtcEngineAudioSessionObserver,NERtcEngineLiveStreamObserver, NERtcEngineVideoSEIObserver>

@optional

/**
 远端用户视频编码配置已更新回调。

 @note 纯音频 SDK 禁用该接口，如需使用请前往云信官网下载并替换成视频 SDK。

 @param userID 远端用户 ID。
 @param maxProfile 视频编码配置，详细信息请参考 {@link NERtcVideoProfileType}。
 */
- (void)onNERtcEngineUserVideoProfileDidUpdate:(uint64_t)userID maxProfile:(NERtcVideoProfileType)maxProfile;

/**
 远端用户暂停或恢复发送视频流回调。 
 
 @note 纯音频 SDK 禁用该接口，如需使用请前往云信官网下载并替换成视频 SDK。

 @param userID 用户 ID，提示是哪个用户的视频流。
 @param muted 是否暂停发送视频流。
 */
- (void)onNERtcEngineUser:(uint64_t)userID videoMuted:(BOOL)muted;

/**
 远端用户暂停或恢复发送音频流的回调。

 @param userID 用户 ID，提示是哪个用户的音频流。
 @param muted 是否暂停发送音频流。
 */
- (void)onNERtcEngineUser:(uint64_t)userID audioMuted:(BOOL)muted;

/**
 已接收到远端视频首帧回调。
 
 第一帧远端视频显示在视图上时，触发此调用。

 @note 纯音频 SDK 禁用该接口，如需使用请前往云信官网下载并替换成视频 SDK。

 @param userID  远端用户 ID，指定是哪个用户的视频流。 
 */
- (void)onNERtcEngineFirstVideoDataDidReceiveWithUserID:(uint64_t)userID;

/**
 已接收到远端音频首帧回调。

 @param userID 远端用户 ID，指定是哪个用户的音频流。  
 */
- (void)onNERtcEngineFirstAudioDataDidReceiveWithUserID:(uint64_t)userID;

/**
 已解码远端音频首帧的回调。

 @param userID 远端用户 ID，指定是哪个用户的音频流。  
*/
- (void)onEngineFirstAudioFrameDecoded:(uint64_t)userID;

/**
 已显示首帧远端视频回调。

 引擎收到第一帧远端视频流并解码成功时，触发此调用。 App 可在此回调中设置该用户的视频画布。

 @note 纯音频 SDK 禁用该接口，如需使用请前往云信官网下载并替换成视频 SDK。

 @param userID 远端用户 ID，指定是哪个用户的视频流。  
 @param width  首帧视频宽，单位为 px。
 @param height 首帧视频高，单位为 px。
*/
- (void)onEngineFirstVideoFrameDecoded:(uint64_t)userID width:(uint32_t)width height:(uint32_t)height;

/**
 音频设备状态已改变回调。

 @param deviceID 设备 ID。
 @param deviceType 设备类型。详细信息请参考 {@link NERtcAudioDeviceType}。
 @param deviceState 设备状态。详细信息请参考 {@link NERtcAudioDeviceState}。
 */
- (void)onNERtcEngineAudioDeviceStateChangeWithDeviceID:(NSString *)deviceID deviceType:(NERtcAudioDeviceType)deviceType deviceState:(NERtcAudioDeviceState)deviceState;

/**
 视频设备变化回调。
 
 该回调提示系统视频设备状态发生改变，比如被拔出或移除。如果设备已使用外接摄像头采集，外接摄像头被拔开后，视频会中断。

 @note 纯音频 SDK 禁用该接口，如需使用请前往云信官网下载并替换成视频 SDK。

 @param deviceID 设备 ID。
 @param deviceType 设备类型。详细信息请参考 {@link NERtcAudioDeviceType}。
 @param deviceState 设备状态。详细信息请参考 {@link NERtcAudioDeviceState}。
 */
- (void)onNERtcEngineVideoDeviceStateChangeWithDeviceID:(NSString *)deviceID deviceType:(NERtcVideoDeviceType)deviceType deviceState:(NERtcVideoDeviceState)deviceState;

/**
 本地网络类型已改变回调。

 本地网络连接类型发生改变时，SDK 会触发该回调，并在回调中声明当前正在使用的网络连接类型。
 
 @param newConnectionType 当前的本地网络类型，详细信息请参考 {@link NERtcNetworkConnectionType}。
 */
- (void)onNERtcEngineNetworkConnectionTypeChanged:(NERtcNetworkConnectionType)newConnectionType;


/**
 本地用户的音乐文件播放状态改变回调。

   调用 startAudioMixing 播放混音音乐文件后，当音乐文件的播放状态发生改变时，会触发该回调。

   - 如果播放音乐文件正常结束，state 会返回相应的状态码 kNERtcAudioMixingStateFinished，error_code 返回 kNERtcAudioMixingErrorOK。
   - 如果播放出错，则返回状态码 kNERtcAudioMixingStateFailed，error_code 返回相应的出错原因。
   - 如果本地音乐文件不存在、文件格式不支持、无法访问在线音乐文件 URL，error_code都会返回 kNERtcAudioMixingErrorCanNotOpen。

   @param state 音乐文件播放状态，详细信息请参考 {@link NERtcAudioMixingState}。
   @param errorCode 错误码，详细信息请参考 {@link NERtcAudioMixingErrorCode}。
   */
- (void)onAudioMixingStateChanged:(NERtcAudioMixingState)state errorCode:(NERtcAudioMixingErrorCode)errorCode;

/**
 本地用户的音乐文件播放进度回调。
 
 调用 startAudioMixingWithOption 播放混音音乐文件后，当音乐文件的播放进度改变时，会触发该回调。

 @param timeStampMS  音乐文件播放进度，单位为毫秒。
 */
- (void)onAudioMixingTimestampUpdate:(uint64_t)timeStampMS;


/** 
 本地音效文件播放已结束回调。 
 * 
 @param effectId 指定音效的 ID。每个音效均有唯一的 ID。
 */
- (void)onAudioEffectFinished:(uint32_t)effectId;


/**
 提示房间内本地用户瞬时音量的回调。 
 
 该回调默认禁用。可以通过  enableAudioVolumeIndication 方法开启。
 
 开启后，本地用户说话，SDK 会按  enableAudioVolumeIndication 方法中设置的时间间隔触发该回调。 
 
 如果本地用户将自己静音，即调用了 muteLocalAudio，SDK 不再报告该回调。

 @param volume 混音后的音量，范围为 0~100。
*/
- (void)onLocalAudioVolumeIndication:(int)volume;


/**
 提示房间内谁正在说话及说话者瞬时音量的回调。
 
 该回调默认为关闭状态。可以通过 enableAudioVolumeIndication 方法开启。开启后，无论房间内是否有人说话，SDK 都会按 enableAudioVolumeIndication 方法中设置的时间间隔触发该回调。
 
 在返回的数组中：
 
 - 如果有 uid 出现在上次返回的数组中，但不在本次返回的数组中，则默认该 uid 对应的远端用户没有说话。 
 - 如果 volume 为 0，表示该用户没有说话。
 - 如果数组为空，则表示此时远端没有人说话。

 @param speakers 每个说话者的用户 ID 和音量信息的数组。详细信息请参考 {@link NERtcAudioVolumeInfo}。
 @param totalVolume 混音后的总音量，范围为 0~100。
 */
-(void)onRemoteAudioVolumeIndication:(nullable NSArray<NERtcAudioVolumeInfo*> *)speakers totalVolume:(int)totalVolume;

/**
 硬件资源已释放回调。
 
 如果回调结果成功，表示 SDK 已经完全释放了硬件资源。

 您可以在收到回调成功后重新配置或者使用硬件资源，例如 AVAudioSession 等配置。

 @param result 释放结果。详细信息请参考 {@link NERtcEngineErrorCode.NERtcError}。
*/
- (void)onNERtcEngineHardwareResourceReleased:(NERtcError)result;

/**
 摄像头对焦区域已改变回调。
 
 该回调表示相机的对焦区域发生了改变。
 
 该回调是由本地用户调用 setCameraFocusPosition 方法改变对焦位置触发的。

 @note 纯音频 SDK 禁用该接口，如需使用请前往云信官网下载并替换成视频 SDK。
 
 @param focusPoint 新的对焦区域位置。
*/
- (void)onNERtcCameraFocusChanged:(CGPoint)focusPoint;

/**
 摄像头曝光区域已改变回调。
 
 该回调是由本地用户调用 setCameraExposurePosition方法改变曝光位置触发的。

 @note 纯音频 SDK 禁用该接口，如需使用请前往云信官网下载并替换成视频 SDK。

 @param exposurePoint 新的曝光区域位置。
*/
- (void)onNERtcCameraExposureChanged:(CGPoint)exposurePoint;



/**
 检测到啸叫回调。

 当声源与扩音设备之间因距离过近时，可能会产生啸叫。NERTC SDK 支持啸叫检测，当检测到有啸叫信号产生的时候，自动触发该回调直至啸叫停止。App 应用层可以在收到啸叫回调时，提示用户静音麦克风，或直接静音麦克风。
 
 @note 啸叫检测功能一般用于语音聊天室或在线会议等纯人声环境，不推荐在包含背景音乐的娱乐场景中使用。
 */
- (void)onNERtcEngineAudioHasHowling;

/**
 跨房间媒体流转发状态发生改变回调。
 @param state 当前跨房间媒体流转发状态。详细信息请参考 {@link NERtcChannelMediaRelayState}。
 @param channelName 媒体流转发的目标房间名。
 */
- (void)onNERtcEngineChannelMediaRelayStateDidChange:(NERtcChannelMediaRelayState)state channelName:(NSString *)channelName;

/**
  媒体流相关转发事件回调。
 @param event 当前媒体流转发事件。详细信息请参考 {@link NERtcChannelMediaRelayEvent}。
 @param channelName 转发的目标房间名。
 @param error 相关错误码。详细信息请参考 {@link NERtcEngineErrorCode.NERtcError}。
 */
- (void)onNERtcEngineDidReceiveChannelMediaRelayEvent:(NERtcChannelMediaRelayEvent)event channelName:(NSString *)channelName error:(NERtcError)error;

/**
  音频录制状态回调。
 
  @param code 音频录制状态码。详细信息请参考 {@link NERtcAudioRecordingCode}。
  @param filePath 音频录制文件保存路径。
*/
- (void)onNERtcEngineAudioRecording:(NERtcAudioRecordingCode)code filePath:(NSString *)filePath;


/**
  本地发布流已回退为音频流、或已恢复为音视频流回调。

  如果您调用了设置本地推流回退选项 setLocalPublishFallbackOption 接口，并将 option 设置为 kNERtcStreamFallbackOptionAudioOnly 后，当上行网络环境不理想、本地发布的媒体流回退为音频流时，或当上行网络改善、媒体流恢复为音视频流时，会触发该回调。  
 
  @since V4.3.0
  @param isFallback 本地发布流已回退或已恢复。
                         - YES： 由于网络环境不理想，发布的媒体流已回退为音频流。
                         - NO：由于网络环境改善，从音频流恢复为音视频流。
 @param streamType 对应的视频流类型，即主流或辅流。详细信息请参考 {@link NERtcStreamChannelType}。
*/
- (void)onNERtcEngineLocalPublishFallbackToAudioOnly:(BOOL)isFallback streamType:(NERtcStreamChannelType)streamType;

/**
  订阅的远端流已回退为音频流、或已恢复为音视频流回调。

  如果你调用了设置远端订阅流回退选项 setRemoteSubscribeFallbackOption 接口并将 option 设置 kNERtcStreamFallbackOptionAudioOnly 后，当下行网络环境不理想、仅接收远端音频流时，或当下行网络改善、恢复订阅音视频流时，会触发该回调。
 
 @since V4.3.0
 @param isFallback 远端订阅流已回退或恢复：
                           - YES： 由于网络环境不理想，订阅的远端流已回退为音频流。
                           - NO：由于网络环境改善，订阅的远端流从音频流恢复为音视频流。
 @param uid 远端用户 ID。
 @param streamType 对应的视频流类型，即主流或辅流。详细信息请参考 {@link NERtcStreamChannelType}。
*/

- (void)onNERtcEngineRemoteSubscribeFallbackToAudioOnly:(uint64_t)uid isFallback:(BOOL)isFallback streamType:(NERtcStreamChannelType)streamType;



@end


/**
 音频数据回调
 如果需要对音频数据做处理，需要实现这个 protocol
 
 */
@protocol NERtcEngineAudioFrameObserver <NSObject>

@optional

/**
 采集音频数据回调，用于声音处理等操作。
 
 - 返回音频数据支持读写。
 - 有本地音频数据驱动就会回调。
 - 需要同步返回，engine 将会继续音频处理流程。

 允许修改 frame 里 void *data 所指向的内容，不允许修改 format。如果对 format 有要求，请通过setParameter :  kNERtcKeyObserveRecordAudioFrameFormat 进行设置。
 
 @param frame 音频帧数据，详细信息请参考 {@link NERtcAudioFrame}。
 */
- (void)onNERtcEngineAudioFrameDidRecord:(NERtcAudioFrame *)frame;


 /**
 播放音频数据回调，用于声音处理等操作。
 
 - 需要同步返回，engine 将会继续音频处理流程。
 - 返回音频数据支持读写。
 - 有本地音频数据驱动就会回调。

 允许修改 frame 里 void *data 所指向的内容，不允许修改 format。如果对 format 有要求，请通过setParameter :  kNERtcKeyObservePlaybackAudioFrameFormat 进行设置。
  
 @param frame 音频帧数据，详细信息请参考 {@link NERtcAudioFrame}。
 */
- (void)onNERtcEngineAudioFrameWillPlayback:(NERtcAudioFrame *)frame;

/**
 获取某一远端用户混音前的音频数据。
 
 成功注册音频观测器后，如果订阅了远端音频（默认订阅）且远端用户开启音频后，SDK 会在捕捉到混音前的音频数据时，触发该回调，将音频数据回调给用户。
 
 @note
 - 返回音频数据只读。
 - 不支持修改 frame 里 void *data 所指向的内容，不支持修改 format。
 
 @param userID 远端用户的 ID。
 @param frame 音频帧数据，详细信息请参考 {@link NERtcAudioFrame}。
*/
- (void)onNERtcEnginePlaybackAudioFrameBeforeMixingWithUserID:(uint64_t)userID frame:(NERtcAudioFrame *)frame;

/**
 获取本地用户和所有远端用户混音后的原始音频数据。

 @note
 - 该回调为只读模式。
 - 返回音频数据只读。
 - 有本地音频数据驱动就会回调。
 
 不支持修改 frame 里 void *data 所指向的内容，不允许修改 format。如果对 format 有要求，请通过setMixedAudioFrameParameters进行设置
 
 @param frame 音频帧数据，详细信息请参考 {@link NERtcAudioFrame}。
*/
- (void)onNERtcEngineMixedAudioFrame:(NERtcAudioFrame *)frame;

@end

NS_ASSUME_NONNULL_END
