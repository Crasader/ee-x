//
//  EEUtils.hpp
//  ee_core
//
//  Created by Zinge on 3/28/17.
//
//

#import <Foundation/Foundation.h>

#if TARGET_OS_IOS
@class UIViewController;
@class UIWindow;
#endif // TARGET_OS_IOS

@interface EEUtils : NSObject

+ (void)registerHandlers;

#if TARGET_OS_IOS
+ (UIWindow* _Nullable)getKeyWindow;
+ (UIViewController* _Nullable)getCurrentRootViewController;

/// Checks whether the device orientation is landscape.
+ (BOOL)isLandscape;
#endif // TARGET_OS_IOS

/// Converts a bool value to string.
+ (NSString* _Nonnull)toString:(BOOL)value;

/// Converts a string value to bool.
+ (BOOL)toBool:(NSString* _Nonnull)value;

+ (BOOL)isMainThread;
+ (BOOL)runOnMainThread;

+ (NSString* _Nonnull)getVersionName;
+ (NSString* _Nonnull)getVersionCode;

+ (BOOL)isApplicationInstalled:(NSString* _Nonnull)applicationId;
+ (BOOL)isTablet;
+ (BOOL)testConnection:(NSString* _Nonnull)hostName;

@end
