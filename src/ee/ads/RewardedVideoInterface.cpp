//
//  RewardedVideoInterface.cpp
//  ee_x
//
//  Created by Zinge on 10/5/17.
//
//

#include "RewardedVideoInterface.hpp"

namespace ee {
namespace ads {
using Self = RewardedVideoInterface;

Self::RewardedVideoInterface() {
    callback_ = nullptr;
}

Self::~RewardedVideoInterface() {}

void Self::setRewardedVideoCallback(const RewardedVideoCallback& callback) {
    callback_ = callback;
}

void Self::invokeRewardedVideoCallback(RewardedVideoResult result,
                                       const std::string& placementId) {
    if (callback_) {
        callback_(result, placementId);
    }
}
} // namespace ads
} // namespace ee
