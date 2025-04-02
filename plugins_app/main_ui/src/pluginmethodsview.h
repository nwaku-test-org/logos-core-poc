#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

class PluginMethodsView : public QWidget
{
    Q_OBJECT

public:
    explicit PluginMethodsView(const QString& pluginName, QWidget* parent = nullptr);
    ~PluginMethodsView();

signals:
    void backClicked();

private slots:
    void onBackButtonClicked();

private:
    void setupUi();

    QString m_pluginName;
    QVBoxLayout* m_layout;
    QLabel* m_titleLabel;
    QLabel* m_contentLabel;
    QPushButton* m_backButton;
}; 