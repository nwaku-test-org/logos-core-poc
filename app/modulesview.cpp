#include "modulesview.h"
#include <QPluginLoader>
#include <QDir>
#include <QDebug>
#include <QCoreApplication>
#include <QLabel>
#include <QFont>
#include <QFileInfo>
#include "mainwindow.h"

ModulesView::ModulesView(QWidget *parent, MainWindow* mainWindow)
    : QWidget(parent), m_mainWindow(mainWindow)
{
    // Get the MDI view from the main window if available
    if (m_mainWindow) {
        m_mdiView = m_mainWindow->getMdiView();
    } else {
        m_mdiView = nullptr;
    }
    
    setupUi();
    
    // Initial plugin setup
    setupPluginButtons(m_buttonLayout);
}

ModulesView::~ModulesView()
{
    // Unload all components
    QStringList components = m_loadedComponents.keys();
    for (const QString& name : components) {
        onUnloadComponent(name);
    }
}

void ModulesView::setupUi()
{
    // Set up main layout
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setContentsMargins(20, 20, 20, 20);
    
    // Create title
    QLabel* titleLabel = new QLabel("Modules", this);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(24);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    
    // Create reload button at the top
    QHBoxLayout* topLayout = new QHBoxLayout();
    QPushButton* reloadButton = new QPushButton("Reload Plugins", this);
    topLayout->addWidget(titleLabel);
    topLayout->addStretch();
    topLayout->addWidget(reloadButton);
    m_mainLayout->addLayout(topLayout);
    
    // Add debug info
    QLabel* debugLabel = new QLabel(this);
    debugLabel->setText("App Path: " + QCoreApplication::applicationDirPath());
    m_mainLayout->addWidget(debugLabel);
    
    QLabel* debugLabel2 = new QLabel(this);
    debugLabel2->setText("Current Path: " + QDir::currentPath());
    m_mainLayout->addWidget(debugLabel2);
    
    // Create a layout for component buttons
    m_buttonLayout = new QVBoxLayout();
    m_mainLayout->addLayout(m_buttonLayout);
    
    // Create a container for component widgets
    m_componentsContainer = new QWidget(this);
    QVBoxLayout* componentsLayout = new QVBoxLayout(m_componentsContainer);
    m_mainLayout->addWidget(m_componentsContainer);
    
    // Add stretch to push everything to the top
    m_mainLayout->addStretch();
    
    // Connect reload button
    connect(reloadButton, &QPushButton::clicked, this, &ModulesView::onReloadPlugins);
}

void ModulesView::clearPluginButtons()
{
    qDebug() << "Clearing plugin buttons...";
    
    // Delete all existing buttons and clear maps
    for (auto button : m_loadButtons.values()) {
        if (button) {
            button->setParent(nullptr);
            button->deleteLater();
        }
    }
    for (auto button : m_unloadButtons.values()) {
        if (button) {
            button->setParent(nullptr);
            button->deleteLater();
        }
    }
    m_loadButtons.clear();
    m_unloadButtons.clear();
    
    // Clear all items from the button layout
    if (m_buttonLayout) {
        QLayoutItem* item;
        while ((item = m_buttonLayout->takeAt(0)) != nullptr) {
            if (item->widget()) {
                item->widget()->setParent(nullptr);
                item->widget()->deleteLater();
            }
            if (item->layout()) {
                // Recursively clear sublayouts
                QLayout* layout = item->layout();
                QLayoutItem* subItem;
                while ((subItem = layout->takeAt(0)) != nullptr) {
                    if (subItem->widget()) {
                        subItem->widget()->setParent(nullptr);
                        subItem->widget()->deleteLater();
                    }
                    delete subItem;
                }
            }
            delete item;
        }
    }
    
    qDebug() << "Plugin buttons cleared.";
}

