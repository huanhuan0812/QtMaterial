// MFilledButton.cpp
#include "MFilledButton.h"
#include <QPainter>
#include <QPainterPath>

MFilledButton::MFilledButton(QWidget *parent)
    : MButtonBase(parent)
    , m_backgroundColor(primaryColor())
{
}

MFilledButton::MFilledButton(const QString &text, QWidget *parent)
    : MButtonBase(text, parent)
    , m_backgroundColor(primaryColor())
{
}

MFilledButton::MFilledButton(const QIcon &icon, const QString &text, QWidget *parent)
    : MButtonBase(icon, text, parent)
    , m_backgroundColor(primaryColor())
{
}

MFilledButton::~MFilledButton()
{
}

QColor MFilledButton::backgroundColor() const { return m_backgroundColor; }
void MFilledButton::setBackgroundColor(const QColor &color) {
    if (m_backgroundColor != color) {
        m_backgroundColor = color;
        update();
    }
}

void MFilledButton::paintBackground(QPainter &painter, const QRect &rect)
{
    QColor bgColor = m_backgroundColor;
    
    if (!isEnabled()) {
        QColor disabledBg = palette().color(QPalette::Button);
        disabledBg.setAlphaF(0.12);
        bgColor = disabledBg;
    }
    
    QPainterPath backgroundPath;
    backgroundPath.addRoundedRect(rect, cornerRadius(), cornerRadius());
    painter.fillPath(backgroundPath, bgColor);
}

void MFilledButton::paintBorder(QPainter &painter, const QRect &rect)
{
    // Filled button 通常不需要边框，但如果需要可以在这里添加
    // 可选：在禁用状态或特殊情况下添加细边框
    if (!isEnabled()) {
        QColor borderColor = palette().color(QPalette::ButtonText);
        borderColor.setAlphaF(0.12);
        QPen borderPen(borderColor, 1.0);
        painter.setPen(borderPen);
        painter.setBrush(Qt::NoBrush);
        
        QRectF borderRect = QRectF(rect).adjusted(0.5, 0.5, -0.5, -0.5);
        painter.drawRoundedRect(borderRect, cornerRadius(), cornerRadius());
    }
}

QColor MFilledButton::getCurrentTextColor() const
{
    if (!isEnabled()) {
        QColor onSurface = palette().color(QPalette::ButtonText);
        onSurface.setAlphaF(0.38);
        return onSurface;
    }
    
    // 根据背景色计算对比色（白色或黑色）
    return getContrastColor(m_backgroundColor);
}

QColor MFilledButton::getCurrentIconColor() const
{
    return getCurrentTextColor();
}

QColor MFilledButton::getContrastColor(const QColor &color) const
{
    // 计算亮度 (YCbCr 算法)
    qreal luminance = (0.299 * color.redF() + 
                       0.587 * color.greenF() + 
                       0.114 * color.blueF());
    
    // 亮度大于 0.5 返回深色文字，否则返回白色文字
    if (luminance > 0.5) {
        return QColor(0x1e, 0x1e, 0x1e);  // 深灰色
    } else {
        return QColor(0xff, 0xff, 0xff);  // 白色
    }
}