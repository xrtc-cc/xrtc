/*
 * Copyright (c) 2021 NetEase, Inc.  All rights reserved.
 */

#import <Foundation/Foundation.h>
#import "NERtcEngineEnum.h"

@protocol NERtcEngineVideoRenderSink;

NS_ASSUME_NONNULL_BEGIN

/**
 以下 key 用于 INERtcEngine::setParameters
 **/

// channel
extern NSString * const kNERtcKeyChannel1V1ModeEnabled; // BOOL, -- 是否开启 1V1 模式，默认值 NO

// video
extern NSString * const kNERtcKeyVideoPreferHWEncode; // BOOL -- 优先使用硬件编码视频数据。默认值 YES，该值建议在调用初始化接口：setupEngineWithContext 之前设置，否则需要在下次初始化之后才生效
extern NSString * const kNERtcKeyVideoPreferHWDecode; // BOOL -- 优先使用硬件解码视频数据。默认值 YES，该值建议在调用初始化接口：setupEngineWithContext 之前设置，否则需要在下次初始化之后才生效
extern NSString * const kNERtcKeyVideoCaptureObserverEnabled; // BOOL -- 是否需要开启视频数据采集回调，从而开发者可以获取到原始视频数据。开启之后需要关闭的话，需要通过调用 destroyEngine 来清除。默认值 NO
extern NSString * const kNERtcKeyVideoStartWithBackCamera; // BOOL -- 第一次开启摄像头时，使用后摄像头。默认值 NO
extern NSString * const kNERtcKeyVideoPreferMetalRender; // BOOL -- 是否优先使用 Metal 渲染，否则使用 OpenGL 渲染。默认值 NO
extern NSString * const kNERtcKeyVideoSendOnPubType; // int，通话前设置有效. 默认为初始发送大流 3

// audio
extern NSString * const kNERtcKeyAutoSubscribeAudio;    // BOOL -- 其他用户打开音频时，自动订阅。只支持在通话前设置，通话中设置无效。 默认值 YES 。注意：如果业务场景中会使用subscribeRemoteAudio进行控制，该 Key 必须设置为 NO
extern NSString * const KNERtcKeyDisableOverrideSpeakerOnReceiver; //BOOL -- 系统切换听筒事件时，禁用切换到扬声器。默认值 NO。 如设置YES 则禁止SDK在系统切换到听筒时做切换扬声器操作，需要用户自己处理切换听筒事件
extern NSString * const KNERtcKeyDisableSWAECOnHeadset; //BOOL -- 设置耳机时不使用软件回声消除功能，默认值 NO。如设置YES 则SDK在耳机模式下不使用软件回声消除功能，会对某些机型下 耳机的音质效果有影响
extern NSString * const KNERtcKeyAudioAINSEnable;//BOOL --设置是否开启AI降噪，开启AI降噪，在嘈杂环境下，让对方更清晰听到您的声音


// server record
extern NSString * const kNERtcKeyRecordHostEnabled; // BOOL -- 是否服务器录制主讲人，默认值 NO
extern NSString * const kNERtcKeyRecordAudioEnabled; // BOOL -- 是否开启通话服务器录制语音，默认值 NO
extern NSString * const kNERtcKeyRecordVideoEnabled; // BOOL -- 是否开启通话服务器录制视频，默认值 NO
extern NSString * const kNERtcKeyRecordType; // int  -- 服务器录制模式，具体见 NERtcRecordType 的定义

// live stream
extern NSString * const kNERtcKeyPublishSelfStreamEnabled; // BOOL -- 是否允许在房间推流时推送自身的媒体流，默认值 NO

// log level
extern NSString * const kNERtcKeyLogLevel; // int -- SDK 日志等级，具体见 NERtcLogLevel 的定义, 默认值 kNERtcLogLevelInfo

extern NSString * const kNERtcKeyExtraInfo; //NSString -- 标识app版本, 上报使用

/**
 视频画布设置
*/
@interface NERtcVideoCanvas : NSObject

/**
 是否使用外部渲染器，默认 NO 不使用，若使用SDK内部会根据externalVideoRender来进行绑定，若不使用SDK内部根据container来进行绑定。暂不支持在通话中切换内部渲染与外部渲染。
 */
