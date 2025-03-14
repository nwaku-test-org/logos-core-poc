#pragma once

#include <IComponent.h>
#include <QObject>

class ColorChangerComponent : public QObject, public IComponent {
    Q_OBJECT
    Q_INTERFACES(IComponent)
    Q_PLUGIN_METADATA(IID IComponent_iid)

public:
    QWidget* createWidget() override;
    void destroyWidget(QWidget* widget) override;
}; 