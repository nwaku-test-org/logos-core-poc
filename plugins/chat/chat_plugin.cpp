#include "chat_plugin.h"
#include "../../core/plugin_registry.h"

ChatPlugin::ChatPlugin() : wakuCtx(nullptr), currentRelayTopic("/waku/2/rs/16/32"), wakuPlugin(nullptr) {
    // Get the waku plugin from the PluginRegistry
    wakuPlugin = PluginRegistry::getPlugin<WakuInterface>("waku");
}

ChatPlugin::~ChatPlugin() {
    // Clean up any resources if needed
    if (wakuCtx != nullptr) {
        // Cleanup code could go here if needed
        wakuCtx = nullptr;
    }
}

bool ChatPlugin::initialize(MessageCallback messageCallback) {
    // Initialize and start Waku
    wakuCtx = ::initAndStart(currentRelayTopic, messageCallback);
    
    // Return success/failure
    return (wakuCtx != nullptr);
}

bool ChatPlugin::joinChannel(const std::string& channelName) {
    if (wakuCtx == nullptr) {
        return false;
    }
    
    return ::joinChannel(wakuCtx, channelName, currentRelayTopic);
}

void ChatPlugin::sendMessage(const std::string& channelName, const std::string& username, const std::string& message) {
    if (wakuCtx == nullptr) {
        return;
    }
    
    ::sendMessage(wakuCtx, channelName, username, message);
}

void ChatPlugin::retrieveHistory(const std::string& channelName, MessageCallback callback) {
    if (wakuCtx == nullptr) {
        return;
    }
    
    ::retrieveHistory(wakuCtx, channelName, callback);
} 