#ifndef MODULESVIEW_H
#define MODULESVIEW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QMap>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QStringList>
#include <QDebug>
#include <IComponent.h>
#include "mdiview.h"

class MainWindow;

class ModulesView : public QWidget
{
    Q_OBJECT

public:
    ModulesView(QWidget *parent = nullptr, MainWindow* mainWindow = nullptr);
    ~ModulesView();

private slots:
    void onLoadComponent(const QString& name);
    void onUnloadComponent(const QString& name);

private:
    void setupUi();
    void setupPluginButtons(QVBoxLayout* buttonLayout);
    QString getPluginPath(const QString& name);
    QStringList findAvailablePlugins();
    void updateButtonStates(const QString& name, bool isEnabled = true);
    
    QMap<QString, IComponent*> m_loadedComponents;
    QMap<QString, QWidget*> m_componentWidgets;
    QMap<QString, QPushButton*> m_loadButtons;
    QMap<QString, QPushButton*> m_unloadButtons;
    
    // Reference to the main window to access the MDI view
    MainWindow* m_mainWindow;
    MdiView* m_mdiView;
    
    QVBoxLayout* m_layout;
    QVBoxLayout* m_buttonLayout;
};

#endif // MODULESVIEW_H 