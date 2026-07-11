// MButtonBase.cpp
#include "MButtonBase.h"
#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent>
#include <QFocusEvent>
#include <QStyleOptionButton>
#include <QStyle>
#include <QtMath>
#include <QRadialGradient>

MButtonBase::MButtonBase(QWidget *parent)
    : QPushButton(parent)
    , m_primaryColor(palette().color(QPalette::Highlight))
    , m_rippleColor(m_primaryColor)
{
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    setCursor(Qt::ArrowCursor);
    setMouseTracking(true);
    setStyleSheet("QPushButton { border: none; background: transparent; }");
    
    setupAnimations();
}

MButtonBase::MButtonBase(const QString &text, QWidget *parent)
    : MButtonBase(parent)
{
    setText(text);
}

MButtonBase::MButtonBase(const QIcon &icon, const QString &text, QWidget *parent)
    : MButtonBase(text, parent)
{
    setIcon(icon);
}

MButtonBase::~MButtonBase()
{
}

void MButtonBase::setupAnimations()
{
    m_rippleOpacityAnimation = new QPropertyAnimation(this, "rippleOpacity", this);
    m_rippleOpacityAnimation->setDuration(m_animationDuration);
    m_rippleOpacityAnimation->setEasingCurve(QEasingCurve::OutCubic);
    
    m_rippleRadiusAnimation = new QPropertyAnimation(this, "rippleRadius", this);
    m_rippleRadiusAnimation->setDuration(m_animationDuration);
    m_rippleRadiusAnimation->setEasingCurve(QEasingCurve::OutCubic);
    
    m_rippleAnimationGroup = new QParallelAnimationGroup(this);
    m_rippleAnimationGroup->addAnimation(m_rippleOpacityAnimation);
    m_rippleAnimationGroup->addAnimation(m_rippleRadiusAnimation);
    
    m_stateLayerAnimation = new QPropertyAnimation(this, "stateLayerOpacity", this);
    m_stateLayerAnimation->setDuration(m_animationDuration);
    m_stateLayerAnimation->setEasingCurve(QEasingCurve::InOutCubic);
    
    m_focusIndicatorAnimation = new QPropertyAnimation(this, "focusIndicatorOpacity", this);
    m_focusIndicatorAnimation->setDuration(m_animationDuration);
    m_focusIndicatorAnimation->setEasingCurve(QEasingCurve::InOutCubic);
    
    m_hoverElevationAnimation = new QPropertyAnimation(this, "elevationOffset", this);
    m_hoverElevationAnimation->setDuration(m_animationDuration);
    m_hoverElevationAnimation->setEasingCurve(QEasingCurve::InOutCubic);
}

// ===================== 属性访问器 =====================

QColor MButtonBase::primaryColor() const { return m_primaryColor; }
void MButtonBase::setPrimaryColor(const QColor &color) {
    if (m_primaryColor != color) {
        m_primaryColor = color;
        if (m_rippleColor == m_primaryColor) {
            m_rippleColor = color;
        }
        update();
    }
}

QColor MButtonBase::rippleColor() const { return m_rippleColor; }
void MButtonBase::setRippleColor(const QColor &color) {
    if (m_rippleColor != color) {
        m_rippleColor = color;
        update();
    }
}

int MButtonBase::cornerRadius() const { return m_cornerRadius; }
void MButtonBase::setCornerRadius(int radius) {
    if (m_cornerRadius != radius) {
        m_cornerRadius = radius;
        update();
    }
}

bool MButtonBase::isRippleEnabled() const { return m_rippleEnabled; }
void MButtonBase::setRippleEnabled(bool enabled) {
    m_rippleEnabled = enabled;
    if (!enabled) {
        m_rippleOpacity = 0.0;
        m_rippleRadius = 0.0;
    }
    update();
}

bool MButtonBase::isStateLayerEnabled() const { return m_stateLayerEnabled; }
void MButtonBase::setStateLayerEnabled(bool enabled) {
    m_stateLayerEnabled = enabled;
    if (!enabled) {
        m_stateLayerOpacity = 0.0;
    }
    update();
}

bool MButtonBase::isFocusIndicatorEnabled() const { return m_focusIndicatorEnabled; }
void MButtonBase::setFocusIndicatorEnabled(bool enabled) {
    m_focusIndicatorEnabled = enabled;
    if (!enabled) {
        m_focusIndicatorOpacity = 0.0;
    }
    update();
}

bool MButtonBase::isHoverElevationEnabled() const { return m_hoverElevationEnabled; }
void MButtonBase::setHoverElevationEnabled(bool enabled) {
    m_hoverElevationEnabled = enabled;
    if (!enabled) {
        m_elevationOffset = 0.0;
    }
    update();
}

int MButtonBase::animationDuration() const { return m_animationDuration; }
void MButtonBase::setAnimationDuration(int ms) {
    m_animationDuration = ms;
    m_rippleOpacityAnimation->setDuration(ms);
    m_rippleRadiusAnimation->setDuration(ms);
    m_stateLayerAnimation->setDuration(ms);
    m_focusIndicatorAnimation->setDuration(ms);
    m_hoverElevationAnimation->setDuration(ms);
}

