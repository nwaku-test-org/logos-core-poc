#include "core_manager.h"
#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QPluginLoader>
#include "../plugin_registry.h"
#include "logos_core.h"

CoreManagerPlugin::CoreManagerPlugin() {
    qDebug() << "CoreManager plugin created";
}

CoreManagerPlugin::~CoreManagerPlugin() {
    cleanup();
}

void CoreManagerPlugin::initialize(int argc, char* argv[]) {
    qDebug() << "Initializing CoreManager plugin";
    // We don't call logos_core_init here because it creates another QApplication
    // instance and we're already creating one in main.cpp
}

void CoreManagerPlugin::setPluginsDirectory(const QString& directory) {
    m_pluginsDirectory = directory;
    qDebug() << "Setting plugins directory to:" << directory;
    logos_core_set_plugins_dir(directory.toUtf8().constData());
}

void CoreManagerPlugin::start() {
    qDebug() << "Starting CoreManager plugin";
    logos_core_start();

    // Register ourselves in the plugin registry
    PluginRegistry::registerPlugin(this, name());
}

void CoreManagerPlugin::cleanup() {
    qDebug() << "Cleaning up CoreManager plugin";
    logos_core_cleanup();
}

void CoreManagerPlugin::helloWorld() {
    qDebug() << "Hello from CoreManager plugin!";
}

QStringList CoreManagerPlugin::getLoadedPlugins() {
    qDebug() << "\n\n----------> Getting loaded plugins\n\n";
    QStringList pluginList;
    char** plugins = logos_core_get_loaded_plugins();
    
    if (!plugins) {
        return pluginList;
    }
    
    for (char** p = plugins; *p != nullptr; ++p) {
        pluginList << QString::fromUtf8(*p);
        delete[] *p;
    }
    delete[] plugins;
    
    return pluginList;
}