void ModulesView::setupPluginButtons(QVBoxLayout* buttonLayout)
{
    qDebug() << "Setting up plugin buttons...";
    
    if (!buttonLayout) {
        qDebug() << "Button layout is null, cannot set up plugin buttons";
        return;
    }
    
    try {
        // Find and create buttons for available plugins
        QStringList plugins = findAvailablePlugins();
        
        if (plugins.isEmpty()) {
            QLabel* noPluginsLabel = new QLabel("No plugins found in the plugins directory.", this);
            buttonLayout->addWidget(noPluginsLabel);
            qDebug() << "No plugins found.";
            return;
        }
        
        qDebug() << "Found" << plugins.size() << "plugins:" << plugins;
        
        for (const QString& plugin : plugins) {
            QHBoxLayout* pluginLayout = new QHBoxLayout();
            
            QLabel* pluginLabel = new QLabel(plugin, this);
            QFont pluginFont = pluginLabel->font();
            pluginFont.setBold(true);
            pluginLabel->setFont(pluginFont);
            pluginLabel->setMinimumWidth(100);
            
            QPushButton* loadButton = new QPushButton("Load", this);
            QPushButton* unloadButton = new QPushButton("Unload", this);
            
            // Store buttons for later use
            m_loadButtons[plugin] = loadButton;
            m_unloadButtons[plugin] = unloadButton;
            
            // Add widgets to layout
            pluginLayout->addWidget(pluginLabel);
            pluginLayout->addStretch();
            pluginLayout->addWidget(loadButton);
            pluginLayout->addWidget(unloadButton);
            
            buttonLayout->addLayout(pluginLayout);
            
            // Connect buttons to slots using lambda functions to pass the plugin name
            connect(loadButton, &QPushButton::clicked, this, [this, plugin]() {
                onLoadComponent(plugin);
            });
            connect(unloadButton, &QPushButton::clicked, this, [this, plugin]() {
                onUnloadComponent(plugin);
            });
            
            // Set initial button states
            loadButton->setEnabled(!m_loadedComponents.contains(plugin));
            unloadButton->setEnabled(m_loadedComponents.contains(plugin));
        }
        
        qDebug() << "Plugin buttons setup complete.";
    } catch (const std::exception& e) {
        qDebug() << "Exception during plugin button setup:" << e.what();
    } catch (...) {
        qDebug() << "Unknown exception during plugin button setup";
    }
}

void ModulesView::onReloadPlugins()
{
    qDebug() << "Reloading plugins...";
    
    try {
        // Make a copy of the keys to avoid modifying the map while iterating
        QStringList currentPlugins = m_loadedComponents.keys();
        qDebug() << "Currently loaded plugins:" << currentPlugins;
        
        // Unload all currently loaded plugins
        for (const QString& plugin : currentPlugins) {
            qDebug() << "Unloading plugin:" << plugin;
            onUnloadComponent(plugin);
        }
        
        // Clear existing buttons
        qDebug() << "Clearing plugin buttons...";
        clearPluginButtons();
        
        // Set up new buttons
        qDebug() << "Setting up new plugin buttons...";
        setupPluginButtons(m_buttonLayout);
        qDebug() << "Plugin reload complete.";
    } catch (const std::exception& e) {
        qDebug() << "Exception during plugin reload:" << e.what();
    } catch (...) {
        qDebug() << "Unknown exception during plugin reload";
    }
}

QStringList ModulesView::findAvailablePlugins()
{
    QStringList result;
    
    // Try multiple possible plugin directories
    QStringList searchPaths;
    // Prioritize the application directory
    searchPaths << QCoreApplication::applicationDirPath() + "/plugins";
    searchPaths << QDir::currentPath() + "/plugins";
    searchPaths << QCoreApplication::applicationDirPath();
    searchPaths << QDir::currentPath();
    
    qDebug() << "Current path:" << QDir::currentPath();
    qDebug() << "Application path:" << QCoreApplication::applicationDirPath();
    qDebug() << "Searching for plugins in:" << searchPaths;
    
    for (const QString& searchPath : searchPaths) {
        QDir pluginsDir(searchPath);
        
        if (!pluginsDir.exists()) {
            qDebug() << "Directory does not exist:" << searchPath;
            continue;
        }
        
        qDebug() << "Checking directory:" << searchPath;
        
        // List all files in the plugins directory
        QStringList entries = pluginsDir.entryList(QDir::Files);
        qDebug() << "Files found in" << searchPath << ":" << entries;
        
        // Filter and clean up plugin names
        for (const QString& entry : entries) {
            QString baseName = entry;
#ifdef Q_OS_MAC
            if (baseName.endsWith(".dylib")) {
                baseName.chop(6); // Remove .dylib
                if (!result.contains(baseName)) {
                    result.append(baseName);
                    qDebug() << "Found plugin:" << baseName << "in" << searchPath;
                }
            }
#else
            if (baseName.startsWith("lib") && baseName.endsWith(".so")) {
                baseName.remove(0, 3); // Remove lib
                baseName.chop(3);      // Remove .so
                if (!result.contains(baseName)) {
                    result.append(baseName);
                    qDebug() << "Found plugin:" << baseName << "in" << searchPath;
                }
            }
#endif
        }
    }
    
    qDebug() << "Available plugins:" << result;
    return result;
}

