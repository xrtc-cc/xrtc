/*
 * Copyright (c) 2021 NetEase, Inc.  All rights reserved.
 */

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

/**
 Engine Error Code
 */
typedef NS_ENUM(int, NERtcError) {
    /** 没有错误 */
    kNERtcNoError = 0,        
    //资源分配错误 （正常请求时不会有此类错误返回）
    /** 没有权限 */
    KNERtcErrChannelReservePermissionDenied = 403,
    /** 请求超时 */  
    kNERtcErrChannelReserveTimeOut = 408,  
    /** 服务器请求参数错误 */          
    kNERtcErrChannelReserveErrorParam = 414,  
    /** 非法的APP KEY */       
    kNERtcErrChannelReserveErrorAppKey = 417, 
    /** 只支持两个用户, 有第三个人试图使用相同的房间名分配房间 */       
    kNERtcErrChannelReserveMoreThanTwoUser = 600,    
    /** 分配房间服务器出错 */
    kNERtcErrChannelReserveServerFail = 601,         
    
    //Engine error code
    /** 通用错误 */
    kNERtcErrFatal = 30001, 
    /** 内存耗尽 */                    
    kNERtcErrOutOfMemory = 30002,  
    /** 错误的参数 */             
    kNERtcErrInvalidParam = 30003,    
    /** 不支持的操作 */          
    kNERtcErrNotSupported = 30004,
    /** 当前状态不支持的操作 */              
    kNERtcErrInvalidState = 30005,  
    /** 资源耗尽 */            
    kNERtcErrLackOfResource = 30006,  
    /** 非法 index */          
    kNERtcErrInvalidIndex = 30007, 
    /** 设备未找到 */             
    kNERtcErrDeviceNotFound = 30008,  
    /** 非法设备 ID */          
    kNERtcErrInvalidDeviceSourceID = 30009,  
    /** 非法的视频 profile type */   
    kNERtcErrInvalidVideoProfile = 30010, 
     /** 设备创建错误 */      
    kNERtcErrCreateDeviceSourceFail = 30011,   
    /** 非法的渲染容器 */
    kNERtcErrInvalidRender = 30012,          
     /** 设备已经打开 */   
    kNERtcErrDevicePreviewAlreadyStarted = 30013,  
    /** 传输错误 */
    kNERtcErrTransmitPendding = 30014,   
     /** 连接服务器错误 */       
    kNERtcErrConnectFail = 30015,              
    /** 重复加入房间 */
    kNERtcErrChannelAlreadyJoined = 30100,      
    /** 尚未加入房间 */
    kNERtcErrChannelNotJoined = 30101,   
     /** 重复离开房间 */       
    kNERtcErrChannelRepleatedlyLeave = 30102,  
    /** 加入房间操作失败 */
    kNERtcErrRequestJoinChannelFail = 30103,    
    /** 会话未找到 */
    kNERtcErrSessionNotFound = 30104,      
    /** 用户未找到 */     
    kNERtcErrUserNotFound = 30105,        
    /** 非法的用户 ID */      
    kNERtcErrInvalidUserID = 30106,   
    /** 用户多媒体数据未连接 */          
    kNERtcErrMediaNotStarted = 30107,  
    /** source 未找到 */         
    kNERtcErrSourceNotFound = 30108,        
    /** 切换房间状态无效 */    
    kNERtcErrSwitchChannelInvalidState = 30109,
    /**媒体流转发状态无效。
     * 
     * 原因通常为重复调用 startChannelMediaRelay。成功调用startChannelMediaRelay后，必须先调用 stopChannelMediaRelay 方法退出当前的转发状态，才能再次调用该方法。*/
    kNERtcErrChannelMediaRelayInvalidState = 30110,
    /**媒体流转发权限不足。
     * 
     * 原因通常包括：
     * - 源房间的房间类型为双人房间（1V1模式）。此时无法转发媒体流。
     * - 调用 startChannelMediaRelay 开启媒体流转发的成员角色为观众角色，仅主播角色可以转发媒体流。*/
    kNERtcErrChannelMediaRelayPermissionDenied = 30111,
    /**停止媒体流转发操作失败。
     * 
     * 原因通常为未开启媒体流转发。请确认调用 stopChannelMediaRelay 前，是否已成功调用 startChannelMediaRelay 开启媒体流转发。*/
    kNERtcErrChannelMediaRelayStopFailed= 30112,
    
    /**设置的媒体流加密密钥与房间中其他成员不一致，加入房间失败。
     * 
     * 请通过 enableEncryption 重新设置加密密钥。
    */
    kNERtcErrEncryptNotSuitable= 30113,
    /** 连接未找到 */
    kNERtcErrConnectionNotFound = 30200, 
    /** 媒体流未找到 */       
    kNERtcErrStreamNotFound = 30201,    
     /** 加入 track 失败 */        
    kNERtcErrAddTrackFail = 30202, 
    /** track 未找到 */            
    kNERtcErrTrackNotFound = 30203,  
    /** 媒体连接断开 */           
    kNERtcErrMediaConnectionDisconnected = 30204,  
    /** 信令连接断开 */ 
    kNERtcErrSignalDisconnected = 30205,       
    /** 被踢出房间 */ 
    kNERtcErrServerKicked = 30206,       
    /** 房间已被关闭 */       
    kNERtcErrChannelBeClosed = 30207,          
    /** 房间被关闭因为切换房间的操作 */ 
    kNERtcErrChannelLeaveBySwitchAction = 30208,
    /** 房间被关闭因为有重复 uid 登录 */
    kNERtcErrChannelLeaveByDuplicateUidLogin = 30209,
    /** 操作系统权限问题 */
    kNERtcErrOSAuthorize = 30300,               
    /** 没有音频设备权限 */
    kNERtcRuntimeErrADMNoAuthorize = 40000,  
    /** 音频采集设备初始化失败 */   
    kNERtcRuntimeErrADMInitRecording = 40001,   
    /** 音频采集设备开始失败 */
    kNERtcRuntimeErrADMStartRecording = 40002, 
    /** 音频采集设备停止失败 */ 
    kNERtcRuntimeErrADMStopRecording= 40003,   
     /** 音频播放设备初始化失败 */ 
    kNERtcRuntimeErrADMInitPlayout = 40004,  
     /** 音频播放设备开始失败 */  
    kNERtcRuntimeErrADMStartPlayout = 40005,   
    /** 音频播放设备停止失败 */
    kNERtcRuntimeErrADMStopPlayout = 40006,     
    /** 没有视频设备权限 */
    kNERtcRuntimeErrVDMNoAuthorize = 50000,     
    /** 非屏幕共享使用辅流 */
    kNERtcRuntimeErrVDMNotScreenUseSubStream = 50001, 
};

