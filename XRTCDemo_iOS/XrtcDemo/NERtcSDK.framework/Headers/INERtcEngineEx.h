/*
 * Copyright (c) 2021 NetEase, Inc.  All rights reserved.
 */

#ifndef INERtcEngineEx_h
#define INERtcEngineEx_h

#import "INERtcEngine.h"
#import "NERtcEngineStatistics.h"
#import "NERtcEngineBase.h"
#import "NERtcEngineDelegate.h"

NS_ASSUME_NONNULL_BEGIN

/**
 NERtcEngine 扩展接口
 */
@protocol INERtcEngineEx <INERtcEngine>

/**
 设置是否开启视频大小流模式。

 该方法设置单流或者双流模式。发送端开启双流模式后，接收端可以选择接收大流还是小流。其中，大流指高分辨率、高码率的视频流，小流指低分辨率、低码率的视频流。

 @note
 - 纯音频 SDK 禁用该接口，如需使用请前往云信官网下载并替换成视频 SDK。
 - 该方法只对摄像头数据生效，自定义输入、屏幕共享等视频流无效。
 - 该方法在加入房间前后都能调用。

 @param enable  YES 发送双流， NO 发送单流。
 @return 操作返回值，成功则返回 0
 */
- (int)enableDualStreamMode:(BOOL)enable;

/**
 设置本地用户的媒体流优先级。

 如果某个用户的优先级为高，那么该用户媒体流的优先级就会高于其他用户，弱网环境下 SDK 会优先保证其他用户收到的、高优先级用户的媒体流的质量。
 
 @note 
 - 请在加入房间（joinChannel）前调用此方法。
 - 快速切换房间 （switchChannel） 后，媒体优先级会恢复为默认值，即普通优先级。
 
 @param priority   本地用户的媒体流优先级，默认为 #kNERtcMediaPriorityNormal，即普通优先级。详细信息请参考 {@link NERtcMediaPriorityType}。 
 @param preemptive 是否开启抢占模式，默认为 NO，即不开启。
                    - 抢占模式开启后，本地用户可以抢占其他用户的高优先级，被抢占的用户的媒体优先级变为普通优先级，在抢占者退出房间后，其他用户的优先级仍旧维持普通优先级。
                    - 抢占模式关闭时，如果房间中已有高优先级用户，则本地用户的高优先级设置不生效，仍旧为普通优先级。
 
 @return 操作返回值，成功则返回 0
 */
- (int)setLocalMediaPriority:(NERtcMediaPriorityType)priority preemptive:(BOOL)preemptive;


/**
 设置弱网条件下发布的音视频流回退选项。

 在网络不理想的环境下，发布的音视频质量都会下降。使用该接口并将 option 设置为 kNERtcStreamFallbackOptionAudioOnly 后:
  - SDK 会在上行弱网且音视频质量严重受影响时，自动关断视频流，尽量保证音频质量。
  - 同时 SDK 会持续监控网络质量，并在网络质量改善时恢复音视频流。
  - 当本地发布的音视频流回退为音频流时，或由音频流恢复为音视频流时，SDK 会触发本地发布的媒体流已回退为音频流 onLocalPublishFallbackToAudioOnly 回调。
 @note 请在加入房间（joinChannel）前调用此方法。
 @since V4.3.0
 
 @param option   发布音视频流的回退选项，默认为不开启回退。 详细信息请参考 {@link NERtcStreamFallbackOptions}。
 
 @return 操作返回值，成功则返回 0
 */
- (int)setLocalPublishFallbackOption:(NERtcStreamFallbackOptions)option;

/**
 设置弱网条件下订阅的音视频流回退选项。

 弱网环境下，订阅的音视频质量会下降。通过该接口设置订阅音视频流的回退选项后：
 - SDK 会在下行弱网且音视频质量严重受影响时，将视频流切换为小流，或关断视频流，从而保证或提高通信质量。
 - SDK 会持续监控网络质量，并在网络质量改善时自动恢复音视频流。
 - 当远端订阅流回退为音频流时，或由音频流恢复为音视频流时，SDK 会触发远端订阅流已回退为音频流回调。
 
 @note 请在加入房间（joinChannel）前调用此方法。
 @since V4.3.0
 
 @param option   订阅音视频流的回退选项，默认为弱网时回退到视频小流。详细信息请参考 {@link NERtcStreamFallbackOptions}。
 
 @return 操作返回值，成功则返回 0
 */
- (int)setRemoteSubscribeFallbackOption:(NERtcStreamFallbackOptions)option;

/**
 开关本地音频发送。
 该方法用于允许或禁止向网络发送本地音频流。

 @note - 该方法不影响音频采集状态，因为并没有禁用音频采集设备。
 - 静音状态会在通话结束后被重置为非静音。

 @param muted 是否开启本地音频发送。
 @return 操作返回值，成功则返回 0
 */
- (int)muteLocalAudio:(BOOL)muted;

/**
 取消或恢复订阅指定远端用户音频流。

 加入房间时，默认订阅所有远端用户的音频流，您可以通过此方法取消或恢复订阅指定远端用户的音频流。

 @note 该方法需要在加入房间后调用。

 @param subscribe 是否订阅指定音频流。
 @param userID 指定远端用户的 ID。
 @return 操作返回值，成功则返回 0

 */
- (int)subscribeRemoteAudio:(BOOL)subscribe forUserID:(uint64_t)userID;

/**
 取消或恢复订阅所有远端用户音频流。

 @note
 - 加入房间时，默认订阅所有远端用户的音频，此时请勿调用 subscribeAllRemoteAudio(YES) 重复订阅所有远端用户的音频流。
 - 该方法需要在加入房间后调用。
 - 对后续加入的用户也同样生效。
 
 @param subscribe 是否取消订阅所有远端用户的音频流。
 @return 操作返回值，成功则返回 0
 */
- (int)subscribeAllRemoteAudio:(BOOL)subscribe;

/**
 订阅或取消订阅指定远端用户的视频流。

 用户加入房间之后，默认不订阅远端用户的视频流，如果希望看到指定远端用户的视频，可以在监听到对方加入房间或发布视频流之后，通过此方法订阅该用户的视频流。

 @note
 - 纯音频 SDK 禁用该接口，如需使用请前往云信官网下载并替换成视频 SDK。
 - 该方法需要在加入房间后调用。

 @param subscribe 是否取消订阅本地视频流。
 @param userID 指定用户的用户 ID。
 @param streamType 订阅的视频流类型，详细信息请参考  {@link NERtcRemoteVideoStreamType}。
 @return 操作返回值，成功则返回 0

 */
- (int)subscribeRemoteVideo:(BOOL)subscribe forUserID:(uint64_t)userID streamType:(NERtcRemoteVideoStreamType)streamType;

/**
 开启视频预览。

 - 该方法用于在进入房间前启动本地视频预览。调用该 API 前，必须调用 setupLocalVideoCanvas 设置视频画布。
 - 启用本地视频预览后，在进入房间前，必须先调用 stopPreview 关闭本地预览。

 @note 纯音频 SDK 禁用该接口，如需使用请前往云信官网下载并替换成视频 SDK。

 @return 操作返回值，成功则返回 0
 */
- (int)startPreview;

/**
 停止视频预览。

@note <br/>

 - 该方法需要在加入房间前调用。
 - 纯音频 SDK 禁用该接口，如需使用请前往云信官网下载并替换成视频 SDK。

 @return 操作返回值，成功则返回 0
 */
- (int)stopPreview;

/**
 设置本端的视频显示模式。

 @note 纯音频 SDK 禁用该接口，如需使用请前往云信官网下载并替换成视频 SDK。

 @param mode 视频显示模式。详细信息请参考 {@link NERtcVideoRenderScaleMode}。
 @return 操作返回值，成功则返回 0
 */
- (int)setLocalRenderScaleMode:(NERtcVideoRenderScaleMode)mode;

