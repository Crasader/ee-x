//
//  jsb_cc_purchase_with_market_builder.cpp
//  ee-x
//
//  Created by Le Van Kiet on 9/14/18.
//

#include "ee/jsb/soomla/jsb_cc_purchase_with_market_builder.hpp"

#include "Soomla/PurchaseTypes/CCPurchaseWithMarket.h"
#include "Soomla/PurchaseTypes/CCPurchaseWithMarketBuilder.h"

#include "ee/jsb/core/jsb_core_common.hpp"
#include "ee/jsb/core/jsb_templates.hpp"

namespace ee {
namespace core {

template <>
soomla::CCPurchaseType* get_value(const se::Value& value) {
    return static_cast<soomla::CCPurchaseType*>(
        value.toObject()->getPrivateData());
}

template <>
void set_value(se::Value& value, soomla::CCPurchaseType* input) {
    set_value_from_pointer(value, input);
}
} // namespace core
} // namespace ee

namespace soomla {
se::Object* __jsb_CCPurchaseWithMarketBuilder_proto = nullptr;
se::Class* __jsb_CCPurchaseWithMarketBuilder_class = nullptr;
se::Class* __jsb_CCPurchaseTypes_class = nullptr;

const auto jsb_CCPurchaseWithMarketBuilder_finalize =
    &ee::core::jsb_finalize<CCPurchaseWithMarketBuilder>;
const auto jsb_CCPurchaseWithMarketBuilder_constructor =
    &ee::core::jsb_constructor<CCPurchaseWithMarketBuilder>;

const auto jsb_CCPurchaseWithMarketBuilder_build =
    &ee::core::jsb_method_get<CCPurchaseWithMarketBuilder,
                              &CCPurchaseWithMarketBuilder::build,
                              CCPurchaseType*>;

const auto jsb_CCPurchaseWithMarketBuilder_setPrice =
    &ee::core::jsb_accessor_set<CCPurchaseWithMarketBuilder,
                                &CCPurchaseWithMarketBuilder::setPrice, float>;

const auto jsb_CCPurchaseWithMarketBuilder_setProductId =
    &ee::core::jsb_accessor_set<CCPurchaseWithMarketBuilder,
                                &CCPurchaseWithMarketBuilder::setProductId,
                                std::string>;

SE_BIND_FINALIZE_FUNC(jsb_CCPurchaseWithMarketBuilder_finalize)
SE_BIND_CTOR(jsb_CCPurchaseWithMarketBuilder_constructor,
             __jsb_CCPurchaseWithMarketBuilder_class,
             jsb_CCPurchaseWithMarketBuilder_finalize)
SE_BIND_FUNC(jsb_CCPurchaseWithMarketBuilder_build)
SE_BIND_FUNC(jsb_CCPurchaseWithMarketBuilder_setPrice)
SE_BIND_FUNC(jsb_CCPurchaseWithMarketBuilder_setProductId)

const auto jsb_CCPurchaseType_finalize =
    &ee::core::jsb_finalize<CCPurchaseType>;

SE_BIND_FINALIZE_FUNC(jsb_CCPurchaseType_finalize)

bool register_cc_purchase_with_market_builder_manual(se::Object* globalObj) {
    se::Object* __soomlaObj = nullptr;
    ee::core::getOrCreatePlainObject_r("soomla", globalObj, &__soomlaObj);

    auto cls =
        se::Class::create("CCPurchaseWithMarketBuilder", __soomlaObj, nullptr,
                          _SE(jsb_CCPurchaseWithMarketBuilder_constructor));
    cls->defineFinalizeFunction(_SE(jsb_CCPurchaseWithMarketBuilder_finalize));

    cls->defineFunction("build", _SE(jsb_CCPurchaseWithMarketBuilder_build));
    cls->defineFunction("setPrice",
                        _SE(jsb_CCPurchaseWithMarketBuilder_setPrice));
    cls->defineFunction("setProductId",
                        _SE(jsb_CCPurchaseWithMarketBuilder_setProductId));

    cls->install();

    JSBClassType::registerClass<CCPurchaseWithMarketBuilder>(cls);

    __jsb_CCPurchaseWithMarketBuilder_proto = cls->getProto();
    __jsb_CCPurchaseWithMarketBuilder_class = cls;

    // CCPurchaseType
    auto clsPurchase =
        se::Class::create("CCPurchaseType", __soomlaObj, nullptr, nullptr);
    cls->defineFinalizeFunction(_SE(jsb_CCPurchaseType_finalize));

    clsPurchase->install();
    JSBClassType::registerClass<CCPurchaseType>(clsPurchase);
    __jsb_CCPurchaseTypes_class = clsPurchase;

    se::ScriptEngine::getInstance()->clearException();
    return true;
}

} // namespace soomla
