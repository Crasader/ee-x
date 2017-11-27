//
//  Metrics.cpp
//  ee_x
//
//  Created by Zinge on 10/6/17.
//
//

#include <cassert>

#include "ee/core/MessageBridge.hpp"
#include "ee/core/Metrics.hpp"

namespace ee {
namespace core {
using Self = Metrics;

namespace {
bool initialized_ = false;
float ratio_;
} // namespace

namespace {
constexpr auto k__getDensity = "Metrics_getDensity";
} // namespace

namespace {
/// Pixel / DP = ratio.
float getPixelToDpRatio() {
    return Self::getDensity();
}

/// Pixel / Point = ratio.
float getPixelToPointRatio() {
    assert(initialized_);
    return ratio_;
}
} // namespace

void Self::initialize(float ratio) {
    initialized_ = true;
    ratio_ = ratio;
}

float Self::getDensity() {
    auto&& bridge = MessageBridge::getInstance();
    auto response = bridge.call(k__getDensity);
    return std::stof(response);
}

Self Self::fromPoint(float value) {
    auto pixelToDp = getPixelToDpRatio();
    auto pixelToPoint = getPixelToPointRatio();
    return Self(value * pixelToPoint, pixelToDp, pixelToPoint);
}

Self Self::fromPixel(float value) {
    auto pixelToDp = getPixelToDpRatio();
    auto pixelToPoint = getPixelToPointRatio();
    return Self(value, pixelToDp, pixelToPoint);
}

Self Self::fromDip(float value) {
    auto pixelToDp = getPixelToDpRatio();
    auto pixelToPoint = getPixelToPointRatio();
    return Self(value * pixelToDp, pixelToDp, pixelToPoint);
}

Self::Metrics(float pixels, float pixelToDp, float pixelToPoint)
    : pixels_(pixels)
    , pixelToDp_(pixelToDp)
    , pixelToPoint_(pixelToPoint) {}

float Self::toPoint() const {
    return pixels_ / pixelToPoint_;
}

float Self::toPixel() const {
    return pixels_;
}

float Self::toDip() const {
    return pixels_ / pixelToDp_;
}
} // namespace core
} // namespace ee
