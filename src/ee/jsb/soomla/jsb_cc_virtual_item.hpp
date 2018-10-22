//
//  jsb_cc_virtual_item.hpp
//  ee-x
//
//  Created by Nguyen Van Quynh on 9/14/18.
//

#ifndef JSB_CC_VIRTUAL_ITEM_HPP
#define JSB_CC_VIRTUAL_ITEM_HPP

#include "ee/jsb/core/jsb_fwd.hpp"

namespace soomla {

extern se::Object* __jsb_CCVirtualItem_proto;

se::Object* getVirtualItemProto();

bool register_cc_virtual_item_manual(se::Object* object);

} // namespace soomla

#endif /* JSB_CC_VIRTUAL_ITEM_HPP */