@property (nonatomic, assign) BOOL useExternalRender;

/**
 视频窗口容器（SDK内部渲染）
 */
@property (nonatomic, nullable, strong) VIEW_CLASS *container;

/**
 视频渲染模式
 */
@property (nonatomic, assign) NERtcVideoRenderScaleMode renderMode;

/**
 视频镜像模式。
 - 本地视图镜像模式：如果你使用前置摄像头，默认启动本地视图镜像模式；如果你使用后置摄像头，默认关闭本地视图镜像模式。
 - 远端用户视图镜像模式：默认关闭远端用户的镜像模式。
 */
@property (nonatomic, assign) NERtcVideoMirrorMode mirrorMode;

/**
 外部渲染器
*/
@property (nonatomic, strong) id<NERtcEngineVideoRenderSink> externalVideoRender;
@end

/**
 本地视频基础发送配置
 */
@interface NERtcBaseVideoEncodeConfiguration : NSObject

/**
 视频编码的分辨率，用于衡量编码质量。详细信息请参考 {@link NERtcVideoProfileType}。
 */
@property (nonatomic, assign) NERtcVideoProfileType maxProfile;

/**
 视频编码的帧率。详细信息请参考 {@link NERtcVideoFrameRate}。
 
 - 使用主流时，默认根据设置的maxProfile决定帧率。
   - maxProfile>= kNERtcVideoProfileStandard，frameRate = kNERtcVideoFrameRateFps30。
   - maxProfile< kNERtcVideoProfileStandard，frameRate = kNERtcVideoFrameRateFps15。
 - 使用辅流时，frameRate 默认为 kNERtcVideoFrameRateFps7。*/
@property (nonatomic, assign) NERtcVideoFrameRate frameRate;

/**
 视频编码的最小帧率。默认为 0，表示使用默认最小帧率。
 */
@property (nonatomic, assign) NSInteger minFrameRate;

/**
 视频编码的码率，单位为 Kbps。您可以根据场景需要，手动设置想要的码率。
 
   - 若设置的视频码率超出合理范围，SDK 会自动按照合理区间处理码率。
   - 若设置为 0，SDK将会自行计算处理。
 */
@property (nonatomic, assign) NSInteger bitrate;

/**
 视频编码的最小码率，单位为 Kbps。您可以根据场景需要，手动设置想要的最小码率，若设置为0，SDK 将会自行计算处理。
 */
@property (nonatomic, assign) NSInteger minBitrate;

@end

/**
 本地视频发送配置
 */
@interface NERtcVideoEncodeConfiguration : NERtcBaseVideoEncodeConfiguration

/**
 视频编码分辨率，衡量编码质量，以宽x高表示。与maxProfile属性二选一。
 width表示视频帧在横轴上的像素，即自定义宽。
 
 - 设置为负数时表示采用 maxProfile档位。
 - 如果需要自定义分辨率场景，则设置此属性，maxProfile属性失效。
 
 自定义视频输入 width 和 height 无效，会自动根据 maxProfile缩放。
 */
@property (nonatomic, assign) int width;
/**
 视频编码分辨率，衡量编码质量，以宽x高表示。与maxProfile属性二选一。
 height表示视频帧在纵轴上的像素，即自定义高。
 
 - 设置为负数时表示采用 maxProfile档位。
 - 如果需要自定义分辨率场景，则设置此属性，maxProfile属性失效。
 
 自定义视频输入width和height无效，会自动根据 maxProfile缩放。
 */
@property (nonatomic, assign) int height;

/**
 视频画面裁剪模式，默认为 kNERtcVideoCropDefault。自定义视频输入不支持设置裁剪模式。
 */
@property (nonatomic, assign) NERtcVideoCropMode cropMode;

