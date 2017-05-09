//
//  Logger-Android.cpp
//  ee-core
//
//  Created by Zinge on 7/6/16.
//
//

#include <android/log.h>

#include "ee/core/Logger.hpp"
#include "ee/core/LogLevel.hpp"

namespace ee {
void Logger::log0(const LogLevel& level, const std::string& tag,
                  const std::string& message) {
    __android_log_print(level.priority, tag.c_str(), "%s", message.c_str());
}
} // namespace ee
