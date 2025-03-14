#include "WakuUIComponent.h"
#include "WakuUIWidget.h"

QWidget* WakuUIComponent::createWidget() {
    return new WakuUIWidget();
}

void WakuUIComponent::destroyWidget(QWidget* widget) {
    delete widget;
} 