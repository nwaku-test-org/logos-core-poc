#include "coremoduleview.h"
#include <QFont>
#include <memory>
#include <QStringList>
#include <QCoreApplication>
#include <QDebug>
#include <QPushButton>
#include <QHBoxLayout>
#include <QStackedWidget>
#include "core/plugin_registry.h"
#include "pluginmethodsview.h"

CoreModuleView::CoreModuleView(QWidget *parent)
    : QWidget(parent)
    , m_layout(nullptr)
    , m_titleLabel(nullptr)
    , m_subtitleLabel(nullptr)
    , m_pluginList(nullptr)
    , m_updateTimer(nullptr)
    , m_stackedWidget(nullptr)
    , m_pluginsListWidget(nullptr)
    , m_currentMethodsView(nullptr)
{
    setupUi();
    
    // Initial update of plugin list
    updatePluginList();
}

CoreModuleView::~CoreModuleView()
{
    // Qt automatically cleans up child widgets
}

void CoreModuleView::setupUi()
{
    // Set the background color for the entire view
    setStyleSheet("QWidget#coreModuleView { background-color: #1e1e1e; }");
    setObjectName("coreModuleView");

    // Create main layout
    m_layout = new QVBoxLayout(this);
    m_layout->setSpacing(0);
    m_layout->setContentsMargins(0, 0, 0, 0);

    // Create stacked widget
    m_stackedWidget = new QStackedWidget(this);
    m_layout->addWidget(m_stackedWidget);
    
    // Create plugins list widget
    m_pluginsListWidget = new QWidget(this);
    QVBoxLayout* pluginsLayout = new QVBoxLayout(m_pluginsListWidget);
    pluginsLayout->setSpacing(20);
    pluginsLayout->setContentsMargins(40, 40, 40, 40);
    
    // Create and style the title
    m_titleLabel = new QLabel("Core Modules", m_pluginsListWidget);
    QFont titleFont = m_titleLabel->font();
    titleFont.setPointSize(24);
    titleFont.setBold(true);
    m_titleLabel->setFont(titleFont);
    m_titleLabel->setStyleSheet("color: #ffffff;");

    // Add a subtitle
    m_subtitleLabel = new QLabel("Currently loaded plugins in the system", m_pluginsListWidget);
    m_subtitleLabel->setStyleSheet("color: #a0a0a0; font-size: 14px; margin-bottom: 20px;");

    pluginsLayout->addWidget(m_titleLabel);
    pluginsLayout->addWidget(m_subtitleLabel);
    
    // Create the plugin list
    createPluginList();
    
    // Add plugins list widget to stacked widget
    m_stackedWidget->addWidget(m_pluginsListWidget);
}

void CoreModuleView::createPluginList()
{
    // Get the plugins layout
    QVBoxLayout* pluginsLayout = qobject_cast<QVBoxLayout*>(m_pluginsListWidget->layout());
    if (!pluginsLayout) {
        return;
    }
    
    // Create a container widget for the list with padding
    QWidget* listContainer = new QWidget(m_pluginsListWidget);
    listContainer->setObjectName("listContainer");
    listContainer->setStyleSheet(
        "QWidget#listContainer {"
        "   background-color: #2d2d2d;"
        "   border-radius: 8px;"
        "   padding: 20px;"
        "   border: 1px solid #3d3d3d;"
        "}"
        "QPushButton {"
        "   background-color: #4b4b4b;"
        "   color: #ffffff;"
        "   border-radius: 4px;"
        "   padding: 6px 12px;"
        "   font-size: 12px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #5b5b5b;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #3b3b3b;"
        "}"
    );
    QVBoxLayout* containerLayout = new QVBoxLayout(listContainer);
    containerLayout->setContentsMargins(20, 20, 20, 20);
    
    // Create the list widget
    m_pluginList = new QListWidget(listContainer);
    m_pluginList->setMinimumHeight(300);
    m_pluginList->setFrameShape(QFrame::NoFrame); // Remove the default border
    m_pluginList->setStyleSheet(
        "QListWidget {"
        "   background-color: transparent;"
        "   border: none;"
        "   margin: 16px 0px;"
        "}"
        "QListWidget::item {"
        "   color: #e0e0e0;"
        "   margin: 0px 0px;"
        "   padding: 0px 0px;"
        "   border-radius: 6px;"
        "   background-color: #363636;"
        "}"
        "QListWidget::item:hover {"
        "   background-color: #404040;"
        "}"
        "QListWidget::item:selected {"
        "   background-color: #404040;"
        "   color: #ffffff;"
        "}"
    );
    
    containerLayout->addWidget(m_pluginList);
    pluginsLayout->addWidget(listContainer);
}