/**
 带宽受限时的视频编码降级偏好。详细信息请参考 {@link NERtcDegradationPreference}。
 
 - DEGRADATION_DEFAULT：（默认）根据场景模式调整适应性偏好。
   - 通信场景中，选择DEGRADATION_BALANCED模式，在编码帧率和视频质量之间保持平衡。
   - 直播场景中，选择DEGRADATION_MAINTAIN_QUALITY模式，降低编码帧率以保证视频质量。
 - DEGRADATION_MAINTAIN_FRAMERATE：流畅优先，降低视频质量以保证编码帧率。在弱网环境下，降低视频清晰度以保证视频流畅，此时画质降低，画面会变得模糊，但可以保持视频流畅。
 - DEGRADATION_MAINTAIN_QUALITY：清晰优先，降低编码帧率以保证视频质量。在弱网环境下，降低视频帧率以保证视频清晰，此时可能会出现一定卡顿。
 - DEGRADATION_BALANCED：在编码帧率和视频质量之间保持平衡。*/
@property (nonatomic, assign) NERtcDegradationPreference degradationPreference;

/**
 设置本地视频编码的镜像模式，即本地发送视频的镜像模式，只影响远端用户看到的视频画面。

 详细信息请参考 {@link NERtcVideoMirrorMode}。
 */
@property (nonatomic, assign) NERtcVideoMirrorMode mirrorMode;

/**
 设置本地视频编码的方向模式，即本地发送视频的方向模式，只影响远端用户看到的视频画面。

 详细信息请参考 {@link NERtcVideoOutputOrientationMode}。
 */
@property (nonatomic, assign) NERtcVideoOutputOrientationMode orientationMode;


@end

/**
 本地辅流发送配置
 */
@interface NERtcVideoSubStreamEncodeConfiguration : NERtcBaseVideoEncodeConfiguration

/**
 屏幕共享功能的编码策略倾向，默认为 kNERtcSubStreamContentPreferMotion。详细信息请参考 {@link NERtcSubStreamContentPrefer}。
 */
@property (nonatomic, assign) NERtcSubStreamContentPrefer contentPrefer;

@end


FOUNDATION_EXPORT uint32_t const NERtcAudioMixingMaxVolume;
FOUNDATION_EXPORT uint32_t const NERtcAudioMixingDefaultVolume;


/// 创建 伴音 的可选项
@interface NERtcCreateAudioMixingOption : NSObject

/**
 待播放的音乐文件的绝对路径或 URL 地址，支持本地绝对路径或 URL 地址。
 - 需精确到文件名及后缀。
 - 支持的音效文件类型包括 MP3、M4A、AAC、3GP、WMA 和 WAV 格式。
 */
@property (nonatomic, copy) NSString *path;

/**
 伴音循环播放的次数：
 
 1：（默认）播放一次。
 ≤ 0：无限循环播放，直至调用 pauseAudioMixing 后暂停，或调用 stopAudioMixing 后停止。
 */
@property (nonatomic, assign) int loopCount;

/**
 是否将伴音发送远端，默认为 YES，即远端用户订阅本端音频流后可听到该伴音。
 */
@property (nonatomic, assign) BOOL sendEnabled;

/**
 是否本地播放伴音。默认为 YES，即本地用户可以听到该伴音。
 */
@property (nonatomic, assign) BOOL playbackEnabled;

/**
 音乐文件的发送音量，取值范围为 0~100。默认为 100，表示使用文件的原始音量。
 @note 若您在通话中途修改了音量设置，则当前通话中再次调用时默认沿用此设置。
 */
@property (nonatomic, assign) uint32_t sendVolume;

/**
 音乐文件的播放音量，取值范围为 0~100。默认为 100，表示使用文件的原始音量。
 @note 若您在通话中途修改了音量设置，则当前通话中再次调用时默认沿用此设置。
 */
@property (nonatomic, assign) uint32_t playbackVolume;

/**
 检查 option 参数值的合法性。
 - 返回 NO：option 参数值合法。
 - 返回 YES：option 参数值设置错误，请检查后重试。
 */
- (BOOL)isInvalid;

@end

/// 创建 音效 的可选项
@interface NERtcCreateAudioEffectOption : NSObject

/**
 待播放的音效文件路径，支持本地绝对路径或 URL 地址。
 - 需精确到文件名及后缀。
 - 支持的音效文件类型包括 MP3、M4A、AAC、3GP、WMA 和 WAV 格式。
 */
