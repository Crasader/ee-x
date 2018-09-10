//
// jsb_vungle_bridge.cpp
// ee_x
//
//  Created by Nguyen Van Quynh on 9/4/18
//
//

#include "ee/Vungle.hpp"
#include "ee/jsb/core/jsb_templates.hpp"
#include "jsb_core_common.hpp"
#include "jsb_logger.hpp"

namespace ee {
namespace core {} // namespace core

namespace vungle {

static se::Object* __jsb_Vungle_proto = nullptr;
static se::Class* __jsb_Vungle_class = nullptr;

static se::Object* __vungleObj = nullptr;

const auto jsb_Vungle_finalize = &core::jsb_finalize<Vungle>;
const auto jsb_Vungle_constructor = &core::jsb_constructor<Vungle>;
const auto jsb_Vungle_initialize = &core::jsb_method_call_on_ui_thread_and_wait<
    Vungle, &Vungle::initialize, const std::string&, const std::string&>;
const auto jsb_Vungle_createRewardedVideo =
    &core::jsb_method_get_on_ui_thread<Vungle, &Vungle::createRewardedVideo,
                                       std::shared_ptr<IRewardedVideo>,
                                       const std::string&>;

SE_BIND_FINALIZE_FUNC(jsb_Vungle_finalize)
SE_BIND_CTOR(jsb_Vungle_constructor, __jsb_Vungle_class, jsb_Vungle_finalize)
SE_BIND_FUNC(jsb_Vungle_initialize)
SE_BIND_FUNC(jsb_Vungle_createRewardedVideo)

bool register_vungle_bridge_manual(se::Object* globalObj) {
    core::getOrCreatePlainObject_r("vungle", core::__eeObj, &__vungleObj);

    auto cls = se::Class::create("Vungle", __vungleObj, nullptr,
                                 _SE(jsb_Vungle_constructor));
    cls->defineFinalizeFunction(_SE(jsb_Vungle_finalize));

    cls->defineFunction("initialize", _SE(jsb_Vungle_initialize));
    cls->defineFunction("createRewardedVideo",
                        _SE(jsb_Vungle_createRewardedVideo));

    cls->install();

    JSBClassType::registerClass<ee::Vungle>(cls);

    __jsb_Vungle_proto = cls->getProto();
    __jsb_Vungle_class = cls;

    se::ScriptEngine::getInstance()->clearException();
    return true;
}

} // namespace vungle
} // namespace ee
