#ifndef PLUGIN_REGISTRY_H
#define PLUGIN_REGISTRY_H

#include <QMap>
#include <QString>
#include <QObject>
#include <QCoreApplication>
#include <QVariant>
#include <QDebug>
#include <QMetaProperty>

// This is a header-only implementation that can be included by both core and plugins
// without creating circular dependencies

// ================================
// TODO: this will be replaced with a remote object registry
// ================================

// Key functions for plugin registration and retrieval
namespace PluginRegistry {

    // Register a plugin in the application properties
    inline void registerPlugin(QObject* plugin, const QString& name) {
        QString pluginKey = name.toLower().replace(" ", "_");
        QCoreApplication::instance()->setProperty(pluginKey.toUtf8().constData(), QVariant::fromValue(plugin));
        qDebug() << "Registered plugin with key:" << pluginKey;
    }

    // Unregister a plugin from the application properties
    inline bool unregisterPlugin(const QString& name) {
        QString pluginKey = name.toLower().replace(" ", "_");
        bool success = QCoreApplication::instance()->setProperty(pluginKey.toUtf8().constData(), QVariant::fromValue(nullptr));
        return true;
    }

    // Get a plugin by name with automatic casting to the requested type
    template<typename T>
    inline T* getPlugin(const QString& name) {
        QString pluginKey = name.toLower().replace(" ", "_");
        QVariant pluginVariant = QCoreApplication::instance()->property(pluginKey.toUtf8().constData());

        if (pluginVariant.isValid()) {
            return qobject_cast<T*>(pluginVariant.value<QObject*>());
        }

        qWarning() << "Plugin not found:" << name;
        return nullptr;
    }

    // Function to get all plugin keys - simpler implementation that doesn't rely on QMetaProperty
    inline QStringList getAllPluginKeys() {
        QStringList result;
        const QMetaObject* metaObj = QCoreApplication::instance()->metaObject();

        for (int i = 0; i < metaObj->propertyCount(); ++i) {
            QMetaProperty prop = metaObj->property(i);
            QString propName = QString::fromUtf8(prop.name());

            // Filter out Qt's built-in properties - this is a simplistic approach
            if (!propName.startsWith("_q_") && propName != "objectName") {
                result.append(propName);
            }
        }

        return result;
    }
}

#endif // PLUGIN_REGISTRY_H 