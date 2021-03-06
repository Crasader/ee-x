/*
 Copyright (C) 2012-2014 Soomla Inc.

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

#include "CCJsonHelper.h"
#include "CCNativeSoomlaStore.h"
#include "CCSoomlaStore.h"
#include "CCSoomlaUtils.h"
#include "CCStoreBridge.h"
#include "CCStoreEventDispatcher.h"
#include "CCStoreInfo.h"

namespace soomla {
#define TAG "SOOMLA SoomlaStore"

USING_NS_CC;

static CCSoomlaStore* s_SharedSoomlaStore = nullptr;

bool CCSoomlaStore::initialized = false;

CCSoomlaStore* CCSoomlaStore::getInstance() {
    if (!s_SharedSoomlaStore) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) ||                                 \
    (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        s_SharedSoomlaStore = new CCNativeSoomlaStore();
#else
        s_SharedSoomlaStore = new CCSoomlaStore();
#endif
        s_SharedSoomlaStore->retain();
    }
    return s_SharedSoomlaStore;
}

void CCSoomlaStore::initialize(soomla::CCStoreAssets* storeAssets,
                               const cocos2d::ValueMap& storeParams) {
    if (initialized) {
        CCStoreEventDispatcher::getInstance()->onUnexpectedStoreError(0, true);
        CCSoomlaUtils::logError(TAG, "SoomlaStore is already initialized. You "
                                     "can't initialize it twice!");
        return;
    }

    CCStoreBridge::initShared();

    CCSoomlaUtils::logDebug(TAG, "CCSoomlaStore Initializing...");

    getInstance()->loadBillingService();

    CCStoreInfo::createShared(storeAssets);

    CCStoreBridge::getInstance()->applyParams(storeParams);

    // #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    // On iOS we only refresh market items
    // CCError *error = nullptr;
    // getInstance()->refreshMarketItemsDetails(&error);
    // #elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    // On Android we refresh market items and restore transactions
    // getInstance()->refreshInventory();
    // #endif

    initialized = true;
    CCStoreEventDispatcher::getInstance()->onSoomlaStoreInitialized(true);
}

void CCSoomlaStore::buyMarketItem(const std::string& productId,
                                  const std::string& payload, CCError** error) {
    CCPurchasableVirtualItem* item =
        CCStoreInfo::sharedStoreInfo()->getPurchasableItemWithProductId(
            productId.c_str(), error);
    if (item == nullptr) {
        return;
    }

    // simulate onMarketPurchaseStarted event
    CCStoreEventDispatcher::getInstance()->onMarketPurchaseStarted(item);

    // in the editor we just give the item... no real market.
    // simulate onMarketPurchase event
    CCStoreEventDispatcher::getInstance()->onMarketPurchase(
        item, payload, ValueMapNull);

    item->give(1);

    // complete purchasing routine
    CCStoreEventDispatcher::getInstance()->onItemPurchased(
        item, payload);
}
} // namespace soomla
