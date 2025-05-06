#pragma once

#include <QWidget>
#include <QColor>

class QPushButton;

class ColorChangerWidget : public QWidget {
    Q_OBJECT

public:
    explicit ColorChangerWidget(QWidget* parent = nullptr);

private slots:
    void changeColor();

private:
    QPushButton* changeColorButton;
    QList<QColor> colors = {Qt::red, Qt::green, Qt::blue, Qt::yellow};
    int currentColorIndex = 0;
}; 