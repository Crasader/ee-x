//
//  CrashlyticsProtocol.hpp
//  ee-crashlytics
//
//  Created by Zinge on 7/4/16.
//
//

#ifndef EE_CRASHLYTICS_CRASHLYTICS_PROTOCOL_HPP_
#define EE_CRASHLYTICS_CRASHLYTICS_PROTOCOL_HPP_

#include <memory>
#include <string>

namespace ee {
namespace core {
class LogLevel;
} // namespace core

namespace crashlytics {
class Crashlytics final {
private:
    static const std::string PluginName;

public:
    Crashlytics();
    ~Crashlytics();

    /// Causes a crash.
    void causeCrash() const;

    /// Causes an exception.
    void causeException() const;

    void log(const core::LogLevel& level, const std::string& tag,
             const std::string& message) const;

    void setString(const std::string& key, const std::string& value) const;

    void setBool(const std::string& key, bool value) const;

    void setInt(const std::string& key, int value) const;

    void setUserIdentifier(const std::string& identifier) const;

    void setUserName(const std::string& name) const;

    void setUserEmail(const std::string& email) const;
};
} // namespace crashlytics
} // namespace ee

#endif /* CrashlyticsProtocol_hpp */
