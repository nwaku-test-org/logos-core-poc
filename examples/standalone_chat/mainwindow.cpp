#include "mainwindow.h"
#include <QApplication>
#include <QCoreApplication>
#include <QPluginLoader>
#include <QDebug>
#include <QLabel>
#include <QVBoxLayout>
#include <QDir>
#include <IComponent.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUi();
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUi()
{
    // Set window properties
    setWindowTitle("Standalone Chat App");
    resize(800, 600);
    
    // Load the chat_ui plugin
    loadChatUiPlugin();
}

void MainWindow::loadChatUiPlugin()
{
    // Determine the appropriate extension based on the platform
#ifdef Q_OS_MAC
    QString extension = ".dylib";
#else
    QString extension = ".so";
#endif

    // Try to load the chat_ui plugin from the bin directory
    QString pluginPath = QCoreApplication::applicationDirPath() + "/chat_ui" + extension;
    qDebug() << "Attempting to load chat UI plugin from:" << pluginPath;
    
    QPluginLoader loader(pluginPath);
    QWidget* chatWidget = nullptr;

    if (loader.load()) {
        QObject* pluginInstance = loader.instance();
        qDebug() << "Plugin loaded, instance created:" << (pluginInstance != nullptr);
        
        if (pluginInstance) {
            // Method 1: Try to cast to IComponent interface
            IComponent* component = qobject_cast<IComponent*>(pluginInstance);
            if (component) {
                qDebug() << "Successfully cast plugin to IComponent";
                chatWidget = component->createWidget();
                if (chatWidget) {
                    qDebug() << "Successfully created widget using component interface";
                } else {
                    qDebug() << "Component->createWidget() returned nullptr";
                }
            } else {
                qDebug() << "Failed to cast plugin to IComponent";
                
                // Method 2: Try using QMetaObject::invokeMethod as fallback
                qDebug() << "Trying QMetaObject::invokeMethod as fallback";
                bool invoked = QMetaObject::invokeMethod(pluginInstance, "createWidget",
                                         Qt::DirectConnection,
                                         Q_RETURN_ARG(QWidget*, chatWidget));
                
                if (invoked) {
                    qDebug() << "QMetaObject::invokeMethod succeeded, widget created:" << (chatWidget != nullptr);
                } else {
                    qDebug() << "QMetaObject::invokeMethod failed to call createWidget()";
                }
            }
        } else {
            qWarning() << "Failed to get plugin instance:" << loader.errorString();
        }
    } else {
        qWarning() << "Failed to load chat UI plugin:" << loader.errorString();
    }

    if (chatWidget) {
        setCentralWidget(chatWidget);
    } else {
        qWarning() << "================================================";
        qWarning() << "Failed to load chat UI plugin from:" << pluginPath;
        qWarning() << "================================================";
        
        // Fallback: show a message when plugin is not found
        QWidget* fallbackWidget = new QWidget(this);
        QVBoxLayout* layout = new QVBoxLayout(fallbackWidget);

        QLabel* messageLabel = new QLabel("Chat UI module not loaded", fallbackWidget);
        QFont font = messageLabel->font();
        font.setPointSize(14);
        messageLabel->setFont(font);
        messageLabel->setAlignment(Qt::AlignCenter);

        layout->addWidget(messageLabel);
        setCentralWidget(fallbackWidget);
    }
} 