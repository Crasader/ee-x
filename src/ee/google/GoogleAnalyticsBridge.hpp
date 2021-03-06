//
//  GoogleAnalytics.hpp
//  ee_x
//
//  Created by Zinge on 10/24/17.
//
//

#ifndef EE_X_GOOGLE_ANALYTICS_BRIDGE_HPP
#define EE_X_GOOGLE_ANALYTICS_BRIDGE_HPP

#include <string>

#include "ee/GoogleAnalyticsFwd.hpp"

namespace ee {
namespace google {
class Analytics {
public:
    Analytics();
    ~Analytics();

    void setDispatchInterval(int seconds);
    void setDryRun(bool enabled);
    void setOptOut(bool enabled);
    void setTrackUncaughtException(bool enabled);
    void dispatch();

    std::shared_ptr<AnalyticsTracker>
    createTracker(const std::string& trackingId);

    int doTests();

protected:
    friend AnalyticsTracker;

    bool destroyTracker(const std::string& trackingId);

private:
    bool testTrackEvent();
    bool testTrackException();
    bool testTrackScreenView();
    bool testTrackSocial();
    bool testTrackTiming();
    bool testCustomDimensionAndMetric();
    bool testEcommerceAction();
    bool testEcommerceImpression();
};
} // namespace google
} // namespace ee

#endif /* EE_X_GOOGLE_ANALYTICS_HPP */
