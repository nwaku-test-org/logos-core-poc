#ifndef CALCULATOR_INTERFACE_H
#define CALCULATOR_INTERFACE_H

#include "../../core/interface.h"

class CalculatorInterface : public PluginInterface
{
public:
    virtual ~CalculatorInterface() {}
    
    // Plugin methods
    virtual int add(int a, int b) = 0;
    virtual int subtract(int a, int b) = 0;
    virtual QString echo(const QString &message) = 0;
};

#define CalculatorInterface_iid "com.example.CalculatorInterface"
Q_DECLARE_INTERFACE(CalculatorInterface, CalculatorInterface_iid)

#endif // CALCULATOR_INTERFACE_H 