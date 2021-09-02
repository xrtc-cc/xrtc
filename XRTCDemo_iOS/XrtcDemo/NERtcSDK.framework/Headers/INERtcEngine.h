/*
 * Copyright (c) 2021 NetEase, Inc.  All rights reserved.
 */

#ifndef INERtcEngine_h
#define INERtcEngine_h

#import <Foundation/Foundation.h>
#import "NERtcEngineEnum.h"
#import "NERtcEngineErrorCode.h"

NS_ASSUME_NONNULL_BEGIN


/**
 加入房间 block。

 @param error 错误，如果成功则为 nil。
 @param channelId 如果成功，会返回有效的 channelId。
 @param elapesd 加入操作总耗时(毫秒)。
 @param uid 客户端本地uid。
 */
typedef void(^NERtcJoinChannelCompletion)(NSError * _Nullable error, uint64_t channelId, uint64_t elapesd, uint64_t uid);


/**
 互动直播推流 block。

 @param taskId 推流任务 id。
 @param errorCode 状态码。详细信息请参考 {@link NERtcEngineErrorCode.kNERtcLiveStreamError}
*/
typedef void(^NERtcLiveStreamCompletion)(NSString *taskId, kNERtcLiveStreamError errorCode);

/**
 * 截图结果 block 回调。
 * 
 * @param errorCode 错误码。详细信息请参考 {@link NERtcEngineErrorCode.NERtcError}。
 * @param image 截图图片。
 */
typedef void(^NERtcTakeSnapshotCallback)(int errorCode, UIImage * _Nullable image);

@class NERtcEngineContext;
@class NERtcVideoEncodeConfiguration;
@class NERtcVideoCanvas;

/**
 NERtcEngine 的常用接口。
 */
@protocol INERtcEngine <NSObject>

/**
 获取当前网络状态。

 @return 当前网络状态。详细信息请参考 {@Link NERtcEngineEnum.NERtcConnectionStateType}。
 */
- (NERtcConnectionStateType)connectionState;

/**
 创建 NERtcEngine 并初始化 NERTC SDK 服务。

 @note
 - 请确保在调用其他 API 前先调用该方法创建并初始化 NERtc 实例。
 - 使用同一个 App Key 的 App 才能进入同一个房间进行通话或直播。
 - 一个App Key 只能用于创建一个NERtc 实例。如需更换 App Key，必须先调用 destroyEngine 方法销毁当前实例，再调用本方法重新创建实例。
 - 若不再使用 NERtc  实例，需要调用 destroyEngine 进行销毁。

 @param context 传入的RTC engine context对象。详细信息请参考 {@link NERtcEngineContext}。
 @return 操作返回值，成功则返回 0
 */
- (int)setupEngineWithContext:(NERtcEngineContext *)context;

/**
 加入音视频房间。

 加入音视频房间时，如果指定房间尚未创建，云信服务器内部会自动创建一个同名房间。

 - SDK 加入房间后，同一个房间内的用户可以互相通话，多个用户加入同一个房间，可以群聊。使用不同 App Key 的 App 之间不能互通。
 - 成功调用该方加入房间后，远端会触发 onNERtcEngineUserDidJoinWithUserID 回调。
 - 用户成功加入房间后，默认订阅房间内所有其他用户的音频流，可能会因此产生用量并影响计费。如果想取消订阅，可以通过调用相应的 mute 方法实现。
 - 直播场景下的观众角色可以通过 switchChannelWithToken 快速切换房间。
 
 @param token 安全认证签名（NERTC Token）。可设置为：
                  - null。非安全模式下可设置为 null。安全性不高，建议在产品正式上线前联系对应商务经理转为安全模式。
                  - 已获取的NERTC Token。安全模式下必须设置为获取到的 Token 。若未传入正确的 Token 将无法进入房间。推荐使用安全模式。
 @param channelName 房间名称，设置相同房间名称的用户会进入同一个通话房间。<br>字符串格式，长度为1~ 64 字节。支持以下89个字符：a-z, A-Z, 0-9, space, !#$%&()+-:;≤.,>? @[]^_{|}~”
 @param uId 用户的唯一标识 id，房间内每个用户的 uid 必须是唯一的。
            <br>uId 可选，默认为 0。如果不指定（即设为 0），SDK 会自动分配一个随机 uid，并在 NERtcJoinChannelCompletion 中返回，App 层必须记住该返回值并维护，SDK 不对该返回值进行维护。
 @param completion 操作完成的 block 回调。
 @return 操作返回值，被执行了则返回 0。
 */
