//
//  jsb_cc_store_info.cpp
//  ee-x
//
//  Created by Nguyen Van Quynh on 9/14/18.
//

#include "CCStoreInfo.h"
#include "Cocos2dxCore.h"
#include "ee/jsb/core/jsb_templates.hpp"
#include "jsb_cc_store_info.hpp"
#include "jsb_core_common.hpp"

namespace soomla {
static se::Object* __jsb_CCStoreInfo_proto = nullptr;
static se::Class* __jsb_CCStoreInfo_class = nullptr;
} // namespace soomla

namespace ee {
namespace core {

template <>
soomla::CCStoreInfo* get_value(const se::Value& value) {
    return static_cast<soomla::CCStoreInfo*>(
        value.toObject()->getPrivateData());
}

template <>
void set_value(se::Value& value, soomla::CCStoreInfo* input) {
    se::Object* obj = nullptr;
    obj = se::Object::createObjectWithClass(soomla::__jsb_CCStoreInfo_class);
    obj->setPrivateData(input);
    value.setObject(obj);
}

template <>
void set_value(se::Value& value, soomla::CCPurchasableVirtualItem* input) {
    se::Object* obj = nullptr;
    obj = se::Object::createObjectWithClass(soomla::__jsb_CCStoreInfo_class);
    obj->setPrivateData(input);
    value.setObject(obj);
}

} // namespace core
} // namespace ee

namespace soomla {

const static auto jsb_CCStoreInfo_sharedStoreInfo =
    &ee::core::jsb_static_get<CCStoreInfo*, &CCStoreInfo::sharedStoreInfo>;
const auto jsb_CCStoreInfo_getItemByItemId =
    &ee::core::jsb_method_get<CCStoreInfo, &CCStoreInfo::getItemByItemId,
                              CCVirtualItem*, const std::string&, CCError**>;
const auto jsb_CCStoreInfo_getPurchasableItemWithProductId =
    &ee::core::jsb_method_get<
        CCStoreInfo, &CCStoreInfo::getPurchasableItemWithProductId,
        CCPurchasableVirtualItem*, const std::string&, CCError**>;

SE_BIND_FUNC(jsb_CCStoreInfo_sharedStoreInfo)
SE_BIND_FUNC(jsb_CCStoreInfo_getItemByItemId)
SE_BIND_FUNC(jsb_CCStoreInfo_getPurchasableItemWithProductId)

bool register_cc_store_info_manual(se::Object* globalObj) {
    se::Object* soomlaObj = nullptr;
    ee::core::getOrCreatePlainObject_r("soomla", globalObj, &soomlaObj);

    auto cls = se::Class::create("CCStoreInfo", soomlaObj, nullptr, nullptr);

    cls->defineFunction("getItemByItemId",
                        _SE(jsb_CCStoreInfo_getItemByItemId));
    cls->defineFunction("getPurchasableItemWithProductId",
                        _SE(jsb_CCStoreInfo_getPurchasableItemWithProductId));

    cls->install();

    JSBClassType::registerClass<CCStoreInfo>(cls);

    __jsb_CCStoreInfo_proto = cls->getProto();
    __jsb_CCStoreInfo_class = cls;

    // Register static member variables and static member functions
    se::Value ctorVal;
    if (soomlaObj->getProperty("CCStoreInfo", &ctorVal) && ctorVal.isObject()) {
        ctorVal.toObject()->defineFunction(
            "sharedStoreInfo", _SE(jsb_CCStoreInfo_sharedStoreInfo));
    }

    se::ScriptEngine::getInstance()->clearException();
    return true;
}

} // namespace soomla