/**
 设置远端的视频显示模式。

 @note 纯音频 SDK 禁用该接口，如需使用请前往云信官网下载并替换成视频 SDK。

 @param mode 视频显示模式。详细信息请参考 {@link NERtcVideoRenderScaleMode}。
 @param userID 远端用户 ID。
 @return 操作返回值，成功则返回 0
 */
- (int)setRemoteRenderScaleMode:(NERtcVideoRenderScaleMode)mode forUserID:(uint64_t)userID;

/**
 设置本地视频画面的旋转模式。
 
 该接口用于设置本地视频画面在本地和远端设备上的旋转模式，可以指定本地画面和系统设备的横屏/竖屏模式一致、或者和 App UI的横屏/竖屏模式一致。
 
 @note 
 - 请在加入房间之前调用此接口。
 - 无论在哪种旋转模式下，采集端和播放端的旋转模式均保持一致。即本地看到的本地画面和远端看到的本地画面总是同样横屏模式或同样竖屏模式。
 - 纯音频 SDK 禁用该接口，如需使用请前往云信官网下载并替换成视频 SDK。

 @since V4.3.0

 @param rotationMode 视频旋转模式。详细信息请参考 {@link NERtcVideoRotationMode}。
 @return 操作返回值，成功则返回 0
 */
- (int)setVideoRotationMode:(NERtcVideoRotationMode)rotationMode;

/**
 取消或恢复发布本地视频流。

 成功调用该方法后，远端会触发 onNERtcEngineUser:videoMuted: 回调。

 @note
 - 纯音频 SDK 禁用该接口，如需使用请前往云信官网下载并替换成视频 SDK。
 - 调用该方法取消发布本地视频流后，SDK 不再发送本地视频流。
 - 该方法在加入房间前后均可调用。
 - 若调用该方法取消发布本地视频流，通话结束后会被重置为默认状态，即默认发布本地视频流。
 - 该方法与 enableLocalVideo 的区别在于，enableLocalVideo 会关闭本地摄像头设备，muteLocalVideoStream 不影响本地视频流采集，不禁用摄像头，且响应速度更快。

 @param muted 是否取消发布本地视频流。
 @return 操作返回值，成功则返回 0
 */
- (int)muteLocalVideo:(BOOL)muted;

/**
 启用或关闭扬声器播放。

 该方法设置是否将语音路由到扬声器，即设备外放。

 @note 该方法需要在加入房间后调用。

 @param enable 是否将音频路由到外放。
 @return 操作返回值，成功则返回 0
 */
- (int)setLoudspeakerMode:(bool)enable;

/**
 检查扬声器状态启用状态。

 @note 该方法可在加入房间前后调用。

 @param enabled 是否正在使用扬声器模式。
 @return 操作返回值，成功则返回 0
 */
- (int)getLoudspeakerMode:(bool *)enabled;


/**
 开始记录音频 dump。

 音频 dump 可用于分析音频问题。

 @return 操作返回值，成功则返回 0
 */
- (int)startAudioDump;

/**
 结束记录音频 dump。

 @return 操作返回值，成功则返回 0
 */
- (int)stopAudioDump;

#pragma mark -- External Video Input

/**
 开启或关闭外部视频源数据输入。

 - 该方法设置内部引擎为启用状态，在 leaveChannel 后仍然有效。如果需要关闭该功能，需要在下次通话前调用接口关闭该功能。
 - 如果使用了外部视频源，请在调用 startScreenCapture、enableLocalVideo 或 startPreview 之前调用此 API。
 - 外部输入视频源是屏幕共享时默认使用辅流通道，非屏幕共享时使用主流通道，此时与 Camera 互斥。
 - 之前使用主流通道或者当前使用主流通道，且主流已经开启时，请勿更改设置。之前使用辅流通道或者当前使用辅流通道，且辅流已经开启时，请勿更改设置。
 
 @note 纯音频 SDK 禁用该接口，如需使用请前往云信官网下载并替换成视频 SDK。

 @param enable 是否使用外部视频源。

 - true：使用外部视频源。
 - false：（默认）不使用外部视频源。
 @param isScreen  使用外部视频源时，外部视频源是否为屏幕共享数据。
 @return 操作返回值，成功则返回 0
*/
- (int)setExternalVideoSource:(BOOL)enable isScreen:(BOOL)isScreen;

/**
 推送外部视频帧。

 该方法主动将视频帧数据用 NERtcVideoFrame 类封装后传递给 SDK。

 @note
 - 该方法设置内部引擎为启用状态，在 leaveChannel 后不再有效。
 - 请确保在您调用本方法前已调用 setExternalVideoSource，并将参数设为 YES，否则调用本方法后会一直报错。
 - 纯音频 SDK 禁用该接口，如需使用请前往云信官网下载并替换成视频 SDK。

 @param frame 视频帧的数据信息。详细信息请参考 {@link NERtcVideoFrame}。
 
 @return 操作返回值，成功则返回 0
*/
- (int)pushExternalVideoFrame:(NERtcVideoFrame*)frame;

#pragma mark -- Sub Stream

/**
 设置本地辅流视频画布。
 该方法设置本地辅流视频显示信息。App 通过调用此接口绑定本地辅流的显示视窗（view）。 在 App 开发中，通常在初始化后调用该方法进行本地视频设置，然后再加入房间。

 @note
 - 若使用外部渲染，建议在加入房间之前设置。
 - 请在初始化后调用该方法，然后再加入房间。
 - 同一个画布只能设置给一个用户。
 - 纯音频 SDK 禁用该接口，如需使用请前往云信官网下载并替换成视频 SDK。

 @param canvas 视频画布。详细信息请参考 {@link NERtcVideoCanvas}。删除画布设置时请传 nil。
 @return 操作返回值，成功则返回 0
 */
- (int)setupLocalSubStreamVideoCanvas:(NERtcVideoCanvas *)canvas;

/**
 开启屏幕共享，屏幕共享内容以辅流形式发送。

 只能在加入房间后调用。

 如果您在加入房间后调用该方法开启辅流，调用成功后，远端触发 onNERtcEngineUserSubStreamDidStartWithUserID 回调。

 @note 纯音频 SDK 禁用该接口，如需使用请前往云信官网下载并替换成视频 SDK。
 
 @param config 本地辅流发送配置，详细信息请参考  {@link NERtcVideoSubStreamEncodeConfiguration}。
 @return 操作返回值，成功则返回 0
 */
- (int)startScreenCapture:(NERtcVideoSubStreamEncodeConfiguration *)config;

/**
 关闭辅流形式的屏幕共享。

 如果您在加入房间后调用该方法关闭辅流，调用成功后，远端触发 onNERtcEngineUserSubStreamDidStop 回调。

 @note 纯音频 SDK 禁用该接口，如需使用请前往云信官网下载并替换成视频 SDK。
 
 @return 操作返回值，成功则返回 0
 */
- (int)stopScreenCapture;

/**
 设置本端的屏幕共享辅流视频显示模式。

 在本端开启辅流形式的屏幕共享时使用。App 可以多次调用此方法更改显示模式。

 @note
 - 纯音频 SDK 禁用该接口，如需使用请前往云信官网下载并替换成视频 SDK。
 - 调用此方法前，必须先通过 setupLocalSubStreamVideoCanvas 设置本地辅流画布。

 @param mode 视频显示模式。详细信息请参考 {@link NERtcVideoRenderScaleMode}。
 @return 操作返回值，成功则返回 0
 */
- (int)setLocalRenderSubStreamScaleMode:(NERtcVideoRenderScaleMode)mode;

