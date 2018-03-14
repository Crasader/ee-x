//
//  ALovinAds.hpp
//  ee_x
//
//  Created by Pham Xuan Han on 5/12/17.
//
//

#ifndef EE_X_APP_LOVIN_BRIDGE_HPP
#define EE_X_APP_LOVIN_BRIDGE_HPP

#include "ee/AppLovinFwd.hpp"
#include "ee/CoreFwd.hpp"
#include "ee/ads/IRewardedVideo.hpp"

namespace ee {
namespace applovin {
class AppLovin final {
public:
    AppLovin();
    ~AppLovin();

    void initialize(const std::string& key);

    /// Disabled by default.
    void setTestAdsEnabled(bool enabled);

    /// Disabled by default.
    void setVerboseLogging(bool enabled);

    /// Disabled by default.
    void setMuted(bool enabled);

    std::shared_ptr<IRewardedVideo> createRewardedVideo();

private:
    friend RewardedVideo;

    bool hasInterstitialAd() const;
    bool showInterstitialAd();

    bool destroyRewardedVideo();
    void loadRewardedVideo();
    bool hasRewardedVideo() const;
    bool showRewardedVideo();

    void onInterstitialAdHidden();
    void onRewardedVideoFailed(int errorCode);
    void onRewardedVideoDisplayed();
    void onRewardedVideoHidden();
    void onUserRewardVerified();

    bool verified_;
    bool errored_;
    RewardedVideo* rewardedVideo_;
    IMessageBridge& bridge_;
};
} // namespace applovin
} // namespace ee

#endif /* EE_X_APP_LOVIN_BRIDGE_HPP */
