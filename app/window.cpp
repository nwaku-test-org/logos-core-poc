#include "window.h"
#include <QApplication>
#include <QScreen>
#include <QDebug>
#include <QLabel>
#include <QVBoxLayout>
#include <QPluginLoader>
#include <QDir>
// #include "core/plugin_registry.h"

Window::Window(QWidget *parent)
    : QMainWindow(parent)
{
    setupUi();
}

Window::~Window()
{
}

void Window::setupUi()
{
    // Try to load the main_ui plugin from the plugins directory
    QString pluginPath = QCoreApplication::applicationDirPath() + "/plugins/main_ui.so";
    QPluginLoader loader(pluginPath);

    QWidget* mainContent = nullptr;

    if (loader.load()) {
        QObject* plugin = loader.instance();
        if (plugin) {
            // Try to create the main window using the plugin's createWidget method
            QMetaObject::invokeMethod(plugin, "createWidget",
                                    Qt::DirectConnection,
                                    Q_RETURN_ARG(QWidget*, mainContent));
        }
    }

    if (mainContent) {
        setCentralWidget(mainContent);
    } else {
        qWarning() << "================================================";
        qWarning() << "Failed to load main UI plugin from:" << pluginPath;
        qWarning() << "================================================";
        // Fallback: show a message when plugin is not found
        QWidget* fallbackWidget = new QWidget(this);
        QVBoxLayout* layout = new QVBoxLayout(fallbackWidget);

        QLabel* messageLabel = new QLabel("No main UI module found", fallbackWidget);
        QFont font = messageLabel->font();
        font.setPointSize(14);
        messageLabel->setFont(font);
        messageLabel->setAlignment(Qt::AlignCenter);

        layout->addWidget(messageLabel);
        setCentralWidget(fallbackWidget);

        qWarning() << "Failed to load main UI plugin from:" << pluginPath;
    }

    // Set window title and size
    setWindowTitle("Logos Core POC");
    resize(1024, 768);
} 
