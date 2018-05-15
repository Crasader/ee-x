//
//  EEAdMob.cpp
//  ee_x
//
//  Created by Zinge on 10/11/17.
//
//

#import <GoogleMobileAds/GoogleMobileAds.h>

#import "ee/admob/EEAdMob.h"
#import "ee/admob/internal/EEAdMobBannerAd.h"
#import "ee/admob/internal/EEAdMobInterstitialAd.h"
#import "ee/admob/internal/EEAdMobNativeAd.h"
#import "ee/core/EEMessageBridge.h"
#import "ee/core/internal/EEJsonUtils.h"
#import "ee/core/internal/EEUtils.h"

@interface EEAdMob () <GADRewardBasedVideoAdDelegate> {
    id<EEIMessageBridge> bridge_;
    NSMutableArray<NSString*>* testDevices_;
    NSMutableDictionary<NSString*, EEAdMobBannerAd*>* bannerAds_;
    NSMutableDictionary<NSString*, EEAdMobNativeAd*>* nativeAds_;
    NSMutableDictionary<NSString*, EEAdMobInterstitialAd*>* interstitialAds_;
}

@end

@implementation EEAdMob

// clang-format off
static NSString* const k__initialize                = @"AdMob_initialize";

static NSString* const k__getEmulatorTestDeviceHash = @"AdMob_getEmulatorTestDeviceHash";
static NSString* const k__addTestDevice             = @"AdMob_addTestDevice";

static NSString* const k__createBannerAd            = @"AdMob_createBannerAd";
static NSString* const k__destroyBannerAd           = @"AdMob_destroyBannerAd";

static NSString* const k__createNativeAd            = @"AdMob_createNativeAd";
static NSString* const k__destroyNativeAd           = @"AdMob_destroyNativeAd";

static NSString* const k__createInterstitialAd      = @"AdMob_createInterstitialAd";
static NSString* const k__destroyInterstitialAd     = @"AdMob_destroyInterstitialAd";

static NSString* const k__hasRewardedVideo          = @"AdMob_hasRewardedVideo";
static NSString* const k__loadRewardedVideo         = @"AdMob_loadRewardedVideo";
static NSString* const k__showRewardedVideo         = @"AdMob_showRewardedVideo";

static NSString* const k__onRewarded                = @"AdMob_onRewarded";
static NSString* const k__onFailedToLoad            = @"AdMob_onFailedToLoad";
static NSString* const k__onLoaded                  = @"AdMob_onLoaded";
static NSString* const k__onOpened                  = @"AdMob_onOpened";
static NSString* const k__onClosed                  = @"AdMob_onClosed";
// clang-format on

// clang-format off
static NSString* const k__ad_id                 = @"ad_id";
static NSString* const k__ad_size               = @"ad_size";
static NSString* const k__layout_name           = @"layout_name";
// clang-format on

- (id)init {
    self = [super init];
    if (self == nil) {
        return self;
    }
    bridge_ = [EEMessageBridge getInstance];
    testDevices_ = [[NSMutableArray alloc] init];
    bannerAds_ = [[NSMutableDictionary alloc] init];
    nativeAds_ = [[NSMutableDictionary alloc] init];
    interstitialAds_ = [[NSMutableDictionary alloc] init];
    [[GADRewardBasedVideoAd sharedInstance] setDelegate:self];
    [self registerHandlers];
    return self;
}

- (void)dealloc {
    [self deregisterHandlers];
    [testDevices_ release];
    testDevices_ = nil;
    [bannerAds_ release];
    bannerAds_ = nil;
    [nativeAds_ release];
    nativeAds_ = nil;
    [interstitialAds_ release];
    interstitialAds_ = nil;
    [super dealloc];
}

