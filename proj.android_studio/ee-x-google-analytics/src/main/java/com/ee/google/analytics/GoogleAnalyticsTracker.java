package com.ee.google.analytics;

import android.support.annotation.NonNull;

import com.ee.core.internal.JsonUtils;
import com.ee.core.internal.MessageBridge;
import com.ee.core.internal.MessageHandler;
import com.ee.core.internal.Utils;
import com.google.android.gms.analytics.*;

import java.util.HashMap;
import java.util.Map;

/**
 * Created by Zinge on 10/25/17.
 */

class GoogleAnalyticsTracker {
    private static final String k__key   = "key";
    private static final String k__value = "value";

    private String  _trackingId;
    private Tracker _tracker;

    GoogleAnalyticsTracker(@NonNull com.google.android.gms.analytics.GoogleAnalytics analytics,
                           @NonNull String trackingId) {
        _trackingId = trackingId;
        _tracker = analytics.newTracker(trackingId);
        registerHandlers();
    }

    void destroy() {
        deregisterHandlers();
        _tracker = null;
        _trackingId = null;
    }

    @NonNull
    private String k__setParameter() {
        return "GoogleAnalytics_setParameter_" + _trackingId;
    }

    @NonNull
    private String k__setAllowIDFACollection() {
        return "GoogleAnalytics_setAllowIDFACollection_" + _trackingId;
    }

    @NonNull
    private String k__send() {
        return "GoogleAnalytics_send_" + _trackingId;
    }

    private void registerHandlers() {
        MessageBridge bridge = MessageBridge.getInstance();

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                Map<String, Object> dict = JsonUtils.convertStringToDictionary(message);
                assert dict != null;
                String key = (String) dict.get(k__key);
                String value = (String) dict.get(k__value);
                setParameter(key, value);
                return "";
            }
        }, k__setParameter());

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                setAdvertisingIdCollectionEnabled(Utils.toBoolean(message));
                return "";
            }
        }, k__setAllowIDFACollection());

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                Map<String, Object> rawDict = JsonUtils.convertStringToDictionary(message);
                assert rawDict != null;

                Map<String, String> dict = new HashMap<>();
                for (String key : rawDict.keySet()) {
                    dict.put(key, (String) rawDict.get(key));
                }
                send(dict);
                return "";
            }
        }, k__send());
    }

    private void deregisterHandlers() {
        MessageBridge bridge = MessageBridge.getInstance();

        bridge.deregisterHandler(k__setAllowIDFACollection());
        bridge.deregisterHandler(k__send());
    }

    void setParameter(@NonNull String key, @NonNull String value) {
        _tracker.set(key, value);
    }

    void setAdvertisingIdCollectionEnabled(boolean enabled) {
        _tracker.enableAdvertisingIdCollection(enabled);
    }

    void setExceptionReportingEnabled(boolean enabled) {
        _tracker.enableExceptionReporting(enabled);
    }

    void send(@NonNull Map<String, String> dict) {
        _tracker.send(dict);
    }
}
