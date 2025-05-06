#pragma once

#include <QtCore/QObject>
#include "../../core/interface.h"
#include <functional>

// Define a callback type for message handling
using MessageCallback = std::function<void(const std::string&, const std::string&, const std::string&)>;

class ChatInterface : public PluginInterface {
public:
    virtual ~ChatInterface() {}

    // Core chat functionality
    Q_INVOKABLE virtual bool initialize(MessageCallback messageCallback = nullptr) = 0;
    Q_INVOKABLE virtual bool joinChannel(const std::string& channelName) = 0;
    Q_INVOKABLE virtual void sendMessage(const std::string& channelName, const std::string& username, const std::string& message) = 0;
    Q_INVOKABLE virtual void retrieveHistory(const std::string& channelName, MessageCallback callback = nullptr) = 0;
};

#define ChatInterface_iid "org.logos.ChatInterface"
Q_DECLARE_INTERFACE(ChatInterface, ChatInterface_iid) 