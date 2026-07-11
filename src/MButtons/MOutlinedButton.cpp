// MOutlinedButton.cpp
#include "MOutlinedButton.h"
#include <QPainter>

MOutlinedButton::MOutlinedButton(QWidget *parent)
    : MButtonBase(parent)
{
}

MOutlinedButton::MOutlinedButton(const QString &text, QWidget *parent)
    : MButtonBase(text, parent)
{
}

MOutlinedButton::MOutlinedButton(const QIcon &icon, const QString &text, QWidget *parent)
    : MButtonBase(icon, text, parent)
{
}

MOutlinedButton::~MOutlinedButton()
{
}

qreal MOutlinedButton::borderWidth() const { return m_borderWidth; }
void MOutlinedButton::setBorderWidth(qreal width) {
    if (!qFuzzyCompare(m_borderWidth, width)) {
        m_borderWidth = width;
        update();
    }
}

void MOutlinedButton::paintBackground(QPainter &painter, const QRect &rect)
{
    // Outlined Button 背景透明，不需要绘制
    Q_UNUSED(painter);
    Q_UNUSED(rect);
}

void MOutlinedButton::paintBorder(QPainter &painter, const QRect &rect)
{
    QColor outline = outlineColor();
    QPen borderPen(outline, m_borderWidth);
    painter.setPen(borderPen);
    painter.setBrush(Qt::NoBrush);
    
    QRectF borderRect = QRectF(rect).adjusted(m_borderWidth/2.0, m_borderWidth/2.0,
                                              -m_borderWidth/2.0, -m_borderWidth/2.0);
    painter.drawRoundedRect(borderRect, cornerRadius(), cornerRadius());
}

QColor MOutlinedButton::getCurrentTextColor() const
{
    if (!isEnabled()) {
        QColor onSurface = palette().color(QPalette::ButtonText);
        onSurface.setAlphaF(0.38);
        return onSurface;
    }
    return primaryColor();
}

QColor MOutlinedButton::getCurrentIconColor() const
{
    return getCurrentTextColor();
}

QColor MOutlinedButton::outlineColor() const
{
    if (!isEnabled()) {
        QColor onSurface = palette().color(QPalette::ButtonText);
        onSurface.setAlphaF(0.12 * 0.38);
        return onSurface;
    }
    
    QColor onSurface = palette().color(QPalette::ButtonText);
    
    if (isHovered() || hasFocus() || isPressed()) {
        return primaryColor();
    }
    
    onSurface.setAlphaF(0.12);
    return onSurface;
}