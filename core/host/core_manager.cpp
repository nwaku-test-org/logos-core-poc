#include "core_manager.h"
#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QPluginLoader>
#include <QMetaMethod>
#include <QJsonObject>
#include <QJsonArray>
#include <QFileInfo>
#include <QFile>
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

QJsonArray CoreManagerPlugin::getKnownPlugins() {
    qDebug() << "CoreManager: Getting known plugins with status";
    QJsonArray pluginsArray;
    
    // Get all known plugins
    char** plugins = logos_core_get_known_plugins();
    if (!plugins) {
        return pluginsArray;
    }
    
    // Get all loaded plugins for status checking
    QStringList loadedPlugins = getLoadedPlugins();
    
    // Populate the JSON array with plugin information
    for (char** p = plugins; *p != nullptr; ++p) {
        QString pluginName = QString::fromUtf8(*p);
        
        // Create a JSON object for each plugin
        QJsonObject pluginObj;
        pluginObj["name"] = pluginName;
        pluginObj["loaded"] = loadedPlugins.contains(pluginName);
        
        // Add to array
        pluginsArray.append(pluginObj);
        
        // Free memory
        delete[] *p;
    }
    delete[] plugins;
    
    return pluginsArray;
}

bool CoreManagerPlugin::loadPlugin(const QString& pluginName) {
    qDebug() << "CoreManager: Loading plugin:" << pluginName;
    int result = logos_core_load_plugin(pluginName.toUtf8().constData());
    return result == 1;
}

bool CoreManagerPlugin::unloadPlugin(const QString& pluginName) {
    qDebug() << "CoreManager: Unloading plugin:" << pluginName;
    int result = logos_core_unload_plugin(pluginName.toUtf8().constData());
    return result == 1;
}

QString CoreManagerPlugin::processPlugin(const QString& filePath) {
    qDebug() << "CoreManager: Processing plugin file:" << filePath;
    char* result = logos_core_process_plugin(filePath.toUtf8().constData());

    if (!result) {
        qWarning() << "Failed to process plugin file:" << filePath;
        return QString();
    }

    // Convert to QString and free the C string
    QString pluginName = QString::fromUtf8(result);
    delete[] result;

    return pluginName;
}

QJsonArray CoreManagerPlugin::getPluginMethods(const QString& pluginName) {
    QJsonArray methodsArray;

    // Get the plugin from the registry
    QObject* plugin = PluginRegistry::getPlugin<QObject>(pluginName);
    if (!plugin) {
        qWarning() << "Plugin not found:" << pluginName;
        return methodsArray;
    }

    // Use QMetaObject for runtime introspection
    const QMetaObject* metaObject = plugin->metaObject();

    // Iterate through methods and add to the JSON array
    for (int i = 0; i < metaObject->methodCount(); ++i) {
        QMetaMethod method = metaObject->method(i);

        // Skip methods from QObject and other base classes
        if (method.enclosingMetaObject() != metaObject) {
            continue;
        }

        // Create a JSON object for each method
        QJsonObject methodObj;
        methodObj["signature"] = QString::fromUtf8(method.methodSignature());
        methodObj["name"] = QString::fromUtf8(method.name());
        methodObj["returnType"] = QString::fromUtf8(method.typeName());

        // Check if the method is invokable via QMetaObject::invokeMethod
        methodObj["isInvokable"] = method.isValid() && (method.methodType() == QMetaMethod::Method || 
                                    method.methodType() == QMetaMethod::Slot);

        // Add parameter information if available
        if (method.parameterCount() > 0) {
            QJsonArray params;
            for (int p = 0; p < method.parameterCount(); ++p) {
                QJsonObject paramObj;
                paramObj["type"] = QString::fromUtf8(method.parameterTypeName(p));

                // Try to get parameter name if available
                QByteArrayList paramNames = method.parameterNames();
                if (p < paramNames.size() && !paramNames.at(p).isEmpty()) {
                    paramObj["name"] = QString::fromUtf8(paramNames.at(p));
                } else {
                    paramObj["name"] = "param" + QString::number(p);
                }

                params.append(paramObj);
            }
            methodObj["parameters"] = params;
        }

        methodsArray.append(methodObj);
    }

    return methodsArray;
}

