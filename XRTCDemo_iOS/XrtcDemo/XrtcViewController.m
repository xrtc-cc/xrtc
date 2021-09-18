//
//  XrtcViewController.m
//  xrtc
//
//  Created by  xrtc on 21-07-10.
//  Copyright 2021 xrtc. All rights reserved.
//

#import "XrtcViewController.h"
#import "XRTCCommon.h"
#import "RtcAccountInfo.h"

@interface XrtcViewController () <XRTCEngineDelegate>

@property (nonatomic,strong) XRTCEngine        *xrtcEngine;  // XRTC对象

@property (nonatomic,assign) BOOL isJoinedRoom;
@property (nonatomic,assign) BOOL isPushedStream;

@property (nonatomic,assign) BOOL isLocalMutedAudio;
@property (nonatomic,assign) BOOL isLocalMutedVideo;

@property (nonatomic,strong) NSMutableDictionary *remoteUserDict;  // 远端用户

@end

@implementation XrtcViewController


// The designated initializer.  Override if you create the controller programmatically and want to perform customization that is not appropriate for viewDidLoad.

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self)
    {
        // Custom initialization.
    }

    return self;
}

// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad
{
    [super viewDidLoad];
    
    self.title=@"正在初始化...";
    
    self._viewOperate.frame=CGRectMake(0, UI_NAVIGATION_BAR_HEIGHT+UI_STATUS_BAR_HEIGHT,
                                       self._viewOperate.frame.size.width, self._viewOperate.frame.size.height);
    
    // 视图比例固定，所以他们的父视图也要长宽等比缩放
    self._videoOperate.frame=CGRectMake(self._videoOperate.frame.origin.x,
                                        self._videoOperate.frame.origin.y,
                                        self._videoOperate.frame.size.width*screen_amplify_x,
                                        self._videoOperate.frame.size.height*screen_amplify_x);
    
    self._viewRemote1.tag=0;
    self._viewRemote2.tag=0;
    self._viewRemote3.tag=0;
    
    //初始化RTC对象
    NSLog(@"-----%@",[XRTCEngine getSdkVersion]);
    self.xrtcEngine=[XRTCEngine shareInstance];
    self.xrtcEngine.delegate=(id)self;
    
    self.remoteUserDict = [[NSMutableDictionary alloc] init];
    
    //自动加入房间
    [self joinRoom];
}

- (void)viewWillDisappear:(BOOL)animated
{
    [super viewWillDisappear:animated];
    
    [self.xrtcEngine leaveChannel];
    
    [self.xrtcEngine destroyRtcEngine];
}

- (void)joinRoom
{
    self.title=@"授权获取中...";
    
    [self.xrtcEngine getAuthorization:@"xrtctest01" xrtcKey:@"" authorizedResult:^(int code, NSString *msg) {
        NSLog(@"校验结果:code=%d,msg=%@",code,msg);
        
        if (code==0)
        {
            dispatch_async(dispatch_get_main_queue(), ^{
                //初始化Rtc
                [self initRtc];
                
                self.title=@"房间加入中...";
                
                //加入频道
                [self.xrtcEngine joinRtcChannel:self.roomId
                                            uid:self.userId.integerValue];
            });
        }
        else
        {
            dispatch_async(dispatch_get_main_queue(), ^{
                self.title=@"授权失败！";
            });
        }
    }];
}

- (void)initRtc
{
    int retValue1=[self.xrtcEngine switchRtc:self.currentRtcType];
    if (retValue1!=0)
    {
        NSLog(@"-----切换Rtc错误!");
        
        return;
    }
    
    XRTCInitParam *engineInfo = [[XRTCInitParam alloc] init];
    if (self.currentRtcType==XRTCRtcTypeAgora)
    {
        engineInfo.appId=[[RtcAccountInfo sharedInstance] getAgoraAppId];
    }
    else if (self.currentRtcType==XRTCRtcTypeHuawei)
    {
        engineInfo.appId=[[RtcAccountInfo sharedInstance] getHuaWeiAppId];
    }
    else if (self.currentRtcType==XRTCRtcTypeTengXun)
    {
        engineInfo.appId=[[RtcAccountInfo sharedInstance] getTengXunAppId];
    }
    else if (self.currentRtcType==XRTCRtcTypeWangYi)
    {
        engineInfo.appId=[[RtcAccountInfo sharedInstance] getWangYiAppId];
    }
    
    engineInfo.token=[[RtcAccountInfo sharedInstance] getToken];
    engineInfo.domain=[[RtcAccountInfo sharedInstance] getDomain];
    engineInfo.secretKey=[[RtcAccountInfo sharedInstance] getSecretKey];
    
    int retValue2=[self.xrtcEngine initRtcEngine:engineInfo];
    if (retValue2!=0)
    {
        NSLog(@"-----初始化Rtc错误");
        
        return;
    }
    
    NSLog(@"-----初始化Rtc成功");
    
    [self.xrtcEngine videoEncoderConfiguration:XRTCVideoEncode_240P];

}

