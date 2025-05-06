#ifndef HELLO_WORLD_INTERFACE_H
#define HELLO_WORLD_INTERFACE_H

#include "../../core/interface.h"

class HelloWorldInterface : public PluginInterface
{
public:
    virtual ~HelloWorldInterface() {}
    
    // Plugin methods
    virtual QString echo(const QString &message) = 0;
};

#define HelloWorldInterface_iid "com.example.HelloWorldInterface"
Q_DECLARE_INTERFACE(HelloWorldInterface, HelloWorldInterface_iid)

#endif // HELLO_WORLD_INTERFACE_H 