bool CoreManagerPlugin::installPlugin(const QString& pluginPath) {
    qDebug() << "CoreManager: Installing plugin:" << pluginPath;

    // Verify the source file exists
    QFileInfo sourceFileInfo(pluginPath);
    if (!sourceFileInfo.exists() || !sourceFileInfo.isFile()) {
        qWarning() << "Source plugin file does not exist or is not a file:" << pluginPath;
        return false;
    }

    // TODO: this usage of CoreManager seems incorrect, as it's likely creating two instances, one here
    // and another in the core_manager plugin itself, so things like setPluginsDirectory is not working properly
    // so for now we're setting up the directory, but this should be setup by setPluginDirectory already
    // solution is to ensure the app/ interacts with logos core through the core plugin as much as possible

    // TODO: can be removed once setPluginDirectory works again
    m_pluginsDirectory = QDir::cleanPath(QCoreApplication::applicationDirPath() + "/bin/modules");

    // print plugins directory
    qDebug() << "Plugins directory:" << m_pluginsDirectory;

    // Make sure we have a valid plugins directory
    if (m_pluginsDirectory.isEmpty()) {
        qWarning() << "Plugins directory is not set. Cannot install plugin.";
        return false;
    }

    // Create the plugins directory if it doesn't exist
    QDir pluginsDir(m_pluginsDirectory);
    if (!pluginsDir.exists()) {
        qDebug() << "Creating plugins directory:" << m_pluginsDirectory;
        if (!pluginsDir.mkpath(".")) {
            qWarning() << "Failed to create plugins directory:" << m_pluginsDirectory;
            return false;
        }
    }

    // Get the filename from the source path
    QString fileName = sourceFileInfo.fileName();
    QString destinationPath = pluginsDir.filePath(fileName);

    // Check if the destination file already exists
    QFileInfo destFileInfo(destinationPath);
    if (destFileInfo.exists()) {
        qDebug() << "Plugin already exists at destination. Overwriting:" << destinationPath;
        QFile destFile(destinationPath);
        if (!destFile.remove()) {
            qWarning() << "Failed to remove existing plugin file:" << destinationPath;
            return false;
        }
    }

    // Copy the plugin file to the plugins directory
    QFile sourceFile(pluginPath);
    if (!sourceFile.copy(destinationPath)) {
        qWarning() << "Failed to copy plugin file to plugins directory:" 
                  << sourceFile.errorString();
        return false;
    }

    qDebug() << "Successfully installed plugin:" << fileName << "to" << destinationPath;
    
    // Read the plugin metadata to check for included files
    QPluginLoader loader(pluginPath);
    QJsonObject metadata = loader.metaData();
    if (!metadata.isEmpty()) {
        QJsonObject metaDataObj = metadata.value("MetaData").toObject();
        QJsonArray includeFiles = metaDataObj.value("include").toArray();
        
        if (!includeFiles.isEmpty()) {
            qDebug() << "Plugin has" << includeFiles.size() << "included files to copy";
            
            // Get the source directory (where the plugin is)
            QDir sourceDir = sourceFileInfo.dir();
            
            // Try to copy each included file
            for (const QJsonValue& includeVal : includeFiles) {
                QString includeFileName = includeVal.toString();
                if (includeFileName.isEmpty()) continue;
                
                QString sourceIncludePath = sourceDir.filePath(includeFileName);
                QString destIncludePath = pluginsDir.filePath(includeFileName);
                
                qDebug() << "Checking for included file:" << sourceIncludePath;
                
                // Check if the source file exists
                QFileInfo includeFileInfo(sourceIncludePath);
                if (includeFileInfo.exists() && includeFileInfo.isFile()) {
                    qDebug() << "Found included file:" << sourceIncludePath;
                    
                    // Check if the destination file already exists
                    QFileInfo destIncludeFileInfo(destIncludePath);
                    if (destIncludeFileInfo.exists()) {
                        qDebug() << "Included file already exists at destination. Overwriting:" << destIncludePath;
                        QFile destIncludeFile(destIncludePath);
                        if (!destIncludeFile.remove()) {
                            qWarning() << "Failed to remove existing included file:" << destIncludePath;
                            // Continue anyway - this isn't a fatal error
                        }
                    }
                    
                    // Copy the included file
                    QFile includeFile(sourceIncludePath);
                    if (includeFile.copy(destIncludePath)) {
                        qDebug() << "Successfully copied included file:" << includeFileName;
                    } else {
                        qWarning() << "Failed to copy included file:" << includeFileName 
                                  << "-" << includeFile.errorString();
                        // Continue anyway - this isn't a fatal error
                    }
                } else {
                    qDebug() << "Included file not found:" << sourceIncludePath;
                    // It's ok if the file doesn't exist - continue with other files
                }
            }
        }
    }
    
    // Process the plugin to register it with the core
    QString pluginName = processPlugin(destinationPath);
    
    if (pluginName.isEmpty()) {
        qWarning() << "Failed to process installed plugin:" << destinationPath;
        return false;
    }
    
    qDebug() << "Successfully processed installed plugin:" << pluginName;
    return true;
}