- (IBAction)leaveRoomAction:(id)sender
{
    if (self.isJoinedRoom==NO)
    {
        [self showTipsMsg:@"请先加入一个房间"];
        return;
    }
    
    [self.navigationController popViewControllerAnimated:YES];
}

- (IBAction)startPushAction:(id)sender
{
    NSLog(@"-----开始推流");
    
    if (self.isJoinedRoom==NO)
    {
        [self showTipsMsg:@"请先加入一个房间"];
        return;
    }
    
    if (self.isPushedStream==YES)
    {
        [self showTipsMsg:@"当前已经开始推流"];
        return;
    }
    
    [self.xrtcEngine setRtcClientRole:XRTCRoleBroadcaster];
    
    UIView *locView=self._viewLocal;
    [self.xrtcEngine bindLocalVideo:locView
                                   uid:self.userId.integerValue
                             channelId:self.roomId
                                  mode:XRTCVideoDisplayModeHidden];
    
    [self.xrtcEngine muteLocalAudioStream:NO];
    [self.xrtcEngine muteLocalVideoStream:NO];
    
    self.isPushedStream=YES;
}

- (IBAction)stopPushAction:(id)sender
{
    NSLog(@"-----停止推流");
    
    if (self.isPushedStream==NO)
    {
        [self showTipsMsg:@"请先开始推流"];
        return;
    }
    
    [self.xrtcEngine setRtcClientRole:XRTCRoleAudience];
    
    [self.xrtcEngine muteLocalAudioStream:YES];
    [self.xrtcEngine muteLocalVideoStream:YES];
    
    UIView *locView=self._viewLocal;
    [self.xrtcEngine unBindLocalVideo:locView
                                     uid:self.userId.integerValue
                               channelId:self.roomId];
    
    [self resetAvAll];
}

- (IBAction)muteAudioAction:(id)sender
{
    if (self.isPushedStream==NO)
    {
        [self showTipsMsg:@"请先开始推流"];
        return;
    }
    
    if (self.isLocalMutedAudio==NO)
    {
        NSLog(@"-----停用音频");
        
        [self.xrtcEngine muteLocalAudioStream:YES];
        self.isLocalMutedAudio=YES;
        
        [self._btnMuteAudio setTitle:@"启用音频" forState:UIControlStateNormal];
    }
    else
    {
        NSLog(@"-----开启音频");
        
        [self.xrtcEngine muteLocalAudioStream:NO];
        self.isLocalMutedAudio=NO;

        [self._btnMuteAudio setTitle:@"停用音频" forState:UIControlStateNormal];
    }
}

- (IBAction)muteVideoAction:(id)sender
{
    if (self.isPushedStream==NO)
    {
        [self showTipsMsg:@"请先开始推流"];
        return;
    }
    
    if (self.isLocalMutedVideo==NO)
    {
        NSLog(@"-----停用视频");
        
        [self.xrtcEngine muteLocalVideoStream:YES];
        self.isLocalMutedVideo=YES;
        
        [self._btnMuteVideo setTitle:@"启用视频" forState:UIControlStateNormal];
    }
    else
    {
        NSLog(@"-----开启视频");
        
        [self.xrtcEngine muteLocalVideoStream:NO];
        self.isLocalMutedVideo=NO;
        
        [self._btnMuteVideo setTitle:@"停用视频" forState:UIControlStateNormal];
    }
    
    if (self.isLocalMutedVideo==YES) {
        [self showOnlyAudioView:self._viewLocal uid:self.userId.integerValue];
    }else {
        [self hiddenOnlyAudioView:self.userId.integerValue];
    }
}

#pragma mark - 自定义方法

- (void) showTipsMsg:(NSString *)msg
{
    UIAlertController *alertController = [UIAlertController alertControllerWithTitle:@"提示" message:msg preferredStyle:UIAlertControllerStyleAlert];
    UIAlertAction *knowAction = [UIAlertAction actionWithTitle:@"确定" style:UIAlertActionStyleDefault handler:^(UIAlertAction * _Nonnull action) {
    }];
    [alertController addAction:knowAction];
    [self presentViewController:alertController animated:YES completion:nil];
}

