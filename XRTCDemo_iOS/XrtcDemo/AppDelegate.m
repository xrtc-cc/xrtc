//
//  AppDelegate.m
//  XrtcDemo
//
//  Created by mac on 2021/7/22.
//

#import "AppDelegate.h"
#import "MainViewController.h"

@interface AppDelegate ()

@end

@implementation AppDelegate


- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    // Override point for customization after application launch.
    
    [self showMainViewController];
    
    return YES;
}

- (void) showMainViewController
{
    MainViewController *rootViewController= [[MainViewController alloc] initWithNibName:@"MainViewController" bundle:nil];
    UINavigationController  *UNmain = [[UINavigationController alloc] initWithRootViewController:rootViewController];
    
    self.window.rootViewController = UNmain;
    [self.window makeKeyAndVisible];
}

#pragma mark -设置只支持竖屏
- (UIInterfaceOrientationMask)application:(UIApplication *)application supportedInterfaceOrientationsForWindow:(nullable UIWindow *)window
{
    return UIInterfaceOrientationMaskPortrait;
}

@end
