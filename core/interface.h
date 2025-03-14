#ifndef PLUGIN_INTERFACE_H
#define PLUGIN_INTERFACE_H

#include <QtPlugin>
#include <QString>

// Define the common base interface for all plugins
class PluginInterface
{
public:
    virtual ~PluginInterface() {}
    
    // Common plugin methods
    virtual QString name() const = 0;
    virtual QString version() const = 0;
};

// Define the interface ID used by Qt's plugin system
#define PluginInterface_iid "com.example.PluginInterface"

Q_DECLARE_INTERFACE(PluginInterface, PluginInterface_iid)

#endif // PLUGIN_INTERFACE_H 