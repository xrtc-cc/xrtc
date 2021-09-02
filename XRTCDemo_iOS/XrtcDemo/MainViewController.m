//
//  MainViewController.m
//  xrtc
//
//  Created by  xrtc on 21-07-10.
//  Copyright 2021 xrtc. All rights reserved.
//

#import "MainViewController.h"
#import "XRTCCommon.h"
#import <XrtcSDK/XRTCEngine.h>
#import "XrtcViewController.h"

@interface MainViewController ()

@property (nonatomic, assign) XRTCRtcType currentRtcType;

@end

@implementation MainViewController


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
    
    self.title=@"XRTC演示";
    
    self._viewOperate.frame=CGRectMake(0, UI_NAVIGATION_BAR_HEIGHT+UI_STATUS_BAR_HEIGHT,
                                       self._viewOperate.frame.size.width, self._viewOperate.frame.size.height);
    
    //隐藏键盘
    UITapGestureRecognizer *tapGestureRecognizer = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(keyboardHide:)];
    //设置成NO表示当前控件响应后会传播到其他控件上，默认为YES。
    tapGestureRecognizer.cancelsTouchesInView = NO;
    //将触摸事件添加到当前view
    [self.view addGestureRecognizer:tapGestureRecognizer];
    
    //权限申请
    [XRTCCommon applyMicOrcameraWithConfirm:^(id  _Nonnull data) {
        int code = 0;
        
        NSString *state = data;
        NSString *stateStr = @"";
        if ([state isEqualToString:@"mic_camera"]) {
            stateStr = @"已开启麦克风和相机权限";
            code = 1;
        }else if ([state isEqualToString:@"noMic_camera"]) {
            stateStr = @"麦克风权限被禁用，相机权限已开启";
            code = 2;
        }else if ([state isEqualToString:@"mic_nocamera"]) {
            stateStr = @"麦克风权限已开启，相机权限被禁用";
            code = 3;
        }else if ([state isEqualToString:@"noMic_nocamera"]) {
            stateStr = @"麦克风和相机权限被禁用";
            code = 4;
        }
        if (code > 1) {
            NSLog(@"-----麦克风和相机权限未开启!");
        }
        else {
            NSLog(@"-----麦克风和相机权限已开启!");
        }
    }];
    
    //默认为声网RTC
    self.currentRtcType=XRTCRtcTypeAgora;
    
    [self setChoiceRtcButtonTitle];
}

-(void)keyboardHide:(UITapGestureRecognizer*)tap
{
    [self._edtRoonId resignFirstResponder];
    [self._edtUserId resignFirstResponder];
}

- (IBAction)choiceRtcAction:(id)sender
{
    UIAlertController *alertController = [UIAlertController alertControllerWithTitle:@"选择Rtc" message:@"" preferredStyle:([XRTCCommon isIpad] ? UIAlertControllerStyleAlert : UIAlertControllerStyleActionSheet)];
    
    UIAlertAction *agoraAction = [UIAlertAction actionWithTitle:@"使用声网Rtc" style:UIAlertActionStyleDefault handler:^(UIAlertAction * _Nonnull action) {
        self.currentRtcType=XRTCRtcTypeAgora;
        
        [self setChoiceRtcButtonTitle];
    }];
    [alertController addAction:agoraAction];
    
    UIAlertAction *huaweiAction = [UIAlertAction actionWithTitle:@"使用华为Rtc" style:UIAlertActionStyleDefault handler:^(UIAlertAction * _Nonnull action) {
        self.currentRtcType=XRTCRtcTypeHuawei;
        
        [self setChoiceRtcButtonTitle];
    }];
    [alertController addAction:huaweiAction];
    
    UIAlertAction *tengxunAction = [UIAlertAction actionWithTitle:@"使用腾讯Rtc" style:UIAlertActionStyleDefault handler:^(UIAlertAction * _Nonnull action) {
        self.currentRtcType=XRTCRtcTypeTengXun;
        
        [self setChoiceRtcButtonTitle];
    }];
    [alertController addAction:tengxunAction];
    
    UIAlertAction *wangyiAction = [UIAlertAction actionWithTitle:@"使用云信Rtc" style:UIAlertActionStyleDefault handler:^(UIAlertAction * _Nonnull action) {
        self.currentRtcType=XRTCRtcTypeWangYi;
        
        [self setChoiceRtcButtonTitle];
    }];
    [alertController addAction:wangyiAction];
    
    //取消按钮
    UIAlertAction *cancelAction = [UIAlertAction actionWithTitle:@"取消" style:UIAlertActionStyleCancel handler:^(UIAlertAction * _Nonnull action) {
        //NSLog(@"点击了取消按钮");
    }];
    [alertController addAction:cancelAction];
    
    [self presentViewController:alertController animated:YES completion:nil];
}


- (IBAction)startCallAction:(id)sender
{
    NSString *roomId=self._edtRoonId.text;
    NSString *userId=self._edtUserId.text;
    
    if ([roomId isEqualToString:@""]==YES)
    {
        [self showTipsMsg:@"请输入房间号"];
        return;
    }
    
    if ([userId isEqualToString:@""]==YES)
    {
        [self showTipsMsg:@"请输入用户ID"];
        return;
    }
    
    XrtcViewController * xrtcVc = [[XrtcViewController alloc] initWithNibName:@"XrtcViewController" bundle:nil];
    xrtcVc.roomId=roomId;
    xrtcVc.userId=userId;
    xrtcVc.currentRtcType=self.currentRtcType;
    [self.navigationController pushViewController:xrtcVc animated:YES];
}

- (void)setChoiceRtcButtonTitle
{
    if (self.currentRtcType==XRTCRtcTypeAgora)
    {
        [self._btnChoiceRtc setTitle:@"选择RTC：声网" forState:UIControlStateNormal];
    }
    else if (self.currentRtcType==XRTCRtcTypeHuawei)
    {
        [self._btnChoiceRtc setTitle:@"选择RTC：华为" forState:UIControlStateNormal];
    }
    else if (self.currentRtcType==XRTCRtcTypeTengXun)
    {
        [self._btnChoiceRtc setTitle:@"选择RTC：腾讯" forState:UIControlStateNormal];
    }
    else if (self.currentRtcType==XRTCRtcTypeWangYi)
    {
        [self._btnChoiceRtc setTitle:@"选择RTC：云信" forState:UIControlStateNormal];
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

@end
