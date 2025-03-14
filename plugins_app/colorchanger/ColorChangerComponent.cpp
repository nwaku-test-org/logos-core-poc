#include "ColorChangerComponent.h"
#include "ColorChangerWidget.h"

QWidget* ColorChangerComponent::createWidget() {
    return new ColorChangerWidget();
}

void ColorChangerComponent::destroyWidget(QWidget* widget) {
    delete widget;
} 