#include "WakuUIWidget.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QDebug>
#include <QCoreApplication>
#include <QMessageBox>
#include "../../core/plugin_registry.h"

WakuUIWidget::WakuUIWidget(QWidget* parent) : QWidget(parent), wakuTestPlugin(nullptr) {
    // Create UI elements
    initButton = new QPushButton("Init Waku", this);
    versionButton = new QPushButton("Get Version", this);
    statusLabel = new QLabel("Status: Not initialized", this);
    
    // Style the status label
    statusLabel->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    statusLabel->setLineWidth(1);
    statusLabel->setMidLineWidth(0);
    statusLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    statusLabel->setMinimumHeight(30);
    statusLabel->setWordWrap(true);

    // Set up layout
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(initButton);
    layout->addWidget(versionButton);
    layout->addWidget(statusLabel);
    
    // Add some spacing and margins
    layout->setSpacing(10);
    layout->setContentsMargins(20, 20, 20, 20);

    // Connect buttons to their slots
    connect(initButton, &QPushButton::clicked, this, &WakuUIWidget::onInitButtonClicked);
    connect(versionButton, &QPushButton::clicked, this, &WakuUIWidget::onVersionButtonClicked);
    
    // Connect to the waku plugin
    bool connected = connectToWakuTestPlugin();
    if (!connected) {
        statusLabel->setText("Status: Failed to connect to waku plugin");
        initButton->setEnabled(false);
        versionButton->setEnabled(false);
    }
}

bool WakuUIWidget::connectToWakuTestPlugin() {
    // Get the WakuTest plugin from the PluginRegistry
    wakuTestPlugin = PluginRegistry::getPlugin<QObject>("waku");
    
    if (!wakuTestPlugin) {
        qWarning() << "Could not find Waku Test Plugin";
        return false;
    }
    
    // Connect to the wakuInitialized and versionReceived signals
    bool connected1 = connect(wakuTestPlugin, SIGNAL(wakuInitialized(bool,QString)), 
                           this, SLOT(onWakuInitialized(bool,QString)));
    
    // Use Qt::QueuedConnection to ensure the signal is processed in the UI thread
    bool connected2 = connect(wakuTestPlugin, SIGNAL(versionReceived(QString)), 
                           this, SLOT(onVersionReceived(QString)),
                           Qt::QueuedConnection);
    
    return connected1 && connected2;
}

void WakuUIWidget::onInitButtonClicked() {
    qDebug() << "Init Waku button clicked";
    
    if (!wakuTestPlugin) {
        if (!connectToWakuTestPlugin()) {
            QMessageBox::warning(this, "Plugin Error", "Could not connect to waku plugin");
            return;
        }
    }
    
    // Call the initWaku method using QMetaObject
    QMetaObject::invokeMethod(wakuTestPlugin, "initWaku");
    statusLabel->setText("Status: Initializing Waku...");
}

void WakuUIWidget::onVersionButtonClicked() {
    qDebug() << "Get Version button clicked";
    
    if (!wakuTestPlugin) {
        if (!connectToWakuTestPlugin()) {
            QMessageBox::warning(this, "Plugin Error", "Could not connect to waku plugin");
            return;
        }
    }
    
    // Call the getVersion method using QMetaObject
    QMetaObject::invokeMethod(wakuTestPlugin, "getVersion");
    statusLabel->setText("Status: Getting version...");
}

void WakuUIWidget::onWakuInitialized(bool success, const QString &message) {
    if (success) {
        statusLabel->setText(QString("Status: %1").arg(message));
    } else {
        statusLabel->setText(QString("Error: %1").arg(message));
    }
}

void WakuUIWidget::onVersionReceived(const QString &version) {
    qDebug() << "===================================================================";
    qDebug() << "onVersionReceived called - version:" << version;

    // With Qt::QueuedConnection, we can use direct setText as we're now in the UI thread
    QString versionText = QString("Waku Version: %1").arg(version);
    statusLabel->setText(versionText);
} 