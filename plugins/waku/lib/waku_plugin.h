#ifndef WAKU_PLUGIN_H
#define WAKU_PLUGIN_H

#include <QObject>
#include "libwaku.h"
#include "interface.h"

class WakuPlugin : public QObject, public WakuInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID WakuInterface_iid FILE "metadata.json")
    Q_INTERFACES(WakuInterface PluginInterface)

public:
    explicit WakuPlugin(QObject *parent = nullptr);
    ~WakuPlugin();

    // Implementation of WakuInterface
    QString name() const override;
    QString version() const override;
    bool start() override;
    bool stop() override;
    bool connect(const QString &peerMultiAddr, unsigned int timeoutMs) override;

private:
    void *m_ctx;
    static void wakuCallback(int callerRet, const char* msg, size_t len, void* userData);
};

#endif // WAKU_PLUGIN_H 