/**
 设置远端的辅流视频画布。

 该方法绑定远端用户和辅流显示视图，即指定某个 userID 使用对应的画布显示。

 @note 
 - 纯音频 SDK 禁用该接口，如需使用请前往云信官网下载并替换成视频 SDK。
 - 若使用外部渲染，建议在收到 onNERtcEngineUserDidJoinWithUserID 后设置。
 - 如果 App 无法事先知道对方的用户 ID，可以在远端加入房间后调用。从 onNERtcEngineUserDidJoinWithUserID 中获取对方的 uid，并通过本方法为该用户设置辅流视频画布。
 - 退出房间后，SDK 清除远端用户和画布的的绑定关系，该设置自动失效。

 @param userID 远端用户 ID。
 @param canvas 视频画布。详细信息请参考 {@link NERtcVideoCanvas}。删除画布设置时请传 nil。
 @return 操作返回值，成功则返回 0
 */
- (int)setupRemoteSubStreamVideoCanvas:(NERtcVideoCanvas *)canvas forUserID:(uint64_t)userID;

/**
 订阅或取消订阅远端的屏幕共享辅流视频，订阅之后才能接收远端的辅流视频数据。

 @note
 - 纯音频 SDK 禁用该接口，如需使用请前往云信官网下载并替换成视频 SDK。
 - 只能在加入房间后调用。

 @param subscribe 是否订阅远端的屏幕共享辅流视频。
 @param userID 远端用户 ID。
 @return 操作返回值，成功则返回 0
 */
- (int)subscribeRemoteSubStreamVideo:(BOOL)subscribe forUserID:(uint64_t)userID;

/**
 设置远端的屏幕共享辅流视频显示模式。

 @note
 - 纯音频 SDK 禁用该接口，如需使用请前往云信官网下载并替换成视频 SDK。
 - 调用本接口之前，请先通过 subscribeRemoteSubStreamVideo 订阅远端的屏幕共享辅流视频。

 @param mode 视频显示模式。详细信息请参考 {@link NERtcVideoRenderScaleMode}。
 @param userID 远端用户 ID。
 @return 操作返回值，成功则返回 0
 */
- (int)setRemoteRenderSubStreamVideoScaleMode:(NERtcVideoRenderScaleMode)mode forUserID:(uint64_t)userID;

#pragma mark -- AUDIO SESSION

/**
 设置 SDK 对 Audio Session 的控制权限。

 该方法仅适用于 iOS 平台。
 该方法限制  SDK 对 Audio Session 的操作权限。在默认情况下，SDK 和 App 对 Audio Session 都有控制权，但某些场景下，App 会希望限制  SDK 对 Audio Session 的控制权限，而使用其他应用或第三方组件对 Audio Session 进行操控。调用该方法可以实现该功能。
 该接口只能在入会之前调用。

 @note 一旦调用该方法限制了 SDK 对 Audio Session 的控制权限， SDK 将无法对 Audio Session 进行相关设置，而需要用户自己或第三方组件进行维护。
 @param restriction SDK 对 Audio Session 的控制权限。详细信息请参考 {@link NERtcAudioSessionOperationRestriction}。

 @return 操作返回值，成功则返回 0
*/
- (int)setAudioSessionOperationRestriction:(NERtcAudioSessionOperationRestriction)restriction;

#pragma mark -- AUDIO DEVICE MANAGER

/**
 设置是否静音音频播放设备。
 
 @param muted 是否静音播放设备。默认为不静音状态。
 @return 操作返回值，成功则返回 0
 */
- (int)setPlayoutDeviceMute:(bool)muted;

/**
 获取音频播放设备的静音状态。
 
 @param muted 是否静音。
 @return 操作返回值，成功则返回 0
 */
- (int)getPlayoutDeviceMute:(bool *)muted;


/**
 设置是否静音音频采集设备。
 
 @param muted 是否静音音频采集设备。默认为不静音。
 @return 操作返回值，成功则返回 0
 */
- (int)setRecordDeviceMute:(bool)muted;


/**
 查询当前音频采集设备是否静音。

 @param muted 是否静音。
 @return 操作返回值，成功则返回 0
 */
- (int)getRecordDeviceMute:(bool *)muted;


#pragma mark -- CAMERA & TORCH SETTINGS

/**
 检测设备当前使用的摄像头是否支持缩放功能。

 该方法需要在相机启动后调用，例如调用 startPreview 或 joinChannel 后。

 @note 纯音频 SDK 禁用该接口，如需使用请前往云信官网下载并替换成视频 SDK。

 @return YES 表示支持，NO 表示支持。
 */
- (BOOL)isCameraZoomSupported;

/**
 检测设备是否支持闪光灯常亮。

 @note
 - 一般情况下，App 默认开启前置摄像头，因此如果设备前置摄像头不支持闪光灯，直接使用该方法会返回 NO。如果需要检查后置摄像头是否支持闪光灯，需要先使用switchCamera切换摄像头，再使用该方法。
 - 该方法需要在相机启动后调用，例如调用 startPreview 或 joinChannel 后。
 - 纯音频 SDK 禁用该接口，如需使用请前往云信官网下载并替换成视频 SDK。

 @return YES 表示支持，NO 表示不支持。
 */
- (BOOL)isCameraTorchSupported;

/**
 检测设备是否支持手动对焦功能。

 @note
 - 该方法需要在相机启动后调用，例如调用 startPreview 或 joinChannel 后。
 - 纯音频 SDK 禁用该接口，如需使用请前往云信官网下载并替换成视频 SDK。

 @return YES 表示支持，NO 表示不支持。
 */
- (BOOL)isCameraFocusSupported;

/**
 检测设备是否支持手动曝光功能。

 @note
 - 该方法需要在相机启动后调用，例如调用 startPreview 或 joinChannel 后。
 - 纯音频 SDK 禁用该接口，如需使用请前往云信官网下载并替换成视频 SDK。

 @return YES 表示支持，NO 表示不支持。
 */
- (BOOL)isCameraExposurePositionSupported;

/**
 设置当前摄像头手动曝光位置。

 成功调用该方法后，本地会触发 onCameraExposureChanged 回调。

 @note
 - 该方法需要在相机启动后调用，例如调用 startPreview 或 joinChannel 后。
 - 纯音频 SDK 禁用该接口，如需使用请前往云信官网下载并替换成视频 SDK。

 @param positionInView 曝光位置点。
 @return 操作返回值，成功则返回 0
 */
- (int)setCameraExposurePosition:(CGPoint)positionInView;

/**
 设置是否打开闪光灯。

 @note
 - 该方法需要在相机启动后调用，例如调用 startPreview 或 joinChannel 后。
 - 纯音频 SDK 禁用该接口，如需使用请前往云信官网下载并替换成视频 SDK。

 @param on YES 表示开启；NO 表示关闭。
 @return 操作返回值，成功则返回 0
 */
- (int)setCameraTorchOn:(BOOL)on;

/**
 查询设备是否开启了闪光灯。

 @note 纯音频 SDK 禁用该接口，如需使用请前往云信官网下载并替换成视频 SDK。

 @return YES 表示开启；NO 表示关闭。
 */
- (BOOL)isCameraTorchOn;

/**
 设置当前摄像头缩放比例。

 @note
 - 该方法需要在相机启动后调用，例如调用 startPreview 或 joinChannel 后。
 - 建议在调用本接口前，先通过 getCameraMaxZoom 查看摄像头支持的最大缩放比例，并根据实际需求合理设置需要的缩放比例。
 - 纯音频 SDK 禁用该接口，如需使用请前往云信官网下载并替换成视频 SDK。

 @param factor 摄像头缩放比例。
 @return 操作返回值，成功则返回 0
 */
- (int)setCameraZoomFactor:(float)factor;

/**
 获取摄像头支持最大缩放比例。

 @note
 - 该方法需要在相机启动后调用，例如调用 startPreview 或 joinChannel 后。
 - 纯音频 SDK 禁用该接口，如需使用请前往云信官网下载并替换成视频 SDK。

 @return 最大缩放比例。
 */
- (float)maxCameraZoomScale;