QString ModulesView::getPluginPath(const QString& name)
{
    // Try multiple possible plugin directories
    QStringList searchPaths;
    // Prioritize the application directory
    searchPaths << QCoreApplication::applicationDirPath() + "/plugins/";
    searchPaths << QDir::currentPath() + "/plugins/";
    
    for (const QString& basePath : searchPaths) {
#ifdef Q_OS_MAC
        QString fullPath = basePath + name + ".dylib";
#else
        QString fullPath = basePath + "lib" + name + ".so";
#endif
        QFileInfo fileInfo(fullPath);
        if (fileInfo.exists()) {
            qDebug() << "Found plugin file at:" << fullPath;
            return fullPath;
        }
    }
    
    // Default path if not found
#ifdef Q_OS_MAC
    QString defaultPath = QCoreApplication::applicationDirPath() + "/plugins/" + name + ".dylib";
#else
    QString defaultPath = QCoreApplication::applicationDirPath() + "/plugins/lib" + name + ".so";
#endif
    qDebug() << "Using default plugin path:" << defaultPath;
    return defaultPath;
}

void ModulesView::onLoadComponent(const QString& name)
{
    qDebug() << "Attempting to load plugin:" << name;
    
    if (m_loadedComponents.contains(name)) {
        qDebug() << "Plugin" << name << "is already loaded";
        return;
    }
    
    try {
        QString pluginPath = getPluginPath(name);
        qDebug() << "Loading plugin from:" << pluginPath;
        
        QPluginLoader loader(pluginPath);
        if (!loader.load()) {
            qDebug() << "Failed to load plugin:" << name << "-" << loader.errorString();
            return;
        }
        
        QObject* plugin = loader.instance();
        if (!plugin) {
            qDebug() << "Failed to get plugin instance:" << name << "-" << loader.errorString();
            return;
        }
        
        IComponent* component = qobject_cast<IComponent*>(plugin);
        if (!component) {
            qDebug() << "Failed to cast plugin to IComponent:" << name;
            loader.unload();
            return;
        }
        
        // Create the component widget
        QWidget* componentWidget = nullptr;
        try {
            componentWidget = component->createWidget();
            if (!componentWidget) {
                qDebug() << "Component returned null widget:" << name;
                loader.unload();
                return;
            }
        } catch (const std::exception& e) {
            qDebug() << "Exception creating widget for" << name << ":" << e.what();
            loader.unload();
            return;
        } catch (...) {
            qDebug() << "Unknown exception creating widget for" << name;
            loader.unload();
            return;
        }
        
        // Store the component and widget
        m_loadedComponents[name] = component;
        m_componentWidgets[name] = componentWidget;
        
        // Add the component widget to the MDI view if available
        if (m_mdiView) {
            qDebug() << "Adding plugin" << name << "to MDI view";
            m_mdiView->addPluginWindow(componentWidget, name);
            
            // Switch to the Apps tab (index 0)
            if (m_mainWindow) {
                QVector<SidebarButton*> buttons = m_mainWindow->findChildren<SidebarButton*>();
                if (!buttons.isEmpty()) {
                    buttons[0]->click();
                }
            }
        } else {
            // Fallback to adding the widget to the components container
            qDebug() << "MDI view not available, adding plugin" << name << "to components container";
            
            // Add a frame around the component widget
            QFrame* frame = new QFrame(this);
            frame->setFrameShape(QFrame::StyledPanel);
            frame->setFrameShadow(QFrame::Raised);
            QVBoxLayout* frameLayout = new QVBoxLayout(frame);
            
            // Add a header with the component name
            QLabel* headerLabel = new QLabel(name, frame);
            QFont headerFont = headerLabel->font();
            headerFont.setBold(true);
            headerLabel->setFont(headerFont);
            
            frameLayout->addWidget(headerLabel);
            frameLayout->addWidget(componentWidget);
            
            // Add the frame to the components container
            if (m_componentsContainer && m_componentsContainer->layout()) {
                m_componentsContainer->layout()->addWidget(frame);
            } else {
                qDebug() << "Components container or its layout is null, cannot add component widget";
                component->destroyWidget(componentWidget);
                frame->deleteLater();
                loader.unload();
                m_loadedComponents.remove(name);
                m_componentWidgets.remove(name);
                return;
            }
        }
        
        // Update button states
        updateButtonStates(name, false);
        
        qDebug() << "Successfully loaded plugin:" << name;
    } catch (const std::exception& e) {
        qDebug() << "Exception during load of" << name << ":" << e.what();
    } catch (...) {
        qDebug() << "Unknown exception during load of" << name;
    }
}

