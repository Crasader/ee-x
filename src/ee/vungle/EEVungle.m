//
//  EEVungle.m
//  ee_x
//
//  Created by Pham Xuan Han on 5/12/17.
//
//

#import <UIKit/UIKit.h>

#import "ee/core/EEMessageBridge.h"
#import "ee/core/internal/EEDictionaryUtils.h"
#import "ee/core/internal/EEJsonUtils.h"
#import "ee/core/internal/EEUtils.h"
#import "ee/vungle/EEVungle.h"

#ifdef EE_X_USE_IRON_SOURCE_MEDIATION
#import <ISVungleAdapter/VungleSDK.h>
#else // EE_X_USE_IRON_SOURCE_MEDIATION
#import <VungleSDK/VungleSDK.h>
#endif // EE_X_USE_IRON_SOURCE_MEDIATION

@interface EEVungle () <VungleSDKDelegate> {
    BOOL initialized_;
    EEMessageBridge* bridge_;
    VungleSDK* sdk_;
}

@end

@implementation EEVungle

// clang-format off
static NSString* const k__initialize        = @"Vungle_initialize";
static NSString* const k__hasRewardedVideo  = @"Vungle_hasRewardedVideo";
static NSString* const k__showRewardedVideo = @"Vungle_showRewardedVideo";
static NSString* const k__onStart           = @"Vungle_onStart";
static NSString* const k__onEnd             = @"Vungle_onEnd";
// clang-format on

- (id)init {
    self = [super init];
    if (self == nil) {
        return self;
    }
    initialized_ = NO;
    bridge_ = [EEMessageBridge getInstance];
    sdk_ = [VungleSDK sharedSDK];
    [self registerHandlers];
    return self;
}

- (void)dealloc {
    [self deregisterHandlers];
    [self destroy];
    [super dealloc];
}

- (void)registerHandlers {
    [bridge_ registerHandler:k__initialize
                    callback:^(NSString* message) {
                        NSDictionary* dict =
                            [EEJsonUtils convertStringToDictionary:message];
                        NSString* gameId = dict[@"gameId"];
                        NSString* placementId = dict[@"placementId"];
                        [self initialize:gameId placementId:placementId];
                        return @"";
                    }];

    [bridge_ registerHandler:k__hasRewardedVideo
                    callback:^(NSString* placementId) {
                        return [EEUtils
                            toString:[self hasRewardedVideo:placementId]];
                    }];

    [bridge_ registerHandler:k__showRewardedVideo
                    callback:^(NSString* placementId) {
                        return [EEUtils
                            toString:[self showRewardedVideo:placementId]];
                    }];
}

- (void)deregisterHandlers {
    [bridge_ deregisterHandler:k__initialize];
    [bridge_ deregisterHandler:k__hasRewardedVideo];
    [bridge_ deregisterHandler:k__showRewardedVideo];
}

- (void)initialize:(NSString*)gameId placementId:(NSString*)placementId {
    if (initialized_) {
        return;
    }
    [sdk_ startWithAppId:gameId
              placements:[NSArray arrayWithObjects:placementId, nil]
                   error:nil];
    [sdk_ setDelegate:self];
    initialized_ = YES;
}

- (void)destroy {
    [sdk_ setDelegate:nil];
}

- (BOOL)hasRewardedVideo:(NSString* _Nonnull)placementId {
    return [sdk_ isAdCachedForPlacementID:placementId];
}

- (BOOL)showRewardedVideo:(NSString* _Nonnull)placementId {
    UIViewController* view = [EEUtils getCurrentRootViewController];
    return [sdk_ playAd:view options:nil placementID:placementId error:nil];
}

- (void)vungleWillShowAdForPlacementID:(nullable NSString*)placementID {
    NSLog(@"%s %@", __PRETTY_FUNCTION__, placementID);
    [bridge_ callCpp:k__onStart];
}

- (void)vungleWillCloseAdWithViewInfo:(VungleViewInfo*)info
                          placementID:(NSString*)placementID {
    BOOL result = [info.completedView boolValue];
    [bridge_ callCpp:k__onEnd message:[EEUtils toString:result]];
}

- (void)vungleAdPlayabilityUpdate:(BOOL)isAdPlayable
                      placementID:(NSString*)placementID {
    NSLog(@"%s: playable = %d id = %@", __PRETTY_FUNCTION__, (int)isAdPlayable,
          placementID);
}

@end
