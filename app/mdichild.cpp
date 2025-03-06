#include "mdichild.h"
#include <QRandomGenerator>

MdiChild::MdiChild(QWidget *parent)
    : QWidget(parent)
{
    // Create a layout for the child window
    layout = new QVBoxLayout(this);
    
    // Create a label with some content
    contentLabel = new QLabel(this);
    
    // Set terminal-like black background with white text
    QString styleSheet = QString("background-color: rgb(0, 0, 0); "
                                "color: rgb(255, 255, 255); "
                                "border: 1px solid #333333; "
                                "padding: 10px; "
                                "font-family: 'Courier New', monospace;");
    contentLabel->setStyleSheet(styleSheet);
    contentLabel->setAlignment(Qt::AlignCenter);
    contentLabel->setText("MDI Child Window Content\n\n"
                         "This is a sample MDI child window.\n"
                         "You can add more windows using the 'Add Window' button.\n"
                         "You can toggle between windowed and tabbed mode using the 'Toggle View Mode' button.");
    
    // Add the label to the layout
    layout->addWidget(contentLabel);
    
    // Set the layout for this widget
    setLayout(layout);
    
    // Set minimum size
    setMinimumSize(300, 200);
}

MdiChild::~MdiChild()
{
} 