- (void)registerHandlers {
    [bridge_ registerHandler:k__initialize
                    callback:^(NSString* message) {
                        NSString* applicationId = message;
                        [self initialize:applicationId];
                        return @"";
                    }];

    [bridge_ registerHandler:k__getEmulatorTestDeviceHash
                    callback:^(NSString* message) {
                        return [self getEmulatorTestDeviceHash];
                    }];

    [bridge_ registerHandler:k__addTestDevice
                    callback:^(NSString* message) {
                        NSString* hash = message;
                        [self addTestDevice:hash];
                        return @"";
                    }];

    [bridge_ registerHandler:k__createBannerAd
                    callback:^(NSString* message) {
                        NSDictionary* dict =
                            [EEJsonUtils convertStringToDictionary:message];
                        NSString* adId = dict[k__ad_id];
                        int adSizeIndex = [dict[k__ad_size] intValue];
                        GADAdSize adSize =
                            [EEAdMobBannerAd adSizeFor:adSizeIndex];
                        return [EEUtils
                            toString:[self createBannerAd:adId size:adSize]];
                    }];

    [bridge_ registerHandler:k__destroyBannerAd
                    callback:^(NSString* message) {
                        NSString* adId = message;
                        return [EEUtils toString:[self destroyBannerAd:adId]];
                    }];

    [bridge_
        registerHandler:k__createNativeAd
               callback:^(NSString* message) {
                   NSDictionary* dict =
                       [EEJsonUtils convertStringToDictionary:message];
                   NSString* adId = dict[k__ad_id];
                   NSString* layoutName = dict[k__layout_name];
                   return [EEUtils
                       toString:
                           [self
                               createNativeAd:adId
                                         type:kGADAdLoaderAdTypeNativeAppInstall
                                       layout:layoutName]];
               }];

    [bridge_ registerHandler:k__destroyNativeAd
                    callback:^(NSString* message) {
                        NSString* adId = message;
                        return [EEUtils toString:[self destroyNativeAd:adId]];
                    }];

    [bridge_ registerHandler:k__createInterstitialAd
                    callback:^(NSString* message) {
                        NSString* placementId = message;
                        return [EEUtils
                            toString:[self createInterstitialAd:placementId]];
                    }];

    [bridge_ registerHandler:k__destroyInterstitialAd
                    callback:^(NSString* message) {
                        NSString* placementId = message;
                        return [EEUtils
                            toString:[self destroyInterstitialAd:placementId]];
                    }];

    [bridge_ registerHandler:k__hasRewardedVideo
                    callback:^(NSString* message) {
                        return [EEUtils toString:[self hasRewardedVideo]];
                    }];

    [bridge_ registerHandler:k__loadRewardedVideo
                    callback:^(NSString* message) {
                        NSString* adId = message;
                        [self loadRewardedVideo:adId];
                        return @"";
                    }];

    [bridge_ registerHandler:k__showRewardedVideo
                    callback:^(NSString* message) {
                        [self showRewardedVideo];
                        return @"";
                    }];
}

- (void)deregisterHandlers {
    [bridge_ deregisterHandler:k__initialize];
    [bridge_ deregisterHandler:k__addTestDevice];
    [bridge_ deregisterHandler:k__createBannerAd];
    [bridge_ deregisterHandler:k__destroyBannerAd];
    [bridge_ deregisterHandler:k__createInterstitialAd];
    [bridge_ deregisterHandler:k__destroyInterstitialAd];
    [bridge_ deregisterHandler:k__hasRewardedVideo];
    [bridge_ deregisterHandler:k__loadRewardedVideo];
    [bridge_ deregisterHandler:k__showRewardedVideo];
}

- (void)initialize:(NSString* _Nonnull)applicationId {
    [GADMobileAds configureWithApplicationID:applicationId];
}

- (NSString* _Nonnull)getEmulatorTestDeviceHash {
    return kGADSimulatorID;
}

- (void)addTestDevice:(NSString* _Nonnull)hash {
    [testDevices_ addObject:hash];
}

- (BOOL)createBannerAd:(NSString* _Nonnull)adId size:(GADAdSize)size {
    if ([bannerAds_ objectForKey:adId] != nil) {
        return NO;
    }
    EEAdMobBannerAd* ad =
        [[[EEAdMobBannerAd alloc] initWithBridge:bridge_
                                            adId:adId
                                            size:size
                                     testDevices:testDevices_] autorelease];
    [bannerAds_ setObject:ad forKey:adId];
    return YES;
}

