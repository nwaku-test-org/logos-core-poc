#include "sidebarbutton.h"
#include <QPainter>
#include <QPaintEvent>

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
    
    // Style the button
    setStyleSheet(
        "SidebarButton {"
        "    text-align: left;"
        "    padding-left: 20px;"
        "    border: none;"
        "    color: #ecf0f1;"
        "    font-size: 14px;"
        "}"
        "SidebarButton:hover {"
        "    background-color: #34495e;"
        "}"
    );
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
        painter.setBrush(QColor("#3498db"));  // Blue indicator
        
        // Draw a vertical bar on the left side
        painter.drawRect(0, 0, 5, height());
    }
}

QSize SidebarButton::sizeHint() const
{
    return QSize(200, 50);
} 