@property (nonatomic, copy) NSString *path;

/**
 音效循环播放的次数：
 
 - 1：（默认）播放音效一次。
 - ≤ 0：无限循环播放音效，直至调用 stopEffect 或 stopAllEffects 后停止。
 */
@property (nonatomic, assign) int loopCount;

/**
 是否将音效发送远端。默认为 YES，即远端用户可以听到该音效。
 */
@property (nonatomic, assign) BOOL sendEnabled;

/**
 是否本地播放该音效。默认为 YES，即本地用户可以听到该音效。
 */
@property (nonatomic, assign) BOOL playbackEnabled;

/**
 音效文件的发送音量，取值范围为 0~100。默认为 100，表示使用文件的原始音量。
 @note 若您在通话中途修改了音量设置，则当前通话中再次调用时默认沿用此设置。
 */
@property (nonatomic, assign) uint32_t sendVolume;

/**
 音效文件的播放音量，取值范围为 0~100。默认为 100，表示使用文件的原始音量。
 @note 若您在通话中途修改了音量设置，则当前通话中再次调用时默认沿用此设置。
 */
@property (nonatomic, assign) uint32_t playbackVolume;

@end


#pragma mark - live stream
/**
 音视频流编码参数等设置
 */
@interface NERtcLiveConfig: NSObject

/**
 单路视频透传开关，默认为关闭状态。
 
 开启后，如果房间中只有一路视频流输入， 则不对输入视频流进行转码，不遵循转码布局，直接推流 CDN。
 
 如果有多个房间成员视频流混合为一路流，则该设置失效，并在恢复为一个成员画面（单路流）时也不会恢复。
 */
@property (nonatomic, assign) BOOL singleVideoPassthrough;
/**
 音频推流码率。
 
 单位为 kbps，取值范围为 10~192。
 
 语音场景建议设置为 64 及以上码率，音乐场景建议设置为 128 及以上码率。
 */
@property (nonatomic, assign) NSInteger audioBitrate;

/**
 音频推流采样率。单位为Hz，可设置为：
 
 - 32000：采样率为 32 kHz。
 - 44100：采样率为 44.1 kHz。
 - （默认）48000：采样率为 48 kHz。
 */
@property (nonatomic, assign) NERtcLiveStreamAudioSampleRate sampleRate;

/**
 音频推流声道数。
 
 - 1：单声道。
 - 2：（默认）双声道。 */
@property (nonatomic, assign) NSInteger channels;

/**
 音频编码规格。
 
 - 0：（默认）LC-AAC 规格，表示基本音频编码规格。
 - 1：HE-AAC 规格，表示高效音频编码规格。 */
@property (nonatomic, assign) NERtcLiveStreamAudioCodecProfile audioCodecProfile;

@end

/**
 直播成员布局
 */
@interface NERtcLiveStreamUserTranscoding: NSObject

/**
 将指定uid对应用户的视频流拉入直播。如果添加多个 users，则 uid 不能重复。
 */
@property (nonatomic, assign) uint64_t uid;

/**
 是否在直播中向观看者播放该用户的对应视频流。可设置为：
 
 - YES：（默认）在直播中播放该用户的视频流。
 - NO：在直播中不播放该用户的视频流。
 
 推流模式为kNERtcLsModeAudio时无效。 */
@property (nonatomic, assign) BOOL videoPush;

/**
 x 参数用于设置用户图像的横轴坐标值。通过 x 和 y 指定画布坐标中的一个点，该点将作为用户图像的左上角。
 
 取值范围为 0~1920，若设置为奇数值，会自动向下取偶。
 
 用户图像范围如果超出超出画布，调用方法时会报错。
 */
@property (nonatomic, assign) NSInteger x;

/**
 x参数用于设置用户图像的纵轴坐标值。通过 x 和 y 指定画布坐标中的一个点，该点将作为用户图像的左上角。
 
 取值范围为 0~1920，若设置为奇数值，会自动向下取偶。
 
 用户图像范围如果超出超出画布，调用方法时会报错。
 */
@property (nonatomic, assign) NSInteger y;

