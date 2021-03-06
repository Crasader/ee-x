cmake_minimum_required(VERSION 3.1)

project(ee_x)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z")

include_directories(
    src/ee/firebase
    third_party/nlohmann/include/ee
    third_party/firebase_cpp_sdk/include
)

include(src/ee/admob/CMakeLists.txt)
include(src/ee/ads/CMakeLists.txt)
include(src/ee/applovin/CMakeLists.txt)
include(src/ee/core/CMakeLists.txt)
include(src/ee/crashlytics/CMakeLists.txt)
include(src/ee/facebook/CMakeLists.txt)
include(src/ee/facebookads/CMakeLists.txt)
include(src/ee/firebase/CMakeLists.txt)
include(src/ee/google/CMakeLists.txt)
include(src/ee/ironsource/CMakeLists.txt)
include(src/ee/notification/CMakeLists.txt)
include(src/ee/unityads/CMakeLists.txt)
include(src/ee/vungle/CMakeLists.txt)
include(src/ee/campaignreceiver/CMakeLists.txt)
include(src/ee/tenjin/CMakeLists.txt)
include(src/ee/appsflyer/CMakeLists.txt)
include(src/ee/play/CMakeLists.txt)

set(LIB_SRC
   jni/cpp/ee/core/JniUtils.cpp
   jni/cpp/ee/core/internal/JniMethodInfo.cpp
   jni/cpp/ee/core/internal/JniString.cpp
   jni/cpp/ee/core/internal/Logger_Android.cpp
   jni/cpp/ee/core/internal/MessageBridge_Android.cpp
   ${EE_X_ADMOB_SRC}
   ${EE_X_ADS_SRC}
   ${EE_X_APPLOVIN_SRC}
   ${EE_X_CORE_SRC}
   ${EE_X_CRASHLYTICS_SRC}
   ${EE_X_FACEBOOK_SRC}
   ${EE_X_FACEBOOK_ADS_SRC}
   ${EE_X_FIREBASE_SRC}
   ${EE_X_GOOGLE_SRC}
   ${EE_X_IRONSOURCE_SRC}
   ${EE_X_NOTIFICATION_SRC}
   ${EE_X_UNITY_ADS_SRC}
   ${EE_X_VUNGLE_SRC}
   ${EE_X_CAMPRECEIVER_SRC}
   ${EE_X_TENJIN_SRC}
   ${EE_X_APPSFLYER_SRC}
   ${EE_X_PLAY_SRC}
)

set(LIB_HEADERS
    jni/cpp/ee/core/JniUtils.hpp
    jni/cpp/ee/core/internal/JnimethodInfo.hpp
    jni/cpp/ee/core/internal/JniString.hpp
    third_party/nlohmann/include/ee/nlohmann/json.hpp
    src/ee/Vungle.hpp
    src/ee/IronSource.hpp
    src/ee/AdsFwd.hpp
    src/ee/FacebookAds.hpp
    src/ee/Firebase.hpp
    src/ee/AppLovinFwd.hpp
    src/ee/AdMob.hpp
    src/ee/UnityAdsFwd.hpp
    src/ee/FacebookFwd.hpp
    src/ee/UnityAds.hpp
    src/ee/FacebookAdsFwd.hpp
    src/ee/AppLovin.hpp
    src/ee/CrashlyticsFwd.hpp
    src/ee/AdMobFwd.hpp
    src/ee/VungleFwd.hpp
    src/ee/GoogleAnalytics.hpp
    src/ee/CoreFwd.hpp
    src/ee/Macro.hpp
    src/ee/GoogleAnalyticsFwd.hpp
    src/ee/IronSourceFwd.hpp
    src/ee/Crashlytics.hpp
    src/ee/Core.hpp
    src/ee/Notification.hpp
    src/ee/NotificationFwd.hpp
    src/ee/FirebaseFwd.hpp
    src/ee/Facebook.hpp
    src/ee/Tenjin.hpp
    src/ee/TenjinFwd.hpp
    src/ee/AppsFlyer.hpp
    src/ee/AppLovinFwd.hpp
    src/ee/AppsFlyer.hpp
    src/ee/AppsFlyerFwd.hpp
    src/ee/Play.hpp
    src/ee/PlayFwd.hpp
    ${EE_X_ADMOB_HEADERS}
    ${EE_X_ADS_HEADERS}
    ${EE_X_APPLOVIN_HEADERS}
    ${EE_X_CORE_HEADERS}
    ${EE_X_CRASHLYTICS_HEADERS}
    ${EE_X_FACEBOOK_HEADERS}
    ${EE_X_FACEBOOK_ADS_HEADERS}
    ${EE_X_FIREBASE_HEADERS}
    ${EE_X_GOOGLE_HEADERS}
    ${EE_X_IRONSOURCE_HEADERS}
    ${EE_X_NOTIFICATION_HEADERS}
    ${EE_X_UNITY_ADS_HEADERS}
    ${EE_X_VUNGLE_HEADERS}
    ${EE_X_CAMPRECEIVER_HEADERS}
    ${EE_X_TENJIN_HEADERS}
    ${EE_X_APPSFLYER_HEADERS}
    ${EE_X_PLAY_HEADERS}
)

add_subdirectory(src/ee/cocos)
add_subdirectory(third_party/soomla/SoomlaCocos2dxStore)

add_library(${PROJECT_NAME} STATIC ${LIB_SRC} ${LIB_HEADERS})

target_include_directories(${PROJECT_NAME} PUBLIC
  src
  jni/cpp
  third_party
  third_party/nlohmann/include/ee
  third_party/firebase_cpp_sdk/include
)

target_link_libraries(${PROJECT_NAME}
    soomla_cocos2dx_store
    ee_x_cocos
    ${CMAKE_CURRENT_LIST_DIR}/third_party/firebase_cpp_sdk/libs/android/${ANDROID_ABI}/libfirebase_dynamic_links.a
    ${CMAKE_CURRENT_LIST_DIR}/third_party/firebase_cpp_sdk/libs/android/${ANDROID_ABI}/libfirebase_app.a
    ${CMAKE_CURRENT_LIST_DIR}/third_party/firebase_cpp_sdk/libs/android/${ANDROID_ABI}/libfirebase_analytics.a
    ${CMAKE_CURRENT_LIST_DIR}/third_party/firebase_cpp_sdk/libs/android/${ANDROID_ABI}/libfirebase_messaging.a
    ${CMAKE_CURRENT_LIST_DIR}/third_party/firebase_cpp_sdk/libs/android/${ANDROID_ABI}/libfirebase_remote_config.a
    ${CMAKE_CURRENT_LIST_DIR}/third_party/firebase_cpp_sdk/libs/android/${ANDROID_ABI}/libfirebase_storage.a
)

set_target_properties(${PROJECT_NAME}
  PROPERTIES
  ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)