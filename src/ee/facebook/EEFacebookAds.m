//
//  EEFacebookAds.m
//  ee_x
//
//  Created by Pham Xuan Han on 5/12/17.
//
//

#import <FBAudienceNetwork/FBAudienceNetwork.h>

#import "ee/core/internal/EEDictionaryUtils.h"
#import "ee/core/internal/EEJsonUtils.h"
#import "ee/core/internal/EEMessageBridge.h"
#import "ee/core/internal/EEUtils.h"
#import "ee/facebook/EEFacebookAds.h"
#import "ee/facebook/internal/EEFacebookBannerAd.h"
#import "ee/facebook/internal/EEFacebookInterstitialAd.h"
#import "ee/facebook/internal/EEFacebookNativeAd.h"

@interface EEFacebookAds () {
    NSMutableDictionary<NSString*, EEFacebookBannerAd*>* bannerAds_;
    NSMutableDictionary<NSString*, EEFacebookNativeAd*>* nativeAds_;
    NSMutableDictionary<NSString*, EEFacebookInterstitialAd*>* interstitialAds_;
}

@end

@implementation EEFacebookAds

// clang-format off
static NSString* const k__getTestDeviceHash     = @"FacebookAds_getTestDeviceHash";
static NSString* const k__addTestDevice         = @"FacebookAds_addTestDevice";
static NSString* const k__clearTestDevices      = @"FacebookAds_clearTestDevices";

static NSString* const k__createBannerAd        = @"FacebookAds_createBannerAd";
static NSString* const k__destroyBannerAd       = @"FacebookAds_destroyBannerAd";

static NSString* const k__createNativeAd        = @"FacebookAds_createNativeAd";
static NSString* const k__destroyNativeAd       = @"FacebookAds_destroyNativeAd";

static NSString* const k__createInterstitialAd  = @"FacebookAds_createInterstitialAd";
static NSString* const k__destroyInterstitialAd = @"FacebookAds_destroyInterstitialAd";
// clang-format on

- (id)init {
    self = [super init];
    if (self == nil) {
        return self;
    }
    bannerAds_ = [[NSMutableDictionary alloc] init];
    nativeAds_ = [[NSMutableDictionary alloc] init];
    interstitialAds_ = [[NSMutableDictionary alloc] init];
    [self registerHandlers];
    return self;
}

- (void)dealloc {
    [self deregisterHandlers];
    [bannerAds_ release];
    bannerAds_ = nil;
    [nativeAds_ release];
    nativeAds_ = nil;
    [interstitialAds_ release];
    interstitialAds_ = nil;
    [super dealloc];
}

- (void)registerHandlers {
    EEMessageBridge* bridge = [EEMessageBridge getInstance];

    [bridge registerHandler:k__getTestDeviceHash
                   callback:^(NSString* message) {
                       return [self getTestDeviceHash];
                   }];

    [bridge registerHandler:k__addTestDevice
                   callback:^(NSString* message) {
                       NSString* hash = message;
                       [self addTestDevice:hash];
                       return @"";
                   }];

    [bridge registerHandler:k__clearTestDevices
                   callback:^(NSString* message) {
                       [self clearTestDevices];
                       return @"";
                   }];

    [bridge registerHandler:k__createBannerAd
                   callback:^(NSString* message) {
                       NSDictionary* dict =
                           [EEJsonUtils convertStringToDictionary:message];
                       NSString* adId = dict[@"adId"];
                       int adSizeIndex = [dict[@"adSize"] intValue];
                       FBAdSize adSize =
                           [EEFacebookBannerAd adSizeFor:adSizeIndex];
                       return [self createBannerAd:adId size:adSize] ? @"true"
                                                                     : @"false";
                   }];

    [bridge registerHandler:k__destroyBannerAd
                   callback:^(NSString* message) {
                       NSString* adId = message;
                       return [self destroyBannerAd:adId] ? @"true" : @"false";
                   }];

    [bridge registerHandler:k__createNativeAd
                   callback:^(NSString* message) {
                       NSDictionary* dict =
                           [EEJsonUtils convertStringToDictionary:message];
                       NSString* adId = dict[@"adId"];
                       NSString* layoutName = dict[@"layoutName"];
                       return [self createNativeAd:adId layout:layoutName]
                                  ? @"true"
                                  : @"false";
                   }];

    [bridge registerHandler:k__destroyNativeAd
                   callback:^(NSString* message) {
                       NSString* adId = message;
                       return [self destroyNativeAd:adId] ? @"true" : @"false";
                   }];

    [bridge registerHandler:k__createInterstitialAd
                   callback:^(NSString* message) {
                       NSString* placementId = message;
                       return [self createInterstitialAd:placementId]
                                  ? @"true"
                                  : @"false";
                   }];

    [bridge registerHandler:k__destroyInterstitialAd
                   callback:^(NSString* message) {
                       NSString* placementId = message;
                       return [self destroyInterstitialAd:placementId]
                                  ? @"true"
                                  : @"false";
                   }];
}

