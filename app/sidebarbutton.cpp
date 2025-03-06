#include "sidebarbutton.h"
#include <QPainter>
#include <QPaintEvent>
#include <QApplication>
#include <QStyle>
#include <QStyleOption>

SidebarButton::SidebarButton(const QString &text, const QIcon &icon, QWidget *parent)
    : QPushButton(parent), m_active(false)
{
    setText(text);
    setIcon(icon);
    setIconSize(QSize(32, 32));
    setCheckable(true);
    setFlat(true);
    setCursor(Qt::PointingHandCursor);
    
    // Set fixed size for a more compact button
    setFixedWidth(60);
    setFixedHeight(50);
    
    // Get system colors
    QPalette palette = QApplication::palette();
    QString textColor = palette.color(QPalette::ButtonText).name();
    QString hoverColor = palette.color(QPalette::Highlight).lighter(150).name();
    
    // Style the button with system colors
    QString styleSheet = QString(
        "SidebarButton {"
        "    border: none;"
        "    color: %1;"
        "    font-size: 12px;"
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
    // Draw the button background
    QStyleOption opt;
    opt.initFrom(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
    
    // Draw the icon centered at the top
    QRect iconRect(0, 5, width(), height() - 25);
    QIcon icon = this->icon();
    if (!icon.isNull()) {
        icon.paint(&painter, iconRect, Qt::AlignCenter, isEnabled() ? QIcon::Normal : QIcon::Disabled, isChecked() ? QIcon::On : QIcon::Off);
    }
    
    // Draw the text centered at the bottom
    painter.setPen(palette().color(QPalette::ButtonText));
    QRect textRect(0, height() - 25, width(), 20);
    painter.drawText(textRect, Qt::AlignCenter, text());
    
    // Draw the active indicator if needed
    if (m_active) {
        painter.setPen(Qt::NoPen);
        painter.setBrush(QApplication::palette().color(QPalette::Highlight));
        painter.drawRect(0, height() - 5, width(), 5);
    }
}

QSize SidebarButton::sizeHint() const
{
    return QSize(60, 50);
} 