- (int)joinChannelWithToken:(NSString *)token
                channelName:(NSString *)channelName
                      myUid:(uint64_t)uId
                 completion:(NERtcJoinChannelCompletion)completion;

/**
 离开房间，即挂断或退出通话。

 结束通话时，必须调用leaveChannel结束通话，否则无法开始下一次通话。

 成功调用该方法离开房间后，本地会触发 onNERtcEngineDidLeaveChannelWithResult 回调，远端会触发 onNERtcEngineUserDidLeaveWithUserID 回调。
 
 @return 操作返回值，成功则返回 0
 */
- (int)leaveChannel;


/**
 快速切换音视频房间。

 房间场景为直播场景时，房间中角色为观众的成员可以调用该方法从当前房间快速切换至另一个房间。

 成功调用该方切换房间后，本端会收到离开房间的回调 onNERtcEngineDidLeaveChannelWithResult；远端用户会收到 onNERtcEngineUserDidLeaveWithUserID 和 onNERtcEngineUserDidJoinWithUserID 的回调。
 
 @note 
 - 房间成员成功切换房间后，默认订阅房间内所有其他成员的音频流，因此产生用量并影响计费。如果想取消订阅，可以通过调用相应的 subscribeRemoteAudio 方法实现。
 - 该方法仅适用于直播场景中，角色为观众的音视频房间成员。即已通过接口 setchannelprofile 设置房间场景为直播，通过 setClientRole 设置房间成员的角色为观众。

 @param token 在服务器端生成的用于鉴权的安全认证签名（Token）。可设置为：
                  - null。非安全模式下可设置为 null。安全性不高，建议在产品正式上线前联系对应商务经理转为安全模式。
                  - 已获取的NERTC Token。安全模式下必须设置为获取到的 Token 。若未传入正确的 Token 将无法进入房间。推荐使用安全模式。
 @param channelName 期望切换到的目标房间名称
 @param completion 操作完成的 block 回调
 @return 操作返回值，0：方法调用成功,  < 0: 方法调用失败
 */
- (int)switchChannelWithToken:(NSString *)token channelName:(NSString *)channelName completion:(NERtcJoinChannelCompletion)completion;

/**
 开启/关闭本地音频采集。

 当 App 加入房间时，语音功能默认为开启状态。

 该方法不影响接收或播放远端音频流，enableLocalAudio(NO) 适用于只下行不上行音频流的场景。

 @note
 - 该方法与 muteLocalAudio 的区别在于，enableLocalAudio 用于开启本地语音采集及处理；muteLocalAudio 用于停止或继续发送本地音频流。
 - 该方法设置内部引擎为启用状态，在 leaveChannel 后仍然有效。
 - 从 V4.4.0 版本开始，开启或关闭本地音频采集的操作不再影响音乐文件播放，即 enableLocalAudio(NO) 后仍旧可以通过 startAudioMixingWithOption: 播放音乐文件。
 
 @param enabled 是否开启本地音频采集。
 @return 操作返回值，成功则返回 0
*/
- (int)enableLocalAudio:(BOOL)enabled;


/**
 是否开启本地视频采集。

 @note
 - 该方法在加入房间前和加入房间后均可调用。
 - 成功启用或禁用本地视频采集后，远端会触发 onNERtcEngineUserVideoDidStartWithUserID 或 onNERtcEngineUserVideoDidStop 回调。

 @param enabled 是否开启本地视频采集。
 @return 操作返回值，成功则返回 0
*/
- (int)enableLocalVideo:(BOOL)enabled;

/**
 设置房间场景。

 房间场景可设置为通话或直播场景，不同的场景中 QoS 策略不同。

 @note 该方法必须在加入房间前调用，进入房间后无法再设置房间场景。
 
 @param channelProfile 设置房间场景。详细信息请参考 {@link NERtcEngineEnum.NERtcChannelProfileType}。
 @return 操作返回值，成功则返回 0
 */
- (int)setChannelProfile:(NERtcChannelProfileType)channelProfile;

