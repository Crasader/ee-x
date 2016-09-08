//
//  PluginManager.hpp
//  ee-core
//
//  Created by Zinge on 7/1/16.
//
//

#ifndef EE_CORE_PLUGIN_MANAGER_HPP_
#define EE_CORE_PLUGIN_MANAGER_HPP_

#include <string>
#include <unordered_map>

namespace ee {
namespace core {
class PluginProtocol;

class PluginManager {
public:
    static PluginProtocol* getPluginByName(const std::string& pluginName);

    static void onNativeCallback(const std::string& pluginName,
                                 const std::string& json, int callbackId);

    static void onNativeCallback(const std::string& pluginName,
                                 const std::string& json,
                                 const std::string& tag);

private:
    friend class PluginProtocol;

    static bool addPlugin(const std::string& pluginName,
                          PluginProtocol* plugin);

    static bool removePlugin(const std::string& pluginName);

    static std::unordered_map<std::string, PluginProtocol*> plugins_;
};
} // namespace core
} // namespace ee

#endif /* EE_CORE_PLUGIN_MANAGER_HPP_ */
