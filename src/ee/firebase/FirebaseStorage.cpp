//
//  FirebaseStorage.cpp
//  ee_x
//
//  Created by Zinge on 5/10/17.
//
//

#include <cassert>

#include "ee/firebase/FirebaseStorage.hpp"
#include "ee/firebase/FirebaseApp.hpp"
#include "ee/firebase/FirebaseScheduler.hpp"
#include "ee/core/ScopeGuard.hpp"

#if defined(EE_X_MOBILE)
#include <firebase/storage.h>
#endif // EE_X_MOBILE

namespace ee {
namespace firebase {
FirebaseStorage::FirebaseStorage() {
    initialized_ = false;
    fetching_ = false;

#if defined(EE_X_MOBILE)
    storage_ = nullptr;
    metadataScheduler_ = nullptr;
    bytesScheduler_ = nullptr;
#endif // EE_X_MOBILE
}

FirebaseStorage::~FirebaseStorage() {
}

bool FirebaseStorage::initialize() {
    if (initialized_) {
        return true;
    }

    FirebaseApp::initialize();

#if defined(EE_X_MOBILE)
    auto app = ::firebase::App::GetInstance();
    if (app == nullptr) {
        return false;
    }

    ::firebase::InitResult initResult;
    storage_ = ::firebase::storage::Storage::GetInstance(
        app, std::addressof(initResult));
    if (initResult != ::firebase::InitResult::kInitResultSuccess) {
        return false;
    }

    metadataScheduler_ =
        std::make_unique<FirebaseScheduler<::firebase::storage::Metadata>>();
    bytesScheduler_ = std::make_unique<FirebaseScheduler<std::size_t>>();
#endif // EE_X_MOBILE

    initialized_ = true;
    fetching_ = false;
    return true;
}

void FirebaseStorage::getHash(const std::string& filePath,
                              const HashCallback& callback) {
    auto guard =
        std::make_shared<core::ScopeGuard>(std::bind(callback, false, ""));
    if (not initialized_) {
        return;
    }
#if defined(EE_X_MOBILE)
    auto file = storage_->GetReference(filePath.c_str());
    if (not file.is_valid()) {
        return;
    }
    metadataScheduler_->push(
        file.GetMetadata(),
        [callback, guard](
            const ::firebase::Future<::firebase::storage::Metadata>& fut) {
            if (fut.error() != ::firebase::storage::kErrorNone) {
                return;
            }
            auto metadata = fut.result();
            if (not metadata->is_valid()) {
                return;
            }
            guard->dismiss();
            // MD5 is not supported yet, use updated_time.
            callback(true, std::to_string(metadata->updated_time()));
        });
#endif // EE_X_MOBILE
}

void FirebaseStorage::getData(const std::string& filePath,
                              const DataCallback& callback) {
    auto guard =
        std::make_shared<core::ScopeGuard>(std::bind(callback, false, ""));
    if (not initialized_) {
        return;
    }

#if defined(EE_X_MOBILE)
    auto file = storage_->GetReference(filePath.c_str());
    if (not file.is_valid()) {
        return;
    }
    if (fetching_) {
        return;
    }
    fetching_ = true;
    bytesScheduler_->push(
        file.GetBytes(std::addressof(buffer_.at(0)), buffer_.size()),
        [this, callback, guard](const ::firebase::Future<std::size_t>& fut) {
            fetching_ = false;
            if (fut.error() != ::firebase::storage::kErrorNone) {
                return;
            }
            guard->dismiss();
            auto bytes = fut.result();
            assert(*bytes <= max_file_size_in_bytes);
            callback(true, std::string(std::addressof(buffer_.at(0)), *bytes));
        });
#endif // EE_X_MOBILE
}
} // namespace firebase
} // namespace ee
