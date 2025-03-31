#include "waku.h"
#include <QDebug>
#include <QThread>
#include "lib/libwaku.h"

namespace {
    // Structure to hold version data
    struct VersionData {
        Waku* waku;
        WakuVersionCallback callback;
    };

    // Static callback for waku_version
    void version_callback(int callerRet, const char* msg, size_t len, void* userData) {
        auto* data = static_cast<VersionData*>(userData);
        QString version;
        
        if (callerRet == RET_OK && msg != nullptr) {
            version = QString::fromUtf8(msg, len);
        } else {
            version = "Error getting version";
        }
        
        // Call the user callback if provided
        if (data->callback) {
            data->callback(version);
        }
        
        delete data;
    }

    // Structure to hold init data
    struct InitData {
        Waku* waku;
        WakuInitCallback callback;
    };

    // Static callback for waku_new
    void init_callback(int callerRet, const char* msg, size_t len, void* userData) {
        bool success = (callerRet == RET_OK);
        QString message;
        
        if (success) {
            message = "Waku initialized successfully";
            qDebug() << message;
        } else {
            message = msg ? QString::fromUtf8(msg, len) : "Unknown error";
            qDebug() << "Waku initialization failed:" << message;
        }
        
        // Call user callback if provided
        if (userData) {
            auto* data = static_cast<InitData*>(userData);
            if (data->callback) {
                data->callback(success, message);
            }
            delete data;
        }
    }
}

Waku::Waku() : wakuCtx(nullptr) {
    qDebug() << "Waku Plugin initialized!";
}

Waku::~Waku() {
    qDebug() << "Waku Plugin destroyed!";
    if (wakuCtx) {
        waku_destroy(wakuCtx, nullptr, nullptr);
    }
}

void Waku::initWaku(WakuInitCallback callback) {
    qDebug() << "Initializing Waku...";
    // Clean up existing instance if any
    if (wakuCtx) {
        waku_destroy(wakuCtx, nullptr, nullptr);
        wakuCtx = nullptr;
    }

    // Save the callback
    InitData* userData = new InitData{this, callback};

    // Initialize Waku - passing the callback data as userData
    wakuCtx = waku_new("{}", init_callback, userData);
    if (!wakuCtx) {
        qDebug() << "Failed to initialize Waku";
        // Call callback for failure case
        if (callback) {
            callback(false, "Failed to initialize Waku");
        }
        delete userData;
    }
}

void Waku::getVersion(WakuVersionCallback callback) {
    qDebug() << "Getting Waku version...";
    if (!wakuCtx) {
        QString errorMsg = "Waku not initialized";
        qDebug() << errorMsg;
        if (callback) {
            callback(errorMsg);
        }
        return;
    }

    // Create version data for the callback, including callback function
    auto* data = new VersionData{this, callback};

    // Get version
    int ret = waku_version(wakuCtx, version_callback, data);
    if (ret != RET_OK) {
        QString errorMsg = "Failed to get version";
        if (callback) {
            callback(errorMsg);
        }
        delete data;
    }
} 