// MButton.cpp
#include "MButton.h"
#include <QMouseEvent>
#include <QApplication>
#include <QStyle>
#include <QDebug>
#include <QStyleOption>
#include <QPainter>

MButton::MButton(QWidget *parent)
    : QPushButton(parent)
    , m_iconFont("Material Icons")
{
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    setFixedHeight(40);
    setFocusPolicy(Qt::StrongFocus);
    setAttribute(Qt::WA_MacShowFocusRect, false);
    setAttribute(Qt::WA_Hover, true);
    
    m_iconFont.setPixelSize(18);
    m_iconFont.setWeight(QFont::Normal);
    
    initShadowEffect();
}

MButton::~MButton()
{
    cleanupAllRipples();
}

void MButton::initShadowEffect()
{
    m_shadowEffect = new QGraphicsDropShadowEffect(this);
    m_shadowEffect->setBlurRadius(0);
    m_shadowEffect->setOffset(0, 0);
    m_shadowEffect->setColor(getShadowColor());
    m_shadowEffect->setEnabled(false);
    setGraphicsEffect(m_shadowEffect);
}

void MButton::setButtonType(Type type)
{
    m_type = type;
    updateShadowEffect();
    updateGeometry();
    update();
}

void MButton::setIcon(const QString &iconText)
{
    m_iconText = iconText;
    update();
}

void MButton::setIconFont(const QFont &font)
{
    m_iconFont = font;
    update();
}

void MButton::setHorizontalPadding(qreal padding)
{
    m_horizontalPadding = padding;
    updateGeometry();
    update();
}

void MButton::setVerticalPadding(qreal padding)
{
    m_verticalPadding = padding;
    updateGeometry();
    update();
}

void MButton::setSpacing(qreal spacing)
{
    m_spacing = spacing;
    update();
}

// ==================== 从 MTheme 获取主题参数 ====================

qreal MButton::getCornerRadius() const
{
    MTheme *theme = MTheme::instance();
    // 使用 shape 中的 cornerLarge 作为按钮圆角
    return static_cast<qreal>(theme->shape().cornerLarge);
}

qreal MButton::getRippleOpacity() const
{
    return 0.15; // 涟漪透明度，可以根据需要调整
}

int MButton::getRippleDuration() const
{
    return 400;
}

int MButton::getRippleFadeDuration() const
{
    return 300;
}

int MButton::getRippleFadeDelay() const
{
    return 200;
}

QColor MButton::getShadowColor() const
{
    return MTheme::instance()->color().shadow;
}

// ==================== 尺寸计算 ====================

QSize MButton::sizeHint() const
{
    qreal contentWidth = calculateContentWidth();
    qreal contentHeight = calculateContentHeight();
    
    int w = qMax(static_cast<int>(contentWidth + m_horizontalPadding * 2), 48);
    int h = qMax(static_cast<int>(contentHeight + m_verticalPadding * 2), 40);
    
    return QSize(w, h);
}

QSize MButton::minimumSizeHint() const
{
    return QSize(48, 40);
}

qreal MButton::calculateContentWidth() const
{
    QFontMetrics fmIcon(m_iconFont);
    QFontMetrics fmText(font());
    
    qreal textWidth = text().isEmpty() ? 0 : fmText.horizontalAdvance(text());
    qreal iconWidth = m_iconText.isEmpty() ? 0 : fmIcon.horizontalAdvance(m_iconText);
    
    if (!m_iconText.isEmpty() && !text().isEmpty()) {
        return iconWidth + m_spacing + textWidth;
    }
    return iconWidth + textWidth;
}

qreal MButton::calculateContentHeight() const
{
    QFontMetrics fmIcon(m_iconFont);
    QFontMetrics fmText(font());
    
    qreal iconHeight = m_iconText.isEmpty() ? 0 : fmIcon.height();
    qreal textHeight = text().isEmpty() ? 0 : fmText.height();
    
    return qMax(iconHeight, textHeight);
}

// ==================== 阴影更新 ====================

