//
//  FirebaseFwd.hpp
//  ee_x
//
//  Created by Zinge on 5/10/17.
//
//

#ifndef EE_X_FIREBASE_FWD_HPP_
#define EE_X_FIREBASE_FWD_HPP_

#include <ee/CoreFwd.hpp>

namespace ee {
namespace firebase {
class App;
class Analytics;
class RemoteConfig;
class Storage;
class Messaging;
class Message;
class DynamicLink;
class Performance;
class PerformanceTrace;
} // namespace firebase

using FirebaseDynamicLink = firebase::DynamicLink;
using FirebaseApp = firebase::App;
using FirebaseAnalytics = firebase::Analytics;
using FirebaseRemoteConfig = firebase::RemoteConfig;
using FirebaseStorage = firebase::Storage;
using FirebaseMessaging = firebase::Messaging;
using FirebasePerformance = firebase::Performance;
using FirebasePerformanceTrace = firebase::PerformanceTrace;
using firebase::Message;
} // namespace ee

#endif /* EE_X_FIREBASE_FWD_HPP_ */
