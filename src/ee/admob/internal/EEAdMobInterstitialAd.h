//
//  EEAdMobInterstitialAd.h
//  ee_x
//
//  Created by Zinge on 10/12/17.
//
//

#import "ee/ads/internal/EEIInterstitialAd.h"

@class EEMessageBridge;

@interface EEAdMobInterstitialAd : NSObject <EEIInterstitialAd>

- (id _Nonnull)initWithBridge:(EEMessageBridge* _Nonnull)bridge
                         adId:(NSString* _Nonnull)adId
                  testDevices:(NSArray<NSString*>* _Nullable)testDevices;

- (void)destroy;

@end
