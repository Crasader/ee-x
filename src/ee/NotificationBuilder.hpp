//
//  NotificationBuilder.hpp
//  ee_x
//
//  Created by Zinge on 4/3/17.
//
//

#ifndef EE_X_NOTIFICATION_BUIDLER_HPP_
#define EE_X_NOTIFICATION_BUIDLER_HPP_

#include <string>

namespace ee {
class Notification;

class NotificationBuilder {
public:
    NotificationBuilder();

    /// Sets the title of the notification.
    /// Default is empty.
    NotificationBuilder& setTitle(const std::string& title);

    /// Sets the body of the notification.
    /// Default is empty.
    NotificationBuilder& setBody(const std::string& body);

    /// Sets the delay of the notification in seconds.
    /// Default is zero, i.e. deliver the notification immediately.
    NotificationBuilder& setDelay(int delay);

    /// Sets the interval of the notification in seconds.
    /// Default is zero, i.e. no repeat.
    NotificationBuilder& setInterval(int interval);

    /// Sets the tag of the notification.
    /// Default is zero.
    NotificationBuilder& setTag(int tag);

private:
    friend Notification;

    std::string title_;
    std::string body_;
    int delay_;
    int interval_;
    int tag_;
};
} // namespace ee

#endif /* EE_X_NOTIFICATION_BUIDLER_HPP_ */
