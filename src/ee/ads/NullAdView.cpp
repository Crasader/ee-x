//
//  NullAdView.cpp
//  ee_x
//
//  Created by Zinge on 10/27/17.
//
//

#include "ee/ads/NullAdView.hpp"

namespace ee {
namespace ads {
using Self = NullAdView;

Self::NullAdView() {
    positionX_ = 0;
    positionY_ = 0;
    anchorX_ = 0.0f;
    anchorY_ = 0.0f;
    width_ = 0;
    height_ = 0;
}

bool Self::isLoaded() const {
    return false;
}

void Self::load() {
    setLoadResult(false);
}

std::pair<float, float> Self::getAnchor() const {
    return std::tie(anchorX_, anchorY_);
}

void Self::setAnchor(float x, float y) {
    positionX_ = positionX_ - static_cast<int>((x - anchorX_) * width_);
    positionY_ = positionY_ - static_cast<int>((y - anchorY_) * height_);
    anchorX_ = x;
    anchorY_ = y;
}

std::pair<int, int> Self::getPosition() const {
    return std::make_pair(positionX_ + anchorX_ * width_,
                          positionY_ + anchorY_ * height_);
}

void Self::setPosition(int x, int y) {
    positionX_ = x - static_cast<int>(anchorX_ * width_);
    positionY_ = y - static_cast<int>(anchorY_ * height_);
}

std::pair<int, int> Self::getSize() const {
    return std::tie(width_, height_);
}

void Self::setSize(int width, int height) {
    positionX_ = positionX_ - static_cast<int>((width - width_) * anchorX_);
    positionY_ = positionY_ - static_cast<int>((height - height_) * anchorY_);
}

void Self::setVisible(bool visible) {
    // No op.
    static_cast<void>(visible);
}
} // namespace ads
} // namespace ee
