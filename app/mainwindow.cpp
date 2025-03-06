#include "mainwindow.h"
#include <QApplication>
#include <QScreen>
#include <QSpacerItem>
#include <QFont>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUi();
    createSidebar();
    createContentPages();
    
    // Set the first button as active by default
    if (!m_sidebarButtons.isEmpty()) {
        m_sidebarButtons[0]->setActive(true);
        m_contentStack->setCurrentIndex(0);
    }
    
    // Set window title and size
    setWindowTitle("Logos Core POC");
    resize(1024, 768);
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUi()
{
    // Create central widget and main layout
    m_centralWidget = new QWidget(this);
    setCentralWidget(m_centralWidget);
    
    m_mainLayout = new QHBoxLayout(m_centralWidget);
    m_mainLayout->setSpacing(0);
    m_mainLayout->setContentsMargins(0, 0, 0, 0);
    
    // Create sidebar frame
    m_sidebar = new QFrame(m_centralWidget);
    m_sidebar->setObjectName("sidebar");
    m_sidebar->setFrameShape(QFrame::NoFrame);
    m_sidebar->setMinimumWidth(80);
    m_sidebar->setMaximumWidth(70);
    
    // Use system palette colors for sidebar
    QPalette palette = QApplication::palette();
    QString sidebarStyle = QString("QFrame#sidebar { "
                                  "background-color: %1; "
                                  "border-right: 1px solid %2; "
                                  "}").arg(palette.color(QPalette::Window).name(),
                                          palette.color(QPalette::Mid).name());
    m_sidebar->setStyleSheet(sidebarStyle);
    
    m_sidebarLayout = new QVBoxLayout(m_sidebar);
    m_sidebarLayout->setSpacing(10);
    m_sidebarLayout->setContentsMargins(10, 10, 10, 10);
    m_sidebarLayout->setAlignment(Qt::AlignCenter);
    
    // Create content stack
    m_contentStack = new QStackedWidget(m_centralWidget);
    
    // Add widgets to main layout
    m_mainLayout->addWidget(m_sidebar);
    m_mainLayout->addWidget(m_contentStack);
}

void MainWindow::createSidebar()
{
    // Create sidebar buttons with icons
    struct ButtonInfo {
        QString text;
        QString iconPath;
    };
    
    ButtonInfo buttonInfos[] = {
        {"Apps", ":/icons/home.png"},
        {"Dashboard", ":/icons/chart.png"},
        {"Modules", ":/icons/modules.png"},
        {"Settings", ":/icons/settings.png"}
    };
    
    for (const auto &info : buttonInfos) {
        SidebarButton *button = new SidebarButton(info.text, QIcon(info.iconPath), m_sidebar);
        connect(button, &SidebarButton::clicked, this, &MainWindow::onSidebarButtonClicked);
        m_sidebarLayout->addWidget(button);
        m_sidebarButtons.append(button);
    }
    
    // Add spacer at the bottom to push buttons to the top
    m_sidebarLayout->addItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding));
}

void MainWindow::createContentPages()
{
    // Create MDI View as the first page
    m_mdiView = new MdiView();
    m_contentStack->addWidget(m_mdiView);
    
    // Create other content pages
    QStringList pageTitles = {"Dashboard", "Modules", "Settings"};
    
    for (const QString &title : pageTitles) {
        QWidget *page = new QWidget();
        QVBoxLayout *layout = new QVBoxLayout(page);
        
        QLabel *titleLabel = new QLabel(title, page);
        QFont titleFont = titleLabel->font();
        titleFont.setPointSize(24);
        titleFont.setBold(true);
        titleLabel->setFont(titleFont);
        titleLabel->setAlignment(Qt::AlignCenter);
        
        QLabel *contentLabel = new QLabel("This is the " + title + " content area.", page);
        contentLabel->setAlignment(Qt::AlignCenter);
        
        layout->addWidget(titleLabel);
        layout->addWidget(contentLabel);
        layout->addStretch();
        
        m_contentStack->addWidget(page);
    }
}

void MainWindow::onSidebarButtonClicked()
{
    SidebarButton *clickedButton = qobject_cast<SidebarButton*>(sender());
    if (!clickedButton)
        return;
    
    // Deactivate all buttons
    for (SidebarButton *button : m_sidebarButtons) {
        button->setActive(false);
    }
    
    // Activate the clicked button
    clickedButton->setActive(true);
    
    // Switch to the corresponding content page
    int index = m_sidebarButtons.indexOf(clickedButton);
    if (index >= 0 && index < m_contentStack->count()) {
        m_contentStack->setCurrentIndex(index);
    }
} 
