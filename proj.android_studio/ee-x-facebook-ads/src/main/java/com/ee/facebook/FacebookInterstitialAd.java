package com.ee.facebook;

import android.content.Context;
import androidx.annotation.NonNull;

import com.ee.ads.InterstitialAdHelper;
import com.ee.ads.IInterstitialAd;
import com.ee.core.IMessageBridge;
import com.ee.core.MessageBridge;
import com.ee.core.MessageHandler;
import com.ee.core.Logger;
import com.ee.core.internal.Utils;
import com.facebook.ads.Ad;
import com.facebook.ads.AdError;
import com.facebook.ads.InterstitialAd;
import com.facebook.ads.InterstitialAdListener;

/**
 * Created by Zinge on 10/11/17.
 */

class FacebookInterstitialAd implements InterstitialAdListener, IInterstitialAd {
    private static final Logger _logger = new Logger(FacebookInterstitialAd.class.getName());

    private Context              _context;
    private InterstitialAd       _interstitialAd;
    private String               _placementId;
    private InterstitialAdHelper _helper;
    private IMessageBridge       _bridge;


    public FacebookInterstitialAd(@NonNull Context context, @NonNull String placementId) {
        Utils.checkMainThread();
        _context = context;
        _placementId = placementId;
        _interstitialAd = null;
        _helper = new InterstitialAdHelper("FacebookInterstitialAd", placementId);
        _bridge = MessageBridge.getInstance();
        createInternalAd();
        registerHandlers();
    }

    void destroy() {
        Utils.checkMainThread();
        deregisterHandlers();
        destroyInternalAd();
        _helper = null;
        _context = null;
        _placementId = null;
        _bridge = null;
    }

    private String kCreateInternalAd() {
        return "FacebookInterstitialAd_createInternalAd_" + _placementId;
    }

    private String kDestroyInternalAd() {
        return "FacebookInterstitialAd_destroyInternalAd_" + _placementId;
    }

    private String kOnLoaded() {
        return "FacebookInterstitialAd_onLoaded_" + _placementId;
    }

    private String kOnFailedToLoad() {
        return "FacebookInterstitialAd_onFailedToLoad_" + _placementId;
    }

    private String kOnClosed() {
        return "FacebookInterstitialAd_onClosed_" + _placementId;
    }

    private String kOnClicked() {
        return "FacebookInterstitialAd_onClicked_" + _placementId;
    }

    private void registerHandlers() {
        Utils.checkMainThread();
        _helper.registerHandlers(this);

        _bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                return Utils.toString(createInternalAd());
            }
        }, kCreateInternalAd());

        _bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                return Utils.toString(destroyInternalAd());
            }
        }, kDestroyInternalAd());
    }

    private void deregisterHandlers() {
        Utils.checkMainThread();
        _helper.deregisterHandlers();
        _bridge.deregisterHandler(kCreateInternalAd());
        _bridge.deregisterHandler(kDestroyInternalAd());
    }

    private boolean createInternalAd() {
        Utils.checkMainThread();
        if (_interstitialAd != null) {
            return false;
        }
        InterstitialAd interstitialAd = new InterstitialAd(_context, _placementId);
        interstitialAd.setAdListener(this);
        _interstitialAd = interstitialAd;
        return true;
    }

    private boolean destroyInternalAd() {
        Utils.checkMainThread();
        if (_interstitialAd == null) {
            return false;
        }
        _interstitialAd.setAdListener(null);
        _interstitialAd.destroy();
        _interstitialAd = null;
        return true;
    }

    @Override
    public boolean isLoaded() {
        Utils.checkMainThread();
        return _interstitialAd != null && _interstitialAd.isAdLoaded();
    }

    @Override
    public void load() {
        Utils.checkMainThread();
        if (_interstitialAd == null) {
            return;
        }
        _logger.info("load");
        _interstitialAd.loadAd();
    }

    @Override
    public boolean show() {
        Utils.checkMainThread();
        return _interstitialAd != null && _interstitialAd.show();
    }

    @Override
    public void onInterstitialDisplayed(Ad ad) {
        _logger.info("onInterstitialDisplayed");
        Utils.checkMainThread();
    }

    @Override
    public void onInterstitialDismissed(Ad ad) {
        _logger.info("onInterstitialDismissed");
        Utils.checkMainThread();
        _bridge.callCpp(kOnClosed());
    }

    @Override
    public void onError(Ad ad, AdError adError) {
        _logger.info("onError: " + adError.getErrorMessage());
        Utils.checkMainThread();
        _bridge.callCpp(kOnFailedToLoad(), adError.getErrorMessage());
    }

    @Override
    public void onAdLoaded(Ad ad) {
        _logger.info("onAdLoaded");
        Utils.checkMainThread();
        _bridge.callCpp(kOnLoaded());
    }

    @Override
    public void onAdClicked(Ad ad) {
        _logger.info("onAdClicked");
        Utils.checkMainThread();
        _bridge.callCpp(kOnClicked());
    }

    @Override
    public void onLoggingImpression(Ad ad) {
        _logger.info("onLoggingImpression");
        Utils.checkMainThread();
    }
}
