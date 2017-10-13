//
//  AdMob.hpp
//  ee_x
//
//  Created by Zinge on 10/11/17.
//
//

#ifndef EE_X_ADMOB_BRIDGE_HPP
#define EE_X_ADMOB_BRIDGE_HPP

#include <map>
#include <memory>

#include "ee/admob/AdMobBannerAdSize.hpp"
#include "ee/ads/AdViewInterface.hpp"
#include "ee/ads/InterstitialAdInterface.hpp"
#include "ee/ads/RewardedVideoInterface.hpp"

namespace ee {
namespace admob {
class BannerAd;
class NativeAd;
class InterstitialAd;
class RewardedVideo;

class AdMob final {
public:
    AdMob();
    ~AdMob();

    std::shared_ptr<AdViewInterface> createBannerAd(const std::string& adId,
                                                    BannerAdSize adSize);

    std::shared_ptr<AdViewInterface> createNativeAd(const std::string& adId);

    std::shared_ptr<InterstitialAdInterface>
    createInterstitialAd(const std::string& adId);

    std::shared_ptr<RewardedVideoInterface>
    createRewardedVideo(const std::string& adId);

private:
    friend BannerAd;
    friend NativeAd;
    friend InterstitialAd;
    friend RewardedVideo;

    bool destroyBannerAd(const std::string& adId);
    bool destroyNativeAd(const std::string& adId);
    bool destroyInterstitialAd(const std::string& adId);

    bool destroyRewardedVideo(const std::string& adId);
    bool hasRewardedVideo() const;
    void loadRewardedVideo(const std::string& adId);
    bool showRewardedVideo();

    void onLoaded();
    void onFailedToLoad(const std::string& message);
    void onReward();
    void onClosed();

    bool loading_;
    bool rewarded_;
    std::string currentId_;

    std::map<std::string, RewardedVideo*> rewardedVideos_;
};
} // namespace admob
} // namespace ee

#endif /* EE_X_ADMOB_BRIDGE_HPP */
