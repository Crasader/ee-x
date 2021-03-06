package com.ee.tenjin;

import android.app.Activity;
import android.content.Intent;

import androidx.annotation.NonNull;

import com.ee.core.IMessageBridge;
import com.ee.core.Logger;
import com.ee.core.MessageBridge;
import com.ee.core.MessageHandler;
import com.ee.core.PluginProtocol;
import com.ee.core.internal.Utils;
import com.tenjin.android.TenjinSDK;

public class Tenjin implements PluginProtocol {
    private static final String k__initialize = "Tenjin_initialize";

    private static final Logger _logger = new Logger(Tenjin.class.getName());

    private IMessageBridge _bridge;
    private Activity _activity;
    private TenjinSDK _tenjin;
    private String _apiKey;

    private boolean _isCreateCalled;
    private boolean _isResumeCalled;
    private boolean _isInitializeCalled;

    public Tenjin() {
        Utils.checkMainThread();
        _bridge = MessageBridge.getInstance();
        _activity = null;
        registerHandlers();
    }

    @NonNull
    @Override
    public String getPluginName() {
        return "Tenjin";
    }

    @Override
    public void onCreate(@NonNull Activity activity) {
        _activity = activity;
        _isCreateCalled = true;
        if (_isInitializeCalled) {
            _tenjin = TenjinSDK.getInstance(_activity, _apiKey);
        }
    }

    @Override
    public void onStart() {
    }

    @Override
    public void onStop() {
    }

    @Override
    public void onResume() {
        _isResumeCalled = true;
        if (_isInitializeCalled) {
            _tenjin.connect();
        }
    }

    @Override
    public void onPause() {
    }

    @Override
    public void onDestroy() {
        _activity = null;
    }

    @Override
    public boolean onBackPressed() {
        return false;
    }

    @Override
    public void destroy() {
        Utils.checkMainThread();
        deregisterHandlers();
    }

    @Override
    public boolean onActivityResult(int requestCode, int responseCode, Intent data) {
        return false;
    }

    private void registerHandlers() {
        _bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                String apiKey = message;
                initialize(apiKey);
                return "";
            }
        }, k__initialize);
    }

    private void deregisterHandlers() {
        _bridge.deregisterHandler(k__initialize);
    }


    @SuppressWarnings("unused")
    private void initialize(String apiKey) {
        _apiKey = apiKey;
        _isInitializeCalled = true;
        if (_isCreateCalled) {
            _tenjin = TenjinSDK.getInstance(_activity, _apiKey);
        }
        if (_isResumeCalled) {
            _tenjin.connect();
        }
    }
}