void MButton::updateShadowEffect()
{
    qreal elevation = getElevation();
    m_currentElevation = elevation;
    
    if (elevation <= 0 || !m_state.enabled) {
        m_shadowEffect->setEnabled(false);
        return;
    }
    
    m_shadowEffect->setEnabled(true);
    
    qreal blur = elevation * 0.06 * 32;
    qreal offset = elevation * 1.5;
    qreal opacity = 0.15 + (elevation * 0.04);
    
    m_shadowEffect->setBlurRadius(qMax(blur, 1.0));
    m_shadowEffect->setOffset(0, offset);
    
    QColor shadowColor = getShadowColor();
    shadowColor.setAlphaF(qMin(opacity, 1.0));
    m_shadowEffect->setColor(shadowColor);
}

// ==================== 涟漪管理 ====================

void MButton::createRipple(const QPointF &pos)
{
    // 清理已完成但未删除的涟漪
    for (int i = m_ripples.size() - 1; i >= 0; --i) {
        RippleData *ripple = m_ripples[i];
        if (!ripple->isActive && !ripple->isFadingOut) {
            cleanupRipple(ripple);
            m_ripples.removeAt(i);
        }
    }
    
    RippleData *ripple = new RippleData();
    ripple->center = pos;
    ripple->radius = 0;
    ripple->opacity = getRippleOpacity();
    ripple->isActive = true;
    ripple->isFadingOut = false;
    
    // 计算最大半径 - 从点击点到最远角的距离
    QRectF rect = QRectF(0, 0, width(), height());
    QPointF corners[] = {
        rect.topLeft(),
        rect.topRight(),
        rect.bottomLeft(),
        rect.bottomRight()
    };
    
    qreal maxDist = 0;
    for (const QPointF &corner : corners) {
        qreal dx = corner.x() - pos.x();
        qreal dy = corner.y() - pos.y();
        qreal dist = sqrt(dx*dx + dy*dy);
        maxDist = qMax(maxDist, dist);
    }
    maxDist *= 1.2; // 稍微扩大
    
    // 半径动画
    QVariantAnimation *radiusAnim = new QVariantAnimation(this);
    radiusAnim->setDuration(getRippleDuration());
    radiusAnim->setEasingCurve(QEasingCurve::OutQuad);
    radiusAnim->setStartValue(0.0);
    radiusAnim->setEndValue(maxDist);
    QObject::connect(radiusAnim, &QVariantAnimation::valueChanged, [this, ripple](const QVariant &value) {
        ripple->radius = value.toReal();
        update();
    });
    
    // 透明度动画（保持透明度不变）
    QVariantAnimation *opacityAnim = new QVariantAnimation(this);
    opacityAnim->setDuration(getRippleDuration());
    opacityAnim->setEasingCurve(QEasingCurve::OutQuad);
    opacityAnim->setStartValue(getRippleOpacity());
    opacityAnim->setEndValue(getRippleOpacity());
    QObject::connect(opacityAnim, &QVariantAnimation::valueChanged, [this, ripple](const QVariant &value) {
        ripple->opacity = value.toReal();
        update();
    });
    
    // 组合动画组
    QParallelAnimationGroup *group = new QParallelAnimationGroup(this);
    group->addAnimation(radiusAnim);
    group->addAnimation(opacityAnim);
    
    QObject::connect(group, &QParallelAnimationGroup::finished, [ripple]() {
        if (!ripple->isFadingOut) {
            ripple->isActive = false;
        }
    });
    
    // 淡出定时器
    QTimer *fadeTimer = new QTimer(this);
    fadeTimer->setSingleShot(true);
    QObject::connect(fadeTimer, &QTimer::timeout, [this, ripple]() {
        startRippleFadeOut(ripple);
    });
    
    ripple->radiusAnim = radiusAnim;
    ripple->opacityAnim = opacityAnim;
    ripple->group = group;
    ripple->fadeTimer = fadeTimer;
    
    m_ripples.append(ripple);
    m_currentRipple = ripple;
    
    // 启动动画
    group->start();
    update();
}

