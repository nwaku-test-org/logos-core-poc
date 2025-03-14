#ifndef CALCULATOR_PLUGIN_H
#define CALCULATOR_PLUGIN_H

#include <QObject>
#include <QTimer>
#include "calculator_interface.h"

class CalculatorPlugin : public QObject, public CalculatorInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID CalculatorInterface_iid FILE "metadata.json")
    Q_INTERFACES(CalculatorInterface PluginInterface)

public:
    explicit CalculatorPlugin(QObject *parent = nullptr);
    ~CalculatorPlugin();
    
    // Implementation of CalculatorInterface
    QString name() const override;
    QString version() const override;

public slots:
    int add(int a, int b) override;
    int subtract(int a, int b) override;
    QString echo(const QString &message) override;

private slots:
    void printCalculatorMessage();

private:
    QString m_name;
    QString m_version;
    QTimer *m_timer;
    QString m_message;
};

#endif // CALCULATOR_PLUGIN_H 