- (void) showOnlyAudioView:(UIView *)videoView uid:(NSUInteger)uid
{
    UIImageView *onlyAudioView = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"audioPlay.jpg"]];
    onlyAudioView.frame=videoView.frame;
    onlyAudioView.tag=uid;
    onlyAudioView.autoresizingMask=UIViewAutoresizingFlexibleHeight|UIViewAutoresizingFlexibleWidth;
    [self._videoOperate addSubview:onlyAudioView];

}

- (void) hiddenOnlyAudioView:(NSUInteger)uid
{
    for (UIView *subView in self._videoOperate.subviews)
    {
        if ([subView isKindOfClass:[UIImageView class]]==YES && subView.tag==uid)
        {
            [subView removeFromSuperview];
        }
    }
}

- (void) resetAvAll
{
    self.isPushedStream=NO;
    self.isLocalMutedAudio=NO;
    self.isLocalMutedVideo=NO;
    
    [self._btnMuteAudio setTitle:@"停用音频" forState:UIControlStateNormal];
    [self._btnMuteVideo setTitle:@"停用视频" forState:UIControlStateNormal];
    
    [self hiddenOnlyAudioView:self.userId.integerValue];
}

- (UIView *) getBlankVideoView:(NSUInteger)uid
{
    UIView *videoView=nil;
    
    if (self._viewRemote1.tag==0)
    {
        videoView=self._viewRemote1;
    }
    else if (self._viewRemote2.tag==0)
    {
        videoView=self._viewRemote2;
    }
    else if (self._viewRemote3.tag==0)
    {
        videoView=self._viewRemote3;
    }
    
    if (videoView!=nil)
    {
        videoView.tag=uid;
    }
    
    return videoView;
}

- (UIView *) getUseVideoView:(NSUInteger)uid
{
    UIView *videoView=nil;
    
    if (self._viewRemote1.tag==uid)
    {
        videoView=self._viewRemote1;
    }
    else if (self._viewRemote2.tag==uid)
    {
        videoView=self._viewRemote2;
    }
    else if (self._viewRemote3.tag==uid)
    {
        videoView=self._viewRemote3;
    }
    
    return videoView;
}

#pragma mark - XRTCEngineDelegate

//本端加入频道成功
- (void)XRTCEngine:(XRTCEngine *)xrtcEngine joinSuccess:(NSUInteger)uid
{
    NSLog(@"-----本端加入频道");
    self.isJoinedRoom=YES;
    
    dispatch_async(dispatch_get_main_queue(), ^{
        self.title=[NSString stringWithFormat:@"加入房间：%@",self.roomId];
    });
}

//本端退出频道
- (void)XRTCEngine:(XRTCEngine *)xrtcEngine didLeaveChannel:(NSUInteger)uid
{
    NSLog(@"-----本端退出频道");
}

//远端加入频道
- (void)XRTCEngine:(XRTCEngine *)xrtcEngine didJoinedOfUid:(NSUInteger)uid
{
    NSLog(@"-----远端加入频道 uid：%lu",(unsigned long)uid);
    
    //远端用户加入字典
    NSMutableDictionary *userDict = [[NSMutableDictionary alloc] init];
    [userDict setObject:@(uid) forKey:@"uid"];
    [userDict setObject:@(NO) forKey:@"isRemoteMutedAudio"];
    [userDict setObject:@(NO) forKey:@"isRemoteMutedVideo"];
    
    NSString *userId = [NSString stringWithFormat: @"%ld", (long)uid];
    [self.remoteUserDict setObject:userDict forKey:userId];
}

//远端退出频道
- (void)XRTCEngine:(XRTCEngine *)xrtcEngine didOfflineOfUid:(NSUInteger)uid;
{
    NSLog(@"-----远端退出频道 uid：%lu",(unsigned long)uid);
    
    dispatch_async(dispatch_get_main_queue(), ^{
        
        //删除远端用户
        NSString *userId = [NSString stringWithFormat: @"%ld", (long)uid];
        [self.remoteUserDict removeObjectForKey:userId];
        
        UIView *remoteView=[self getUseVideoView:uid];
        [self.xrtcEngine unBindRemoteVideo:remoteView
                                          uid:uid
                                    channelId:self.roomId];
        
        [self hiddenOnlyAudioView:uid];
        
        remoteView.tag=0;
    });
}

//收到远端音频首帧
- (void)XRTCEngine:(XRTCEngine *)xrtcEngine firstRemoteAudioFrameOfUid:(NSUInteger)uid
{
    NSLog(@"-----收到远端音频首帧 uid：%lu",(unsigned long)uid);
}

