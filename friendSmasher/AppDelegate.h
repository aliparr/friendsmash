//
//  AppDelegate.h
//  friendSmasher
//
//  Created by Ali Parr on 17/07/2012.
//  Copyright (c) 2012 Facebook. All rights reserved.
//

#import <UIKit/UIKit.h>

#import "FBConnect.h"

#define APP_HANDLED_URL @"APP_HANDLED_URL"

@interface AppDelegate : UIResponder <UIApplicationDelegate>

@property (strong, nonatomic) FBSession *session;
@property (nonatomic, retain) Facebook *facebook;

@property (strong, nonatomic) UIWindow *window;

@property (nonatomic, retain) NSURL *openedURL;

@end
