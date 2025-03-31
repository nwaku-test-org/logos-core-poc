#pragma once

#include <QtCore/QObject>
#include "../../core/interface.h"

// Callback type definitions
using WakuInitCallback = std::function<void(bool success, const QString &message)>;
using WakuVersionCallback = std::function<void(const QString &version)>;

class WakuInterface : public PluginInterface {
public:
    virtual ~WakuInterface() {}

    // Plugin methods
    virtual void initWaku(WakuInitCallback callback = nullptr) = 0;
    virtual void getVersion(WakuVersionCallback callback = nullptr) = 0;
};

#define WakuInterface_iid "com.logos.WakuInterface"
Q_DECLARE_INTERFACE(WakuInterface, WakuInterface_iid) 