- (BOOL)destroyBannerAd:(NSString* _Nonnull)adId {
    if ([bannerAds_ objectForKey:adId] == nil) {
        return NO;
    }
    EEAdMobBannerAd* ad = [bannerAds_ objectForKey:adId];
    [ad destroy];
    [bannerAds_ removeObjectForKey:adId];
    return YES;
}

- (BOOL)createNativeAd:(NSString* _Nonnull)adId
                  type:(GADAdLoaderAdType _Nonnull)type
                layout:(NSString* _Nonnull)layoutName {
    if ([nativeAds_ objectForKey:adId] != nil) {
        return NO;
    }
    EEAdMobNativeAd* ad =
        [[[EEAdMobNativeAd alloc] initWithBridge:bridge_
                                            adId:adId
                                           types:@[type]
                                          layout:layoutName
                                     testDevices:testDevices_] autorelease];
    [nativeAds_ setObject:ad forKey:adId];
    return YES;
}

- (BOOL)destroyNativeAd:(NSString* _Nonnull)adId {
    if ([nativeAds_ objectForKey:adId] == nil) {
        return NO;
    }
    EEAdMobNativeAd* ad = [nativeAds_ objectForKey:adId];
    [ad destroy];
    [nativeAds_ removeObjectForKey:adId];
    return YES;
}

- (BOOL)createInterstitialAd:(NSString* _Nonnull)adId {
    if ([interstitialAds_ objectForKey:adId] != nil) {
        return NO;
    }
    EEAdMobInterstitialAd* ad = [[[EEAdMobInterstitialAd alloc]
        initWithBridge:bridge_
                  adId:adId
           testDevices:testDevices_] autorelease];
    [interstitialAds_ setObject:ad forKey:adId];
    return YES;
}

- (BOOL)destroyInterstitialAd:(NSString* _Nonnull)adId {
    if ([interstitialAds_ objectForKey:adId] == nil) {
        return NO;
    }
    EEAdMobInterstitialAd* ad = [interstitialAds_ objectForKey:adId];
    [ad destroy];
    [interstitialAds_ removeObjectForKey:adId];
    return YES;
}

- (bool)hasRewardedVideo {
    return [[GADRewardBasedVideoAd sharedInstance] isReady];
}

- (void)loadRewardedVideo:(NSString* _Nonnull)adId {
    GADRequest* request = [GADRequest request];
    [[GADRewardBasedVideoAd sharedInstance] loadRequest:request
                                           withAdUnitID:adId];
}

- (void)showRewardedVideo {
    UIViewController* rootView = [EEUtils getCurrentRootViewController];
    [[GADRewardBasedVideoAd sharedInstance]
        presentFromRootViewController:rootView];
}

- (void)rewardBasedVideoAd:(GADRewardBasedVideoAd*)rewardBasedVideoAd
    didRewardUserWithReward:(GADAdReward*)reward {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    [bridge_ callCpp:k__onRewarded];
}

- (void)rewardBasedVideoAd:(GADRewardBasedVideoAd*)rewardBasedVideoAd
    didFailToLoadWithError:(NSError*)error {
    NSLog(@"%s: %@", __PRETTY_FUNCTION__, [error description]);
    [bridge_ callCpp:k__onFailedToLoad message:[error description]];
}

- (void)rewardBasedVideoAdDidReceiveAd:
    (GADRewardBasedVideoAd*)rewardBasedVideoAd {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    [bridge_ callCpp:k__onLoaded];
}

- (void)rewardBasedVideoAdDidOpen:(GADRewardBasedVideoAd*)rewardBasedVideoAd {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    [bridge_ callCpp:k__onOpened];
}

- (void)rewardBasedVideoAdDidStartPlaying:
    (GADRewardBasedVideoAd*)rewardBasedVideoAd {
    NSLog(@"%s", __PRETTY_FUNCTION__);
}

- (void)rewardBasedVideoAdDidClose:(GADRewardBasedVideoAd*)rewardBasedVideoAd {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    [bridge_ callCpp:k__onClosed];
}

- (void)rewardBasedVideoAdWillLeaveApplication:
    (GADRewardBasedVideoAd*)rewardBasedVideoAd {
    NSLog(@"%s", __PRETTY_FUNCTION__);
}

@end