/**
 该用户图像在画布中的宽度。
 
 取值范围为 0~1920，默认为0。若设置为奇数值，会自动向下取偶。
 
 用户图像范围如果超出超出画布，调用方法时会报错。
 */
@property (nonatomic, assign) NSInteger width;

/**
 该用户图像在画布中的高度。
 
 取值范围为 0~1920，默认为0。若设置为奇数值，会自动向下取偶。
 
 用户图像范围如果超出超出画布，调用方法时会报错。
 */
@property (nonatomic, assign) NSInteger height;

/**
 是否在直播中混流该用户的对应音频流。可设置为：
 
 - YES：（默认）在直播中混流该用户的对应音频流。
 - NO：在直播中将该用户设置为静音。 */
@property (nonatomic, assign) BOOL audioPush;

/**
 图层编号，用来决定渲染层级, 取值 0-100 , 0位于最底层，100位于最顶层，相同层级的渲染区域按照现有的覆盖逻辑实现（按照数组中顺序进行渲染，index递增依次往上叠加）
 */
@property (nonatomic, assign) NSInteger zOrder;

/**
 直播推流视频和画布的适应属性。详细信息请参考 {@link NERtcLiveStreamVideoScaleMode}。
 */
@property (nonatomic, assign) NERtcLiveStreamVideoScaleMode adaption; 

@end


/**
 图片布局
 */
@interface NERtcLiveStreamImageInfo: NSObject

/**
 占位图片的URL。
 */
@property (nonatomic, copy) NSString *url;

/**
 x 参数用于设置画布的横轴坐标值。
 
 通过 x 和 y 指定画布坐标中的一个点，该点将作为占位图片的左上角。
 
 取值范围为 0~1920，若设置为奇数值，会自动向下取偶。
 */
@property (nonatomic, assign) NSInteger x;

/**
 x 参数用于设置画布的纵轴坐标值。
 
 通过 x 和 y 指定画布坐标中的一个点，该点将作为占位图片的左上角。
 
 取值范围为 0~1920，若设置为奇数值，会自动向下取偶。
 */
@property (nonatomic, assign) NSInteger y;

/**
 该占位图片在画布中的宽度。
 
 取值范围为 0~1920，若设置为奇数值，会自动向下取偶。
 */
@property (nonatomic, assign) NSInteger width;

/**
 该占位图片在画布中的高度。
 
 取值范围为 0~1920，若设置为奇数值，会自动向下取偶。
 */
@property (nonatomic, assign) NSInteger height;

@end


/**
 直播布局
 */
@interface NERtcLiveStreamLayout: NSObject

/**
 整体画布的宽度，单位为 px。取值范围为 0~1920，若设置为奇数值，会自动向下取偶。
 */
@property (nonatomic, assign) NSInteger width;

/**
 整体画布的高度，单位为 px。取值范围为 0~1920，若设置为奇数值，会自动向下取偶。
 */
@property (nonatomic, assign) NSInteger height;

/**
 画面背景颜色，格式为 256 ✖ 256 ✖ R + 256 ✖ G + B的和。请将对应 RGB 的值分别带入此公式计算即可。若未设置，则默认为0。
 */
@property (nonatomic, assign) NSUInteger backgroundColor;

/**
 成员布局数组，详细信息请参考 {@link NERtcLiveStreamUserTranscoding}。
 */
@property (nonatomic, strong) NSArray<NERtcLiveStreamUserTranscoding *> * users;

/**
 占位图片。详细信息请参考 {@link NERtcLiveStreamImageInfo}。
 */
@property (nonatomic, nullable, strong) NERtcLiveStreamImageInfo* bgImage;

@end

/**
 直播推流任务的配置
 */
@interface NERtcLiveStreamTaskInfo: NSObject

/**
 自定义的推流任务ID。字母、数字、下划线组成的 64 位以内的字符串。请保证此ID唯一。
 */
@property (nonatomic, copy) NSString *taskID;

/**
 推流地址，例如 rtmp://test.url。
 此处的推流地址可设置为网易云信直播产品中服务端 API创建房间的返回参数pushUrl。
 */
@property (nonatomic, copy) NSString *streamURL;