typedef NS_ENUM(int, NERtcWarning) {
    /** 没有错误 */
    kNERtcNoWarning = 0,
    //资源分配错误 （正常请求时不会有此类错误返回）
    /** 当前客户端设备视频编解码能力与房间不匹配，例如设备不支持 VP8 等编码类型。在此房间中可能无法成功进行视频编解码，即本端可能无法正常显示某些远端的视频画面，同样远端也可能无法显示本端画面。 */
    KNERtcWarningChannelAbilityNotMatch = 406,
};
/**
 LiveStream Error Code
*/

typedef NS_ENUM(int, kNERtcLiveStreamError) {
    /** task请求无效，被后续操作覆盖*/
    kNERtcLiveStreamErrorRequestIsInvaild = 1301, 
    /** task参数格式错误 */         
    kNERtcLiveStreamErrorIsInvaild = 1400, 
    /** 房间已经退出 */               
    kNERtcLiveStreamErrorRoomExited = 1401,                
    /** 推流任务超出上限 */
    kNERtcLiveStreamErrorNumLimit = 1402,   
    /** 推流ID重复 */                 
    kNERtcLiveStreamErrorDuplicateId = 1403,        
    /** taskId任务不存在，或房间不存在 */        
    kNERtcLiveStreamErrorNotFound = 1404,        
     /** 请求失败 */            
    kNERtcLiveStreamErrorRequestErr = 1417,             
     /** 服务器内部错误 */  
    kNERtcLiveStreamErrorInternalServerErr = 1500,   
    /** 布局参数错误 */    
    kNERtcErrLsTaskInvalidLayout = 1501,          
    /** 用户图片错误 */        
    kNERtcErrLsTaskUserPicErr = 1502,                     
};


extern NSString * const NERtcReserveChannelErrorDomain;
extern NSString * const NERtcRoomServerErrorDomain;
extern NSString * const NERtcLocalErrorDomain;
extern NSString * const NERtcRemoteErrorDomain;

#ifdef __cplusplus
extern "C" {
#endif
    
    /**
     获取错误码描述

     @param errorCode 错误码，详细信息请参考 {@link NERtcEngineErrorCode.NERtcError}。
     @return 描述字符串。
    */
    NSString * NERtcErrorDescription(int errorCode);
    
#ifdef __cplusplus
} // extern "C"
#endif

NS_ASSUME_NONNULL_END
