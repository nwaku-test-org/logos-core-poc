#pragma once

#include <QtCore/QObject>
#include <functional>
#include "waku_interface.h"

class Waku : public QObject, public WakuInterface {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID WakuInterface_iid FILE "metadata.json")
    Q_INTERFACES(WakuInterface PluginInterface)

public:
    Waku();
    ~Waku();

    // PluginInterface
    QString name() const override { return "waku"; }
    QString version() const override { return "0.1.0"; }

    // WakuInterface implementation
    void initWaku(WakuInitCallback callback = nullptr) override;
    void getVersion(WakuVersionCallback callback = nullptr) override;

private:
    void* wakuCtx;
    WakuInitCallback initCallback;
    WakuVersionCallback versionCallback;
}; 