/**
 旁路推流是否需要进行音视频录制。默认为关闭状态。
 */
@property (nonatomic, assign) BOOL serverRecordEnabled;

/**
 直播推流模式。默认为 kNERtcLsModeVideo。详细信息请参考 {@link NERtcLiveStreamMode}。
 */
@property (nonatomic, assign) NERtcLiveStreamMode lsMode;

/**
 设置互动直播的画面布局。详细信息请参考 {@link NERtcLiveStreamLayout}。
 */
@property (nonatomic, nullable, strong) NERtcLiveStreamLayout *layout;

/**
 音视频流编码参数等设置。详细信息请参考 {@link NERtcLiveConfig}。
 */
@property (nonatomic, nullable, strong) NERtcLiveConfig *config;

/**
 预留参数，用户自定义的发送到旁路推流客户端的信息，用于填充 H264/H265 视频中 SEI 帧内容。长度限制：4096 字节。
 */
@property (nonatomic, nullable, copy) NSString *extraInfo;

/**
 配置的选项是不是合法
 */
- (BOOL)isInvalid;

@end


#pragma mark -- Audio Frame Observer
/**
 音频原始数据回调格式（默认不设置）
 */
@interface NERtcAudioFrameRequestFormat : NSObject
/**
 音频声道数量。如果是立体声，则数据是交叉的。
 
 可设置为：
 
 - 1: 单声道。
 - 2: 双声道。 */
@property (nonatomic, assign) uint32_t channels;
/**
 音频采样率。
 */
@property (nonatomic, assign) uint32_t sampleRate;
/**
 音频数据读写权限。详细信息请参考 {@link NERtcAudioFrameOpMode}。
 */
@property (nonatomic, assign) NERtcAudioFrameOpMode mode;

@end

/**
 音频格式
 */
@interface NERtcAudioFormat : NSObject
/**
 音频 PCM 类型。详细信息请参考 {@link NERtcAudioType}。
 */
@property (nonatomic, assign) NERtcAudioType type;
/**
 音频声道数。
 
 1：单声道。
 2：双声道。
 */
@property (nonatomic, assign) uint32_t channels;
/**
 音频采样率。
 */
@property (nonatomic, assign) uint32_t sampleRate;
/**
 每个采样点的字节数。
 */
@property (nonatomic, assign) uint32_t bytesPerSample;
/**
 每个声道的采样点数。
 */
@property (nonatomic, assign) uint32_t samplesPerChannel;

@end

/**
 音频帧
 */
@interface NERtcAudioFrame : NSObject
/**
 音频格式。详细信息请参考 {@link NERtcAudioFormat}。
 */
@property (nonatomic, strong) NERtcAudioFormat* format;
/**
 音频裸数据。
 */
@property (nonatomic, assign) void* data;

@end


#pragma mark -- Volume info

/**
 声音音量信息
 */
@interface NERtcAudioVolumeInfo : NSObject

/**
 用户 ID，指定是哪个用户的音量
 */
@property (nonatomic, assign) uint64_t uid;

/**
 说话者的音量，范围为0~100。
 */
@property (nonatomic, assign) unsigned int volume;

@end


#pragma mark -- Video Frame

/**
视频帧信息
*/
@interface NERtcVideoFrame : NSObject

/**
 视频帧格式，详细信息请参考 {@link NERtcVideoFormatType}。
 */
@property(nonatomic, assign) NERtcVideoFormatType format;

/**
 视频时间戳，单位为毫秒。
 */
@property(nonatomic, assign) uint64_t timestamp;

/**
 视频高，即视频帧在横轴上的像素。
 */
@property(nonatomic, assign) uint32_t width;

/**
 视频宽，即视频帧在纵轴上的像素。
 */
@property(nonatomic, assign) uint32_t height;

/**
 视频顺时针旋转角度。详细信息请参考 {@link NERtcVideoRotationType}。
 */
@property(nonatomic, assign) NERtcVideoRotationType rotation;

/**
 视频数据，支持 CVPixelBuffer、NSData的bytes。
 */
@property(nonatomic) void * buffer;

@end

#pragma mark- MediaRelay