void CoreModuleView::updatePluginList()
{
    qDebug() << "\n\n----------> Updating plugin list\n\n";
    // Get the core_manager plugin
    QObject* coreManagerPlugin = PluginRegistry::getPlugin<QObject>("core_manager");
    if (!coreManagerPlugin) {
        qWarning() << "Core manager plugin not found!";
        return;
    }

    // Get the list of loaded plugins using invokeMethod
    QStringList plugins;
    QMetaObject::invokeMethod(coreManagerPlugin, "getLoadedPlugins", 
                            Qt::DirectConnection,
                            Q_RETURN_ARG(QStringList, plugins));
    
    // Clear the current list
    m_pluginList->clear();
    
    if (plugins.isEmpty()) {
        qDebug() << "No plugins loaded";
        QListWidgetItem* item = new QListWidgetItem("No plugins loaded");
        item->setIcon(QIcon(":/icons/plugin.png"));
        item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        m_pluginList->addItem(item);
        return;
    } else {
        qDebug() << "Plugins loaded:";
        foreach (const QString &pluginName, plugins) {
            qDebug() << "  " << pluginName;
        }
    }
    
    // Add each plugin to the list
    foreach (const QString &pluginName, plugins) {
        // Create a widget to hold both the plugin name and the button
        QWidget* itemWidget = new QWidget();
        QHBoxLayout* itemLayout = new QHBoxLayout(itemWidget);
        itemLayout->setContentsMargins(10, 10, 10, 10);
        
        // Add the plugin name
        QLabel* nameLabel = new QLabel(pluginName);
        nameLabel->setStyleSheet("color: #e0e0e0; font-size: 16px;");
        itemLayout->addWidget(nameLabel);
        
        // Add spacer to push the button to the right
        itemLayout->addStretch();
        
        // Add "View Methods" button
        QPushButton* viewMethodsButton = new QPushButton("View Methods");
        viewMethodsButton->setProperty("pluginName", pluginName);
        viewMethodsButton->setMinimumHeight(30); // Set minimum height for button
        connect(viewMethodsButton, &QPushButton::clicked, this, &CoreModuleView::onViewMethodsClicked);
        itemLayout->addWidget(viewMethodsButton);
        
        // Add the widget to the list
        QListWidgetItem* item = new QListWidgetItem();
        itemWidget->setMinimumHeight(50); // Set minimum height for the item
        item->setSizeHint(QSize(itemWidget->sizeHint().width(), 50)); // Force height to be 50 pixels
        m_pluginList->addItem(item);
        m_pluginList->setItemWidget(item, itemWidget);
    }
}

void CoreModuleView::onViewMethodsClicked()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (!button) {
        return;
    }
    
    QString pluginName = button->property("pluginName").toString();
    if (pluginName.isEmpty()) {
        qDebug() << "Plugin name not found in button properties";
        return;
    }
    
    // Create the plugin methods view
    m_currentMethodsView = new PluginMethodsView(pluginName, this);
    connect(m_currentMethodsView, &PluginMethodsView::backClicked, this, &CoreModuleView::onBackToPluginList);
    
    // Add it to the stacked widget
    m_stackedWidget->addWidget(m_currentMethodsView);
    
    // Show the methods view
    m_stackedWidget->setCurrentWidget(m_currentMethodsView);
}

void CoreModuleView::onBackToPluginList()
{
    // Switch back to the plugins list view
    m_stackedWidget->setCurrentWidget(m_pluginsListWidget);
    
    // If there is a current methods view, remove and delete it
    if (m_currentMethodsView) {
        m_stackedWidget->removeWidget(m_currentMethodsView);
        delete m_currentMethodsView;
        m_currentMethodsView = nullptr;
    }
} 