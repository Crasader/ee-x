package com.ee.core.internal;

import android.support.annotation.NonNull;

/**
 * Created by Zinge on 3/29/17.
 */
public interface MessageHandler {
    @NonNull
    String handle(@NonNull String message);
}