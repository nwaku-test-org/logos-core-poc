#include "waku.h"
#include <QDebug>
#include <QThread>
#include "lib/libwaku.h"

namespace {
    // Structure to hold version data
    struct VersionData {
        QString* version;
        bool* received;
        Waku* waku; // Add pointer to Waku object
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
        
        // Emit signal with the received version
        if (data->waku) {
            data->waku->versionReceived(version);
        }
        
        delete data;
    }

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
        
        // Emit signal if userData contains Waku pointer
        if (userData) {
            Waku* waku = static_cast<Waku*>(userData);
            waku->wakuInitialized(success, message);
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

void Waku::initWaku() {
    qDebug() << "Initializing Waku...";
    // Clean up existing instance if any
    if (wakuCtx) {
        waku_destroy(wakuCtx, nullptr, nullptr);
        wakuCtx = nullptr;
    }

    // Initialize Waku - passing 'this' as userData to access signals
    wakuCtx = waku_new("{}", init_callback, this);
    if (!wakuCtx) {
        qDebug() << "Failed to initialize Waku";
        // Emit signal for failure case
        emit wakuInitialized(false, "Failed to initialize Waku");
    }
}

QString Waku::getVersion() {
    qDebug() << "Getting Waku version...";
    if (!wakuCtx) {
        QString errorMsg = "Waku not initialized";
        qDebug() << errorMsg;
        emit versionReceived(errorMsg);
        return errorMsg;
    }

    // Create version data for the callback, including this pointer for signal emission
    auto* data = new VersionData{nullptr, nullptr, this};

    // Get version
    int ret = waku_version(wakuCtx, version_callback, data);
    if (ret != RET_OK) {
        QString errorMsg = "Failed to get version";
        delete data;
        emit versionReceived(errorMsg);
        return errorMsg;
    }

    // We don't need to wait for the callback anymore, as we're using signals and slots
    // The UI will be updated when the callback emits the versionReceived signal
    return "Version request sent";
} 