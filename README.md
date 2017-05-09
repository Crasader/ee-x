# EE-X library

*Author: Hai Hoang*

## Modules

- Crashlytics
- Local notifications

## Getting started

- Clone the repository.

- Modify `settings.gradle` (`CLONE_PATH` is where you cloned the repository to):

```
include ':ee-x'
project(':ee-x').projectDir = new File('CLONE_PATH/ee-x/proj.android_studio')
```

- Modify `build.gradle`:

```
dependencies {
    compile project(':ee-x')
}
```

- Modify `Android.mk`:

```
$(call import-add-path, CLONE_PATH)
$(call import-module, ee-x/proj.android_studio/src/main/jni)
```

- Modify `main.cpp`:

```
#include <jni.h>
#include <ee/Core.hpp>

void cocos_android_app_init(JNIEnv* env) {
    ...
    JavaVm* vm;
    env->GetJavaVm(&vm);
    ee::JniUtils::setVm(vm);
}
```

- Modify `AppActivity.java`:

```
import com.ee.PluginManager;

public class AppActivity extends Cocos2dxActivity {
    @Override
    protected void onStart() {
        PluginManager.getInstance().onStart();
    }
    
    @Override
    protected void onStop() {
        PluginManager.getInstance().onStop();
    }

    @Override
    protected void onResume() {
        PluginManager.getInstance().onResume();
    }
    
    @Override
    protected void onPause() {
        PluginManager.getInstance().onPause();
    }
    
    @Override
    protected void onDestroy() {
        PluginManager.getInstance().onDestroy();
    }
    
    @Override
    protected void onBackPressed() {
        if (PluginManager.getInstance().onBackPressed()) {
            return;
        }
    }
    
    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        if (PluginManager.getInstance().onActivityResult(requestCode, resultCode, data)) {
            return;
        }
    }
}

```

## Crashlytics

- Supported platforms: Android, iOS, Mac OSX.

- Modify `Android.mk`:

```
LOCAL_STATIC_LIBRARIES += ee_x_crashlytics_static
```

- Modify `AppActivity.java`:

```
import com.ee.Crashlytics;

public class AppActivity extends Cocos2dxActivity {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        PluginManager.getInstance().addPlugin(new Crashlytics(this));
    }
}

```

- Modify `Podfile`:

```
pod 'ee-x/crashlytics', :path => 'CLONE_PATH/ee-x/ee-x.podspec'
```

- Modify `AppController.mm`:

```
#include <ee/Core.hpp>

- (BOOL)application:(UIApplication*)application
    didFinishLaunchingWithOptions:(NSDictionary*)launchOptions {
    [[EEPluginManager getInstance] addPlugin:@"Crashlytics"];
    ...
}
```

- Sample C++ class:

 
```
// CrashlyticsAgent.hpp

#include <ee/CrashlyticsFwd.hpp>

class CrashlyticsAgent {
public:
    static CrashlyticsAgent* getInstance();
    
    void initialize();    
    void causeCrash();
    void causeException();
    void logDebug(const std::string& message);
    void logInfo(const td::string& message);
    void logError(const std::string& message);
    
private:
    std::unique_ptr<ee::Crashlytics> protocol_;
};
```

```
// CrashlyticsAgent.cpp

#include "CrashlyticsAgent.hpp"

#include <ee/Crashlytics.hpp>
#include <ee/Core.hpp>

CrashlyticsAgent* CrashlyticsAgent::getInstance() {
    static Crashlytics sharedInstance;
    return &sharedInstance;
}

void Crashlytics::initialize() {
    protocol_ = std::make_unique<ee::Crashlytics>();
}

void Crashlytics::causeCrash() {
    protocol_->causeCrash();
}

void Crashlytics::causeException() {
    protocol_->causeException();
}

void Crashlytics::logDebug(const std::string& message) {
    protocol_->log(ee::LogLevel::Debug, "your_log_tag", message);
}

void Crashlytics::logInfo(const td::string& message) {
    protocol_->log(ee::LogLevel::Info, "your_log_tag", message);
}

void Crashlytics::logError(const std::string& message) {
    protocol_->log(ee::LogLevel::Error, "your_log_tag", message);
}
```

## Local Notifications

- Supported platforms: Android, iOS.

- Notes for Android: `mipmap/ic_launcher` (or `mipmap/icon_silhouette` for API 21 and above) will be used as icon for notifications.

- Modify `Android.mk`:

```
LOCAL_STATIC_LIBRARIES += ee_x_notification_static
```

- Modify `AppActivity.java`:

```
import com.ee.Notification;

public class AppActivity extends Cocos2dxActivity {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        PluginManager.getInstance().addPlugin(new Notification(this));
    }
}

```

- Modify `Podfile`:

```
pod 'ee-x/notification', :path => 'CLONE_PATH/ee-x/ee-x.podspec'
```

- Modify `AppController.mm`:

```
#include <ee/Core.hpp>

- (BOOL)application:(UIApplication*)application
    didFinishLaunchingWithOptions:(NSDictionary*)launchOptions {
    [[EEPluginManager getInstance] addPlugin:@"Notification"];
    ...
}
```

- Sample C++ class:

 
```
// NotificationAgent.hpp

#include <ee/NotificationFwd.hpp>

class NotificationAgent {
public:
    static NotificationAgent* getInstance();
    
    void initialize();
    
    /// Should be called when entering background.
    /// E.g. AppDelegate::applicationDidEnterBackground.
    void scheduleAll();
    
    /// Should be called when entering foreground.
    /// E.g. AppDelegate::applicationWillEnterForeground.
    void unscheduleAll();
    
private:
    std::unique_ptr<ee::Notification> protocol_;
};
```

```
// NotificationAgent.cpp

#include "NotificationAgent.hpp"

#include <ee/Notification.hpp>

namespace tag {
constexpr int notification_0 = 0;
constexpr int notification_1 = 1;
} // namespace tag

NotificationAgent* NotificationAgent::getInstance() {
    static NotificationAgent sharedInstance;
    return &sharedInstance;
}

void NotificationAgent::initialize() {
    protocol_ = std::make_unique<ee::Notification>();
}

void NotificationAgent::scheduleAll() {
    // Delayed 5 minutes.
    protocol_->schedule(ee::NotificationBuilder()
                            .setTitle("Title 1")
                            .setBody("Body 2")
                            .setDelay(300)
                            .setTag(tag::notification_0));
    
    // Delayed 10 minutes and repeat every 5 minutes.
    protocol_->schedule(ee::NotificationBuilder()
                            .setTitle("Title 2")
                            .setBody("Body 2")
                            .setDelay(600)
                            .setInterval(300)
                            .setTag(tag::notification_1));
}

void NotificationAgent::unscheduleAll() {
    protocol_->unschedule(tag::notification_0);
    protocol_->unschedule(tag::notification_1);
    
    // Also clear all notifications.
    protocol_->clearAll();
}
```