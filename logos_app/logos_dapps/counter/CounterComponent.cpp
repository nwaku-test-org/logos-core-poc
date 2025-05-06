#include "CounterComponent.h"
#include "CounterWidget.h"

QWidget* CounterComponent::createWidget() {
    return new CounterWidget();
}

void CounterComponent::destroyWidget(QWidget* widget) {
    delete widget;
} 