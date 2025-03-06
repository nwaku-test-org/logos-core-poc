#ifndef MODULESVIEW_H
#define MODULESVIEW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QMap>
#include "interfaces/IComponent.h"

class ModulesView : public QWidget
{
    Q_OBJECT

public:
    explicit ModulesView(QWidget *parent = nullptr);
    ~ModulesView();

private slots:
    void onLoadComponent(const QString& name);
    void onUnloadComponent(const QString& name);
    void onReloadPlugins();

private:
    void setupUi();
    QString getPluginPath(const QString& name);
    QStringList findAvailablePlugins();
    void setupPluginButtons(QVBoxLayout* buttonLayout);
    void clearPluginButtons();
    void updateButtonStates(const QString& name, bool isEnabled = true);
    
    QMap<QString, IComponent*> m_loadedComponents;
    QMap<QString, QWidget*> m_componentWidgets;
    QMap<QString, QPushButton*> m_loadButtons;
    QMap<QString, QPushButton*> m_unloadButtons;
    QVBoxLayout* m_mainLayout;
    QVBoxLayout* m_buttonLayout;
    QWidget* m_componentsContainer;
};

#endif // MODULESVIEW_H 