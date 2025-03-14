#pragma once

#include <QWidget>

class QPushButton;
class QLabel;
class QObject;

class WakuUIWidget : public QWidget {
    Q_OBJECT

public:
    explicit WakuUIWidget(QWidget* parent = nullptr);

private slots:
    void onInitButtonClicked();
    void onVersionButtonClicked();
    void onWakuInitialized(bool success, const QString &message);
    void onVersionReceived(const QString &version);

private:
    QPushButton* initButton;
    QPushButton* versionButton;
    QLabel* statusLabel;
    QObject* wakuTestPlugin;

    bool connectToWakuTestPlugin();
}; 