//
//  jsb_multi_adview.cpp
//  ee-x
//
//  Created by Kiet Le on 9/05/18.
//

#include "ee/jsb/ads/jsb_multi_adview.hpp"

#include <ee/Ads.hpp>

#include "ee/jsb/core/jsb_core_common.hpp"
#include "ee/jsb/core/jsb_templates.hpp"

namespace ee {
namespace ads {

se::Object* __jsb_Multi_AdView_proto = nullptr;
se::Class* __jsb_Multi_AdView_class = nullptr;

constexpr auto jsb_Multi_AdView_finalize = &core::jsb_finalize<MultiAdView>;
constexpr auto jsb_Multi_AdView_contructor =
    &core::jsb_constructor<MultiAdView>;
constexpr auto jsb_Multi_AdView_isLoaded =
    &core::jsb_accessor_get_on_ui_thread<MultiAdView, &MultiAdView::isLoaded,
                                         bool>;
constexpr auto jsb_Multi_AdView_load =
    &core::jsb_method_call_on_ui_thread<MultiAdView, &MultiAdView::load>;
constexpr auto jsb_Multi_AdView_addItem =
    &core::jsb_method_call_on_ui_thread<MultiAdView, &MultiAdView::addItem,
                                        std::shared_ptr<IAdView>>;
const auto jsb_Multi_AdView_getAnchor =
    &core::jsb_accessor_get_on_ui_thread<MultiAdView, &MultiAdView::getAnchor,
                                         const std::pair<float, float>&>;
const auto jsb_Multi_AdView_setAnchor =
    &core::jsb_method_call_on_ui_thread<MultiAdView, &MultiAdView::setAnchor,
                                        float, float>;
const auto jsb_Multi_AdView_getPosition =
    &core::jsb_accessor_get_on_ui_thread<MultiAdView, &MultiAdView::getPosition,
                                         const std::pair<float, float>&>;
const auto jsb_Multi_AdView_setPosition =
    &core::jsb_method_call_on_ui_thread<MultiAdView, &MultiAdView::setPosition,
                                        float, float>;
const auto jsb_Multi_AdView_getSize = &core::jsb_accessor_get_on_ui_thread<
    MultiAdView, &MultiAdView::getSize,
    const std::pair<std::int32_t, std::int32_t>&>;
const auto jsb_Multi_AdView_setSize =
    &core::jsb_method_call_on_ui_thread<MultiAdView, &MultiAdView::setSize,
                                        std::int32_t, std::int32_t>;
const auto jsb_Multi_AdView_setVisible =
    &core::jsb_accessor_set_on_ui_thread<MultiAdView, &MultiAdView::setVisible,
                                         bool>;
const auto jsb_Multi_AdView_setLoadCallback =
    &core::jsb_set_callback<MultiAdView, &MultiAdView::setLoadCallback, bool>;

SE_BIND_FINALIZE_FUNC(jsb_Multi_AdView_finalize)
SE_BIND_CTOR(jsb_Multi_AdView_contructor, __jsb_Multi_AdView_class,
             jsb_Multi_AdView_finalize)
SE_BIND_FUNC(jsb_Multi_AdView_isLoaded)
SE_BIND_FUNC(jsb_Multi_AdView_load)
SE_BIND_FUNC(jsb_Multi_AdView_addItem)
SE_BIND_FUNC(jsb_Multi_AdView_getAnchor)
SE_BIND_FUNC(jsb_Multi_AdView_setAnchor)
SE_BIND_FUNC(jsb_Multi_AdView_getPosition)
SE_BIND_FUNC(jsb_Multi_AdView_setPosition)
SE_BIND_FUNC(jsb_Multi_AdView_getSize)
SE_BIND_FUNC(jsb_Multi_AdView_setSize)
SE_BIND_FUNC(jsb_Multi_AdView_setVisible)
SE_BIND_FUNC(jsb_Multi_AdView_setLoadCallback)

bool register_multi_adview_manual(se::Object* globalObj) {
    se::Object* adsObj = nullptr;
    se::Object* eeObj = nullptr;
    core::getOrCreatePlainObject_r("ee", globalObj, &eeObj);
    core::getOrCreatePlainObject_r("ads", eeObj, &adsObj);

    auto cls = se::Class::create("MultiAdView", adsObj, nullptr,
                                 _SE(jsb_Multi_AdView_contructor));
    cls->defineFinalizeFunction(_SE(jsb_Multi_AdView_finalize));

    cls->defineFunction("isLoaded", _SE(jsb_Multi_AdView_isLoaded));
    cls->defineFunction("load", _SE(jsb_Multi_AdView_load));
    cls->defineFunction("addItem", _SE(jsb_Multi_AdView_addItem));
    cls->defineFunction("getAnchor", _SE(jsb_Multi_AdView_getAnchor));
    cls->defineFunction("setAnchor", _SE(jsb_Multi_AdView_setAnchor));
    cls->defineFunction("getPosition", _SE(jsb_Multi_AdView_getPosition));
    cls->defineFunction("setPosition", _SE(jsb_Multi_AdView_setPosition));
    cls->defineFunction("getSize", _SE(jsb_Multi_AdView_getSize));
    cls->defineFunction("setSize", _SE(jsb_Multi_AdView_setSize));
    cls->defineFunction("setVisible", _SE(jsb_Multi_AdView_setVisible));
    cls->defineFunction("setLoadCallback",
                        _SE(jsb_Multi_AdView_setLoadCallback));

    cls->install();

    JSBClassType::registerClass<MultiAdView>(cls);

    __jsb_Multi_AdView_proto = cls->getProto();
    __jsb_Multi_AdView_class = cls;

    se::ScriptEngine::getInstance()->clearException();

    return true;
}

} // namespace ads
} // namespace ee
