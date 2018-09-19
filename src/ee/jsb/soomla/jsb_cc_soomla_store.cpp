#include "ee/jsb/core/jsb_core_common.hpp"

#include "Cocos2dxStore.h"
#include "ee/jsb/core/jsb_templates.hpp"
#include "jsb_cc_soomla_store.hpp"

namespace ee {
namespace core {

template <>
soomla::CCStoreAssets* get_value(const se::Value& value) {
    return static_cast<soomla::CCStoreAssets*>(
        value.toObject()->getPrivateData());
}

template <>
void set_value(se::Value& value, soomla::CCSoomlaStore* input) {
    set_value_from_pointer(value, input);
}

} // namespace core
} // namespace ee

namespace soomla {

static se::Object* __jsb_CCSoomlaStore_proto = nullptr;
static se::Class* __jsb_CCSoomlaStore_class = nullptr;

const static auto jsb_CCSoomlaStore_initialize =
    &ee::core::jsb_static_call<&CCSoomlaStore::initialize, CCStoreAssets*,
                               cocos2d::ValueMap>;
const static auto jsb_CCSoomlaStore_getInstance =
    &ee::core::jsb_static_get<CCSoomlaStore*, &CCSoomlaStore::getInstance>;

const auto jsb_CCSoomlaStore_refreshMarketItemsDetails =
    &ee::core::jsb_method_call<
        CCSoomlaStore, &CCSoomlaStore::refreshMarketItemsDetails, CCError**>;

SE_BIND_FUNC(jsb_CCSoomlaStore_initialize)
SE_BIND_FUNC(jsb_CCSoomlaStore_getInstance)
SE_BIND_FUNC(jsb_CCSoomlaStore_refreshMarketItemsDetails)

bool register_cc_soomla_store_manual(se::Object* globalObj) {
    se::Object* soomlaObj = nullptr;
    ee::core::getOrCreatePlainObject_r("soomla", globalObj, &soomlaObj);

    auto cls = se::Class::create("CCSoomlaStore", soomlaObj, nullptr, nullptr);
    cls->defineFunction("refreshMarketItemsDetails",
                        _SE(jsb_CCSoomlaStore_refreshMarketItemsDetails));

    cls->install();

    JSBClassType::registerClass<soomla::CCSoomlaStore>(cls);

    __jsb_CCSoomlaStore_proto = cls->getProto();
    __jsb_CCSoomlaStore_class = cls;

    // Register static member variables and static member functions
    se::Value ctorVal;
    if (soomlaObj->getProperty("CCSoomlaStore", &ctorVal) &&
        ctorVal.isObject()) {
        ctorVal.toObject()->defineFunction("initialize",
                                           _SE(jsb_CCSoomlaStore_initialize));
        ctorVal.toObject()->defineFunction("getInstance",
                                           _SE(jsb_CCSoomlaStore_getInstance));
    }

    se::ScriptEngine::getInstance()->clearException();
    return true;
}
} // namespace soomla