void MButton::startRippleFadeOut(RippleData *ripple)
{
    if (!ripple || ripple->isFadingOut) return;
    
    ripple->isFadingOut = true;
    ripple->isActive = false;
    
    // 停止当前动画组
    if (ripple->group) {
        ripple->group->stop();
    }
    
    // 创建新的透明度动画用于淡出
    QVariantAnimation *fadeAnim = new QVariantAnimation(this);
    fadeAnim->setDuration(getRippleFadeDuration());
    fadeAnim->setEasingCurve(QEasingCurve::OutQuad);
    fadeAnim->setStartValue(ripple->opacity);
    fadeAnim->setEndValue(0.0);
    QObject::connect(fadeAnim, &QVariantAnimation::valueChanged, [this, ripple](const QVariant &value) {
        ripple->opacity = value.toReal();
        update();
    });
    QObject::connect(fadeAnim, &QVariantAnimation::finished, [this, ripple]() {
        // 清理涟漪
        cleanupRipple(ripple);
        int index = m_ripples.indexOf(ripple);
        if (index >= 0) {
            m_ripples.removeAt(index);
        }
        update();
    });
    fadeAnim->start(QAbstractAnimation::DeleteWhenStopped);
}

void MButton::cleanupRipple(RippleData *ripple)
{
    if (!ripple) return;
    
    if (ripple->radiusAnim) {
        ripple->radiusAnim->disconnect();
        ripple->radiusAnim->deleteLater();
    }
    if (ripple->opacityAnim) {
        ripple->opacityAnim->disconnect();
        ripple->opacityAnim->deleteLater();
    }
    if (ripple->group) {
        ripple->group->disconnect();
        ripple->group->deleteLater();
    }
    if (ripple->fadeTimer) {
        ripple->fadeTimer->disconnect();
        ripple->fadeTimer->deleteLater();
    }
    delete ripple;
}

void MButton::cleanupAllRipples()
{
    for (RippleData *ripple : m_ripples) {
        cleanupRipple(ripple);
    }
    m_ripples.clear();
    m_currentRipple = nullptr;
}

// ==================== 主题更新 ====================

void MButton::updateTheme()
{
    // 主题从 MTheme 单例获取，无需额外操作
}

// ==================== 颜色计算（从 MTheme 获取） ====================

QColor MButton::getContainerColor() const
{
    MTheme::ColorScheme colors = MTheme::instance()->color();
    
    if (!m_state.enabled) {
        QColor c = colors.onSurfaceColor;
        c.setAlphaF(0.12);
        return c;
    }
    
    switch (m_type) {
        case Elevated: return colors.surfaceContainerLow;
        case Filled: return colors.primary;
        case FilledTonal: return colors.secondaryContainer;
        case Outlined: return Qt::transparent;
        case Text: return Qt::transparent;
        default: return colors.primary;
    }
}

QColor MButton::getContentColor() const
{
    MTheme::ColorScheme colors = MTheme::instance()->color();
    
    if (!m_state.enabled) {
        QColor c = colors.onSurfaceColor;
        c.setAlphaF(0.38);
        return c;
    }
    
    switch (m_type) {
        case Elevated: return colors.primary;
        case Filled: return colors.onPrimaryColor;
        case FilledTonal: return colors.onSecondaryContainerColor;
        case Outlined: return colors.primary;
        case Text: return colors.primary;
        default: return colors.onPrimaryColor;
    }
}

QColor MButton::getStateLayerColor() const
{
    MTheme::ColorScheme colors = MTheme::instance()->color();
    
    if (!m_state.enabled) {
        return Qt::transparent;
    }
    
    switch (m_type) {
        case Filled: return colors.onPrimaryColor;
        case FilledTonal: return colors.onSecondaryContainerColor;
        default: return colors.primary;
    }
}

