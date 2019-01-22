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

#include <ee/nlohmann/json.hpp>

#include <ee/Core.hpp>

#include "cocos/scripting/js-bindings/jswrapper/SeApi.h"
#include "cocos/scripting/js-bindings/manual/jsb_conversions.hpp"
#include "cocos/scripting/js-bindings/manual/jsb_helper.hpp"
#include "ee/jsb/core/jsb_utils.hpp"

namespace ee {
namespace core {
/// Converts between se::Object and normal value.
template <typename T>
se::Object* create_JSON_object(const T& value);

/// Converts between se::Object and normal value.
template <typename T>
T from_JSON_object(se::Object* object);

/// Converts between se::Value and normal value.
template <typename T>
T get_value(const se::Value& value);

/// Converts between se::Value and normal value.
template <typename T>
void set_value(se::Value& value, T input);

template <typename T>
void set_value_from_pointer(se::Value& value, T* input) {
    if (input == nullptr) {
        value.setNull();
        return;
    }
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

    if constexpr (std::is_convertible<T*, cocos2d::Ref*>::value) {
        static_cast<cocos2d::Ref*>(input)->retain();
    }
}

namespace internal {
template <class T>
se::Value to_value(T&& arg) {
    se::Value value;
    set_value(value, std::forward<T>(arg));
    return value;
}

/// https://stackoverflow.com/questions/28410697/c-convert-vector-to-tuple
template <class T, std::size_t... Indices>
decltype(auto) makeTupleImpl(const std::vector<T>& args,
                             std::index_sequence<Indices...>) {
    return std::make_tuple(args[Indices]...);
}

template <std::size_t N, class T>
decltype(auto) makeTuple(const std::vector<T>& args) {
    return makeTupleImpl(args, std::make_index_sequence<N>());
}
} // namespace internal

template <typename... Args>
se::ValueArray to_value_array(Args&&... values) {
    se::ValueArray args = {internal::to_value(std::forward<Args>(values))...};
    return args;
}

namespace internal {
/// https://stackoverflow.com/questions/36797770/get-function-parameters-count
template <class ResultType, class... ArgTypes>
struct FunctionTraits {
    using Result = ResultType;
    using Args = std::tuple<ArgTypes...>;
};

template <class ResultType, class InstanceType, class... ArgTypes>
struct MemberFunctionTraits : FunctionTraits<ResultType, ArgTypes...> {
    using Instance = InstanceType;
};

template <class PropertyType>
struct PropertyTraits {
    using Property = PropertyType;
};

template <class PropertyType, class InstanceType>
struct MemberPropertyTraits {
    using Property = PropertyType;
    using Instance = InstanceType;
};

/// Static functions.
template <class Result, class... Args>
constexpr auto makeFunctionTraits(Result(Args...)) {
    return FunctionTraits<Result, Args...>();
}

/// Member functions.
template <class Result, class Instance, class... Args>
constexpr auto makeFunctionTraits(Result (Instance::*)(Args...)) {
    return MemberFunctionTraits<Result, Instance, Args...>();
}

/// Const member functions.
template <class Result, class Instance, class... Args>
constexpr auto makeFunctionTraits(Result (Instance::*)(Args...) const) {
    return MemberFunctionTraits<Result, Instance, Args...>();
}

/// Static properties.
template <class Property>
constexpr auto makePropertyTraits(Property*) {
    return PropertyTraits<Property>();
}

/// Member properties.
template <class Property, class Instance>
constexpr auto makePropertyTraits(Property(Instance::*)) {
    return MemberPropertyTraits<Property, Instance>();
}
} // namespace internal

template <auto Function>
using FunctionInstance =
    typename decltype(internal::makeFunctionTraits(Function))::Instance;

template <auto Function>
using FunctionResult =
    typename decltype(internal::makeFunctionTraits(Function))::Result;

template <auto Function>
using FunctionArgs =
    typename decltype(internal::makeFunctionTraits(Function))::Args;

template <auto Property>
using PropertyInstance =
    typename decltype(internal::makePropertyTraits(Property))::Instance;

template <auto Property>
using PropertyType =
    typename decltype(internal::makePropertyTraits(Property))::Property;

template <auto Function>
constexpr auto FunctionArity = std::tuple_size_v<FunctionArgs<Function>>;

template <class T>
using UnifyType =
    std::conditional_t<std::is_pointer_v<T>, T, std::add_lvalue_reference_t<T>>;

namespace internal {
template <auto Function, std::size_t... Indices>
decltype(auto) callStaticFunctionImpl(const se::ValueArray& args,
                                      std::index_sequence<Indices...>) {
    using Args = FunctionArgs<Function>;
    return std::invoke(
        Function, get_value<std::decay_t<std::tuple_element_t<Indices, Args>>>(
                      args[Indices])...);
}

template <auto Function, class Instance, std::size_t... Indices>
decltype(auto) callInstanceFunctionImpl(Instance* instance,
                                        const se::ValueArray& args,
                                        std::index_sequence<Indices...>) {
    using Args = FunctionArgs<Function>;
    return std::invoke(
        Function, instance,
        get_value<std::decay_t<std::tuple_element_t<Indices, Args>>>(
            args[Indices])...);
}

template <class Instance, class... Args, std::size_t... Indices>
decltype(auto) makeObjectImpl(const se::ValueArray& args,
                              std::index_sequence<Indices...>) {
    return std::make_unique<Instance>(
        get_value<std::decay_t<Args>>(args[Indices])...);
}
} // namespace internal

/// Calls a static function.
template <auto Function>
decltype(auto) callStaticFunction(const se::ValueArray& args) {
    constexpr auto Arity = FunctionArity<Function>;
    return internal::callStaticFunctionImpl<Function>(
        args, std::make_index_sequence<Arity>());
}

/// Calls a member instance function.
template <auto Function, class Instance>
decltype(auto) callInstanceFunction(Instance* instance,
                                    const se::ValueArray& args) {
    constexpr auto Arity = FunctionArity<Function>;
    return internal::callInstanceFunctionImpl<Function, Instance>(
        instance, args, std::make_index_sequence<Arity>());
}

/// Instantiates a new object (unique_ptr).
template <class Instance, class... Args>
decltype(auto) makeObject(const se::ValueArray& args) {
    return internal::makeObjectImpl<Instance, Args...>(
        args, std::make_index_sequence<sizeof...(Args)>());
}

template <class Instance>
void makeDisposeCallback(Instance* instance) {
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

template <class Instance, class... Args>
bool makeConstructor(se::State& state) {
    constexpr auto Arity = sizeof...(Args);
    auto&& args = state.args();
    if (Arity == args.size()) {
        auto object = makeObject<Instance, Args...>(args).release();
        state.thisObject()->setPrivateData(object);
        return true;
    }
    SE_REPORT_ERROR("Wrong number of arguments: %zu, was expecting: %zu.",
                    args.size(), Arity);
    return false;
}

template <typename T, typename... Args>
[[deprecated("Use makeConstructor")]] bool jsb_constructor(se::State& s) {
    return makeConstructor<T, Args...>(s);
}

template <class Instance, class... Args>
bool makeConstructorWithDisposeCallback(se::State& state) {
    constexpr auto Arity = sizeof...(Args);
    auto&& args = state.args();
    if (Arity == args.size()) {
        auto object = makeObject<Instance, Args...>(args).release();
        object->setDisposeCallback(&makeDisposeCallback<Instance>);
        state.thisObject()->setPrivateData(object);
        return true;
    }
    SE_REPORT_ERROR("Wrong number of arguments: %zu, was expecting: %zu.",
                    args.size(), Arity);
    return false;
}

template <typename T, typename... Args>
[[deprecated("Use makeConstructorWithDisposeCallback")]] bool
jsb_constructor_with_dispose_callback(se::State& s) {
    return makeConstructorWithDisposeCallback<T, Args...>(s);
}

template <class T>
bool makeFinalize(se::State& state) {
    auto&& object = state.nativeThisObject();
    if constexpr (std::is_convertible_v<T*, cocos2d::Ref*>) {
        static_cast<cocos2d::Ref*>(object)->release();
    } else {
        auto&& pointer = static_cast<std::shared_ptr<T>*>(object);
        if (pointer == nullptr) {
            delete static_cast<T*>(object);
        }
    }
    return true;
}

template <typename T>
[[deprecated("Use makeFinalize")]] bool jsb_finalize(se::State& s) {
    return makeFinalize<T>(s);
}

template <auto Function>
bool makeStaticMethod(se::State& state) {
    constexpr auto Arity = FunctionArity<Function>;
    auto&& args = state.args();
    if (Arity == args.size()) {
        using Result = FunctionResult<Function>;
        if constexpr (std::is_same_v<Result, void>) {
            callStaticFunction<Function>(args);
        } else {
            auto&& result = callStaticFunction<Function>(args);
            set_value<UnifyType<Result>>(state.rval(), result);
        }
        return true;
    }
    SE_REPORT_ERROR("Wrong number of arguments: %zu, was expecting: %zu.",
                    args.size(), Arity);
    return false;
}

template <auto Function>
bool makeInstanceMethod(se::State& state) {
    constexpr auto Arity = FunctionArity<Function>;
    auto&& args = state.args();
    if (Arity == args.size()) {
        using Instance = FunctionInstance<Function>;
        auto&& instance = static_cast<Instance*>(state.nativeThisObject());
        using Result = FunctionResult<Function>;
        if constexpr (std::is_same_v<Result, void>) {
            callInstanceFunction<Function>(instance, args);
        } else {
            auto&& result = callInstanceFunction<Function>(instance, args);
            set_value<UnifyType<Result>>(state.rval(), result);
        }
        return true;
    }
    SE_REPORT_ERROR("Wrong number of arguments: %zu, was expecting: %zu.",
                    args.size(), Arity);
    return false;
}

template <auto Function>
bool makeInstanceMethodOnUiThread(se::State& state) {
    constexpr auto Arity = FunctionArity<Function>;
    auto&& args = state.args();
    if (Arity == args.size()) {
        using Instance = FunctionInstance<Function>;
        auto&& instance = static_cast<Instance*>(state.nativeThisObject());
        using Result = FunctionResult<Function>;
        if constexpr (std::is_same_v<Result, void>) {
            runOnUiThread([instance, args] {
                callInstanceFunction<Function>(instance, args);
            });
        } else {
            return false;
        }
        return true;
    }
    SE_REPORT_ERROR("Wrong number of arguments: %zu, was expecting: %zu.",
                    args.size(), Arity);
    return false;
}

template <auto Function>
bool makeInstanceMethodOnUiThreadAndWait(se::State& state) {
    constexpr auto Arity = FunctionArity<Function>;
    auto&& args = state.args();
    if (Arity == args.size()) {
        using Instance = FunctionInstance<Function>;
        auto&& instance = static_cast<Instance*>(state.nativeThisObject());
        using Result = FunctionResult<Function>;
        if constexpr (std::is_same_v<Result, void>) {
            runOnUiThreadAndWait([instance, args] {
                callInstanceFunction<Function>(instance, args);
            });
        } else {
            auto&& result =
                runOnUiThreadAndWaitResult<Result>([instance, args] {
                    return callInstanceFunction<Function>(instance, args);
                });
            set_value<UnifyType<Result>>(state.rval(), result);
        }
        return true;
    }
    SE_REPORT_ERROR("Wrong number of arguments: %zu, was expecting: %zu.",
                    args.size(), Arity);
    return false;
}

template <auto Property>
bool makeStaticProperty(se::State& state) {
    using Type = PropertyType<Property>;
    set_value<UnifyType<Type>>(state.rval(), *Property);
    return true;
}

template <auto Property>
bool makeInstanceProperty(se::State& state) {
    using Instance = PropertyInstance<Property>;
    auto instance = static_cast<Instance*>(state.nativeThisObject());
    using Type = PropertyType<Property>;
    set_value<UnifyType<Type>>(state.rval(), instance->*Property);
    return true;
}

template <auto Function, typename... Args>
[[deprecated("Use makeStaticMethod")]] bool jsb_static_call(se::State& s) {
    return makeStaticMethod<Function>(s);
}

template <typename ReturnType, auto FunctionPtr, typename... Args>
[[deprecated("Use makeStaticMethod")]] bool jsb_static_get(se::State& s) {
    return makeStaticMethod<FunctionPtr>(s);
}

template <typename ReturnType, auto MemberPtr>
[[deprecated("Use makeStaticProperty")]] bool jsb_static_property_get(
    se::State& s) { //
    return makeStaticProperty<MemberPtr>(s);
}

template <typename InstanceType, auto FunctionPtr, typename... Args>
[[deprecated("Use makeInstanceMethod")]] bool jsb_method_call(se::State& s) {
    return makeInstanceMethod<FunctionPtr>(s);
}

template <typename InstanceType, auto FunctionPtr, typename... Args>
[[deprecated("Use makeInstanceMethodOnUiThread")]] //
    bool jsb_method_call_on_ui_thread(se::State& s) {
        return makeInstanceMethodOnUiThread<FunctionPtr>(s);
    }

template <typename InstanceType, auto FunctionPtr, typename... Args>
[[deprecated("Use makeInstanceMethodOnUiThreadAndWait")]] //
    bool jsb_method_call_on_ui_thread_and_wait(se::State& s) {
        return makeInstanceMethodOnUiThreadAndWait<FunctionPtr>(s);
    }

template <typename InstanceType, auto FunctionPtr, typename ReturnType,
          typename... Args>
[[deprecated("Use makeInstanceMethod")]] bool jsb_method_get(se::State& s) {
    return makeInstanceMethod<FunctionPtr>(s);
}

template <typename InstanceType, auto FunctionPtr, typename ReturnType,
          typename... Args>
[[deprecated("Use makeInstanceMethodOnUiThreadAndWait")]] //
    bool jsb_method_get_on_ui_thread(se::State& s) {
        return makeInstanceMethodOnUiThreadAndWait<FunctionPtr>(s);
    }

template <typename InstanceType, auto FunctionPtr, typename ReturnType>
[[deprecated("Use makeInstanceMethod")]] bool jsb_accessor_get(se::State& s) {
    return makeInstanceMethod<FunctionPtr>(s);
}

template <typename InstanceType, auto FunctionPtr, typename ReturnType>
[[deprecated("Use makeInstanceMethodOnUiThreadAndWait")]] //
    bool jsb_accessor_get_on_ui_thread(se::State& s) {
        return makeInstanceMethodOnUiThreadAndWait<FunctionPtr>(s);
    }

template <typename InstanceType, auto FunctionPtr, typename ArgumentType>
[[deprecated("Use makeInstanceMethod")]] bool jsb_accessor_set(se::State& s) {
    return makeInstanceMethod<FunctionPtr>(s);
}

template <typename InstanceType, auto FunctionPtr, typename ArgumentType>
[[deprecated("Use makeInstanceMethodOnUiThread")]] //
    bool jsb_accessor_set_on_ui_thread(se::State& s) {
        return makeInstanceMethodOnUiThread<FunctionPtr>(s);
    }

template <typename InstanceType, auto MemberPtr, typename ReturnType>
[[deprecated("Use makeInstanceProperty")]] bool jsb_propterty_get(
    se::State& s) { //
    return makeInstanceProperty<MemberPtr>(s);
}

template <typename InstanceType, auto MemberPtr, typename ArgumentType>
bool jsb_propterty_set(se::State& s) {
    const auto& args = s.args();
    if (args.size() == 1) {
        auto cObj = static_cast<InstanceType*>(s.nativeThisObject());
        cObj->*MemberPtr = get_value<std::decay_t<ArgumentType>>(args[0]);
        return true;
    }
    SE_REPORT_ERROR("Wrong number of arguments: %zu, was expecting: %d.",
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

            std::bind(FunctionPtr, cObj, [jsFunc, jsTarget](auto&&... values) {
                runOnCocosThread([=] {
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
    SE_REPORT_ERROR("Wrong number of arguments: %zu, was expecting: %d.",
                    args.size(), 1);
    return false;
}

template <auto FunctionPtr, typename... Args>
bool jsb_static_set_callback(se::State& s) {
    const auto& args = s.args();
    auto argc = args.size();
    if (argc >= 1) {
        se::Value jsFunc = args[0];
        se::Value jsTarget = argc > 1 ? args[1] : se::Value::Undefined;

        if (jsFunc.isNullOrUndefined()) {
            std::bind(FunctionPtr, nullptr)();
        } else {
            assert(jsFunc.isObject() && jsFunc.toObject()->isFunction());

            s.thisObject()->attachObject(jsFunc.toObject());

            if (jsTarget.isObject()) {
                s.thisObject()->attachObject(jsTarget.toObject());
            }

            std::bind(FunctionPtr, [jsFunc, jsTarget](auto&&... values) {
                runOnCocosThread([=] {
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
    SE_REPORT_ERROR("Wrong number of arguments: %zu, was expecting: %d.",
                    args.size(), 1);
    return false;
}

template <class T>
class SharedPtrHandler {
public:
    static std::unique_ptr<SharedPtrHandler> create(se::Class* clazz) {
        return std::make_unique<SharedPtrHandler<T>>(clazz);
    }

    explicit SharedPtrHandler(se::Class* clazz)
        : clazz_(clazz) {}

    se::Class* getClass() {
        return clazz_;
    }

    std::shared_ptr<T> getValue(const se::Value& value) const {
        auto delegatePtr = static_cast<T*>(value.toObject()->getPrivateData());
        auto iter = std::find_if(archive_.cbegin(), archive_.cend(),
                                 [=](const std::shared_ptr<T>& ptr) {
                                     return delegatePtr == ptr.get();
                                 });
        if (iter != archive_.cend()) {
            return *iter;
        } else {
            return std::shared_ptr<T>(delegatePtr);
        }
    }

    void setValue(se::Value& value, std::shared_ptr<T> input) {
        if (input != nullptr) {
            se::Object* obj = nullptr;
            if (delegates_.count(input) != 0) {
                obj = delegates_.at(input);
            } else {
                archive_.push_back(input);
                obj = se::Object::createObjectWithClass(clazz_);
                obj->setPrivateData(input.get());
            }
            value.setObject(obj);
        } else {
            value.setNull();
        }
    }

    bool finalize(se::State& s) {
        auto delegatePtr = static_cast<T*>(s.nativeThisObject());
        auto iter = std::find_if(archive_.cbegin(), archive_.cend(),
                                 [=](const std::shared_ptr<T>& ptr) {
                                     return delegatePtr == ptr.get();
                                 });
        if (iter != archive_.cend()) {
            archive_.erase(iter);
        } else {
            delete delegatePtr;
        }
        return true;
    }

private:
    se::Class* clazz_;
    std::unordered_map<std::shared_ptr<T>, se::Object*> delegates_;
    std::vector<std::shared_ptr<T>> archive_;
};
} // namespace core
} // namespace ee

#endif /* JSB_TEMPLATES_HPP__ */