/**
 设置视频编码属性。

 @note <br/>

 - 纯音频 SDK 禁用该接口，如需使用请前往云信官网下载并替换成视频 SDK。
 - 可以在加入房间前或加入房间后调用。
 - 设置成功后，下一次开启本端视频时生效。
 - 每个属性对应一套视频参数，例如分辨率、帧率、码率等。所有设置的参数均为理想情况下的最大值。当视频引擎因网络环境等原因无法达到设置的分辨率、帧率或码率的最大值时，会取最接近最大值的那个值。
 
 @param config 视频编码属性配置，详细信息请参考 {@link NERtcVideoEncodeConfiguration}。
 @return 操作返回值，成功则返回 0
 */
- (int)setLocalVideoConfig:(NERtcVideoEncodeConfiguration *)config;

/**
 设置音频编码属性。

 @note 该方法需要在 joinChannel 之前设置，joinChannel 后设置不生效。

 @param profile 设置采样率、码率、编码模式和声道数。详细信息请参考 {@link NERtcAudioProfileType}。
 @param scenario 设置音频应用场景。详细信息请参考 {@link NERtcAudioScenarioType}。
 @return 操作返回值，成功则返回 0
*/
- (int)setAudioProfile:(NERtcAudioProfileType)profile scenario:(NERtcAudioScenarioType)scenario;

/**
 设置本地视图。

 该方法设置本地视频显示信息。只影响本地用户看到的视频画面，不影响远端。 App 通过调用此接口绑定本地视频流的显示视窗（view）。 在 App 开发中，通常在初始化后调用该方法进行本地视频设置，然后再加入房间。

 @note 纯音频 SDK 禁用该接口，如需使用请前往云信官网下载并替换成视频 SDK。

 @param canvas 视频画布。详细信息请参考 {@link NERtcVideoCanvas}。如果需要删除则传 nil。
 @return 操作返回值，成功则返回 0
 */
- (int)setupLocalVideoCanvas:(NERtcVideoCanvas * _Nullable)canvas;

/**
 设置远端用户视图。

 该方法绑定远端用户和显示视图，并设置远端用户视图在本地显示时的渲染模式和镜像模式，只影响本地用户看到的视频画面。

 @note 
 - 纯音频 SDK 禁用该接口，如需使用请前往云信官网下载并替换成视频 SDK。
 - 如果 App 无法事先知道对方的用户 ID，可以在 APP 收到 onNERtcEngineUserDidJoinWithUserID 事件时设置。
 - 解除某个用户的绑定视图可以把 canvas 设置为空。
 - 退出房间后，SDK 会清除远程用户和视图的绑定关系。
 
 @param userID 远端用户 ID。
 @param canvas 视频窗口，如果需要删除则传 nil
 @return 操作返回值，成功则返回 0
 */
- (int)setupRemoteVideoCanvas:(NERtcVideoCanvas *)canvas forUserID:(uint64_t)userID;

/**
 切换前置/后置摄像头。
 
 该方法需要在相机启动后调用，例如调用 startPreview 或 joinChannel 后。

 @note 纯音频 SDK 禁用该接口，如需使用请前往云信官网下载并替换成视频 SDK。

 @return 操作返回值，成功则返回 0
 */
- (int)switchCamera;

/**
 在直播场景中设置用户角色。

 用户角色支持设置为主播或观众，主播和观众的权限不同。

 - 主播：可以开关摄像头等设备、可以发布流、可以操作互动直播推流相关接口、上下线对其他房间内用户可见
 - 观众：不可以开关摄像头等设备、不可以发布流、不可以操作互动直播推流相关接口、上下线对其他房间内用户不可见
 
@note <br/>

 - 默认情况下用户以主播角色加入房间。在加入房间前，用户可以调用本接口切换本端模式为观众。在加入房间后，用户也可以通过本接口切换用户模式。
 - 用户切换为观众角色时，SDK 会自动关闭音视频设备。

 @param role 用户角色。详细信息请参考 {@link NERtcClientRole}。
 @return 操作返回值，成功则返回 0
 */
- (int)setClientRole:(NERtcClientRole)role;

/**
 设置音视频通话的相关参数。

@note<br/>

- 此接口可在加入房间前后调用。使用参数 kNERtcKeyVideoPreferHWEncode、kNERtcKeyVideoPreferHWDecode 时，请在初始化前调用此接口。
- 此方法提供技术预览或特别定制功能，若您需要使用此接口，请咨询技术支持获取帮助。

 @param parameters 参数内容  参数 key，请参阅 NERtcEngineBase.h 中的定义
 @return 操作返回值，成功则返回 0
 */
- (int)setParameters:(NSDictionary *)parameters;

@end


NS_ASSUME_NONNULL_END

#endif /* INERtcEngine_h */
