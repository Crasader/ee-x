#include "ee/vungle/VungleBridge.hpp"
#include "ee/ads/NullRewardedVideo.hpp"
#include "ee/ads/internal/MediationManager.hpp"
#include "ee/core/Logger.hpp"
#include "ee/core/Utils.hpp"
#include "ee/core/internal/MessageBridge.hpp"
#include "ee/vungle/internal/VungleRewardedVideo.hpp"

#include <ee/nlohmann/json.hpp>

namespace ee {
namespace vungle {
using Self = Vungle;

namespace {
// clang-format off
constexpr auto k__initialize        = "Vungle_initialize";
constexpr auto k__hasRewardedVideo  = "Vungle_hasRewardedVideo";
constexpr auto k__showRewardedVideo = "Vungle_showRewardedVideo";
constexpr auto k__onStart           = "Vungle_onStart";
constexpr auto k__onEnd             = "Vungle_onEnd";
constexpr auto k__onUnavailable     = "Vungle_onUnavailable";
// clang-format on
} // namespace

Self::Vungle() {
    Logger::getSystemLogger().debug(__PRETTY_FUNCTION__);
    errored_ = false;
    
    auto&& bridge = core::MessageBridge::getInstance();
    bridge.registerHandler(
        [this](const std::string& message) {
            onStart();
            return "";
        },
        k__onStart);
    bridge.registerHandler(
        [this](const std::string& message) {
            onEnd(core::toBool(message));
            return "";
        },
        k__onEnd);
    bridge.registerHandler(
        [this](const std::string& message) {
            onUnavailable();
            return "";
        },
        k__onUnavailable);
}

Self::~Vungle() {
    Logger::getSystemLogger().debug(__PRETTY_FUNCTION__);
    auto&& bridge = core::MessageBridge::getInstance();
    bridge.deregisterHandler(k__onStart);
    bridge.deregisterHandler(k__onEnd);
    bridge.deregisterHandler(k__onUnavailable);
}

void Self::initialize(const std::string& gameId, const std::string& placementId) {
    Logger::getSystemLogger().debug("%s: gameId = %s placementId = %s", __PRETTY_FUNCTION__,
                                    gameId.c_str(), placementId.c_str());
    
    nlohmann::json json;
    json["gameId"] = gameId;
    json["placementId"] = placementId;
    
    auto&& bridge = core::MessageBridge::getInstance();
    bridge.call(k__initialize, json.dump());
}

std::shared_ptr<RewardedVideoInterface> Self::createRewardedVideo(const std::string& placementId) {    
    Logger::getSystemLogger().debug("%s: placementId = %s", __PRETTY_FUNCTION__,
                                    placementId.c_str());
    if (rewardedVideos_.count(placementId) != 0) {
        return std::make_shared<NullRewardedVideo>();
    }
    auto result = new RewardedVideo(this, placementId);
    rewardedVideos_[placementId] = result;
    return std::shared_ptr<RewardedVideoInterface>(result);
}

bool Self::destroyRewardedVideo(const std::string& placementId) {
    Logger::getSystemLogger().debug("%s: placementId = %s", __PRETTY_FUNCTION__,
                                    placementId.c_str());
    if (rewardedVideos_.count(placementId) == 0) {
        return false;
    }
    rewardedVideos_.erase(placementId);
    return true;
}

bool Self::hasRewardedVideo(const std::string& placementId) const {
    auto&& bridge = core::MessageBridge::getInstance();
    auto result = bridge.call(k__hasRewardedVideo, placementId);
    return core::toBool(result);
}

bool Self::showRewardedVideo(const std::string& placementId) {
    if (not hasRewardedVideo(placementId)) {
        return false;
    }
    Logger::getSystemLogger().debug("%s: placementId = %s", __PRETTY_FUNCTION__,
                                    placementId.c_str());
    errored_ = false;
    auto&& bridge = core::MessageBridge::getInstance();
    auto response = bridge.call(k__showRewardedVideo, placementId);
    return not errored_ && core::toBool(response);
}

void Self::onStart() {
    Logger::getSystemLogger().debug(__PRETTY_FUNCTION__);
}

void Self::onEnd(bool wasSuccessfulView) {
    Logger::getSystemLogger().debug("%s: %s", __PRETTY_FUNCTION__,
                                    core::toString(wasSuccessfulView).c_str());
    auto&& mediation = ads::MediationManager::getInstance();
    auto successful = mediation.setRewardedVideoResult(wasSuccessfulView);
    assert(successful);

    /*
     Don't care which ad is displaying.
    if (rewardedVideo_ == nullptr) {
        // Other mediation network.

    } else {
        assert(not errored_);
        rewardedVideo_->setResult(wasSuccessfulView);
        auto successful = mediation.finishRewardedVideo(rewardedVideo_);
        assert(successful);
    }
     */
}

void Self::onUnavailable() {
    Logger::getSystemLogger().debug(__PRETTY_FUNCTION__);
    if (not errored_) {
        errored_ = true;
    }
}
} // namespace vungle
} // namespace ee
