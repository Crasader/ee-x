//
//  NotificationProtocol.hpp
//  ee_core
//
//  Created by Zinge on 3/28/17.
//
//

#ifndef EE_X_NOTIFICATION_NOTIFICATION_BRIDGE_HPP_
#define EE_X_NOTIFICATION_NOTIFICATION_BRIDGE_HPP_

#include <string>

#include "ee/NotificationFwd.hpp"

namespace ee {
namespace notification {
class Notification final {
public:
    Notification();
    
    /// Schedules a notification.
    void schedule(const NotificationBuilder& builder);

    void schedule(const std::string& msg, int tag, int delay, int interval);

    /// Unschedules notifications whose the specified tag.
    void unschedule(int tag);

    /// Clear all notifications.
    void clearAll();

private:
    /// Unschedule all notifications.
    /// Only supported on iOS.
    void unscheduleAll();
    
    IMessageBridge& bridge_;
};
} // namespace notification
} // namespace ee

#endif /* EE_X_NOTIFICATION_NOTIFICATION_BRIDGE_HPP_ */
