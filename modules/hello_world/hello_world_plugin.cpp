#include "hello_world_plugin.h"
#include <QDebug>
#include <QCoreApplication>
#include "../../core/plugin_registry.h"

HelloWorldPlugin::HelloWorldPlugin(QObject *parent)
    : QObject(parent)
    , m_name("Hello World Plugin")
    , m_version("1.0.0")
    , m_timer(new QTimer(this))
{
    qDebug() << "Hello World Plugin initialized!";
    
    // Connect the timer to the printMessage slot
    connect(m_timer, &QTimer::timeout, this, &HelloWorldPlugin::printMessage);
    
    // Call the calculator plugin after a short delay to ensure it's loaded
    QTimer::singleShot(1000, this, &HelloWorldPlugin::callCalculatorPlugin);
}

HelloWorldPlugin::~HelloWorldPlugin()
{
    // Stop the timer if it's running
    if (m_timer->isActive()) {
        m_timer->stop();
    }
    
    qDebug() << "Hello World Plugin destroyed!";
}

QString HelloWorldPlugin::echo(const QString &message)
{
    qDebug() << "================================================";
    qDebug() << "called Hello World Plugin echo method";
    qDebug() << "================================================";

    // Store the message
    m_message = message;
    
    // Start the timer to print the message every 2 seconds (2000 ms)
    if (m_timer->isActive()) {
        m_timer->stop();
    }
    m_timer->start(2000);
    
    QString result = "Hello World: " + message;
    qDebug() << result;
    qDebug() << "Timer started. Will print message every 2 seconds.";
    
    // Emit the signal with both the original message and the response
    emit messageEchoed(message, result);
    
    return result;
}

void HelloWorldPlugin::printMessage()
{
    qDebug() << "Timer message: Hello World: " << m_message;
}

QString HelloWorldPlugin::name() const
{
    return m_name;
}

QString HelloWorldPlugin::version() const
{
    return m_version;
}

// Method to get a reference to another plugin
QObject* HelloWorldPlugin::getPlugin(const QString& pluginName)
{
    // Use the new PluginRegistry namespace function
    return PluginRegistry::getPlugin<QObject>(pluginName);
}

// Method to call the calculator plugin
void HelloWorldPlugin::callCalculatorPlugin()
{
    qDebug() << "\n------------------------------------------";
    qDebug() << "Hello World Plugin is requesting Calculator Plugin...";
    
    // Get the calculator plugin using the PluginRegistry namespace function
    CalculatorInterface* calculator = PluginRegistry::getPlugin<CalculatorInterface>("simple calculator plugin");
    
    if (calculator) {
        // Call the add method
        int result = calculator->add(5, 3);
        qDebug() << "Hello World Plugin called Calculator Plugin: 5 + 3 =" << result;
    } else {
        qWarning() << "Failed to get Calculator Plugin";
    }
} 