/**
 设置手动对焦位置。

 成功调用该方法后，本地会触发  onNERtcCameraFocusChanged 回调。

 @note
 - 该方法需要在相机启动后调用，例如调用 startPreview 或 joinChannel 后。
 - 纯音频 SDK 禁用该接口，如需使用请前往云信官网下载并替换成视频 SDK。

 @param focusX 触摸点相对于视图的横坐标，范围为 0~1。
 @param focusY 触摸点相对于视图的纵坐标，范围为 0~1。
 @return 操作返回值，成功则返回 0
 */
- (int)setCameraFocusPositionX:(float)focusX Y:(float)focusY;

#pragma mark -- AUDIO MIXING

/**
 开始播放音乐文件。
 
 该方法指定本地或在线音频文件来和录音设备采集的音频流进行混音。
 
 - 支持的音乐文件类型包括 MP3、M4A、AAC、3GP、WMA 和 WAV 格式，支持本地文件或在线 URL。
 - 成功调用该方法后，如果播放状态改变，本地会触发 onAudioMixingStateChanged 回调。
 
 @note 
 - 请在加入房间后调用该方法。
 - 从 V4.3.0 版本开始，若您在通话中途调用此接口播放音乐文件时，手动设置了伴音播放音量或发送音量，则当前通话中再次调用时默认沿用此设置。
 - 从 V4.4.0 版本开始，开启或关闭本地音频采集的操作不再影响音乐文件播放，即 enableLocalAudio(NO) 后仍旧可以播放音乐文件。

 @param option 创建混音任务配置的选项，包括混音任务类型、混音文件全路径或 URL 等。详细信息请参考 {@link NERtcCreateAudioMixingOption}。
 @return 操作返回值，成功则返回 0
 */
- (int)startAudioMixingWithOption:(NERtcCreateAudioMixingOption *)option;

/**
 停止播放音乐文件及混音。

 该方法停止播放伴奏。请在房间内调用该方法。

 @return 操作返回值，成功则返回 0
 */
- (int)stopAudioMixing;

/**
 暂停播放音乐文件及混音。

 该方法暂停播放伴奏。请在房间内调用该方法。

 @return 操作返回值，成功则返回 0
 */
- (int)pauseAudioMixing;

/**
 该方法恢复混音，继续播放伴奏。请在房间内调用该方法。

 @return 操作返回值，成功则返回 0
 */
- (int)resumeAudioMixing;

/**
 设置伴奏的发送音量。

 @param volume 伴奏发送音量。取值范围为 0~100。默认 100，即原始文件音量。
 @return 操作返回值，成功则返回 0
 */
- (int)setAudioMixingSendVolume:(uint32_t)volume;

/**
 获取伴奏发送音量。

 该方法获取混音里伴奏的发送音量大小。请在房间内调用该方法。

 @param volume 伴奏发送音量。
 @return 操作返回值，成功则返回 0
 */
- (int)getAudioMixingSendVolume:(uint32_t *)volume;

/**
 调节伴奏播放音量。

 该方法调节混音里伴奏的播放音量大小。请在房间内调用该方法。

 @param volume  伴奏播放音量。取值范围为 0~100。默认 100，即原始文件音量。
 @return 操作返回值，成功则返回 0
 */
- (int)setAudioMixingPlaybackVolume:(uint32_t)volume;

/**
 获取伴奏播放音量。

 该方法获取混音里伴奏的播放音量大小。请在房间内调用该方法。

 @param volume  伴奏播放音量。范围为 0~100。
 @return 操作返回值，成功则返回 0
 */
- (int)getAudioMixingPlaybackVolume:(uint32_t *)volume;

/**
 获取伴奏时长。

 该方法获取伴奏时长，单位为毫秒。请在房间内调用该方法。

 @param duration 伴奏时长，单位为毫秒。
 @return 操作返回值，成功则返回 0
 */
- (int)getAudioMixingDuration:(uint64_t *)duration;

/**
 获取音乐文件的播放进度。

 该方法获取当前伴奏播放进度，单位为毫秒。请在房间内调用该方法。

 @param position 音乐文件的播放位置，单位为毫秒。
 @return 操作返回值，成功则返回 0
 */
- (int)getAudioMixingCurrentPosition:(uint64_t *)position;

/**
 设置音乐文件的播放位置。

 该方法可以设置音频文件的播放位置，这样你可以根据实际情况播放文件，而非从头到尾播放整个文件。

 @param position 音乐文件的播放位置，单位为毫秒。
 @return 操作返回值，成功则返回 0
 */
- (int)setAudioMixingPosition:(uint64_t)position;

#pragma mark -- AUDIO EFFECT
/**
 播放指定音效文件。

 该方法播放指定的本地或在线音效文件。

 - 成功调用该方法后，如果播放结束，本地会触发 onAudioEffectFinished 回调。
 - 支持的音效文件类型包括 MP3、M4A、AAC、3GP、WMA 和 WAV 格式，支持本地文件和在线 URL。

 @note
 - 请在加入房间后调用该方法。
 - 您可以多次调用该方法，通过传入不同的音效文件的 effectId 和option ，同时播放多个音效文件，实现音效叠加。为获得最佳用户体验，建议同时播放的音效文件不超过 3 个。

 @param effectId 指定音效的 ID。每个音效均应有唯一的 ID。
 @param option 音效相关参数，包括混音任务类型、混音文件路径等。详细信息请参考 {@link NERtcCreateAudioEffectOption}。
 @return 操作返回值，成功则返回 0
 */
- (int)playEffectWitdId:(uint32_t)effectId effectOption:(NERtcCreateAudioEffectOption *)option;

/**
 停止播放指定音效文件。

 请在加入房间后调用该方法。

 @param effectId 指定音效的 ID。每个音效均有唯一的 ID。
 @return 操作返回值，成功则返回 0
 */
- (int)stopEffectWitdId:(uint32_t)effectId;

/**
 停止播放所有音效文件。

 请在加入房间后调用该方法。

 @return 操作返回值，成功则返回 0
 */
- (int)stopAllEffects;

/**
 暂停播放指定音效文件。

 请在加入房间后调用该方法。
 @param effectId 音效 ID。
 @return 操作返回值，成功则返回 0
 */
- (int)pauseEffectWitdId:(uint32_t)effectId;

/**
 恢复播放指定音效文件。

 请在加入房间后调用该方法。

 @param effectId 指定音效的 ID。每个音效均有唯一的 ID。
 @return 操作返回值，成功则返回 0
 */
- (int)resumeEffectWitdId:(uint32_t)effectId;

/**
 暂停播放所有音效文件。

 请在加入房间后调用该方法。

 @return 操作返回值，成功则返回 0
 */
- (int)pauseAllEffects;

/**
 恢复播放所有音效文件。

 请在加入房间后调用该方法。

 @return 操作返回值，成功则返回 0
 */
- (int)resumeAllEffects;

/**
 设置音效文件发送音量。

 请在加入房间后调用该方法。

 @param effectId 指定音效的 ID。每个音效均有唯一的 ID。
 @param volume 音效发送音量。范围为0~100，默认为  100，表示原始音量。
 @return 操作返回值，成功则返回 0
 */
- (int)setEffectSendVolumeWithId:(uint32_t)effectId volume:(uint32_t)volume;

/**
 获取指定音效文件发送音量。
 
 请在加入房间后调用该方法。

 @param effectId 指定音效的 ID。每个音效均有唯一的 ID。
 @param volume 返回的发送音量。
 @return 操作返回值，成功则返回 0
 */
- (int)getEffectSendVolumeWithId:(uint32_t)effectId volume:(uint32_t *)volume;

/**
 获取音效文件播放音量。

 请在加入房间后调用该方法。

 @param effectId 指定音效的 ID。每个音效均有唯一的 ID。
 @param volume 音效播放音量。范围为0~100，默认为100。
 @return 操作返回值，成功则返回 0
 */