qreal MButtonBase::rippleOpacity() const { return m_rippleOpacity; }
void MButtonBase::setRippleOpacity(qreal opacity) {
    m_rippleOpacity = opacity;
    update();
}

qreal MButtonBase::rippleRadius() const { return m_rippleRadius; }
void MButtonBase::setRippleRadius(qreal radius) {
    m_rippleRadius = radius;
    update();
}

qreal MButtonBase::stateLayerOpacity() const { return m_stateLayerOpacity; }
void MButtonBase::setStateLayerOpacity(qreal opacity) {
    m_stateLayerOpacity = opacity;
    update();
}

qreal MButtonBase::focusIndicatorOpacity() const { return m_focusIndicatorOpacity; }
void MButtonBase::setFocusIndicatorOpacity(qreal opacity) {
    m_focusIndicatorOpacity = opacity;
    update();
}

qreal MButtonBase::elevationOffset() const { return m_elevationOffset; }
void MButtonBase::setElevationOffset(qreal offset) {
    m_elevationOffset = offset;
    update();
}

// ===================== 事件处理 =====================

void MButtonBase::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_pressed = true;
        if (m_rippleEnabled) {
            m_rippleCenter = event->pos();
            startRippleAnimation(m_rippleCenter);
        }
        if (m_stateLayerEnabled) {
            startStateLayerAnimation(true);
        }
    }
    QPushButton::mousePressEvent(event);
}

void MButtonBase::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_pressed = false;
        if (m_stateLayerEnabled && !m_hovered) {
            startStateLayerAnimation(false);
        } else if (m_stateLayerEnabled && m_hovered) {
            m_stateLayerOpacity = 0.08;
            update();
        }
    }
    QPushButton::mouseReleaseEvent(event);
}

void MButtonBase::mouseMoveEvent(QMouseEvent *event)
{
    m_mousePos = event->pos();
    QPushButton::mouseMoveEvent(event);
}

void MButtonBase::enterEvent(QEnterEvent *event)
{
    m_hovered = true;
    
    if (m_stateLayerEnabled && !m_pressed) {
        startStateLayerAnimation(true);
    }
    if (m_hoverElevationEnabled) {
        startHoverElevationAnimation(true);
    }
    
    QPushButton::enterEvent(event);
}

void MButtonBase::leaveEvent(QEvent *event)
{
    m_hovered = false;
    
    if (m_stateLayerEnabled && !m_pressed) {
        startStateLayerAnimation(false);
    }
    if (m_hoverElevationEnabled) {
        startHoverElevationAnimation(false);
    }
    
    QPushButton::leaveEvent(event);
}

void MButtonBase::focusInEvent(QFocusEvent *event)
{
    if (m_focusIndicatorEnabled) {
        startFocusIndicatorAnimation(true);
    }
    QPushButton::focusInEvent(event);
}

void MButtonBase::focusOutEvent(QFocusEvent *event)
{
    if (m_focusIndicatorEnabled) {
        startFocusIndicatorAnimation(false);
    }
    QPushButton::focusOutEvent(event);
}

void MButtonBase::resizeEvent(QResizeEvent *event)
{
    QPushButton::resizeEvent(event);
}

// ===================== 动画控制 =====================

void MButtonBase::startRippleAnimation(const QPointF &center)
{
    m_rippleCenter = center;
    
    qreal maxRadius = qSqrt(width() * width() + height() * height());
    
    m_rippleAnimationGroup->stop();
    
    m_rippleOpacity = 0.15;
    m_rippleRadius = 0.0;
    
    m_rippleOpacityAnimation->setStartValue(0.15);
    m_rippleOpacityAnimation->setEndValue(0.0);
    
    m_rippleRadiusAnimation->setStartValue(0.0);
    m_rippleRadiusAnimation->setEndValue(maxRadius);
    
    m_rippleAnimationGroup->start();
}

void MButtonBase::startStateLayerAnimation(bool visible)
{
    m_stateLayerAnimation->stop();
    
    qreal targetOpacity = 0.0;
    if (visible) {
        if (m_pressed) {
            targetOpacity = 0.12;
        } else if (m_hovered) {
            targetOpacity = 0.08;
        }
    }
    
    m_stateLayerAnimation->setStartValue(m_stateLayerOpacity);
    m_stateLayerAnimation->setEndValue(targetOpacity);
    m_stateLayerAnimation->start();
}

void MButtonBase::startFocusIndicatorAnimation(bool visible)
{
    m_focusIndicatorAnimation->stop();
    
    qreal targetOpacity = visible ? 1.0 : 0.0;
    
    m_focusIndicatorAnimation->setStartValue(m_focusIndicatorOpacity);
    m_focusIndicatorAnimation->setEndValue(targetOpacity);
    m_focusIndicatorAnimation->start();
}

void MButtonBase::startHoverElevationAnimation(bool hovered)
{
    m_hoverElevationAnimation->stop();
    
    qreal targetOffset = hovered ? 1.0 : 0.0;
    
    m_hoverElevationAnimation->setStartValue(m_elevationOffset);
    m_hoverElevationAnimation->setEndValue(targetOffset);
    m_hoverElevationAnimation->start();
}

