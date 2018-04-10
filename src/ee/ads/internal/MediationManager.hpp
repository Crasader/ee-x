//
//  MediationManager.hpp
//  ee_x
//
//  Created by Zinge on 10/23/17.
//
//

#ifndef EE_X_ADS_MEDIATION_MANAGER_HPP
#define EE_X_ADS_MEDIATION_MANAGER_HPP

#include "ee/AdsFwd.hpp"
#include "ee/core/internal/SpinLock.hpp"
#include <functional>
namespace ee {
namespace ads {
/// Fix issue where AdMob consumes other ads' callbacks.
    using OnCloseCallback = std::function<void()>;
class MediationManager {
private:
    using Self = MediationManager;

public:
    static Self& getInstance();

    bool startInterstitialAd(const OnCloseCallback& callback);
    bool finishInterstitialAd();
    bool destroyInterstitialAd();
    bool setInterstitialAdDone();

    bool startRewardedVideo(IRewardedVideo* ad);
    bool finishRewardedVideo(IRewardedVideo* ad);
    bool destroyRewardedVideo(IRewardedVideo* ad);
    bool setRewardedVideoResult(bool result);

protected:
    MediationManager();
    ~MediationManager();

    MediationManager(const Self&) = delete;
    Self& operator=(const Self&) = delete;

    bool registerInterstitialAd(const OnCloseCallback& callback);
    bool deregisterInterstitialAd(bool destroyed);

    bool registerRewardedVideo(IRewardedVideo* ad);
    bool deregisterRewardedVideo(IRewardedVideo* ad, bool destroyed);

private:
    OnCloseCallback _onCloseCallback;
    bool interstitialAdDestroyed_;
    IRewardedVideo* rewardedVideo_;
    bool rewardedVideoDestroyed_;
    core::SpinLock locker_;
};
} // namespace ads
} // namespace ee

#endif /* EE_X_ADS_MEDIATION_MANAGER_HPP */
