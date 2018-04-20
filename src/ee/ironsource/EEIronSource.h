#import <Foundation/Foundation.h>

@interface EEIronSource : NSObject

- (void)initialize:(NSString* _Nonnull)gameId;

- (BOOL)hasRewardedVideo;
- (void)showRewardedVideo:(NSString* _Nonnull)placementId;

- (BOOL)hasInterstitial;
- (void)showInterstitial:(NSString* _Nonnull)placementId;

@end
