//
//  EEFacebookInterstitialAd.hpp
//  ee_x
//
//  Created by Zinge on 10/11/17.
//
//

#import <ee/ads/EEIInterstitialAd.h>

@protocol EEIMessageBridge;

@interface EEFacebookInterstitialAd : NSObject <EEIInterstitialAd>

- (id _Nonnull)initWithBridge:(id<EEIMessageBridge> _Nonnull)bridge
                  placementId:(NSString* _Nonnull)placementId;

- (void)destroy;

@end