- (int)setEffectPlaybackVolumeWithId:(uint32_t)effectId volume:(uint32_t)volume;

/**
 获取音效的回放音量

 @param effectId 音效 ID。
 @param volume 返回的音量值
 @return 操作返回值，成功则返回 0
 */
- (int)getEffectPlaybackVolumeWithId:(uint32_t)effectId volume:(uint32_t *)volume;

/**
 获取音效文件时长。

 @since V4.4.0

 该方法获取音效文件时长，单位为毫秒。
 
 @note 请在房间内调用该方法。

 @param[in] effectId 音效 ID。
 @param[out] duration 音效文件时长，单位为毫秒。
 @return 操作返回值，成功则返回 0
 */
- (int)getEffectDurationWithId:(uint32_t)effectId duration:(uint64_t *)duration;

/**
 获取音效的播放进度。

 @since V4.4.0 

 该方法获取当前音效播放进度，单位为毫秒。

 @note 请在房间中调用该方法。

 @param[in]  effectId 音效 ID。
 @param[out] position 音效文件的播放位置，单位为毫秒。
 @return 操作返回值，成功则返回 0
 */
- (int)getEffectCurrentPositionWithId:(uint32_t)effectId position:(uint64_t *)position;

#pragma mark -- 耳返
/**
 开启耳返功能。

 @note
 - 请在房间内调用该方法。
 - 开启耳返功能后，必须连接上耳机或耳麦，才能正常使用耳返功能。建议通过 onAudioDeviceChanged  监听播放设备的变化，当监听到播放设备切换为耳机时才开启耳返。
 - 在V4.0.0 版本中，enableEarback 的 volume 参数无效，请使用 setEarbackVolume 接口设置耳返音量。

 @param enabled 开启耳返功能。
 @param volume 设置耳返音量，可设置为0~100，默认为 100。
 @return 操作返回值，成功则返回 0
 */
- (int)enableEarback:(BOOL)enabled volume:(uint32_t)volume;

/**
 设置耳返音量。

 @param volume 设置耳返音量，可设置为0~100，默认为 100。
 @return 操作返回值，成功则返回 0
 */
- (int)setEarbackVolume:(uint32_t)volume;

#pragma mark -- audio recording
/**
* 开始客户端录音。
* 
* 调用该方法后，客户端会录制房间内所有用户混音后的音频流，并将其保存在本地一个录音文件中。录制开始或结束时，自动触发 onAudioRecording() 回调。
* 
* 指定的录音音质不同，录音文件会保存为不同格式：
* - WAV：音质保真度高，文件大。
* - AAC：音质保真度低，文件小。
*
* @note
* - 请在加入房间（joinchannel）后调用此方法。
* - 客户端只能同时运行一个录音任务，正在录音时，如果重复调用 startAudioRecording，会结束当前录制任务，并重新开始新的录音任务。
* - 当前用户离开房间时，自动停止录音。您也可以在通话中随时调用 stopAudioRecording 手动停止录音。
* 
* @param filePath 录音文件在本地保存的绝对路径，需要精确到文件名及格式。例如：sdcard/xxx/audio.aac。
*                   - 请确保指定的路径存在并且可写。
*                   - 目前仅支持 WAV 或 AAC 文件格式。
* @param sampleRate 录音采样率（Hz），可以设为 16000、32000（默认）、44100 或 48000。
* @param quality 录音音质，只在 AAC 格式下有效。详细信息请参考 {@link NERtcAudioRecordingQuality}。
 @return 操作返回值，成功则返回 0
*/
- (int)startAudioRecording:(NSString *)filePath sampleRate:(int)sampleRate quality:(NERtcAudioRecordingQuality)quality;

/**
* 停止客户端录音。
* 
* 本端离开房间时自动停止录音，您也可以在通话中随时调用 stopAudioRecording 手动停止录音。
* 
* @note 该接口需要在 leaveChannel 之前调用。
* 
* @return 操作返回值，成功则返回 0
*/
- (int)stopAudioRecording;

#pragma mark --External Audio
/**
 开启或关闭外部音频源数据输入。

 当该方法调用成功后，音频输入设备选择和异常重启会失效。调用成功后可以使用 pushExternalAudioFrame 接口发送音频 PCM 数据。

 @note
 - 请在加入房间前调用该方法。
 - 该方法设置内部引擎为启用状态，启动时将用虚拟设备代替扬声器工作，在 leaveChannel 后仍然有效。如果需要关闭该功能，需要在下次通话前调用接口关闭外部音频数据输入功能。
 - 启用外部音频数据输入功能后，SDK 内部实现部分扬声器由外部输入数据代替，扬声器相关的设置会失败或不在通话中生效。例如进行 loopback 检测时，会听到输入的外部数据。

 @param enabled 是否开启外部数据输入。默认为 NO。
 @param sampleRate 外部音频源的数据采样率，单位为 Hz。建议设置为 8000，16000，32000，44100 或 48000。
 @param channels 外部音频源的数据声道数。可设置为：

 - 1：单声道。
 - 2：双声道。

 @return 操作返回值，成功则返回 0
*/
- (int)setExternalAudioSource:(BOOL)enabled sampleRate:(int32_t)sampleRate channels:(int32_t)channels;

/**
 推送外部音频帧。

 将外部音频数据帧推送给内部引擎。 通过 setExternalAudioSource 启用外部音频数据输入功能成功后，可以使用 pushExternalAudioFrame 接口发送音频 PCM 数据。

 @note
 - 该方法需要在加入房间后调用。
 - 数据帧时长建议匹配 10ms 周期。
 - 该方法在音频输入设备关闭后不再生效。例如关闭本地音频、通话结束、通话前麦克风设备测试关闭等情况下，该设置不再生效。

 @param frame 外部音频数据帧。数据长度不能超过 7680 字节。
 @return 操作返回值，成功则返回 0
*/

- (int)pushExternalAudioFrame:(NERtcAudioFrame *)frame;


/**
 设置外部音频渲染。

 该方法适用于需要自行渲染音频的场景。默认为关闭状态。当该方法调用成功后，音频播放设备选择和异常重启失效。

 调用成功后可以使用 pullExternalAudioFrame 接口获取音频 PCM 数据。

 @note
 - 请在加入房间前调用该方法。
 - 该方法设置内部引擎为启用状态，启动时将用虚拟设备代替扬声器工作，在 leaveChannel 后仍然有效。如果需要关闭该功能，需要在下次通话前调用接口关闭外部音频数据渲染功能。
 - 启用外部音频渲染功能后，SDK 内部实现部分扬声器由外部输入数据代替，扬声器相关的设置会失败或不在通话中生效。例如进行 loopback 检测时，需要由外部渲染播放。

 @param enabled 设置是否开启外部音频渲染。
 @param sampleRate 外部音频渲染的采样率 (Hz)，可设置为 16000，32000，44100 或 48000。
 @param channels 外部音频渲染的声道数，可设置为：

 - 1：单声道
 - 2：双声道

 @return 操作返回值，成功则返回 0
*/
- (int)setExternalAudioRender:(BOOL)enabled sampleRate:(int32_t)sampleRate channels:(int32_t)channels;

/**
 拉取外部音频数据。

 该方法将从内部引擎拉取音频数据。 通过 setExternalAudioRender 启用外部音频数据渲染功能成功后，可以使用 pullExternalAudioFrame 接口获取音频 PCM 数据。

 @note
 - 该方法需要在加入房间后调用。
 - 数据帧时长建议匹配 10ms 周期。
 - 该方法在音频渲染设备关闭后不再生效，此时会返回空数据。例如通话结束、通话前扬声器设备测试关闭等情况下，该设置不再生效。

 @param data 数据指针。
 @param len 待拉取音频数据的字节数，单位为 byte。建议音频数据的时长至少为 10 毫秒，数据长度不能超过 7680字节。<br>计算公式为： len = sampleRate/1000 × 2 × channels × 音频数据时长（毫秒）。
 @return 操作返回值，成功则返回 0
*/
- (int)pullExternalAudioFrame:(void *_Nonnull)data length:(int)len;

