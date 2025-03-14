#define LOGOS_CORE_LIBRARY

#include "logos_core.h"
#include <QCoreApplication>
#include <QPluginLoader>
#include <QObject>
#include <QDebug>
#include <QDir>
#include <QMetaProperty>
#include <QMetaMethod>
#include <QTimer>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include "../interface.h"
#include "../plugin_registry.h"

// Declare QObject* as a metatype so it can be stored in QVariant
Q_DECLARE_METATYPE(QObject*)

// Global application pointer
static QCoreApplication* g_app = nullptr;

// Custom plugins directory
static QString g_plugins_dir = "";

// Global list to store loaded plugin names
static QStringList g_loaded_plugins;

// Helper function to load and process a plugin
static void loadAndProcessPlugin(const QString &pluginPath)
{
    qDebug() << "\n------------------------------------------";
    qDebug() << "Loading plugin from:" << pluginPath;

    // Load the plugin
    QPluginLoader loader(pluginPath);

    // Read the metadata before loading the plugin
    QJsonObject metadata = loader.metaData();
    if (metadata.isEmpty()) {
        qWarning() << "No metadata found for plugin:" << pluginPath;
        return;
    }

    // Read our custom metadata from the metadata.json file
    QJsonObject customMetadata = metadata.value("MetaData").toObject();
    if (!customMetadata.isEmpty()) {
        qDebug() << "Plugin Metadata:";
        qDebug() << " - Name:" << customMetadata.value("name").toString();
        qDebug() << " - Version:" << customMetadata.value("version").toString();
        qDebug() << " - Description:" << customMetadata.value("description").toString();
        qDebug() << " - Author:" << customMetadata.value("author").toString();
        qDebug() << " - Type:" << customMetadata.value("type").toString();
        
        // Log capabilities
        QJsonArray capabilities = customMetadata.value("capabilities").toArray();
        if (!capabilities.isEmpty()) {
            qDebug() << " - Capabilities:";
            for (const QJsonValue &cap : capabilities) {
                qDebug() << "   *" << cap.toString();
            }
        }

        // Check dependencies
        QJsonArray dependencies = customMetadata.value("dependencies").toArray();
        if (!dependencies.isEmpty()) {
            qDebug() << " - Dependencies:";
            for (const QJsonValue &dep : dependencies) {
                QString dependency = dep.toString();
                qDebug() << "   *" << dependency;
                if (!g_loaded_plugins.contains(dependency)) {
                    qWarning() << "Required dependency not loaded:" << dependency;
                }
            }
        }
    }

    QObject *plugin = loader.instance();

    if (!plugin) {
        qWarning() << "Failed to load plugin:" << loader.errorString();
        return;
    }

    qDebug() << "Plugin loaded successfully.";

    // Cast to the base PluginInterface
    PluginInterface *basePlugin = qobject_cast<PluginInterface *>(plugin);
    if (!basePlugin) {
        qWarning() << "Plugin does not implement the PluginInterface";
        return;
    }

    // Verify that the plugin name matches the metadata
    if (!customMetadata.isEmpty()) {
        QString metadataName = customMetadata.value("name").toString();
        QString pluginName = basePlugin->name();
        if (metadataName != pluginName) {
            qWarning() << "Plugin name mismatch! Metadata:" << metadataName << "Plugin:" << pluginName;
        }
    }

    qDebug() << "Plugin name:" << basePlugin->name();
    qDebug() << "Plugin version:" << basePlugin->version();

    // Add the plugin name to our global list
    g_loaded_plugins.append(basePlugin->name());

    // Register the plugin using the PluginRegistry namespace function
    PluginRegistry::registerPlugin(plugin, basePlugin->name());
    qDebug() << "Registered plugin with key:" << basePlugin->name().toLower().replace(" ", "_");

    // Use QObject reflection (QMetaObject) for runtime inspection
    const QMetaObject *metaObject = plugin->metaObject();
    qDebug() << "\nPlugin class name:" << metaObject->className();

    // List properties
    qDebug() << "\nProperties:";
    for (int i = 0; i < metaObject->propertyCount(); ++i) {
        QMetaProperty property = metaObject->property(i);
        qDebug() << " -" << property.name() << "=" << plugin->property(property.name());
    }

    // List methods
    qDebug() << "\nMethods:";
    for (int i = 0; i < metaObject->methodCount(); ++i) {
        QMetaMethod method = metaObject->method(i);
        qDebug() << " -" << method.methodSignature();
    }
}

