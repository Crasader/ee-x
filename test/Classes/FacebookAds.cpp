//
//  FacebookAds.cpp
//  ee_x_test
//
//  Created by Zinge on 10/16/17.
//
//

#include "FacebookAds.hpp"
#include "Utils.hpp"

#include <ee/Core.hpp>

#include <base/CCDirector.h>
#include <platform/CCGLView.h>

namespace eetest {
ee::FacebookAds* getFacebookAds() {
    static auto plugin = ee::FacebookAds();
    static bool initialized = false;
    if (not initialized) {
        ee::runOnUiThreadAndWait([] {
            plugin.clearTestDevices();
            plugin.addTestDevice(plugin.getTestDeviceHash());
            plugin.addTestDevice(
                "ad45c323f6a9b07f7a9c072549efb279"); // BlueStacks.
        });
        initialized = true;
    }
    return &plugin;
}

std::string getFacebookInterstitialAdId() {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    return "869337403086643_1447442535276124";
#else  // CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    return "869337403086643_1447441308609580";
#endif // CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
}

std::shared_ptr<ee::IAdView> createFacebookNativeAd() {
    return getFacebookAds()->createNativeAd(
        "869337403086643_1444948412192203", "fb_native_spin",
        ee::FacebookNativeAdLayout()
            .setAdChoices("ad_choices_container")
            .setBody("native_ad_body")
            .setCallToAction("native_ad_call_to_action")
            .setIcon("native_ad_icon")
            .setMedia("native_ad_media")
            .setSocialContext("native_ad_social_context")
            .setTitle("native_ad_title"));
}

std::shared_ptr<ee::IInterstitialAd> createFacebookInterstitialAd() {
    return getFacebookAds()->createInterstitialAd(
        getFacebookInterstitialAdId());
}

void testFacebookNativeAd() {
    auto&& frameSize =
        cocos2d::Director::getInstance()->getOpenGLView()->getFrameSize();
    int screenWidth = static_cast<int>(frameSize.width);
    int screenHeight = static_cast<int>(frameSize.height);

    auto nativeAd =
        ee::runOnUiThreadAndWaitResult<std::shared_ptr<ee::IAdView>>([] {
            getLogger().info("Create Facebook native ad");
            auto ad = createFacebookNativeAd();
            ad->setVisible(true);
            return ad;
        });

    float delay = 0.0f;
    scheduleForever(delay += 1.0f, 5.0f, [nativeAd] {
        ee::runOnUiThread([nativeAd] { nativeAd->load(); });
    });

    scheduleForever(delay += 1.0f, 8.0f, [screenWidth, screenHeight, nativeAd] {
        ee::runOnUiThread([screenWidth, screenHeight, nativeAd] {
            getLogger().info("Resize = screen size / 4");
            nativeAd->setAnchor(0.5f, 0.5f);
            nativeAd->setPosition(screenWidth / 2, screenHeight / 2);
            nativeAd->setSize(screenWidth / 4, screenHeight / 4);
        });
    });

    scheduleForever(delay += 1.0f, 8.0f, [nativeAd] {
        ee::runOnUiThread([nativeAd] {
            getLogger().info("Move to top-left");
            nativeAd->setAnchor(0, 0);
            nativeAd->setPosition(0, 0);
        });
    });

    scheduleForever(delay += 1.0f, 8.0f, [screenWidth, nativeAd] {
        ee::runOnUiThread([screenWidth, nativeAd] {
            getLogger().info("Move to top-right");
            nativeAd->setAnchor(1, 0);
            nativeAd->setPosition(screenWidth, 0);
        });
    });

    scheduleForever(delay += 1.0f, 8.0f, [screenWidth, screenHeight, nativeAd] {
        ee::runOnUiThread([screenWidth, screenHeight, nativeAd] {
            getLogger().info("Move to bottom-right");
            nativeAd->setAnchor(1, 1);
            nativeAd->setPosition(screenWidth, screenHeight);
        });
    });

    scheduleForever(delay += 1.0f, 8.0f, [screenHeight, nativeAd] {
        ee::runOnUiThread([screenHeight, nativeAd] {
            getLogger().info("Move to bottom-left");
            nativeAd->setAnchor(0, 1);
            nativeAd->setPosition(0, screenHeight);
        });
    });

    scheduleForever(delay += 1.0f, 8.0f, [screenWidth, screenHeight, nativeAd] {
        ee::runOnUiThread([screenWidth, screenHeight, nativeAd] {
            getLogger().info("Move to center");
            nativeAd->setAnchor(0.5f, 0.5f);
            nativeAd->setPosition(screenWidth / 2, screenHeight / 2);
        });
    });

    scheduleForever(delay += 1.0f, 8.0f, [screenWidth, screenHeight, nativeAd] {
        ee::runOnUiThread([screenWidth, screenHeight, nativeAd] {
            getLogger().info("Resize = screen size");
            nativeAd->setSize(screenWidth, screenHeight);
        });
    });

    scheduleForever(delay += 1.0f, 8.0f, [screenWidth, screenHeight, nativeAd] {
        ee::runOnUiThread([screenWidth, screenHeight, nativeAd] {
            getLogger().info("Resize = screen size / 2");
            nativeAd->setSize(screenWidth / 2, screenHeight / 2);
        });
    });
}

void testFacebookInterstitialAd() {
    auto interstitialAd = createFacebookInterstitialAd();
    scheduleForever(1.0f, 3.0f, [interstitialAd] {
        logCurrentThread();
        ee::runOnUiThread([interstitialAd] {
            logCurrentThread();
            interstitialAd->load();
        });
    });
    scheduleForever(2.0f, 3.0f, [interstitialAd] {
        logCurrentThread();
        ee::runOnUiThread([interstitialAd] {
            logCurrentThread();
            interstitialAd->show();
        });
    });
}
} // namespace eetest
