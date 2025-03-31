#pragma once

#include <QWidget>
#include "../../plugins/waku/waku_interface.h"

class QPushButton;
class QLabel;

class WakuUIWidget : public QWidget {
    Q_OBJECT

public:
    explicit WakuUIWidget(QWidget* parent = nullptr);

private:
    void onInitButtonClicked();
    void onVersionButtonClicked();

    QPushButton* initButton;
    QPushButton* versionButton;
    QLabel* statusLabel;
    WakuInterface* wakuPlugin;

    bool connectToWakuPlugin();
}; 