#include "mainwindow.h"
#include "core_manager.h"

#include <QApplication>
#include <QDir>
#include <QDebug>
#include <iostream>
#include <memory>

int main(int argc, char *argv[])
{
    // Create QApplication first
    QApplication app(argc, argv);
    
    // Initialize CoreManager
    CoreManager& core = CoreManager::instance();
    core.initialize(argc, argv);

    // Set the plugins directory
    QString pluginsDir = QDir::cleanPath(QCoreApplication::applicationDirPath() + "/modules");
    std::cout << "Setting plugins directory to: " << pluginsDir.toStdString() << std::endl;
    core.setPluginsDirectory(pluginsDir);

    // Start the core
    core.start();
    std::cout << "Logos Core started successfully!" << std::endl;

    // Explicitly load plugins in specified order
    std::cout << "Loading plugins in specified order..." << std::endl;
    
    // Load waku plugin first
    if (core.loadPlugin("waku")) {
        std::cout << "Successfully loaded waku plugin" << std::endl;
    } else {
        std::cerr << "Failed to load waku plugin" << std::endl;
    }
    
    // Then load chat plugin
    if (core.loadPlugin("chat")) {
        std::cout << "Successfully loaded chat plugin" << std::endl;
    } else {
        std::cerr << "Failed to load chat plugin" << std::endl;
    }

    // Print all loaded plugins
    QStringList plugins = core.getLoadedPlugins();
    if (plugins.isEmpty()) {
        qInfo() << "No plugins loaded.";
    } else {
        qInfo() << "Currently loaded plugins:";
        for (const QString &plugin : plugins) {
            qInfo() << "  -" << plugin;
        }
        qInfo() << "Total plugins:" << plugins.size();
    }
    
    // Create and show the main window
    MainWindow window;
    window.show();
    
    // Run the application
    int result = app.exec();
    
    // Cleanup core before exit
    core.cleanup();
    
    return result;
} 