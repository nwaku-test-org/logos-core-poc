#pragma once

#include <QtCore/QObject>
#include "../../core/interface.h"

class WakuInterface : public PluginInterface {
public:
    virtual ~WakuInterface() {}

    // Plugin methods
    virtual void initWaku() = 0;
    virtual QString getVersion() = 0;
};

#define WakuInterface_iid "com.logos.WakuInterface"
Q_DECLARE_INTERFACE(WakuInterface, WakuInterface_iid) 