/**
 媒体流转发相关的数据结构。
 */
@interface NERtcChannelMediaRelayInfo : NSObject

/**
 房间名。
 */
@property (nonatomic, copy)NSString *channelName;
/**
 能加入房间的 Token。
 */
@property (nonatomic, copy)NSString *token;
/**
 用户 ID。
 */
@property (nonatomic, assign)uint64_t uid;

@end

@interface NERtcChannelMediaRelayConfiguration : NSObject

/**
 转发媒体流的目标房间配置信息。详细信息请参考 {@link NERtcChannelMediaRelayInfo}。

 - channelName：目标房间的房间名。
 - token：可以加入目标房间的 token。
 - uid：标识目标房间中的转发媒体流的 UID。请确保不要将该参数设为目标房间的主播的 UID，并与目标房间中的 所有 UID 都不同。
 */
@property(nonatomic, strong, readonly)NSDictionary<NSString *, NERtcChannelMediaRelayInfo *> * _Nullable destinationInfos;

/**
 源房间的信息。详细信息请参考 {@link NERtcChannelMediaRelayInfo}。

 - channelName：源房间名。
 - token：能加入源房间的 token。
 - uid：标识源房间中的转发媒体流的 UID。
 */
@property(nonatomic, strong)NERtcChannelMediaRelayInfo *sourceInfo;

/**
 设置跨房间媒体流转发的目标房间信息。
 
 如果您需要将媒体流转发到多个房间，可以多次调用该方法，设置多个房间的 NERtcChannelMediaRelayInfo。该方法支持最多设置 4 个目标房间。

 @param destinationInfo 目标房间信息。详细信息请参考 {@link NERtcChannelMediaRelayInfo}。
 @param channelName     目标房间名称。该参数必填，且需与该方法 destinationInfo 参数中的 channelName 一致。
 @return 成功返回YES，失败返回NO
 */
- (BOOL)setDestinationInfo:(NERtcChannelMediaRelayInfo *_Nonnull)destinationInfo forChannelName:(NSString *_Nonnull)channelName;

/**
 删除跨房间媒体流转发的房间信息。

 @param channelName 需要取消转发媒体流的目标房间名称。
 @return 成功返回YES，失败返回NO
 */
- (BOOL)removeDestinationInfoForChannelName:(NSString *_Nonnull)channelName;

@end

#pragma mark - watermark

/**
 * 文字水印设置参数。
 * 
 * 最多可添加 10 个文字水印。
 */
@interface NERtcTextWatermarkConfig : NSObject
/**
* 文字内容，设置为空时，表示不添加文字水印。
* 
* - 字符串长度无限制。最终显示受字体大小和水印框大小的影响。超出水印框的部分不显示。
* - 如果设置了水印框宽度，当文字内容长度超过水印框宽度时，会自动换行，如果超出水印框高度，超出部分不显示。
* - 未设置水印框宽度和高度时，文字不换行，超出水印框的部分不显示。
*/
@property (nonatomic, copy, nullable) NSString *content;

/**
 * 字体大小。默认值为 17。
 */
@property (nonatomic, assign) CGFloat fontSize;

/**
 * 字体颜色。ARGB 格式。默认为 0xFFFFFFFF，即白色。
 */
@property (nonatomic, strong, nullable) UIColor *fontColor;

/**
 * 水印框内背景颜色。ARGB格式，默认为 0x88888888，即灰色。支持透明度设置。
 */
@property (nonatomic, strong, nullable) UIColor *wmColor;

/**
 * 图片的 frame 布局。如果未设置宽或高，或均设置为0，表示按照原始图像大小插入水印。
 */
@property (nonatomic, assign) CGRect frame;

@end

/**
 * 时间戳水印。
 * 
 * - 只能添加 1 个时间戳水印，格式为 yyyy-MM-dd HH:mm:ss。
 * - 时间戳水印的时间和当前时间相同，且实时变化。
 */
@interface NERtcTimestampWatermarkConfig : NSObject

/**
 * 字体大小。默认值为 17。
 */
@property (nonatomic, assign) CGFloat fontSize;

/**
 * 字体颜色。ARGB 格式。默认为 0xFFFFFFFF，即白色。
 */
