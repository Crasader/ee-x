//
//  LoggerInterface.hpp
//  ee_x
//
//  Created by Zinge on 11/1/17.
//
//

#ifndef EE_X_LOGGER_HPP
#define EE_X_LOGGER_HPP

#include <cstdarg>
#include <functional>
#include <string>

#include "ee/CoreFwd.hpp"

namespace ee {
namespace core {
class Logger final {
private:
    using Self = Logger;

public:
    using LogCallback =
        std::function<void(const LogLevel& level, const std::string& tag,
                           const std::string& message)>;

    /// Gets the logger used by the library.
    static Self& getSystemLogger();

    /// Sets the logger used by the library.
    static void setSystemLogger(const Self& logger);

    /// Constructs an empty logger (doesn't log anything).
    static Self nullLogger();

    /// Logs a message to logcat (Android) or to console (iOS and macOS).
    static void log(const LogLevel& level, const std::string& tag,
                    const std::string& message);

    /// Constructs a logger with the specified tag and a default callback
    /// (Android Logcat or iOS/macOS console).
    explicit Logger(const std::string& tag);

    /// Constructs a logger with the specified tag and callback.
    explicit Logger(const std::string& tag, const LogCallback& callback);

    ~Logger() = default;

    void setEnabled(bool enabled);

    void log(const LogLevel& level, std::string formatString, ...);
    void verbose(std::string formatString, ...);
    void debug(std::string formatString, ...);
    void info(std::string formatString, ...);
    void warn(std::string formatString, ...);
    void error(std::string formatString, ...);
    void wtf(std::string formatString, ...);

protected:
    void log(const LogLevel& level, std::string formatString,
             std::va_list args);

private:
    bool enabled_;
    std::string tag_;
    LogCallback callback_;
};
} // namespace core
} // namespace ee

#endif /* EE_X_LOGGER_HPP */
