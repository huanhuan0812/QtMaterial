// MTextButton.cpp
#include "MTextButton.h"
#include <QPainter>

MTextButton::MTextButton(QWidget *parent)
    : MButtonBase(parent)
{
}

MTextButton::MTextButton(const QString &text, QWidget *parent)
    : MButtonBase(text, parent)
{
}

MTextButton::MTextButton(const QIcon &icon, const QString &text, QWidget *parent)
    : MButtonBase(icon, text, parent)
{
}

MTextButton::~MTextButton()
{
}

void MTextButton::paintBackground(QPainter &painter, const QRect &rect)
{
    // Text Button 背景透明，不需要绘制
    // 状态层会在基类中绘制，提供悬停和按下效果
    Q_UNUSED(painter);
    Q_UNUSED(rect);
}

void MTextButton::paintBorder(QPainter &painter, const QRect &rect)
{
    // Text Button 无边框
    Q_UNUSED(painter);
    Q_UNUSED(rect);
}

QColor MTextButton::getCurrentTextColor() const
{
    return textColor();
}

QColor MTextButton::getCurrentIconColor() const
{
    return textColor();
}

QColor MTextButton::textColor() const
{
    if (!isEnabled()) {
        QColor onSurface = palette().color(QPalette::ButtonText);
        onSurface.setAlphaF(0.38);
        return onSurface;
    }
    
    QColor color = primaryColor();
    
    // 根据状态调整透明度
    if (isPressed()) {
        color.setAlphaF(0.5);
    } else if (isHovered()) {
        color.setAlphaF(0.8);
    }
    
    return color;
}