//收到远端视频首帧
- (void)XRTCEngine:(XRTCEngine *)xrtcEngine firstRemoteVideoDecodedOfUid:(NSUInteger)uid
{
    NSLog(@"-----收到远端视频首帧 uid：%lu",(unsigned long)uid);
    
    dispatch_async(dispatch_get_main_queue(), ^{
        UIView *remoteView=[self getBlankVideoView:uid];
        
        if (remoteView!=nil)
        {
            [self.xrtcEngine bindRemoteVideo:remoteView
                                            uid:uid
                                      channelId:self.roomId mode:XRTCVideoDisplayModeHidden];
        }
        else
        {
            [self.xrtcEngine muteRemoteAudioStream:uid mute:YES];
            [self.xrtcEngine muteRemoteVideoStream:uid mute:YES];
            
            UIAlertController *alertController = [UIAlertController alertControllerWithTitle:@"提示" message:@"当前房间只能容纳4个人" preferredStyle: UIAlertControllerStyleAlert];
            
            UIAlertAction *okAction = [UIAlertAction actionWithTitle:@"知道了" style:UIAlertActionStyleDefault handler:^(UIAlertAction * _Nonnull action) {
            }];
            [alertController addAction:okAction];
        }
    });
}

//远端音频禁止
- (void)XRTCEngine:(XRTCEngine *)xrtcEngine didAudioMuted:(BOOL)muted byUid:(NSUInteger)uid;
{
    NSLog(@"-----远端音频禁止 muted:%d,uid：%lu",muted,(unsigned long)uid);
    
    NSString *userId = [NSString stringWithFormat: @"%ld", (long)uid];
    NSMutableDictionary *userDict = [self.remoteUserDict objectForKey:userId];
    [userDict setObject:@(muted) forKey:@"isRemoteMutedAudio"];
    
    dispatch_async(dispatch_get_main_queue(), ^{
        UIView *remoteView=[self getUseVideoView:uid];
        
        BOOL isRemoteMutedAudio=[[userDict objectForKey:@"isRemoteMutedAudio"] boolValue];
        BOOL isRemoteMutedVideo=[[userDict objectForKey:@"isRemoteMutedVideo"] boolValue];
        
        if (isRemoteMutedVideo==YES && isRemoteMutedAudio==NO) {
            [self showOnlyAudioView:remoteView uid:uid];
        }else {
            [self hiddenOnlyAudioView:uid];
        }
    });
}

//远端视频禁止
- (void)XRTCEngine:(XRTCEngine *)xrtcEngine didVideoMuted:(BOOL)muted byUid:(NSUInteger)uid
{
    NSLog(@"-----远端视频禁止 muted:%d,uid：%lu",muted,(unsigned long)uid);
    
    NSString *userId = [NSString stringWithFormat: @"%ld", (long)uid];
    NSMutableDictionary *userDict = [self.remoteUserDict objectForKey:userId];
    [userDict setObject:@(muted) forKey:@"isRemoteMutedVideo"];
    
    dispatch_async(dispatch_get_main_queue(), ^{
        UIView *remoteView=[self getUseVideoView:uid];
        
        BOOL isRemoteMutedAudio=[[userDict objectForKey:@"isRemoteMutedAudio"] boolValue];
        BOOL isRemoteMutedVideo=[[userDict objectForKey:@"isRemoteMutedVideo"] boolValue];
        
        if (isRemoteMutedVideo==YES && isRemoteMutedAudio==NO) {
            [self showOnlyAudioView:remoteView uid:uid];
        }else {
            [self hiddenOnlyAudioView:uid];
        }
    });
}

- (void)XRTCEngine:(XRTCEngine *)xrtcEngine onError:(NSInteger)errorCode errorMsg:(NSString *)errorMsg
{
    NSLog(@"onError,errorCode=%ld,errorMsg=%@", (long)errorCode,errorMsg);
}

- (void)XRTCEngine:(XRTCEngine *)xrtcEngine onWarning:(NSInteger)warningCode warningMsg:(NSString *)warningMsg
{
    NSLog(@"onWarning,warningCode=%ld,warningMsg=%@", (long)warningCode,warningMsg);
}

- (void)XRTCEngine:(XRTCEngine *)xrtcEngine onConnStateChange:(XRTCConnStateType)state
{
    NSLog(@"onConnStateChange,state=%ld", (long)state);
}

- (void)XRTCEngine:(XRTCEngine *)xrtcEngine
   localAudioStats:(XRTCLocalAudioStats *)localAudioStats
   localVideoStats:(XRTCLocalVideoStats *)localVideoStats
{
    //每隔2s触发
}

- (void)XRTCEngine:(XRTCEngine *)xrtcEngine
  remoteAudioStats:(XRTCRemoteAudioStats *)remoteAudioStats
  remoteVideoStats:(XRTCRemoteVideoStats *)remoteVideoStats
{
    //每隔2s触发
}

@end
