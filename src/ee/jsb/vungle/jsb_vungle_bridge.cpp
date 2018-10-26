//
// jsb_vungle_bridge.cpp
// ee_x
//
//  Created by Nguyen Van Quynh on 9/4/18
//
//

#include "ee/jsb/vungle/jsb_vungle_bridge.hpp"

#include <ee/Vungle.hpp>

#include "ee/jsb/core/jsb_core_common.hpp"
#include "ee/jsb/core/jsb_logger.hpp"
#include "ee/jsb/core/jsb_templates.hpp"

namespace ee {
namespace vungle {
se::Class* __jsb_Vungle_class = nullptr;

const auto jsb_Vungle_finalize = &core::jsb_finalize<Vungle>;
const auto jsb_Vungle_constructor = &core::jsb_constructor<Vungle>;
const auto jsb_Vungle_createRewardedVideo =
    &core::jsb_method_get_on_ui_thread<Vungle, &Vungle::createRewardedVideo,
                                       std::shared_ptr<IRewardedVideo>,
                                       const std::string&>;

SE_BIND_FINALIZE_FUNC(jsb_Vungle_finalize)
SE_BIND_CTOR(jsb_Vungle_constructor, __jsb_Vungle_class, jsb_Vungle_finalize)
SE_BIND_FUNC(jsb_Vungle_createRewardedVideo)

bool register_vungle_bridge_manual(se::Object* globalObj) {
    se::Object* eeObj = nullptr;
    se::Object* vungleObj = nullptr;
    core::getOrCreatePlainObject_r("ee", globalObj, &eeObj);
    core::getOrCreatePlainObject_r("vungle", eeObj, &vungleObj);

    auto cls = se::Class::create("Vungle", vungleObj, nullptr,
                                 _SE(jsb_Vungle_constructor));
    cls->defineFinalizeFunction(_SE(jsb_Vungle_finalize));

    cls->defineFunction("createRewardedVideo",
                        _SE(jsb_Vungle_createRewardedVideo));

    cls->install();

    JSBClassType::registerClass<Vungle>(cls);

    __jsb_Vungle_class = cls;

    se::ScriptEngine::getInstance()->clearException();
    return true;
}

} // namespace vungle
} // namespace ee
