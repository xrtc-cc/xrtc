//
//  LoginViewController.h
//  xrtc
//
//  Created by  xrtc on 21-07-10.
//  Copyright 2021 xrtc. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface MainViewController : UIViewController

@property (nonatomic, strong) IBOutlet UIView *_viewOperate;
@property (nonatomic, strong) IBOutlet UIButton *_btnChoiceRtc;
@property (nonatomic, strong) IBOutlet UITextField * _edtRoonId;
@property (nonatomic, strong) IBOutlet UITextField * _edtUserId;


- (IBAction)choiceRtcAction:(id)sender;
- (IBAction)startCallAction:(id)sender;


@end

