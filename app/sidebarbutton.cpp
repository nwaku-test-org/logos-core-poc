#include "sidebarbutton.h"
#include <QPainter>
#include <QPaintEvent>
#include <QApplication>

SidebarButton::SidebarButton(const QString &text, const QIcon &icon, QWidget *parent)
    : QPushButton(parent), m_active(false)
{
    setText(text);
    setIcon(icon);
    setIconSize(QSize(24, 24));
    setCheckable(true);
    setFlat(true);
    setCursor(Qt::PointingHandCursor);
    
    // Set fixed height for consistent look
    setFixedHeight(50);
    
    // Get system colors
    QPalette palette = QApplication::palette();
    QString textColor = palette.color(QPalette::ButtonText).name();
    QString hoverColor = palette.color(QPalette::Highlight).lighter(150).name();
    
    // Style the button with system colors
    QString styleSheet = QString(
        "SidebarButton {"
        "    text-align: left;"
        "    padding-left: 20px;"
        "    border: none;"
        "    color: %1;"
        "    font-size: 14px;"
        "}"
        "SidebarButton:hover {"
        "    background-color: %2;"
        "}"
    ).arg(textColor, hoverColor);
    
    setStyleSheet(styleSheet);
}

void SidebarButton::setActive(bool active)
{
    m_active = active;
    setChecked(active);
    update();  // Trigger a repaint
}

bool SidebarButton::isActive() const
{
    return m_active;
}

void SidebarButton::paintEvent(QPaintEvent *event)
{
    QPushButton::paintEvent(event);
    
    if (m_active) {
        QPainter painter(this);
        painter.setPen(Qt::NoPen);
        
        // Use system highlight color for the indicator
        painter.setBrush(QApplication::palette().color(QPalette::Highlight));
        
        // Draw a vertical bar on the left side
        painter.drawRect(0, 0, 5, height());
    }
}

QSize SidebarButton::sizeHint() const
{
    return QSize(200, 50);
} 