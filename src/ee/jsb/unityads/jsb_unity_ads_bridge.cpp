//
// jsb_unity_ads_bridge.cpp
// ee_x
//
//  Created by Nguyen Van Quynh on 8/23/18
//
//

#include "ee/UnityAds.hpp"
#include "ee/jsb/core/jsb_templates.hpp"
#include "jsb_core_common.hpp"
#include "jsb_logger.hpp"

namespace ee {
namespace core {} // namespace core

namespace unityads {

static se::Object* __jsb_UnityAds_proto = nullptr;
static se::Class* __jsb_UnityAds_class = nullptr;

static se::Object* __unityadsObj = nullptr;

const auto jsb_UnityAds_finalize = &core::jsb_finalize<UnityAds>;
const auto jsb_UnityAds_constructor = &core::jsb_constructor<UnityAds>;
const auto jsb_UnityAds_initialize =
    &core::jsb_method_call_on_ui_thread_and_wait<
        UnityAds, &UnityAds::initialize, const std::string&, bool>;
const auto jsb_UnityAds_setDebugModeEnabled =
    &core::jsb_method_call_on_ui_thread<UnityAds,
                                        &UnityAds::setDebugModeEnabled, bool>;
const auto jsb_UnityAds_createRewardedVideo =
    &core::jsb_method_get_on_ui_thread<UnityAds, &UnityAds::createRewardedVideo,
                                       std::shared_ptr<IRewardedVideo>,
                                       const std::string&>;
const auto jsb_UnityAds_createInterstitialAd =
    &core::jsb_method_get_on_ui_thread<
        UnityAds, &UnityAds::createInterstitialAd,
        std::shared_ptr<IInterstitialAd>, const std::string&>;
    
SE_BIND_FINALIZE_FUNC(jsb_UnityAds_finalize)
SE_BIND_CTOR(jsb_UnityAds_constructor, __jsb_UnityAds_class, jsb_UnityAds_finalize)
SE_BIND_FUNC(jsb_UnityAds_initialize)
SE_BIND_FUNC(jsb_UnityAds_setDebugModeEnabled)
SE_BIND_FUNC(jsb_UnityAds_createRewardedVideo)
SE_BIND_FUNC(jsb_UnityAds_createInterstitialAd)

bool register_unity_ads_bridge_manual(se::Object* globalObj) {
    core::getOrCreatePlainObject_r("unityads", core::__eeObj, &__unityadsObj);
    
    auto cls = se::Class::create("UnityAds", __unityadsObj, nullptr, _SE(jsb_UnityAds_constructor));
    cls->defineFinalizeFunction(_SE(jsb_UnityAds_finalize));
    
    cls->defineFunction("initialize", _SE(jsb_UnityAds_initialize));
    cls->defineFunction("setDebugModeEnabled", _SE(jsb_UnityAds_setDebugModeEnabled));
    cls->defineFunction("createRewardedVideo", _SE(jsb_UnityAds_createRewardedVideo));
    cls->defineFunction("createInterstitialAd", _SE(jsb_UnityAds_createInterstitialAd));
    
    cls->install();
    
    JSBClassType::registerClass<ee::UnityAds>(cls);
    
    __jsb_UnityAds_proto = cls->getProto();
    __jsb_UnityAds_class = cls;
    
    se::ScriptEngine::getInstance()->clearException();
    return true;
}

} // namespace unityads
} // namespace ee