// Helper function to find and load all plugins in a directory
static QStringList findPlugins(const QString &pluginsDir)
{
    QDir dir(pluginsDir);
    QStringList plugins;
    
    qDebug() << "Searching for plugins in:" << dir.absolutePath();
    
    if (!dir.exists()) {
        qWarning() << "Plugins directory does not exist:" << dir.absolutePath();
        return plugins;
    }
    
    // Get all files in the directory
    QStringList entries = dir.entryList(QDir::Files);
    qDebug() << "Files found:" << entries;
    
    // Filter for plugin files based on platform
    QStringList nameFilters;
#ifdef Q_OS_WIN
    nameFilters << "*.dll";
#elif defined(Q_OS_MAC)
    nameFilters << "*.dylib";
#else
    nameFilters << "*.so";
#endif
    
    dir.setNameFilters(nameFilters);
    QStringList pluginFiles = dir.entryList(QDir::Files);
    
    for (const QString &fileName : pluginFiles) {
        QString filePath = dir.absoluteFilePath(fileName);
        plugins.append(filePath);
        qDebug() << "Found plugin:" << filePath;
    }
    
    return plugins;
}

// C API implementation

void logos_core_init(int argc, char *argv[])
{
    // Create the application instance
    g_app = new QCoreApplication(argc, argv);
    
    // Register QObject* as a metatype
    qRegisterMetaType<QObject*>("QObject*");
}

void logos_core_set_plugins_dir(const char* plugins_dir)
{
    if (plugins_dir) {
        g_plugins_dir = QString(plugins_dir);
        qDebug() << "Custom plugins directory set to:" << g_plugins_dir;
    }
}

void logos_core_start()
{
    qDebug() << "Simple Plugin Example";
    qDebug() << "Current directory:" << QDir::currentPath();
    
    // Clear the list of loaded plugins before loading new ones
    g_loaded_plugins.clear();
    
    // Define the plugins directory path
    QString pluginsDir;
    if (!g_plugins_dir.isEmpty()) {
        // Use the custom plugins directory if set
        pluginsDir = g_plugins_dir;
    } else {
        // Use the default plugins directory
        pluginsDir = QDir::cleanPath(QCoreApplication::applicationDirPath() + "/../plugins");
    }
    qDebug() << "Looking for plugins in:" << pluginsDir;
    
    // Find all plugins in the directory
    QStringList pluginPaths = findPlugins(pluginsDir);
    
    if (pluginPaths.isEmpty()) {
        qWarning() << "No plugins found in:" << pluginsDir;
    } else {
        qDebug() << "Found" << pluginPaths.size() << "plugins";
        
        // Load and process each plugin
        for (const QString &pluginPath : pluginPaths) {
            loadAndProcessPlugin(pluginPath);
        }
    }
}

int logos_core_exec()
{
    if (g_app) {
        return g_app->exec();
    }
    return -1;
}

void logos_core_cleanup()
{
    delete g_app;
    g_app = nullptr;
}

// Implementation of the function to get loaded plugins
char** logos_core_get_loaded_plugins()
{
    int count = g_loaded_plugins.size();
    
    if (count == 0) {
        // Return an array with just a NULL terminator
        char** result = new char*[1];
        result[0] = nullptr;
        return result;
    }
    
    // Allocate memory for the array of strings
    char** result = new char*[count + 1];  // +1 for null terminator
    
    // Copy each plugin name
    for (int i = 0; i < count; ++i) {
        QByteArray utf8Data = g_loaded_plugins[i].toUtf8();
        result[i] = new char[utf8Data.size() + 1];
        strcpy(result[i], utf8Data.constData());
    }
    
    // Null-terminate the array
    result[count] = nullptr;
    
    return result;
} 