@property (nonatomic, strong) UIColor *fontColor;

/**
 * 图片的 frame 布局。如果未设置宽或高，或均设置为0，表示按照原始图像大小插入水印。
 */
@property (nonatomic, assign) CGRect frame;

/**
 * 水印框内背景颜色。ARGB格式，默认为 0x88888888，即灰色。支持透明度设置。
 */
@property (nonatomic, strong) UIColor *wmColor;

@end

/**
 * 图片水印设置参数。
 * 
 * 支持设置 4 个图片水印。
 */
@interface NERtcImageWatermarkConfig : NSObject

/**
 * 水印图片。
 */
@property (nonatomic, strong, nullable) NSArray<UIImage *> *images;

/**
 * 播放帧率。默认 0 帧/秒，即不自动切换图片，图片单帧静态显示。
 */
@property (nonatomic, assign) NSUInteger fps;

/**
 * 是否设置循环。默认循环，设置为 false 后水印数组播放完毕后消失。
 */
@property (nonatomic, assign) BOOL loop;

/**
 * 图片的 frame 布局。如果未设置宽或高，或均设置为 0，表示按照原始图像大小插入水印。
 */
@property (nonatomic, assign) CGRect frame;

@end


/**
 * 画布水印设置。
 * 
 * 同时设置文字、时间戳或图片水印时，如果不同类型的水印位置有重叠，会按照图片、文本、时间戳的顺序进行图层覆盖。
 */
@interface NERtcCanvasWatermarkConfig : NSObject

/**
* 文字水印。最多可以添加 10 个文字水印。
*/
@property (nonatomic, strong, nullable) NSArray<NERtcTextWatermarkConfig *> *textWatermarks;

/**
* 时间戳水印。只能添加 1 个时间戳水印。
*/
@property (nonatomic, strong, nullable) NERtcTimestampWatermarkConfig *timestampWatermark;

/**
 * 图片水印，最多可以添加 4 个图片水印。
 */
@property (nonatomic, strong, nullable) NSArray<NERtcImageWatermarkConfig *> *imageWaterMarks;

@end

/**
 * 配置媒体流加密模式和密钥。
 */
@interface NERtcEncryptionConfig : NSObject
 
/**
 * 媒体流加密模式。
 */
@property (nonatomic, assign) NERtcEncryptionMode mode;
 
/**
 * 媒体流加密密钥。字符串类型，推荐设置为英文字符串。
 */
@property (nonatomic, copy, nullable) NSString *key;

@end
#pragma mark - privatization

/**
 * 私有化服务器配置项
 * @note 如需启用私有化功能，请联系技术支持获取详情
 */
@interface NERtcServerAddresses : NSObject

/**
* 获取通道信息服务器
*/
@property (nonatomic, copy, nullable) NSString *channelServer;

/**
* 统计上报服务器
*/
@property (nonatomic, copy, nullable) NSString *statisticsServer;

/**
* roomServer服务器
*/
@property (nonatomic, copy, nullable) NSString *roomServer;

/**
* 兼容性配置服务器
*/
@property (nonatomic, copy, nullable) NSString *compatServer;

/**
* nos 域名解析服务器
*/
@property (nonatomic, copy, nullable) NSString *nosLbsServer;

/**
* 默认nos 上传服务器
*/
@property (nonatomic, copy, nullable) NSString *nosUploadSever;

/**
* 获取NOS token 服务器
*/
@property (nonatomic, copy, nullable) NSString *nosTokenServer;

/**
* 是否使用ipv6地址(默认为NO)
*/
@property (nonatomic, assign) BOOL useIPv6;

@end

#pragma mark -- Log setting

/**
 日志相关设置
 */
@interface NERtcLogSetting : NSObject

/**
 日志目录的完整路径，采用UTF-8 编码。
 */
@property (nonatomic, copy) NSString *logDir;

/**
 打印的日志级别。默认为 info 级别。详细信息请参考 {@link NERtcLogLevel}。
 */
@property (nonatomic, assign) NERtcLogLevel logLevel;

@end
NS_ASSUME_NONNULL_END
