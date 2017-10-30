//
//  FacebookInterstitialAd.cpp
//  ee_x
//
//  Created by Zinge on 10/11/17.
//
//

#include <cassert>

#include "ee/ads/internal/MediationManager.hpp"
#include "ee/core/Utils.hpp"
#include "ee/core/internal/MessageBridge.hpp"
#include "ee/facebook/FacebookAdsBridge.hpp"
#include "ee/facebook/internal/FacebookInterstitialAd.hpp"

namespace ee {
namespace facebook {
using Self = InterstitialAd;

namespace {
auto k__createInternalAd(const std::string& id) {
    return "FacebookInterstitialAd_createInternalAd_" + id;
}

auto k__destroyInternalAd(const std::string& id) {
    return "FacebookInterstitialAd_destroyInternalAd_" + id;
}

auto k__isLoaded(const std::string& id) {
    return "FacebookInterstitialAd_isLoaded_" + id;
}

auto k__load(const std::string& id) {
    return "FacebookInterstitialAd_load_" + id;
}

auto k__show(const std::string& id) {
    return "FacebookInterstitialAd_show_" + id;
}

auto k__onLoaded(const std::string& id) {
    return "FacebookInterstitialAd_onLoaded_" + id;
}

auto k__onFailedToLoad(const std::string& id) {
    return "FacebookInterstitialAd_onFailedToLoad_" + id;
}

auto k__onClosed(const std::string& id) {
    return "FacebookInterstitialAd_onClosed_" + id;
}
} // namespace

Self::InterstitialAd(FacebookAds* plugin, const std::string& placementId) {
    loading_ = false;
    displaying_ = false;
    plugin_ = plugin;
    placementId_ = placementId;

    auto&& bridge = core::MessageBridge::getInstance();
    bridge.registerHandler(
        [this](const std::string& message) {
            onLoaded();
            return "";
        },
        k__onLoaded(placementId_));
    bridge.registerHandler(
        [this](const std::string& message) {
            onFailedToLoad(message);
            return "";
        },
        k__onFailedToLoad(placementId_));
    bridge.registerHandler(
        [this](const std::string& message) {
            onClosed();
            return "";
        },
        k__onClosed(placementId_));

    createInternalAd();
}

Self::~InterstitialAd() {
    destroyInternalAd();

    auto&& bridge = core::MessageBridge::getInstance();
    bridge.deregisterHandler(k__onLoaded(placementId_));
    bridge.deregisterHandler(k__onFailedToLoad(placementId_));
    bridge.deregisterHandler(k__onClosed(placementId_));
    plugin_->destroyInterstitialAd(placementId_);
}

bool Self::createInternalAd() {
    auto&& bridge = core::MessageBridge::getInstance();
    auto response = bridge.call(k__createInternalAd(placementId_));
    return core::toBool(response);
}

bool Self::destroyInternalAd() {
    auto&& bridge = core::MessageBridge::getInstance();
    auto response = bridge.call(k__destroyInternalAd(placementId_));
    return core::toBool(response);
}

bool Self::isLoaded() const {
    auto&& bridge = core::MessageBridge::getInstance();
    auto response = bridge.call(k__isLoaded(placementId_));
    return core::toBool(response);
}

void Self::load() {
    if (isLoaded()) {
        return;
    }
    if (loading_) {
        return;
    }
    loading_ = true;
    auto&& bridge = core::MessageBridge::getInstance();
    bridge.call(k__load(placementId_));
}

bool Self::show() {
    if (not isLoaded()) {
        return false;
    }
    auto&& bridge = core::MessageBridge::getInstance();
    bridge.call(k__show(placementId_));
    displaying_ = true;
    auto&& mediation = ads::MediationManager::getInstance();
    auto successful = mediation.startInterstitialAd(this);
    assert(successful);
    return true;
}

void Self::onLoaded() {
    loading_ = false;
}

void Self::onFailedToLoad(const std::string& message) {
    loading_ = false;
}

void Self::onClosed() {
    auto&& mediation = ads::MediationManager::getInstance();
    if (displaying_) {
        displaying_ = false;
        destroyInternalAd();
        createInternalAd();
        setDone();
        auto successful = mediation.finishInterstitialAd(this);
        assert(successful);
    } else {
        auto successful = mediation.setInterstitialAdDone();
        assert(successful);
    }
}
} // namespace facebook
} // namespace ee
