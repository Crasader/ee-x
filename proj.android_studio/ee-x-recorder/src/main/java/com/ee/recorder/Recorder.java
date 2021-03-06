package com.ee.recorder;

import android.app.Activity;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.media.projection.MediaProjectionManager;
import android.os.Build;
import android.os.IBinder;

import androidx.annotation.NonNull;

import com.ee.core.Logger;
import com.ee.core.MessageBridge;
import com.ee.core.MessageHandler;
import com.ee.core.PluginProtocol;
import com.ee.core.internal.Utils;

import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Locale;

/**
 * Created by Pham Xuan Han on 17/05/17.
 */
public class Recorder implements PluginProtocol {
    private static final String k__isSupported = "Recorder_isSupported";
    private static final String k__startRecording = "Recorder_startRecording";
    private static final String k__stopRecording = "Recorder_stopRecording";
    private static final String k__cancelRecording = "Recorder_cancelRecording";
    private static final String k__getRecordingUrl = "Recorder_getRecordingUrl";

    private static final Logger _logger = new Logger(Recorder.class.getName());

    private static final int PERMISSION_CODE = 1;

    private Activity _activity;
    private RecordService _recordService;
    private ServiceConnection _serviceConnection;
    private MediaProjectionManager _mediaProjectionManager;
    private String _filePath;

    public Recorder(Context context) {
        Utils.checkMainThread();
        _activity = null;
        registerHandlers();
        if (isSupported()) {
            _mediaProjectionManager = (MediaProjectionManager) context.getSystemService(Context.MEDIA_PROJECTION_SERVICE);
        }
    }

    @NonNull
    @Override
    public String getPluginName() {
        return "Recorder";
    }

    @Override
    public void onCreate(@NonNull Activity activity) {
        _activity = activity;
        if (isSupported()) {
            Intent service = new Intent(_activity, RecordService.class);
            _serviceConnection = new ServiceConnection() {
                @Override
                public void onServiceConnected(ComponentName name, IBinder service) {
                    RecordService.ServiceBinder binder = (RecordService.ServiceBinder) service;
                    _recordService = binder.getServiceSystem();
                }

                @Override
                public void onServiceDisconnected(ComponentName name) {
                    _recordService = null;
                }
            };
            _activity.bindService(service, _serviceConnection, Context.BIND_AUTO_CREATE);
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
    }

    @Override
    public void onPause() {
    }

    @Override
    public void onDestroy() {
        _activity = null;
        if (isSupported()) {
            _serviceConnection = null;
        }
    }

    @Override
    public void destroy() {
        Utils.checkMainThread();
        deregisterHandlers();
    }

    @Override
    public boolean onBackPressed() {
        return false;
    }

    private void registerHandlers() {
        MessageBridge bridge = MessageBridge.getInstance();

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                return Utils.toString(isSupported());
            }
        }, k__isSupported);

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                startRecording();
                return "";
            }
        }, k__startRecording);

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                stopRecording();
                return "";
            }
        }, k__stopRecording);

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                cancelRecording();
                return "";
            }
        }, k__cancelRecording);

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                return getRecordingUrl();
            }
        }, k__getRecordingUrl);
    }

    private void deregisterHandlers() {
        MessageBridge bridge = MessageBridge.getInstance();

        bridge.deregisterHandler(k__startRecording);
        bridge.deregisterHandler(k__stopRecording);
        bridge.deregisterHandler(k__cancelRecording);
        bridge.deregisterHandler(k__getRecordingUrl);
    }

    @Override
    public boolean onActivityResult(int requestCode, int responseCode, Intent data) {
        if (requestCode != PERMISSION_CODE || responseCode != Activity.RESULT_OK) {
            return false;
        }
        if (!isSupported()) {
            return false;
        }
        _filePath = generateFilePath();

        Intent intent = new Intent(_activity, RecordService.class);
        intent.setAction(RecordService.ACTION_START);
        intent.putExtra("responseCode", responseCode);
        intent.putExtra("data", data);
        intent.putExtra("path", _filePath);
        _activity.startService(intent);
        return true;
    }

    public boolean isSupported() {
        return Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP;
    }

    @SuppressWarnings("WeakerAccess")
    public void startRecording() {
        if (!isSupported()) {
            return;
        }
        _activity.startActivityForResult(_mediaProjectionManager.createScreenCaptureIntent(), PERMISSION_CODE);
    }

    @SuppressWarnings("WeakerAccess")
    public void stopRecording() {
        if (!isSupported()) {
            return;
        }
        Intent intent = new Intent(_activity, RecordService.class);
        intent.setAction(RecordService.ACTION_STOP);
        _activity.startService(intent);
    }

    @SuppressWarnings("WeakerAccess")
    public void cancelRecording() {
        if (!isSupported()) {
            return;
        }
        Intent intent = new Intent(_activity, RecordService.class);
        intent.setAction(RecordService.ACTION_CANCEL);
        _activity.startService(intent);
    }

    @SuppressWarnings("WeakerAccess")
    public String getRecordingUrl() {
        return _filePath;
    }

    private String generateFilePath() {
        SimpleDateFormat formatter = new SimpleDateFormat("yyyy-MM-dd-HH-mm-ss", Locale.getDefault());
        Date curDate = new Date(System.currentTimeMillis());
        String curTime = formatter.format(curDate).replace(" ", "");
        return String.format("%s/capture_%s.mp4", _activity.getApplicationInfo().dataDir, curTime);
    }
}
