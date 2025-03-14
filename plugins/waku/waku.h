#pragma once

#include <QtCore/QObject>
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

public slots:
    // WakuInterface implementation
    void initWaku() override;
    QString getVersion() override;

signals:
    // Signal emitted when Waku is initialized
    void wakuInitialized(bool success, const QString &message);
    
    // Signal emitted when getVersion is called
    void versionReceived(const QString &version);

private:
    void* wakuCtx;
}; 