#pragma mark -- Media Statistic Observer

/**
 注册媒体统计信息观测器。
 
 @param observer 统计信息观测器。详细信息请参考  {@link NERtcEngineMediaStatsObserver}。
 @return 操作返回值，成功则返回 0
 */
- (int)addEngineMediaStatsObserver:(id<NERtcEngineMediaStatsObserver>)observer;

/**
 移除指定媒体统计信息观测器。
 
 @param observer 统计信息观测器
 @return 操作返回值，成功则返回 0
 */
- (int)removeEngineMediaStatsObserver:(id<NERtcEngineMediaStatsObserver>)observer;

/**
 清除全部媒体统计信息观测器。
 
 @return 操作返回值，成功则返回 0
 */
- (int)cleanupEngineMediaStatsObserver;


#pragma mark -- Live Stream
/**
 添加房间推流任务。

 成功调用该方法后，当前用户可以收到该直播流的状态通知。

 @param taskInfo 推流任务信息，详细信息请参考  {@link NERtcLiveStreamTaskInfo}。
      
 @param completion 操作结果回调，方法调用成功后会触发对应回调。详细信息请参考 {@link NERtcLiveStreamCompletion}。

 @return 操作返回值，成功则返回 0

 @note
 - 该方法仅适用直播场景。
 - 请在房间内调用该方法，该方法在通话中有效。
 - 该方法每次只能增加一路旁路推流地址。如需推送多路流，则需多次调用该方法。同一个音视频房间（即同一个 channelid）可以创建 3 个不同的推流任务。
*/
- (int)addLiveStreamTask:(NERtcLiveStreamTaskInfo *)taskInfo compeltion:(NERtcLiveStreamCompletion)completion;

/**
 更新房间推流任务。

 @note
 - 该方法仅适用直播场景。
 - 请在房间内调用该方法，该方法在通话中有效。

 @param taskInfo 推流任务信息，详细信息请参考 {@link NERtcLiveStreamTaskInfo}。
      
 @param completion 操作结果回调，方法调用成功后会触发对应回调。详细信息请参考 {@link NERtcLiveStreamCompletion}。

 @return 操作返回值，成功则返回 0
*/
- (int)updateLiveStreamTask:(NERtcLiveStreamTaskInfo *)taskInfo compeltion:(NERtcLiveStreamCompletion)completion;

/**
 删除房间推流任务。

 @note
 - 该方法仅适用直播场景。
 - 请在房间内调用该方法，该方法在通话中有效。

 @param taskId 推流任务 ID。
      
 @param completion 操作结果回调，方法调用成功后会触发对应回调。详细信息请参考 {@link NERtcLiveStreamCompletion}。

 @return 操作返回值，成功则返回 0
*/
- (int)removeLiveStreamTask:(NSString *)taskId compeltion:(NERtcLiveStreamCompletion)completion;

#pragma mark -- Audio Frame Observer
/**
 设置采集的音频格式。

 该方法设置 onNERtcEngineAudioFrameDidRecord 回调的录制声音格式。

 @note
 - 该方法在加入房间前后均可设置或修改。
 - 取消监听，重置为空。

 @param format 指定 onNERtcEngineAudioFrameDidRecord 中返回数据的采样率和数据的通道数。<br>允许传入 nil，默认为 nil，表示使用音频的原始格式。详细信息请参考  {@link NERtcAudioFrameRequestFormat}。
     
 @return 操作返回值，成功则返回 0
*/
- (int)setRecordingAudioFrameParameters:(nullable NERtcAudioFrameRequestFormat *)format;

  
/**
 设置播放的声音格式。

 该方法设置 onNERtcEngineAudioFrameWillPlayback 回调的播放声音格式。

 @note
 - 该方法在加入房间前后均可设置或修改。
 - 取消监听，重置为空。

 @param format 指定 onNERtcEngineAudioFrameDidRecord 中返回数据的采样率和数据的通道数。<br>允许传入 nil，默认为 nil，表示使用音频的原始格式。详细信息请参考 {@link NERtcAudioFrameRequestFormat}。
     
 @return 操作返回值，成功则返回 0
*/
- (int)setPlaybackAudioFrameParameters:(nullable NERtcAudioFrameRequestFormat *)format;

/**
 设置录制和播放声音混音后的数据格式。

 该方法设置 onNERtcEngineMixedAudioFrame 回调的声音格式。

 - 该方法在加入房间前后均可设置或修改，leaveChannel 后重置为空。
 - 目前只支持设置采样率。
 - 未调用该接口设置数据格式时，回调中的采样率返回 SDK 默认值。

 @param format 指定 onNERtcEngineMixedAudioFrame 中返回数据的采样率和数据的通道数。允许传入 nil，默认为 nil，表示使用音频的原始格式。详细信息请参考 {@link NERtcAudioFrameRequestFormat}。
     
 @return 操作返回值，成功则返回 0
*/
- (int)setMixedAudioFrameParameters:(nullable NERtcAudioFrameRequestFormat *)format;

/**
 注册语音观测器对象。

 该方法用于设置音频采集/播放PCM回调，可用于声音处理等操作。当需要引擎返回onPlaybackFrame等回调时，需要使用该方法注册回调。

 @note 该方法在加入房间前后均可设置或修改。

 @param observer 音频数据帧观测器。<br>如果传入 NULL，则取消注册，同时会清理 NERtcAudioFrameRequestFormat 相关设置。详细信息请参考 {@link NERtcEngineAudioFrameObserver}。
 @return 操作返回值，成功则返回 0
*/
- (int)setAudioFrameObserver:(nullable id<NERtcEngineAudioFrameObserver>)observer;
  

#pragma mark -- Volume indication
/**
 启用说话者音量提示。

 该方法允许 SDK 定期向 App 反馈本地发流用户和瞬时音量最高的远端用户（最多 3 位）的音量相关信息，即当前谁在说话以及说话者的音量。

 启用该方法后，只要房间内有发流用户，无论是否有人说话，SDK 都会在加入房间后根据预设的时间间隔触发 onRemoteAudioVolumeIndication 回调。

 @param enable 是否启用说话者音量提示。
 @param interval 指定音量提示的时间间隔。单位为毫秒。必须设置为 100 毫秒的整数倍值，建议设置为 200 毫秒以上。

 @return 操作返回值，成功则返回 0
*/
- (int)enableAudioVolumeIndication:(BOOL)enable interval:(uint64_t)interval;


#pragma mark -- Signal Volume
/**
 调节采集信号音量。

 @param volume 采集信号音量，取值范围为 [0, 400]。其中：

 - 0：静音。
 - 100：（默认）原始音量。
 - 400：最大可为原始音量的 4 倍（自带溢出保护）。

 @return 操作返回值，成功则返回 0
*/
- (int)adjustRecordingSignalVolume:(uint32_t)volume;

/**
 调节本地播放的所有远端用户信号音量。

 @param volume 播放音量。取值范围为 [0, 400]。其中：

 - 0：静音。
 - 100：（默认）原始音量。
 - 400：最大可为原始音量的 4 倍（自带溢出保护）。

 @return 操作返回值，成功则返回 0
*/
- (int)adjustPlaybackSignalVolume:(uint32_t)volume;

/**
  调节本地播放的指定远端用户的信号音量。
  
  加入房间后，您可以多次调用该方法设置本地播放的不同远端用户的音量；也可以反复调节本地播放的某个远端用户的音量。

  @note
  - 请在成功加入房间后调用该方法。
  - 该方法在本次通话中有效。如果远端用户中途退出房间，则再次加入此房间时仍旧维持该设置，通话结束后设置失效。
  - 该方法调节的是本地播放的指定远端用户混音后的音量，且每次只能调整一位远端用户。若需调整多位远端用户在本地播放的音量，则需多次调用该方法。
 
  @param userID    远端用户 ID。
  @param volume 播放音量，取值范围为 [0,100]。
                  - 0：静音。
                  - 100：原始音量。
  @return 操作返回值，成功则返回 0
 */
