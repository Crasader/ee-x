//
//  Logger-iOS.m
//  ee-core
//
//  Created by Zinge on 7/6/16.
//
//

#import <Foundation/Foundation.h>

#include "ee/core/Logger.hpp"
#include "ee/core/LogLevel.hpp"

namespace ee {
namespace core {
void Logger::log0(const LogLevel& level, const std::string& tag,
                  const std::string& message) {
    NSLog(@"%s %s: %s", level.desc.c_str(), tag.c_str(), message.c_str());
}
} // namespace core
} // namespace ee