void ModulesView::onUnloadComponent(const QString& name)
{
    qDebug() << "Attempting to unload plugin:" << name;
    
    if (!m_loadedComponents.contains(name)) {
        qDebug() << "Plugin" << name << "is not loaded, nothing to unload";
        return;
    }
    
    try {
        // Get the component and widget
        IComponent* component = m_loadedComponents.value(name);
        QWidget* componentWidget = m_componentWidgets.value(name);
        
        if (!component) {
            qDebug() << "Component for" << name << "is null, removing from loaded components";
            m_loadedComponents.remove(name);
            
            if (componentWidget) {
                qDebug() << "Removing component widget for" << name;
                
                // Remove from MDI view if available
                if (m_mdiView) {
                    m_mdiView->removePluginWindow(componentWidget);
                }
                
                componentWidget->hide();
                componentWidget->deleteLater();
                m_componentWidgets.remove(name);
            }
            
            // Update button states
            updateButtonStates(name);
            return;
        }
        
        if (!componentWidget) {
            qDebug() << "Component widget for" << name << "is null, only removing component";
            m_loadedComponents.remove(name);
            
            // Update button states
            updateButtonStates(name);
            return;
        }
        
        // Remove from MDI view if available
        if (m_mdiView) {
            qDebug() << "Removing plugin" << name << "from MDI view";
            m_mdiView->removePluginWindow(componentWidget);
        } else {
            // Fallback to removing from components container
            qDebug() << "MDI view not available, removing plugin" << name << "from components container";
            
            // Find the container widget (parent frame)
            QWidget* containerWidget = componentWidget->parentWidget();
            while (containerWidget && containerWidget->parentWidget() != m_componentsContainer) {
                containerWidget = containerWidget->parentWidget();
            }
            
            // Remove the container from the layout if found
            if (containerWidget && m_componentsContainer && m_componentsContainer->layout()) {
                qDebug() << "Removing container from layout for" << name;
                m_componentsContainer->layout()->removeWidget(containerWidget);
                containerWidget->hide();
                containerWidget->deleteLater();
            }
        }
        
        // Destroy the component widget
        qDebug() << "Destroying component widget for" << name;
        try {
            // Disconnect any signals/slots
            componentWidget->disconnect();
            
            // Call the component's destroyWidget method
            component->destroyWidget(componentWidget);
        } catch (const std::exception& e) {
            qDebug() << "Exception while destroying widget for" << name << ":" << e.what();
        } catch (...) {
            qDebug() << "Unknown exception while destroying widget for" << name;
        }
        
        // Remove from maps
        qDebug() << "Removing" << name << "from loaded components and widgets maps";
        m_loadedComponents.remove(name);
        m_componentWidgets.remove(name);
        
        // Update button states
        updateButtonStates(name);
        
        qDebug() << "Successfully unloaded plugin:" << name;
    } catch (const std::exception& e) {
        qDebug() << "Exception during unload of" << name << ":" << e.what();
    } catch (...) {
        qDebug() << "Unknown exception during unload of" << name;
    }
}

void ModulesView::updateButtonStates(const QString& name, bool isEnabled)
{
    // Update button states
    if (m_loadButtons.contains(name)) {
        m_loadButtons[name]->setEnabled(isEnabled);
    }
    if (m_unloadButtons.contains(name)) {
        m_unloadButtons[name]->setEnabled(!isEnabled);
    }
} 