- (int)adjustUserPlaybackSignalVolume:(uint32_t)volume forUserID:(uint64_t)userID;

#pragma mark - voice effects

/**
 设置本地语音音调。

 该方法改变本地说话人声音的音调。

 @note
 - 通话结束后该设置会重置，默认为 1.0。
 - 此方法与 setAudioEffectPreset 互斥，调用此方法后，已设置的变声效果会被取消。

 @param pitch 语音频率。可以在 [0.5, 2.0] 范围内设置。取值越小，则音调越低。默认值为 1.0，表示不需要修改音调。

 @return 操作返回值，成功则返回 0
 */
- (int)setLocalVoicePitch:(double)pitch;

/**
 设置本地语音音效均衡，即自定义设置本地人声均衡波段的中心频率。

 @note 该方法在加入房间前后都能调用，通话结束后重置为默认关闭状态。

 @param bandFrequency 频谱子带索引，取值范围是 [0-9]，分别代表 10 个频带，对应的中心频率是 [31，62，125，250，500，1k，2k，4k，8k，16k] Hz。
 @param gain 每个 band 的增益，单位是 dB，每一个值的范围是 [-15，15]，默认值为 0。

 @return 操作返回值，成功则返回 0
 */
- (int)setLocalVoiceEqualizationOfBandFrequency:(NERtcAudioEqualizationBandFrequency)bandFrequency withGain:(NSInteger)gain;

/**
 设置 SDK 预设的美声效果。

 调用该方法可以为本地发流用户设置 SDK 预设的人声美声效果。

 @note 该方法在加入房间前后都能调用，通话结束后重置为默认关闭状态。

 @param type 预设的美声效果模式。默认关闭美声效果。详细信息请参考 {@link NERtcVoiceBeautifierType}。

 @return 操作返回值，成功则返回 0
 */
- (int)setVoiceBeautifierPreset:(NERtcVoiceBeautifierType)type;

/**
 设置 SDK 预设的人声的变声音效。

 设置变声音效可以将人声原因调整为多种特殊效果，改变声音特性。

 @note
 - 此方法在加入房间前后都能调用，通话结束后重置为默认关闭状态。
 - 此方法和 setLocalVoicePitch 互斥，调用此方法后，本地语音语调会恢复为默认值 1.0。

 @param type 预设的变声音效。默认关闭变声音效。详细信息请参考  {@link NERtcVoiceChangerType}。

 @return 操作返回值，成功则返回 0
 */

- (int)setAudioEffectPreset:(NERtcVoiceChangerType)type;

#pragma mark - waterMark

/**
 * 添加本地视频画布水印。
 * 
 * @note 
 * - setLocalCanvasWatermarkConfigs 方法作用于本地视频画布，不影响视频流。画布被移除时，水印也会自动移除。
 * - 设置水印之前，需要先通过画布相关方法设置画布。
 * 
 * @param type 视频流类型。支持设置为主流或辅流。详细信息请参考 {@link NERtcStreamChannelType}。
 * @param config 画布水印设置。支持设置文字水印、图片水印和时间戳水印，设置为 null 表示清除水印。详细信息请参考 {@link NERtcCanvasWatermarkConfig}。
 * @note 纯音频SDK禁用该接口，如需使用请前往云信官网下载并替换成视频SDK
 * @return 操作返回值，成功则返回 0
 */
- (int)setLocalCanvasWatermarkConfigs:(nullable NERtcCanvasWatermarkConfig *)config
                       withStreamType:(NERtcStreamChannelType)type;

/**
 * 添加远端视频画布水印。
 * 
 * @note 
 * - setRemoteCanvasWatermarkConfigs 方法作用于远端视频画布，不影响视频流。画布被移除时，水印也会自动移除。
 * - 设置水印之前，需要先通过画布相关方法设置画布。
 * 
 * @param userID 远端用户 ID。
 * @param type 视频流类型。支持设置为主流或辅流。详细信息请参考 {@link NERtcStreamChannelType}。
 * @param config 画布水印设置。支持设置文字水印、图片水印和时间戳水印，设置为 null 表示清除水印。详细信息请参考 {@link NERtcCanvasWatermarkConfig}。
 * @note 纯音频SDK禁用该接口，如需使用请前往云信官网下载并替换成视频 SDK。
 * @return 操作返回值，成功则返回 0
 */
- (int)setRemoteCanvasWatermarkConfigs:(nullable NERtcCanvasWatermarkConfig *)config
                             forUserID:(uint64_t)userID
                        withStreamType:(NERtcStreamChannelType)type;

#pragma mark -- snapshot
/**
 * 本地视频画面截图。
 * 
 * 调用 takeLocalSnapshot 截取本地主流或本地辅流的视频画面，并通过 NERtcTakeSnapshotCallback 的回调返回截图画面的数据。
 * 
 * @note<br>
 * - 本地主流截图，需要在 startPreview 或者 enableLocalVideo 并 joinChannel 成功之后调用。
 * - 本地辅流截图，需要在 joinChannel 并 startScreenCapture 之后调用。
 * - 同时设置文字、时间戳或图片水印时，如果不同类型的水印位置有重叠，会按照图片、文本、时间戳的顺序进行图层覆盖。
 * 
 * @param streamType 截图的视频流类型。支持设置为主流或辅流。
 * @param callback 截图回调。
 * @note 纯音频SDK禁用该接口，如需使用请前往云信官网下载并替换成视频 SDK。
 * @return 操作返回值，成功则返回 0
 */
- (int)takeLocalSnapshot:(NERtcStreamChannelType)streamType callback:(NERtcTakeSnapshotCallback)callback;

/**
 * 远端视频画面截图。
 * 调用 takeRemoteSnapshot 截取指定 uid 远端主流和远端辅流的视频画面，并通过 NERtcTakeSnapshotCallback 的回调返回截图画面的数据。
 * 
 * @note<br>
 * - takeRemoteSnapshot 需要在收到 onUserVideoStart 与 onNERtcEngineUserSubStreamDidStartWithUserID 回调之后调用。
 * - 同时设置文字、时间戳或图片水印时，如果不同类型的水印位置有重叠，会按照图片、文本、时间戳的顺序进行图层覆盖。
 * 
 * @param userID 远端用户 ID。
 * @param streamType 截图的视频流类型。支持设置为主流或辅流。
 * @param callback 截图回调。
 * @note 纯音频SDK禁用该接口，如需使用请前往云信官网下载并替换成视频 SDK。
 * @return 操作返回值，成功则返回 0
 */
- (int)takeRemoteSnapshot:(NERtcStreamChannelType)streamType forUserID:(uint64_t)userID callback:(NERtcTakeSnapshotCallback)callback;

#pragma mark -- Other
/**
 上传 SDK 信息。

 上传的信息包括 log 和 Audio dump 等文件。

 @note 只能在加入房间后调用。
 
 @return 操作返回值，成功则返回 0
 */
- (int)uploadSdkInfo;

#pragma mark - SEI

