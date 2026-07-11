// MElevatedButton.cpp
#include "MElevatedButton.h"
#include <QPainter>
#include <QPainterPath>
#include <QGraphicsDropShadowEffect>
#include <QPropertyAnimation>
#include <QMouseEvent>

MElevatedButton::MElevatedButton(QWidget *parent)
    : MButtonBase(parent)
    , m_backgroundColor(primaryColor())
{
    // 初始化阴影动画
    m_elevationAnimation = new QPropertyAnimation(this, "elevationOffset", this);
    m_elevationAnimation->setDuration(animationDuration());
    m_elevationAnimation->setEasingCurve(QEasingCurve::OutCubic);
    
    // 设置默认背景色
    m_backgroundColor = primaryColor();
    
    // Elevated button 默认启用悬停提升效果
    setHoverElevationEnabled(true);
}

MElevatedButton::MElevatedButton(const QString &text, QWidget *parent)
    : MElevatedButton(parent)
{
    setText(text);
}

MElevatedButton::MElevatedButton(const QIcon &icon, const QString &text, QWidget *parent)
    : MElevatedButton(text, parent)
{
    setIcon(icon);
}

MElevatedButton::~MElevatedButton()
{
}

qreal MElevatedButton::elevation() const { return m_elevation; }
void MElevatedButton::setElevation(qreal elevation) {
    if (!qFuzzyCompare(m_elevation, elevation)) {
        m_elevation = elevation;
        if (!isPressed()) {
            m_currentElevation = elevation;
            update();
        }
    }
}

qreal MElevatedButton::pressedElevation() const { return m_pressedElevation; }
void MElevatedButton::setPressedElevation(qreal elevation) {
    if (!qFuzzyCompare(m_pressedElevation, elevation)) {
        m_pressedElevation = elevation;
        update();
    }
}

QColor MElevatedButton::backgroundColor() const { return m_backgroundColor; }
void MElevatedButton::setBackgroundColor(const QColor &color) {
    if (m_backgroundColor != color) {
        m_backgroundColor = color;
        update();
    }
}

void MElevatedButton::paintBackground(QPainter &painter, const QRect &rect)
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

void MElevatedButton::paintBorder(QPainter &painter, const QRect &rect)
{
    // Elevated button 通常不需要边框
    Q_UNUSED(painter);
    Q_UNUSED(rect);
}

void MElevatedButton::paintShadow(QPainter &painter, const QRect &rect)
{
    paintElevatedShadow(painter, rect, m_currentElevation);
}

void MElevatedButton::paintElevatedShadow(QPainter &painter, const QRect &rect, qreal elevation)
{
    if (elevation <= 0.01) return;
    
    painter.save();
    painter.setRenderHint(QPainter::Antialiasing, true);
    
    // 根据 Material Design 规范，阴影强度随高度增加
    int alpha = qMin(75, static_cast<int>(25 + elevation * 6));
    int blurRadius = static_cast<int>(2 + elevation * 1.5);
    int yOffset = static_cast<int>(1 + elevation * 0.8);
    
    // 绘制多层阴影以模拟真实的 Material Design 阴影
    QColor shadowColor(0, 0, 0, alpha * 0.3);
    QPainterPath shadowPath;
    shadowPath.addRoundedRect(rect.translated(0, yOffset), 
                              cornerRadius(), cornerRadius());
    painter.fillPath(shadowPath, shadowColor);
    
    if (elevation > 4) {
        QColor shadowColor2(0, 0, 0, alpha * 0.2);
        QPainterPath shadowPath2;
        shadowPath2.addRoundedRect(rect.translated(0, yOffset * 2), 
                                   cornerRadius(), cornerRadius());
        painter.fillPath(shadowPath2, shadowColor2);
    }
    
    painter.restore();
}

QColor MElevatedButton::getCurrentTextColor() const
{
    if (!isEnabled()) {
        QColor onSurface = palette().color(QPalette::ButtonText);
        onSurface.setAlphaF(0.38);
        return onSurface;
    }
    
    return getContrastColor(m_backgroundColor);
}

QColor MElevatedButton::getCurrentIconColor() const
{
    return getCurrentTextColor();
}

QColor MElevatedButton::getContrastColor(const QColor &color) const
{
    qreal luminance = (0.299 * color.redF() + 
                       0.587 * color.greenF() + 
                       0.114 * color.blueF());
    
    if (luminance > 0.5) {
        return QColor(0x1e, 0x1e, 0x1e);
    } else {
        return QColor(0xff, 0xff, 0xff);
    }
}

void MElevatedButton::animateElevation(bool pressed)
{
    m_elevationAnimation->stop();
    
    qreal targetElevation = pressed ? m_pressedElevation : m_elevation;
    
    m_elevationAnimation->setStartValue(m_currentElevation);
    m_elevationAnimation->setEndValue(targetElevation);
    m_elevationAnimation->start();
    
    // 连接动画更新信号
    connect(m_elevationAnimation, &QPropertyAnimation::valueChanged,
            this, [this](const QVariant &value) {
                m_currentElevation = value.toReal();
                update();
            });
}

void MElevatedButton::updateElevation()
{
    animateElevation(isPressed());
}

// 重写事件处理以支持阴影动画
void MElevatedButton::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        animateElevation(true);
    }
    MButtonBase::mousePressEvent(event);
}

void MElevatedButton::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        animateElevation(false);
    }
    MButtonBase::mouseReleaseEvent(event);
}

void MElevatedButton::enterEvent(QEnterEvent *event)
{
    MButtonBase::enterEvent(event);
    if (!isPressed() && isHoverElevationEnabled()) {
        animateElevation(false); // 悬停时会通过基类的 elevationOffset 处理
    }
}

void MElevatedButton::leaveEvent(QEvent *event)
{
    MButtonBase::leaveEvent(event);
    if (!isPressed()) {
        animateElevation(false);
    }
}