QColor MButton::getOutlineColor() const
{
    MTheme::ColorScheme colors = MTheme::instance()->color();
    
    if (!m_state.enabled) {
        QColor c = colors.onSurfaceColor;
        c.setAlphaF(0.12);
        return c;
    }
    if (m_state.focused) return colors.primary;
    return colors.outline;
}

qreal MButton::getElevation() const
{
    if (!m_state.enabled) return 0.0;
    
    switch (m_type) {
        case Elevated:
            return static_cast<qreal>(MTheme::instance()->elevation().level1);
        case Filled:
        case FilledTonal:
            return static_cast<qreal>(MTheme::instance()->elevation().level0);
        default:
            return static_cast<qreal>(MTheme::instance()->elevation().level0);
    }
}

qreal MButton::getStateLayerOpacity() const
{
    if (!m_state.enabled) return 0.0;
    
    MTheme::StateOpacity state = MTheme::instance()->state();
    
    if (m_state.pressed) return state.pressedStateLayerOpacity;
    if (m_state.hovered) return state.hoverStateLayerOpacity;
    if (m_state.focused) return state.focusStateLayerOpacity;
    return 0.0;
}

// ==================== 绘制 ====================

void MButton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    
    QRectF rect = QRectF(0, 0, width(), height());
    
    // 更新阴影
    updateShadowEffect();
    
    // 1. 绘制背景
    drawBackground(&painter, rect);
    
    // 2. 绘制边框（outlined 类型）
    if (m_type == Outlined) {
        drawOutline(&painter, rect);
    }
    
    // 3. 绘制状态层（hover/focus/press）
    drawStateLayer(&painter, rect);
    
    // 4. 绘制涟漪（所有活跃的涟漪）
    drawRipples(&painter, rect);
    
    // 5. 绘制内容
    drawContent(&painter, rect);
}

void MButton::drawRipples(QPainter *painter, const QRectF &rect)
{
    if (m_ripples.isEmpty()) return;
    
    QColor color = getStateLayerColor();
    if (color.alpha() == 0) return;
    
    QPainterPath clipPath;
    clipPath.addRoundedRect(rect, getCornerRadius(), getCornerRadius());
    
    painter->save();
    painter->setClipPath(clipPath);
    
    for (RippleData *ripple : m_ripples) {
        if (!ripple->isActive && !ripple->isFadingOut) continue;
        if (ripple->radius <= 0 || ripple->opacity <= 0) continue;
        
        QColor rippleColor = color;
        rippleColor.setAlphaF(ripple->opacity);
        
        QPainterPath ripplePath;
        ripplePath.addEllipse(ripple->center, ripple->radius, ripple->radius);
        
        painter->setPen(Qt::NoPen);
        painter->setBrush(rippleColor);
        painter->drawPath(ripplePath);
    }
    
    painter->restore();
}

void MButton::drawBackground(QPainter *painter, const QRectF &rect)
{
    QColor color = getContainerColor();
    
    QPainterPath path;
    path.addRoundedRect(rect, getCornerRadius(), getCornerRadius());
    
    painter->save();
    painter->setPen(Qt::NoPen);
    
    if (color.alpha() > 0) {
        painter->setBrush(color);
        painter->drawPath(path);
    }
    
    painter->restore();
}

void MButton::drawOutline(QPainter *painter, const QRectF &rect)
{
    QColor color = getOutlineColor();
    if (color.alpha() == 0) return;
    
    qreal radius = getCornerRadius();
    QPainterPath path;
    path.addRoundedRect(rect.adjusted(0.5, 0.5, -0.5, -0.5), 
                         radius - 0.5, 
                         radius - 0.5);
    
    painter->save();
    painter->setPen(QPen(color, 1));
    painter->setBrush(Qt::NoBrush);
    painter->drawPath(path);
    painter->restore();
}

void MButton::drawStateLayer(QPainter *painter, const QRectF &rect)
{
    qreal opacity = getStateLayerOpacity();
    if (opacity <= 0) return;
    
    QColor color = getStateLayerColor();
    if (color.alpha() == 0) return;
    
    color.setAlphaF(opacity);
    
    QPainterPath path;
    path.addRoundedRect(rect, getCornerRadius(), getCornerRadius());
    
    painter->save();
    painter->setPen(Qt::NoPen);
    painter->setBrush(color);
    painter->drawPath(path);
    painter->restore();
}

