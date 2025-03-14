#ifndef HELLO_WORLD_PLUGIN_H
#define HELLO_WORLD_PLUGIN_H

#include <QObject>
#include <QTimer>
#include "hello_world_interface.h"
#include "../calculator/calculator_interface.h"

class HelloWorldPlugin : public QObject, public HelloWorldInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID HelloWorldInterface_iid FILE "metadata.json")
    Q_INTERFACES(HelloWorldInterface PluginInterface)

public:
    explicit HelloWorldPlugin(QObject *parent = nullptr);
    ~HelloWorldPlugin();
    
    // Implementation of HelloWorldInterface
    QString echo(const QString &message) override;
    QString name() const override;
    QString version() const override;

signals:
    void messageEchoed(const QString &message, const QString &response);

private slots:
    void printMessage();
    
    // New method to call the calculator plugin
    void callCalculatorPlugin();

private:
    QString m_name;
    QString m_version;
    QTimer *m_timer;
    QString m_message;
    
    // Method to get a reference to another plugin
    QObject* getPlugin(const QString& pluginName);
};

#endif // HELLO_WORLD_PLUGIN_H 