#include "pluginmethodsview.h"
#include <QFont>
#include <QHBoxLayout>

PluginMethodsView::PluginMethodsView(const QString& pluginName, QWidget* parent)
    : QWidget(parent)
    , m_pluginName(pluginName)
    , m_layout(nullptr)
    , m_titleLabel(nullptr)
    , m_contentLabel(nullptr)
    , m_backButton(nullptr)
{
    setupUi();
}

PluginMethodsView::~PluginMethodsView()
{
    // Qt automatically cleans up child widgets
}

void PluginMethodsView::setupUi()
{
    // Set up the layout
    m_layout = new QVBoxLayout(this);
    m_layout->setSpacing(20);
    m_layout->setContentsMargins(40, 40, 40, 40);

    // Create a header layout with back button and title
    QHBoxLayout* headerLayout = new QHBoxLayout();
    
    // Create back button
    m_backButton = new QPushButton("← Back", this);
    m_backButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #4b4b4b;"
        "   color: #ffffff;"
        "   border-radius: 4px;"
        "   padding: 8px 16px;"
        "   font-size: 14px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #5b5b5b;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #3b3b3b;"
        "}"
    );
    connect(m_backButton, &QPushButton::clicked, this, &PluginMethodsView::onBackButtonClicked);
    headerLayout->addWidget(m_backButton);
    
    // Add horizontal spacer after back button
    headerLayout->addSpacing(20);

    // Create and style the title
    m_titleLabel = new QLabel(m_pluginName + " Methods", this);
    QFont titleFont = m_titleLabel->font();
    titleFont.setPointSize(24);
    titleFont.setBold(true);
    m_titleLabel->setFont(titleFont);
    m_titleLabel->setStyleSheet("color: #ffffff;");
    headerLayout->addWidget(m_titleLabel);
    
    // Add stretch to push everything to the left
    headerLayout->addStretch();
    
    // Add the header layout to main layout
    m_layout->addLayout(headerLayout);

    // Add a content label (placeholder for now)
    m_contentLabel = new QLabel("Plugin methods will be displayed here", this);
    m_contentLabel->setStyleSheet("color: #a0a0a0; font-size: 16px; margin-top: 20px;");
    m_contentLabel->setAlignment(Qt::AlignCenter);

    // Add widgets to layout
    m_layout->addWidget(m_contentLabel);
    m_layout->addStretch();

    // Set style for the view
    setStyleSheet("background-color: #1e1e1e;");
}

void PluginMethodsView::onBackButtonClicked()
{
    emit backClicked();
}