void MButton::drawContent(QPainter *painter, const QRectF &rect)
{
    if (text().isEmpty() && m_iconText.isEmpty()) return;
    
    QColor color = getContentColor();
    if (color.alpha() == 0) return;
    
    QRectF contentRect = getContentRect(rect);
    
    QFontMetrics fmIcon(m_iconFont);
    QFontMetrics fmText(font());
    
    qreal textWidth = text().isEmpty() ? 0 : fmText.horizontalAdvance(text());
    qreal iconWidth = m_iconText.isEmpty() ? 0 : fmIcon.horizontalAdvance(m_iconText);
    qreal totalWidth = iconWidth + (m_iconText.isEmpty() ? 0 : m_spacing) + textWidth;
    
    qreal x = contentRect.center().x() - totalWidth / 2;
    qreal y = contentRect.center().y();
    
    painter->save();
    painter->setPen(color);
    
    // 绘制图标
    if (!m_iconText.isEmpty()) {
        painter->setFont(m_iconFont);
        QRectF iconRect(x, y - fmIcon.height() / 2, iconWidth, fmIcon.height());
        painter->drawText(iconRect, Qt::AlignCenter, m_iconText);
        x += iconWidth + m_spacing;
    }
    
    // 绘制文本
    if (!text().isEmpty()) {
        painter->setFont(this->font());
        QRectF textRect(x, y - fmText.height() / 2, textWidth, fmText.height());
        painter->drawText(textRect, Qt::AlignCenter, text());
    }
    
    painter->restore();
}

void MButton::drawShadow(QPainter *painter, const QRectF &rect)
{
    Q_UNUSED(painter)
    Q_UNUSED(rect)
}

QRectF MButton::getContentRect(const QRectF &rect) const
{
    qreal left = rect.left() + m_horizontalPadding;
    qreal right = rect.right() - m_horizontalPadding;
    qreal top = rect.top() + m_verticalPadding;
    qreal bottom = rect.bottom() - m_verticalPadding;
    
    return QRectF(left, top, right - left, bottom - top);
}

// ==================== 事件处理 ====================

void MButton::enterEvent(QEnterEvent *event)
{
    QPushButton::enterEvent(event);
    m_state.hovered = true;
    update();
}

void MButton::leaveEvent(QEvent *event)
{
    QPushButton::leaveEvent(event);
    m_state.hovered = false;
    update();
}

void MButton::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && m_state.enabled) {
        m_state.pressed = true;
        createRipple(event->position());
        update();
    }
    QPushButton::mousePressEvent(event);
}

void MButton::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && m_state.enabled) {
        m_state.pressed = false;
        
        if (m_currentRipple && m_currentRipple->fadeTimer) {
            m_currentRipple->fadeTimer->start(getRippleFadeDelay());
        }
        update();
    }
    QPushButton::mouseReleaseEvent(event);
}

void MButton::focusInEvent(QFocusEvent *event)
{
    m_state.focused = true;
    update();
    QPushButton::focusInEvent(event);
}

void MButton::focusOutEvent(QFocusEvent *event)
{
    m_state.focused = false;
    update();
    QPushButton::focusOutEvent(event);
}

void MButton::resizeEvent(QResizeEvent *event)
{
    QPushButton::resizeEvent(event);
}

void MButton::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::EnabledChange) {
        m_state.enabled = isEnabled();
        updateShadowEffect();
        update();
    }
    QPushButton::changeEvent(event);
}

bool MButton::event(QEvent *event)
{
    if (event->type() == QEvent::HoverEnter) {
        m_state.hovered = true;
        update();
    } else if (event->type() == QEvent::HoverLeave) {
        m_state.hovered = false;
        update();
    }
    return QPushButton::event(event);
}