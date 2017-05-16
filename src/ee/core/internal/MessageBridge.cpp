//
//  BridgeManager.cpp
//  ee_core
//
//  Created by Zinge on 3/28/17.
//
//

#include <cassert>
#include <mutex>

#include "ee/core/internal/MessageBridge.hpp"

namespace ee {
namespace core {
MessageBridge& MessageBridge::getInstance() {
    static MessageBridge sharedInstance;
    return sharedInstance;
}

MessageBridge::MessageBridge() {
}

MessageBridge::~MessageBridge() {
}

std::string MessageBridge::call(const std::string& tag) {
    return call(tag, "");
}

std::string MessageBridge::callCpp(const std::string& tag,
                                   const std::string& msg) {
    std::lock_guard<SpinLock> guard(handlerLock_);
    auto iter = handlers_.find(tag);
    if (iter == handlers_.cend()) {
        assert(false);
        return "";
    }
    return iter->second(msg);
}

bool MessageBridge::registerHandler(const MessageHandler& handler,
                                    const std::string& tag) {
    std::lock_guard<SpinLock> guard(handlerLock_);
    if (handlers_.count(tag) > 0) {
        assert(false);
        return false;
    }
    handlers_[tag] = handler;
    return true;
}

bool MessageBridge::deregisterHandler(const std::string& tag) {
    std::lock_guard<SpinLock> guard(handlerLock_);
    if (handlers_.count(tag) == 0) {
        assert(false);
        return false;
    }
    handlers_.erase(tag);
    return true;
}
} // namespace core
} // namespace ee