- (void)deregisterHandlers {
    EEMessageBridge* bridge = [EEMessageBridge getInstance];

    [bridge deregisterHandler:k__getTestDeviceHash];
    [bridge deregisterHandler:k__addTestDevice];
    [bridge deregisterHandler:k__clearTestDevices];

    [bridge deregisterHandler:k__createBannerAd];
    [bridge deregisterHandler:k__destroyBannerAd];

    [bridge deregisterHandler:k__createNativeAd];
    [bridge deregisterHandler:k__destroyNativeAd];

    [bridge deregisterHandler:k__createInterstitialAd];
    [bridge deregisterHandler:k__destroyInterstitialAd];
}

- (NSString* _Nonnull)getTestDeviceHash {
    return [FBAdSettings testDeviceHash];
}

- (void)addTestDevice:(NSString* _Nonnull)hash {
    [FBAdSettings addTestDevice:hash];
}

- (void)clearTestDevices {
    [FBAdSettings clearTestDevices];
}

- (BOOL)createBannerAd:(NSString* _Nonnull)adId size:(FBAdSize)size {
    if ([bannerAds_ objectForKey:adId] != nil) {
        return NO;
    }
    EEFacebookBannerAd* ad =
        [[[EEFacebookBannerAd alloc] initWithAdId:adId size:size] autorelease];
    [bannerAds_ setObject:ad forKey:adId];
    return YES;
}

- (BOOL)destroyBannerAd:(NSString* _Nonnull)adId {
    if ([bannerAds_ objectForKey:adId] == nil) {
        return NO;
    }
    [bannerAds_ removeObjectForKey:adId];
    return YES;
}

- (BOOL)createNativeAd:(NSString*)adId layout:(NSString*)layout {
    if ([nativeAds_ objectForKey:adId] != nil) {
        return NO;
    }
    EEFacebookNativeAd* ad =
        [[[EEFacebookNativeAd alloc] initWithAdId:adId layout:layout]
            autorelease];
    [nativeAds_ setObject:ad forKey:adId];
    return YES;
}

- (BOOL)destroyNativeAd:(NSString*)adId {
    if ([nativeAds_ objectForKey:adId] == nil) {
        return NO;
    }
    [nativeAds_ removeObjectForKey:adId];
    return YES;
}

- (BOOL)createInterstitialAd:(NSString* _Nonnull)placementId {
    if ([interstitialAds_ objectForKey:placementId] != nil) {
        return NO;
    }
    EEFacebookInterstitialAd* ad = [[[EEFacebookInterstitialAd alloc]
        initWithPlacementId:placementId] autorelease];
    [interstitialAds_ setObject:ad forKey:placementId];
    return YES;
}

- (BOOL)destroyInterstitialAd:(NSString* _Nonnull)placementId {
    if ([interstitialAds_ objectForKey:placementId] == nil) {
        return NO;
    }
    [interstitialAds_ removeObjectForKey:placementId];
    return YES;
}

@end