// ===================== 公共绘制函数 =====================

void MButtonBase::paintEvent(QPaintEvent * /*event*/)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    
    const QRect rect = this->rect();
    
    // 1. 绘制悬停提升效果
    paintHoverElevation(painter, rect);
    
    // 2. 绘制背景（子类实现）
    paintBackground(painter, rect);
    
    // 3. 绘制状态层
    paintStateLayer(painter, rect);
    
    // 4. 绘制边框（子类实现）
    paintBorder(painter, rect);
    
    // 5. 绘制焦点指示器
    paintFocusIndicator(painter, rect);
    
    // 6. 绘制水波纹
    paintRipple(painter, rect);
    
    // 7. 绘制图标和文字
    paintLabel(painter);
}

void MButtonBase::paintRipple(QPainter &painter, const QRect &rect)
{
    if (!m_rippleEnabled || m_rippleOpacity <= 0.001 || m_rippleRadius <= 0.0) {
        return;
    }
    
    painter.save();
    
    QPainterPath clipPath;
    clipPath.addRoundedRect(rect, m_cornerRadius, m_cornerRadius);
    painter.setClipPath(clipPath);
    
    QRadialGradient rippleGradient(m_rippleCenter, m_rippleRadius);
    QColor rippleStart = m_rippleColor;
    rippleStart.setAlphaF(m_rippleOpacity);
    QColor rippleEnd = m_rippleColor;
    rippleEnd.setAlphaF(0.0);
    
    rippleGradient.setColorAt(0.0, rippleStart);
    rippleGradient.setColorAt(0.7, rippleStart);
    rippleGradient.setColorAt(1.0, rippleEnd);
    
    painter.setPen(Qt::NoPen);
    painter.setBrush(rippleGradient);
    painter.drawEllipse(m_rippleCenter, m_rippleRadius, m_rippleRadius);
    
    painter.restore();
}

void MButtonBase::paintStateLayer(QPainter &painter, const QRect &rect)
{
    if (!m_stateLayerEnabled || m_stateLayerOpacity <= 0.001) {
        return;
    }
    
    QColor stateLayerColor = m_primaryColor;
    stateLayerColor.setAlphaF(m_stateLayerOpacity);
    
    QPainterPath statePath;
    statePath.addRoundedRect(rect, m_cornerRadius, m_cornerRadius);
    painter.fillPath(statePath, stateLayerColor);
}

void MButtonBase::paintFocusIndicator(QPainter &painter, const QRect &rect)
{
    if (!m_focusIndicatorEnabled || !hasFocus() || m_focusIndicatorOpacity <= 0.001) {
        return;
    }
    
    int focusInset = 4;
    QRectF focusRect = rect.adjusted(focusInset, focusInset, -focusInset, -focusInset);
    
    QPen focusPen(m_primaryColor, 2.0);
    focusPen.setStyle(Qt::DashLine);
    focusPen.setDashPattern({4, 4});
    
    QColor focusColor = m_primaryColor;
    focusColor.setAlphaF(m_focusIndicatorOpacity * 0.5);
    focusPen.setColor(focusColor);
    
    painter.setPen(focusPen);
    painter.setBrush(Qt::NoBrush);
    painter.drawRoundedRect(focusRect, m_cornerRadius - 2, m_cornerRadius - 2);
}

void MButtonBase::paintHoverElevation(QPainter &painter, const QRect &rect)
{
    if (!m_hoverElevationEnabled || m_elevationOffset <= 0.01) {
        return;
    }
    
    QColor shadowColor(0, 0, 0, static_cast<int>(30 * m_elevationOffset));
    QRect shadowRect = rect.adjusted(0, 0, 0, static_cast<int>(2 * m_elevationOffset));
    
    painter.save();
    painter.setPen(Qt::NoPen);
    painter.setBrush(shadowColor);
    painter.drawRoundedRect(shadowRect, m_cornerRadius, m_cornerRadius);
    painter.restore();
}

void MButtonBase::paintLabel(QPainter &painter)
{
    QStyleOptionButton option;
    initStyleOption(&option);
    
    option.features = QStyleOptionButton::None;
    option.palette.setColor(QPalette::ButtonText, getCurrentTextColor());
    option.palette.setColor(QPalette::WindowText, getCurrentTextColor());
    
    style()->drawControl(QStyle::CE_PushButtonLabel, &option, &painter, this);
}

void MButtonBase::setCheckable(bool checkable)
{
    m_checkable = checkable;
    QPushButton::setCheckable(checkable);
}

bool MButtonBase::isCheckable() const
{
    return m_checkable;
}

QColor MButtonBase::getContrastColor(const QColor &color) const
{
    qreal luminance = (0.299 * color.redF() + 
                       0.587 * color.greenF() + 
                       0.114 * color.blueF());
    
    if (luminance > 0.5) {
        return QColor(0x1e, 0x1e, 0x1e);  // 深灰色
    } else {
        return QColor(0xff, 0xff, 0xff);  // 白色
    }
}