/**
 指定主流或辅流通道发送媒体增强补充信息（SEI）。

 在本端推流传输音视频流数据同时，发送流媒体补充增强信息来同步一些其他附加信息。当推流方发送 SEI 后，拉流方可通过监听 onRecvSEIMsg 的回调获取 SEI 内容。

  - 调用时机：视频流（主流、辅流）开启后，可调用此函数。
  - 数据长度限制： SEI 最大数据长度为 4096 字节，超限会发送失败。如果频繁发送大量数据会导致视频码率增大，可能会导致视频画质下降甚至卡顿。
  - 发送频率限制：最高为视频发送的帧率，建议不超过 10 次/秒。
  - 生效时间：调用本接口之后，最快在下一帧视频数据帧之后发送 SEI 数据，最慢在接下来的 5 帧视频之后发送。

 @note
 - SEI 数据跟随视频帧发送，由于在弱网环境下可能丢帧，SEI 数据也可能随之丢失，所以建议在发送频率限制之内多次发送，保证接收端收到的概率。
 - 指定通道发送 SEI 之前，需要提前开启对应的数据流通道。
 - 纯音频 SDK 禁用该接口，如需使用请前往云信官网下载并替换成视频 SDK。

 @param data 自定义 SEI 数据。
 @param type  发送 SEI 时，使用的流通道类型。详细信息请参考 {@link NERtcStreamChannelType}。

 @return 操作返回值，成功则返回 0
 - 成功:  成功进入待发送队列，会在最近的视频帧之后发送该数据。
 - 失败:  数据被限制发送，可能发送的频率太高，队列已经满了，或者数据大小超过最大值 4k。
 */
- (int)sendSEIMsg:(NSData *)data streamChannelType:(NERtcStreamChannelType)type;

/**
 通过主流通道发送媒体增强补充信息（SEI）。

 在本端推流传输音视频流数据同时，发送流媒体补充增强信息来同步一些其他附加信息。当推流方发送 SEI 后，拉流方可通过监听 onRecvSEIMsg 的回调获取 SEI 内容。

  - 调用时机：视频流（主流、辅流）开启后，可调用此函数。
  - 数据长度限制： SEI 最大数据长度为 4096 字节，超限会发送失败。如果频繁发送大量数据会导致视频码率增大，可能会导致视频画质下降甚至卡顿。
  - 发送频率限制：最高为视频发送的帧率，建议不超过 10 次/秒。
  - 生效时间：调用本接口之后，最快在下一帧视频数据帧之后发送 SEI 数据，最慢在接下来的 5 帧视频之后发送。

 @note
 - SEI 数据跟随视频帧发送，由于在弱网环境下可能丢帧，SEI 数据也可能随之丢失，所以建议在发送频率限制之内多次发送，保证接收端收到的概率。
 - 调用本接口时，默认使用主流通道发送 SEI。
 - 纯音频 SDK 禁用该接口，如需使用请前往云信官网下载并替换成视频 SDK。

 @param data 自定义 SEI 数据。

 @return 操作返回值，成功则返回 0
 - 成功:  成功进入待发送队列，会在最近的视频帧之后发送该数据
 - 失败:  数据被限制发送，可能发送的频率太高，队列已经满了，或者数据大小超过最大值 4k
 */
- (int)sendSEIMsg:(NSData *)data;

#pragma mark- MediaRealy

/**
 开始跨房间媒体流转发。
 - 该方法可用于实现跨房间连麦等场景。支持同时转发到 4 个房间，同一个房间可以有多个转发进来的媒体流。
 - 成功调用该方法后，SDK 会触发 onNERtcEngineChannelMediaRelayStateDidChange 和 onNERtcEngineDidReceiveChannelMediaRelayEvent 回调，并在回调中报告当前的跨房间媒体流转发状态和事件。
 
 @note
 - 请在成功加入房间后调用该方法。调用此方法前需要通过 config 中的 setDestinationInfo 设置目标房间。
 - 该方法仅对直播场景下的主播角色有效。
 - 成功调用该方法后，若您想再次调用该方法，必须先调用 stopChannelMediaRelay 方法退出当前的转发状态。
 - 成功开始跨房间转发媒体流后，如果您需要修改目标房间，例如添加或删减目标房间等，可以调用方法 updateChannelMediaRelay 更新目标房间信息。
 
 @param config 跨房间媒体流转发参数配置信息。详细信息请参考 {@link NERtcChannelMediaRelayConfiguration}。
 @return 成功返回0，其他则失败
 */
- (int)startChannelMediaRelay:(NERtcChannelMediaRelayConfiguration *_Nonnull)config;

/**
 更新媒体流转发的目标房间。
 
 成功开始跨房间转发媒体流后，如果你希望将流转发到多个目标房间，或退出当前的转发房间，可以调用该方法。
 - 成功开始跨房间转发媒体流后，如果您需要修改目标房间，例如添加或删减目标房间等，可以调用此方法。
 - 成功调用此方法后，SDK 会触发 onNERtcEngineChannelMediaRelayStateDidChange 回调。如果报告 NERtcChannelMediaRelayStateRunning，则表示已成功转发媒体流。

 @note
 - 请在加入房间并成功调用 startChannelMediaRelay 开始跨房间媒体流转发后，调用此方法。调用此方法前需要通过 config 中的 setDestinationInfo 设置目标房间。
 - 跨房间媒体流转发最多支持 4 个目标房间，您可以在调用该方法之前，通过 NERtcChannelMediaRelayConfiguration 中的 removeDestinationInfoForChannelName 方法移除不需要的房间，再添加新的目标房间。
 @param config 跨房间媒体流转发参数配置信息。详细信息请参考 {@link NERtcChannelMediaRelayConfiguration}。
 @return 成功返回0，其他则失败
 */
- (int)updateChannelMediaRelay:(NERtcChannelMediaRelayConfiguration *_Nonnull)config;

/**
 停止跨房间媒体流转发。
 
 主播离开房间时，跨房间媒体流转发自动停止，您也可以在需要的时候随时调用 stopChannelMediaRelay 方法，此时主播会退出所有目标房间。
 - 成功调用该方法后，SDK 会触发 onNERtcEngineChannelMediaRelayStateDidChange 回调。如果报告 NERtcChannelMediaRelayStateIdle，则表示已停止转发媒体流。
 - 如果该方法调用不成功，SDK 会触发 onNERtcEngineChannelMediaRelayStateDidChange 回调，并报告状态码 NERtcChannelMediaRelayStateFailure。
 @return 成功返回0，其他则失败
 */
- (int)stopChannelMediaRelay;

/**
 * 启用或停止 AI 超分。
 * 
 * @since V4.4.0
 * 
 * @note 
 * - 使用 AI 超分功能之前，请联系技术支持开通 AI 超分功能。
 * - AI 超分仅对以下类型的视频流有效：
 *   - 必须为本端接收到第一路 360P 的视频流。
 *   - 必须为摄像头采集到的主流大流视频。AI 超分功能暂不支持复原重建小流和屏幕共享辅流。
 *
 * @param enabled 是否启用 AI 超分。默认为关闭状态。
 * @return {@code 0} 方法调用成功，其他调用失败
 */
- (int)enableSuperResolution:(BOOL)enabled;

/**
 * 开启或关闭媒体流加密。
 * 
 * @since V4.4.0
 * 
 * 在金融行业等安全性要求较高的场景下，您可以在加入房间前通过此方法设置媒体流加密模式。
 * 
 * @note 
 * - 请在加入房间前调用该方法，加入房间后无法修改加密模式与密钥。用户离开房间后，SDK 会自动关闭加密。如需重新开启加密，需要在用户再次加入房间前调用此方法。
 * - 同一房间内，所有开启媒体流加密的用户必须使用相同的加密模式和密钥，否则使用不同密钥的成员加入房间时会报错 kNERtcErrEncryptNotSuitable（30113）。 
 * - 安全起见，建议每次启用媒体流加密时都更换新的密钥。
 *
 * @param enable 是否开启媒体流加密。
 *                  - YES: 开启
 *                  - NO:（默认）关闭
 * @param config 媒体流加密方案。详细信息请参考 NERtcEncryptionConfig。
 * @return {@code 0} 方法调用成功，其他调用失败
 */
- (int)enableEncryption:(BOOL)enable config:(NERtcEncryptionConfig *)config;
@end

NS_ASSUME_NONNULL_END

#endif /* INERtcEngineEx_h */
