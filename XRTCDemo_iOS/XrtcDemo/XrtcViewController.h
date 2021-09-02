//
//  LoginViewController.h
//  xrtc
//
//  Created by  xrtc on 21-07-10.
//  Copyright 2021 xrtc. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <XrtcSDK/XRTCEngine.h>

@interface XrtcViewController : UIViewController <XRTCEngineDelegate>

@property (nonatomic, strong) IBOutlet UIView *_viewOperate;
@property (nonatomic, strong) IBOutlet UIButton *_btnMuteAudio;
@property (nonatomic, strong) IBOutlet UIButton *_btnMuteVideo;

@property (nonatomic, strong) IBOutlet UIView *_videoOperate;
@property (nonatomic, strong) IBOutlet UIView *_viewLocal;
@property (nonatomic, strong) IBOutlet UIView *_viewRemote1;
@property (nonatomic, strong) IBOutlet UIView *_viewRemote2;
@property (nonatomic, strong) IBOutlet UIView *_viewRemote3;

- (IBAction)leaveRoomAction:(id)sender;
- (IBAction)startPushAction:(id)sender;
- (IBAction)stopPushAction:(id)sender;
- (IBAction)muteAudioAction:(id)sender;
- (IBAction)muteVideoAction:(id)sender;

@property (nonatomic,strong) NSString *roomId;
@property (nonatomic,strong) NSString *userId;
@property (nonatomic,assign) XRTCRtcType currentRtcType;

@end

