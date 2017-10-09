//
//  FacebookAds.hpp
//  ee_x
//
//  Created by Pham Xuan Han on 5/12/17.
//
//

#ifndef EE_X_FACEBOOK_ADS_BRIDGE_HPP_
#define EE_X_FACEBOOK_ADS_BRIDGE_HPP_

#include <functional>
#include <memory>
#include <string>

namespace ee {
namespace facebook {
class FacebookBannerAd;
class FacebookNativeAd;
class FacebookNativeAdBuilder;

enum class FacebookBannerAdSize {

};

class FacebookAds final {
public:
    using AdCallback = std::function<void(const std::string& message)>;

    FacebookAds();
    ~FacebookAds();

    std::shared_ptr<FacebookBannerAd>
    createBannerAd(const std::string& adId, FacebookBannerAdSize adSize);

    std::shared_ptr<FacebookNativeAd>
    createNativeAd(const FacebookNativeAdBuilder& builder);

    void cacheInterstitialAd(const std::string& adId) const;
    bool showInterstitialAd() const;

private:
    friend FacebookBannerAd;
    friend FacebookNativeAd;

    bool destroyBannerAd(const std::string& adId);
    bool destroyNativead(const std::string& adId);
};
} // namespace facebook
} // namespace ee

#endif /* EE_X_FACEBOOK_ADS_BRIDGE_HPP_ */
