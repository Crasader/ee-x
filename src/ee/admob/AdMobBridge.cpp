//
//  AdMob.cpp
//  ee_x
//
//  Created by Zinge on 10/11/17.
//
//

#include "ee/admob/AdMobBridge.hpp"
#include "ee/admob/internal/AdMobBannerAd.hpp"
#include "ee/admob/internal/AdMobInterstitialAd.hpp"
#include "ee/admob/internal/AdMobRewardedVideo.hpp"
#include "ee/core/Utils.hpp"
#include "ee/core/internal/MessageBridge.hpp"

#include <ee/nlohmann/json.hpp>

namespace ee {
namespace admob {
using Self = AdMob;

namespace {
// clang-format off
constexpr auto k__createBannerAd        = "AdMob_createBannerAd";
constexpr auto k__destroyBannerAd       = "AdMob_destroyBannerAd";

constexpr auto k__createNativeAd        = "AdMob_createNativeAd";
constexpr auto k__destroyNativeAd       = "AdMob_destroyNativeAd";

constexpr auto k__createInterstitialAd  = "AdMob_createInterstitialAd";
constexpr auto k__destroyInterstitialAd = "AdMob_destroyInterstitialAd";

constexpr auto k__hasRewardedVideo      = "AdMob_hasRewardedVideo";
constexpr auto k__loadRewardedVideo     = "AdMob_loadRewardedVideo";
constexpr auto k__showRewardedVideo     = "AdMob_showRewardedVideo";

constexpr auto k__onRewarded            = "AdMob_onRewarded";
constexpr auto k__onFailedToLoad        = "AdMob_onFailedToLoad";
constexpr auto k__onLoaded              = "AdMob_onLoaded";
constexpr auto k__onClosed              = "AdMob_onClosed";
// clang-format on
} // namespace

Self::AdMob() {
    loading_ = false;
    rewarded_ = false;

    auto&& bridge = core::MessageBridge::getInstance();
    bridge.registerHandler(
        [this](const std::string& message) {
            onReward();
            return "";
        },
        k__onRewarded);
    bridge.registerHandler(
        [this](const std::string& message) {
            onFailedToLoad(message);
            return "";
        },
        k__onFailedToLoad);
    bridge.registerHandler(
        [this](const std::string& message) {
            onLoaded();
            return "";
        },
        k__onLoaded);
    bridge.registerHandler(
        [this](const std::string& message) {
            onClosed();
            return "";
        },
        k__onClosed);
}

Self::~AdMob() {
    auto&& bridge = core::MessageBridge::getInstance();
    bridge.deregisterHandler(k__onRewarded);
    bridge.deregisterHandler(k__onFailedToLoad);
    bridge.deregisterHandler(k__onLoaded);
    bridge.deregisterHandler(k__onClosed);
}

std::shared_ptr<AdViewInterface> Self::createBannerAd(const std::string& adId,
                                                      BannerAdSize adSize) {
    nlohmann::json json;
    json["adId"] = adId;
    json["adSize"] = static_cast<int>(adSize);

    auto&& bridge = core::MessageBridge::getInstance();
    auto response = bridge.call(k__createBannerAd, json.dump());
    if (response == "false") {
        return nullptr;
    }
    return std::shared_ptr<AdViewInterface>(new BannerAd(this, adId));
}

bool Self::destroyBannerAd(const std::string& adId) {
    auto&& bridge = core::MessageBridge::getInstance();
    auto response = bridge.call(k__destroyBannerAd, adId);
    return response == "true";
}

std::shared_ptr<InterstitialAdInterface>
Self::createInterstitialAd(const std::string& adId) {
    auto&& bridge = core::MessageBridge::getInstance();
    auto response = bridge.call(k__createInterstitialAd, adId);
    if (response == "false") {
        return nullptr;
    }
    return std::shared_ptr<InterstitialAdInterface>(
        new InterstitialAd(this, adId));
}

bool Self::destroyInterstitialAd(const std::string& adId) {
    auto&& bridge = core::MessageBridge::getInstance();
    auto&& response = bridge.call(k__destroyInterstitialAd, adId);
    return response == "true";
}

std::shared_ptr<RewardedVideoInterface>
Self::createRewardedVideo(const std::string& adId) {
    if (rewardedVideos_.count(adId) != 0) {
        return nullptr;
    }
    auto result = new RewardedVideo(this, adId);
    rewardedVideos_[adId] = result;
    return std::shared_ptr<RewardedVideoInterface>(result);
}

bool Self::destroyRewardedVideo(const std::string& adId) {
    if (rewardedVideos_.count(adId) == 0) {
        return false;
    }
    rewardedVideos_.erase(adId);
    return true;
}

bool Self::hasRewardedVideo() const {
    auto&& bridge = core::MessageBridge::getInstance();
    auto response = bridge.call(k__hasRewardedVideo);
    return core::toBool(response);
}

void Self::loadRewardedVideo(const std::string& adId) {
    if (loading_) {
        return;
    }
    loading_ = true;
    currentId_ = adId;
    auto&& bridge = core::MessageBridge::getInstance();
    bridge.call(k__loadRewardedVideo);
}

bool Self::showRewardedVideo() {
    if (not hasRewardedVideo()) {
        return false;
    }
    rewarded_ = false;
    auto&& bridge = core::MessageBridge::getInstance();
    bridge.call(k__showRewardedVideo);
    return true;
}

void Self::onLoaded() {
    loading_ = false;
}

void Self::onFailedToLoad(const std::string& message) {
    loading_ = false;
}

void Self::onReward() {
    rewarded_ = true;
}

void Self::onClosed() {
    auto ad = rewardedVideos_.at(currentId_);
    ad->setResult(rewarded_);
}
} // namespace admob
} // namespace ee
