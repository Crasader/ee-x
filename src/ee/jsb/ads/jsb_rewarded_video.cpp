//
//  jsb_rewarded_video.cpp
//  ee-x
//
//  Created by Duc Nguyen on 7/17/18.
//

#include "ee/jsb/ads/jsb_rewarded_video.hpp"

#include <unordered_map>

#include "ee/ads/IRewardedVideo.hpp"
#include "ee/jsb/core/jsb_core_common.hpp"
#include "ee/jsb/core/jsb_templates.hpp"

namespace ee {
namespace core {
namespace {
std::unique_ptr<SharedPtrHandler<IRewardedVideo>> handler;
} // namespace

template <>
std::shared_ptr<IRewardedVideo> get_value(const se::Value& value) {
    return handler->getValue(value);
}

template <>
void set_value(se::Value& value, std::shared_ptr<IRewardedVideo>& input) {
    handler->setValue(value, input);
}

template <>
bool jsb_finalize<IRewardedVideo>(se::State& state) {
    return handler->finalize(state);
}
} // namespace core

namespace ads {
namespace {
using Self = IRewardedVideo;

// clang-format off
constexpr auto finalize          = &core::makeFinalize<Self>;
constexpr auto isLoaded          = &core::makeInstanceMethodOnUiThreadAndWait<&Self::isLoaded>;
constexpr auto load              = &core::makeInstanceMethodOnUiThread<&Self::load>;
constexpr auto show              = &core::makeInstanceMethodOnUiThreadAndWait<&Self::show>;
constexpr auto setResultCallback = &core::jsb_set_callback<Self, &Self::setResultCallback, bool>;
// clang-format on

SE_BIND_FINALIZE_FUNC(finalize)
SE_BIND_FUNC(isLoaded)
SE_BIND_FUNC(load)
SE_BIND_FUNC(show)
SE_BIND_FUNC(setResultCallback);
} // namespace

se::Class* getIRewardedVideoClass() {
    CCASSERT(core::handler != nullptr, "__jsb_RewardedVideo_class is null");
    return core::handler->getClass();
}

bool register_rewarded_video_manual(se::Object* globalObj) {
    se::Object* adsObj = nullptr;
    core::getOrCreatePlainObject_r("ads", globalObj, &adsObj);

    auto cls = se::Class::create("RewardedVideo", adsObj, nullptr, nullptr);
    cls->defineFinalizeFunction(_SE(finalize));

    EE_JSB_DEFINE_FUNCTION(cls, isLoaded);
    EE_JSB_DEFINE_FUNCTION(cls, load);
    EE_JSB_DEFINE_FUNCTION(cls, show);
    EE_JSB_DEFINE_FUNCTION(cls, setResultCallback);

    cls->install();

    JSBClassType::registerClass<Self>(cls);
    core::handler = core::SharedPtrHandler<Self>::create(cls);

    se::ScriptEngine::getInstance()->clearException();
    return true;
}
} // namespace ads
} // namespace ee
