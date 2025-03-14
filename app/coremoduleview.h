#ifndef COREMODULEVIEW_H
#define COREMODULEVIEW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QTimer>
#include <QListWidget>
#include <QPushButton>

class CoreModuleView : public QWidget
{
    Q_OBJECT

public:
    explicit CoreModuleView(QWidget *parent = nullptr);
    ~CoreModuleView();

private slots:
    void updatePluginList();

private:
    void setupUi();
    void createPluginList();

    QVBoxLayout *m_layout;
    QLabel *m_titleLabel;
    QLabel *m_subtitleLabel;
    QListWidget *m_pluginList;
    QTimer *m_updateTimer;
};

#endif // COREMODULEVIEW_H 