//
//  jsb_templates.hpp
//  hello_world-mobile
//
//  Created by Duc Nguyen on 7/13/18.
//

#ifndef JSB_TEMPLATES_HPP__
#define JSB_TEMPLATES_HPP__

#include <type_traits>
#include <unordered_map>

#include "cocos/scripting/js-bindings/jswrapper/SeApi.h"
#include "cocos/scripting/js-bindings/manual/jsb_conversions.hpp"
#include "cocos/scripting/js-bindings/manual/jsb_helper.hpp"

#include <ee/Core.hpp>
#include <ee/nlohmann/json.hpp>

namespace ee {
namespace core {

template <typename T>
se::Object* create_JSON_object(const T& value);

template <>
inline se::Object* create_JSON_object(const std::pair<float, float>& value) {
    auto&& jsonArray = nlohmann::json::array();
    jsonArray.push_back(value.first);
    jsonArray.push_back(value.second);
    return se::Object::createJSONObject(jsonArray.dump());
}

template <>
inline se::Object* create_JSON_object(const std::pair<int, int>& value) {
    auto&& jsonArray = nlohmann::json::array();
    jsonArray.push_back(value.first);
    jsonArray.push_back(value.second);
    return se::Object::createJSONObject(jsonArray.dump());
}

template <>
inline se::Object*
create_JSON_object(const std::map<std::string, std::string>& value) {
    auto&& jsonObj = nlohmann::json();
    for (auto it = value.begin(); it != value.end(); it++) {
        jsonObj[it->first] = it->second;
    }
    return se::Object::createJSONObject(jsonObj.dump());
}

template <>
inline se::Object*
create_JSON_object(const std::unordered_map<std::string, std::string>& value) {
    auto&& jsonObj = nlohmann::json();
    for (auto it = value.begin(); it != value.end(); it++) {
        jsonObj[it->first] = it->second;
    }
    return se::Object::createJSONObject(jsonObj.dump());
}

template <>
inline se::Object* create_JSON_object(const int& value) {
    auto&& jsonObj = nlohmann::json();
    jsonObj.push_back(value);
    return se::Object::createJSONObject(jsonObj.dump());
}

template <>
inline se::Object* create_JSON_object(const std::string& value) {
    auto&& jsonObj = nlohmann::json();
    jsonObj.push_back(value);
    return se::Object::createJSONObject(jsonObj.dump());
}

template <>
inline se::Object* create_JSON_object(const bool& value) {
    auto&& jsonObj = nlohmann::json();
    jsonObj.push_back(value);
    return se::Object::createJSONObject(jsonObj.dump());
}

template <>
inline se::Object* create_JSON_object(const std::vector<std::string>& value) {
    auto&& jsonArray = nlohmann::json::array();
    for (auto it = value.begin(); it != value.end(); it++) {
        jsonArray.push_back(*it);
    }
    return se::Object::createJSONObject(jsonArray.dump());
}

template <typename T>
T from_JSON_object(se::Object* jsonObj);

template <>
inline std::unordered_map<std::string, std::string>
from_JSON_object(se::Object* jsonObj) {
    std::vector<std::string> allKeys;
    std::unordered_map<std::string, std::string> ret;

    jsonObj->getAllKeys(&allKeys);
    for (auto& key : allKeys) {
        se::Value value;
        jsonObj->getProperty(key.c_str(), &value);
        ret[key] = value.toString();
    }
    return ret;
}

template <>
inline std::unordered_map<std::string, cocos2d::Value>
from_JSON_object(se::Object* jsonObj) {
    std::vector<std::string> allKeys;
    std::unordered_map<std::string, cocos2d::Value> ret;

    jsonObj->getAllKeys(&allKeys);
    for (auto& key : allKeys) {
        se::Value value;
        jsonObj->getProperty(key.c_str(), &value);

        if (value.isNumber()) {
            ret[key] = cocos2d::Value(value.toNumber());
        } else if (value.isString()) {
            ret[key] = cocos2d::Value(value.toString());
        } else if (value.isBoolean()) {
            ret[key] = cocos2d::Value(value.toBoolean());
        } else {
            ret[key] = cocos2d::Value::Null;
        }
    }
    return ret;
}

template <>
inline std::map<std::string, std::string>
from_JSON_object(se::Object* jsonObj) {
    std::vector<std::string> allKeys;
    std::map<std::string, std::string> ret;

    jsonObj->getAllKeys(&allKeys);
    for (auto& key : allKeys) {
        se::Value value;
        jsonObj->getProperty(key.c_str(), &value);
        ret[key] = value.toString();
    }
    return ret;
}

template <typename T>
T get_value(const se::Value& value);

template <>
inline bool get_value(const se::Value& value) {
    return value.toBoolean();
}

template <>
inline std::int32_t get_value(const se::Value& value) {
    return value.toInt32();
}

template <>
inline std::int64_t get_value(const se::Value& value) {
    return static_cast<std::int64_t>(value.toLong());
}

template <>
inline float get_value(const se::Value& value) {
    return value.toFloat();
}

template <>
inline double get_value(const se::Value& value) {
    return value.toNumber();
}

template <>
inline std::string get_value(const se::Value& value) {
    return value.toString();
}

template <>
inline const std::string& get_value(const se::Value& value) {
    return value.toString();
}

template <>
inline std::unordered_map<std::string, std::string>
get_value(const se::Value& value) {
    return from_JSON_object<std::unordered_map<std::string, std::string>>(
        value.toObject());
}

template <>
inline std::map<std::string, std::string> get_value(const se::Value& value) {
    return from_JSON_object<std::map<std::string, std::string>>(
        value.toObject());
}

template <>
inline std::unordered_map<std::string, cocos2d::Value>
get_value(const se::Value& value) {
    return from_JSON_object<std::unordered_map<std::string, cocos2d::Value>>(
        value.toObject());
}

template <>
inline cocos2d::Value get_value(const se::Value& value) {
    if (value.isNumber()) {
        return cocos2d::Value(value.toNumber());
    } else if (value.isString()) {
        return cocos2d::Value(value.toString());
    } else if (value.isBoolean()) {
        return cocos2d::Value(value.toBoolean());
    } else {
        return cocos2d::Value::Null;
    }
}

template <typename T>
void set_value(se::Value& value, T input);

template <>
inline void set_value(se::Value& value, std::int32_t input) {
    value.setInt32(input);
}

template <>
inline void set_value(se::Value& value, std::int64_t input) {
    auto temp = static_cast<std::int32_t>(input);
    value.setInt32(temp);
}

template <>
inline void set_value(se::Value& value, double input) {
    value.setFloat(input);
}

template <>
inline void set_value(se::Value& value, float input) {
    value.setFloat(input);
}

template <>
inline void set_value(se::Value& value, std::string input) {
    value.setString(input);
}

template <>
inline void set_value(se::Value& value, const char* input) {
    value.setString(input);
}

template <>
inline void set_value(se::Value& value, bool input) {
    value.setBoolean(input);
}

template <>
inline void set_value(se::Value& value, se::Object* obj) {
    value.setObject(obj);
}

template <>
inline void set_value(se::Value& value, std::pair<float, float> input) {
    auto obj = create_JSON_object<std::pair<float, float>>(input);
    value.setObject(obj);
}

template <>
inline void set_value(se::Value& value, std::pair<int, int> input) {
    auto obj = create_JSON_object<std::pair<int, int>>(input);
    value.setObject(obj);
}

template <>
inline void set_value(se::Value& value,
                      std::map<std::string, std::string> input) {
    auto obj = create_JSON_object<std::map<std::string, std::string>>(input);
    value.setObject(obj);
}

template <typename T>
inline void set_value_from_pointer(se::Value& value, T* input) {
    if (input != nullptr) {
        auto clazz = JSBClassType::findClass(input);
        CCASSERT(clazz, "ERROR: Class is not registered yet.");
        se::Object* obj = nullptr;
        auto found = se::NativePtrToObjectMap::find(input);
        if (found != se::NativePtrToObjectMap::end()) {
            obj = found->second;
        } else {
            obj = se::Object::createObjectWithClass(clazz);
            obj->setPrivateData(input);
        }
        value.setObject(obj);

        if (std::is_convertible<T*, cocos2d::Ref*>::value) {
            static_cast<cocos2d::Ref*>(input)->retain();
        }
    } else {
        value.setNull();
    }
}

template <typename... Args>
se::ValueArray to_value_array(Args... values) {
    se::ValueArray args = {
        se::Value(create_JSON_object(std::forward<Args>(values)))...};
    return args;
}

template <auto FunctionPtr, typename... Args, std::size_t... Indices>
auto call_static_func(const se::ValueArray& args,
                      std::index_sequence<Indices...>) {
    return (*FunctionPtr)(get_value<Args>(args[Indices])...);
}

template <typename InstanceType, auto FunctionPtr, typename... Args,
          std::size_t... Indices>
auto call_instance_func(InstanceType* instance, const se::ValueArray& args,
                        std::index_sequence<Indices...>) {
    return (instance->*FunctionPtr)(get_value<Args>(args[Indices])...);
}

template <typename T, typename... Args, std::size_t... Indices>
auto make_object(const se::ValueArray& args, std::index_sequence<Indices...>) {
    return new T(get_value<Args>(args[Indices])...);
}

template <typename InstanceType>
void jsb_dispose_callback(InstanceType* instance) {
    se::Object* seObj = nullptr;

    auto iter = se::NativePtrToObjectMap::find(instance);
    if (iter != se::NativePtrToObjectMap::end()) {
        // Save se::Object pointer for being used in cleanup method.
        seObj = iter->second;
        // Unmap native and js object since native object was destroyed.
        // Otherwise, it may trigger 'assertion' in se::Object::setPrivateData
        // later since native obj is already released and the new native object
        // may be assigned with the same address.
        se::NativePtrToObjectMap::erase(iter);
    } else {
        return;
    }

    auto cleanup = [seObj]() {
        auto se = se::ScriptEngine::getInstance();
        if (!se->isValid() || se->isInCleanup())
            return;

        se::AutoHandleScope hs;
        se->clearException();

        // The mapping of native object & se::Object was cleared in above code.
        // The private data (native object) may be a different object associated
        // with other se::Object. Therefore, don't clear the mapping again.
        seObj->clearPrivateData(false);
        seObj->unroot();
        seObj->decRef();
    };

    if (!se::ScriptEngine::getInstance()->isGarbageCollecting()) {
        cleanup();
    } else {
        CleanupTask::pushTaskToAutoReleasePool(cleanup);
    }
}

template <typename T, typename... Args>
bool jsb_constructor(se::State& s) {
    auto argc = sizeof...(Args);
    const auto& args = s.args();

    using Indices = std::make_index_sequence<sizeof...(Args)>;

    if (argc == args.size()) {
        auto cObj = make_object<T, Args...>(args, Indices());
        s.thisObject()->setPrivateData(cObj);
        return true;
    }
    SE_REPORT_ERROR("Wrong number of arguments: %ld, was expecting: %d.", argc,
                    2);
    return false;
}

template <typename T, typename... Args>
bool jsb_constructor_with_dispose_callback(se::State& s) {
    auto argc = sizeof...(Args);
    const auto& args = s.args();

    using Indices = std::make_index_sequence<sizeof...(Args)>;

    if (argc == args.size()) {
        auto cObj = make_object<T, Args...>(args, Indices());
        cObj->setDisposeCallback(&jsb_dispose_callback<T>);
        s.thisObject()->setPrivateData(cObj);
        return true;
    }
    SE_REPORT_ERROR("Wrong number of arguments: %ld, was expecting: %d.", argc,
                    2);
    return false;
}

template <typename T>
bool jsb_finalize(se::State& s) {
    if (std::is_convertible<T*, cocos2d::Ref*>::value) {
        static_cast<cocos2d::Ref*>(s.nativeThisObject())->release();
    } else {
        std::shared_ptr<T>* casted =
            static_cast<std::shared_ptr<T>*>(s.nativeThisObject());
        if (casted == nullptr) {
            T* cObj = static_cast<T*>(s.nativeThisObject());
            delete cObj;
        }
    }
    return true;
}

template <auto Function, typename... Args>
bool jsb_static_call(se::State& s) {
    auto argc = sizeof...(Args);
    const auto& args = s.args();

    using Indices = std::make_index_sequence<sizeof...(Args)>;

    if (argc == args.size()) {
        call_static_func<Function, Args...>(args, Indices());
        return true;
    }
    SE_REPORT_ERROR("Wrong number of arguments: %ld, was expecting: %ld.",
                    args.size(), argc);
    return false;
}

template <typename ReturnType, auto FunctionPtr, typename... Args>
bool jsb_static_get(se::State& s) {
    auto argc = sizeof...(Args);
    const auto& args = s.args();

    using Indices = std::make_index_sequence<sizeof...(Args)>;

    if (argc == args.size()) {
        auto result = call_static_func<FunctionPtr, Args...>(args, Indices());
        set_value<ReturnType>(s.rval(), result);
        return true;
    }

    SE_REPORT_ERROR("Wrong number of arguments: %ld, was expecting: %ld.",
                    args.size(), argc);
    return false;
}

template <typename ReturnType, auto MemberPtr>
bool jsb_static_property_get(se::State& s) {
    set_value<ReturnType>(s.rval(), *MemberPtr);
    return true;
}

template <typename InstanceType, auto FunctionPtr, typename... Args>
bool jsb_method_call(se::State& s) {
    auto argc = sizeof...(Args);
    const auto& args = s.args();
    using Indices = std::make_index_sequence<sizeof...(Args)>;

    if (argc == args.size()) {
        auto cObj = static_cast<InstanceType*>(s.nativeThisObject());
        call_instance_func<InstanceType, FunctionPtr, Args...>(cObj, args,
                                                               Indices());
        return true;
    }

    SE_REPORT_ERROR("Wrong number of arguments: %ld, was expecting: %ld.",
                    args.size(), argc);
    return false;
}

template <typename InstanceType, auto FunctionPtr, typename... Args>
bool jsb_method_call_on_ui_thread(se::State& s) {
    auto argc = sizeof...(Args);
    const auto& args = s.args();
    using Indices = std::make_index_sequence<sizeof...(Args)>;

    if (argc == args.size()) {
        auto cObj = static_cast<InstanceType*>(s.nativeThisObject());
        ee::runOnUiThread([cObj, args] {
            call_instance_func<InstanceType, FunctionPtr, Args...>(cObj, args,
                                                                   Indices());
        });
        return true;
    }

    SE_REPORT_ERROR("Wrong number of arguments: %ld, was expecting: %ld.",
                    args.size(), argc);
    return false;
}

template <typename InstanceType, auto FunctionPtr, typename... Args>
bool jsb_method_call_on_ui_thread_and_wait(se::State& s) {
    auto argc = sizeof...(Args);
    const auto& args = s.args();
    using Indices = std::make_index_sequence<sizeof...(Args)>;

    if (argc == args.size()) {
        auto cObj = static_cast<InstanceType*>(s.nativeThisObject());
        ee::runOnUiThreadAndWait([cObj, args] {
            call_instance_func<InstanceType, FunctionPtr, Args...>(cObj, args,
                                                                   Indices());
        });
        return true;
    }

    SE_REPORT_ERROR("Wrong number of arguments: %ld, was expecting: %ld.",
                    args.size(), argc);
    return false;
}

template <typename InstanceType, auto FunctionPtr, typename ReturnType,
          typename... Args>
bool jsb_method_get(se::State& s) {
    auto argc = sizeof...(Args);
    const auto& args = s.args();
    using Indices = std::make_index_sequence<sizeof...(Args)>;

    if (argc == args.size()) {
        auto cObj = static_cast<InstanceType*>(s.nativeThisObject());
        set_value<ReturnType>(
            s.rval(), call_instance_func<InstanceType, FunctionPtr, Args...>(
                          cObj, args, Indices()));
        return true;
    }

    SE_REPORT_ERROR("Wrong number of arguments: %ld, was expecting: %ld.",
                    args.size(), argc);
    return false;
}

template <typename InstanceType, auto FunctionPtr, typename ReturnType,
          typename... Args>
bool jsb_method_get_on_ui_thread(se::State& s) {
    auto argc = sizeof...(Args);
    const auto& args = s.args();
    using Indices = std::make_index_sequence<sizeof...(Args)>;

    if (argc == args.size()) {
        auto cObj = static_cast<InstanceType*>(s.nativeThisObject());
        set_value<ReturnType>(
            s.rval(),
            ee::runOnUiThreadAndWaitResult<ReturnType>(
                [cObj, args]() -> ReturnType {
                    return std::forward<ReturnType>(
                        call_instance_func<InstanceType, FunctionPtr, Args...>(
                            cObj, args, Indices()));
                }));
        return true;
    }

    SE_REPORT_ERROR("Wrong number of arguments: %ld, was expecting: %ld.",
                    args.size(), argc);
    return false;
}

template <typename InstanceType, auto FunctionPtr, typename ReturnType>
bool jsb_accessor_get(se::State& s) {
    auto cObj = static_cast<InstanceType*>(s.nativeThisObject());
    set_value<ReturnType>(s.rval(), std::bind(FunctionPtr, cObj)());
    return true;
}

template <typename InstanceType, auto FunctionPtr, typename ReturnType>
bool jsb_accessor_get_on_ui_thread(se::State& s) {
    auto cObj = static_cast<InstanceType*>(s.nativeThisObject());
    set_value<ReturnType>(
        s.rval(),
        ee::runOnUiThreadAndWaitResult<ReturnType>(
            [cObj]() -> ReturnType { return std::bind(FunctionPtr, cObj)(); }));
    return true;
}

template <typename InstanceType, auto FunctionPtr, typename ArgumentType>
bool jsb_accessor_set(se::State& s) {
    const auto& args = s.args();
    if (args.size() == 1) {
        auto* cObj = static_cast<InstanceType*>(s.nativeThisObject());
        std::bind(FunctionPtr, cObj, get_value<ArgumentType>(args[0]))();
        return true;
    }
    SE_REPORT_ERROR("Wrong number of arguments: %ld, was expecting: %d.",
                    args.size(), 1);
    return false;
}

template <typename InstanceType, auto FunctionPtr, typename ArgumentType>
bool jsb_accessor_set_on_ui_thread(se::State& s) {
    const auto& args = s.args();
    if (args.size() == 1) {
        auto* cObj = static_cast<InstanceType*>(s.nativeThisObject());
        ee::runOnUiThread([cObj, args] {
            (cObj->*FunctionPtr)(get_value<ArgumentType>(args[0]));
        });
        return true;
    }
    SE_REPORT_ERROR("Wrong number of arguments: %ld, was expecting: %d.",
                    args.size(), 1);
    return false;
}

template <typename InstanceType, auto MemberPtr, typename ReturnType>
bool jsb_propterty_get(se::State& s) {
    auto cObj = static_cast<InstanceType*>(s.nativeThisObject());
    auto input(cObj->*MemberPtr);
    set_value<ReturnType>(s.rval(), std::move(input));
    return true;
}

template <typename InstanceType, auto MemberPtr, typename ArgumentType>
bool jsb_propterty_set(se::State& s) {
    const auto& args = s.args();
    if (args.size() == 1) {
        auto* cObj = static_cast<InstanceType*>(s.nativeThisObject());
        cObj->*MemberPtr = get_value<std::decay_t<ArgumentType>>(args[0]);
        return true;
    }
    SE_REPORT_ERROR("Wrong number of arguments: %ld, was expecting: %d.",
                    args.size(), 1);
    return false;
}

template <typename InstanceType, auto FunctionPtr, typename... Args>
bool jsb_set_callback(se::State& s) {
    const auto& args = s.args();
    auto argc = args.size();
    if (argc >= 1) {
        InstanceType* cObj = static_cast<InstanceType*>(s.nativeThisObject());

        se::Value jsFunc = args[0];
        se::Value jsTarget = argc > 1 ? args[1] : se::Value::Undefined;

        if (jsFunc.isNullOrUndefined()) {
            std::bind(FunctionPtr, cObj, nullptr)();
        } else {
            assert(jsFunc.isObject() && jsFunc.toObject()->isFunction());

            s.thisObject()->attachObject(jsFunc.toObject());

            if (jsTarget.isObject()) {
                s.thisObject()->attachObject(jsTarget.toObject());
            }

            std::bind(FunctionPtr, cObj, [jsFunc, jsTarget](Args... values) {
                cocos2d::Director::getInstance()
                    ->getScheduler()
                    ->performFunctionInCocosThread([=]() -> void {
                        se::ScriptEngine::getInstance()->clearException();
                        se::AutoHandleScope hs;

                        auto&& args = to_value_array(values...);
                        se::Object* target =
                            jsTarget.isObject() ? jsTarget.toObject() : nullptr;
                        se::Object* func = jsFunc.toObject();
                        func->call(args, target);
                    });
            })();
        }

        return true;
    }
    SE_REPORT_ERROR("Wrong number of arguments: %ld, was expecting: %d.",
                    args.size(), 1);
    return false;
}
} // namespace core
} // namespace ee

#endif /* JSB_TEMPLATES_HPP__ */
