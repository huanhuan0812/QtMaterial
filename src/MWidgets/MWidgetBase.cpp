#include "MWidgetBase.h"
#include <QPainter>
#include <QStyleOption>
#include <QApplication>
#include <QScreen>
#include <QWindow>

MWidgetBase::MWidgetBase(QWidget *parent)
    : QWidget(parent)
{
    initWidget();
}

MWidgetBase::~MWidgetBase()
{
    if (m_elevationAnimation) {
        m_elevationAnimation->stop();
        delete m_elevationAnimation;
    }
    if (m_colorAnimation) {
        m_colorAnimation->stop();
        delete m_colorAnimation;
    }
    if (m_animationGroup) {
        m_animationGroup->stop();
        delete m_animationGroup;
    }
}

void MWidgetBase::initWidget()
{
    // 设置窗口属性
    setAttribute(Qt::WA_StyledBackground, true);
    setAttribute(Qt::WA_TranslucentBackground, true);
    setFocusPolicy(Qt::StrongFocus);
    
    // 设置默认颜色
    m_colorScheme = MColorScheme();
    m_backgroundColor = m_colorScheme.surface;
    
    // 设置默认可视化属性
    m_cornerRadius = 12.0;
    m_elevation = 0.0;
    
    // 初始化阴影
    setupShadow();
    
    // 初始化动画
    setupAnimations();
    
    // 应用主题
    applyTheme();
}

void MWidgetBase::setupShadow()
{
    m_shadowEffect = new QGraphicsDropShadowEffect(this);
    m_shadowEffect->setBlurRadius(20);
    m_shadowEffect->setOffset(0, 4);
    m_shadowEffect->setColor(QColor(0, 0, 0, 40));
    setGraphicsEffect(m_shadowEffect);
    updateShadowGeometry();
}

void MWidgetBase::setupAnimations()
{
    // 高度动画
    m_elevationAnimation = new QPropertyAnimation(this, "elevation");
    m_elevationAnimation->setEasingCurve(QEasingCurve::OutCubic);
    
    // 颜色动画
    m_colorAnimation = new QPropertyAnimation(this, "backgroundColor");
    m_colorAnimation->setEasingCurve(QEasingCurve::OutCubic);
    
    // 动画组
    m_animationGroup = new QParallelAnimationGroup(this);
    m_animationGroup->addAnimation(m_elevationAnimation);
    m_animationGroup->addAnimation(m_colorAnimation);
}

void MWidgetBase::setWindowType(MWindowType type)
{
    m_windowType = type;
    updateShadowGeometry();
    update();
}

void MWidgetBase::setThemeMode(MThemeMode mode)
{
    m_themeMode = mode;
    applyTheme();
    onThemeChanged();
}

void MWidgetBase::setColorScheme(const MColorScheme &scheme)
{
    m_colorScheme = scheme;
    applyTheme();
}

QColor MWidgetBase::getPrimaryColor() const
{
    return m_colorScheme.primary;
}

QColor MWidgetBase::getSurfaceColor() const
{
    return m_colorScheme.surface;
}

QColor MWidgetBase::getOnSurfaceColor() const
{
    return m_colorScheme.onSurface;
}

QColor MWidgetBase::getOnSurfaceVariant() const
{
    return m_colorScheme.onSurfaceVariant;
}

QColor MWidgetBase::getErrorColor() const
{
    return m_colorScheme.error;
}

QColor MWidgetBase::getOutlineColor() const
{
    return m_colorScheme.outline;
}

void MWidgetBase::setCornerRadius(qreal radius)
{
    if (m_cornerRadius != radius) {
        m_cornerRadius = radius;
        update();
    }
}

void MWidgetBase::setElevation(qreal elevation)
{
    if (m_elevation != elevation) {
        m_elevation = elevation;
        updateShadowGeometry();
        update();
    }
}

void MWidgetBase::setShadowEnabled(bool enabled)
{
    if (m_shadowEnabled != enabled) {
        m_shadowEnabled = enabled;
        if (m_shadowEffect) {
            m_shadowEffect->setEnabled(enabled);
        }
        update();
    }
}

void MWidgetBase::setHovered(bool hovered)
{
    if (m_hovered != hovered) {
        m_hovered = hovered;
        if (hovered) {
            onHoverEnter();
            animateElevation(m_elevation + 2.0, 150);
        } else {
            onHoverLeave();
            animateElevation(qMax(0.0, m_elevation - 2.0), 150);
        }
        update();
    }
}

void MWidgetBase::setPressed(bool pressed)
{
    if (m_pressed != pressed) {
        m_pressed = pressed;
        if (pressed) {
            onPress();
        } else {
            onRelease();
        }
        update();
    }
}

void MWidgetBase::setFocused(bool focused)
{
    if (m_focused != focused) {
        m_focused = focused;
        if (focused) {
            onFocusIn();
        } else {
            onFocusOut();
        }
        update();
    }
}

void MWidgetBase::animateElevation(qreal targetElevation, int duration)
{
    if (m_elevationAnimation) {
        m_elevationAnimation->setEndValue(targetElevation);
        m_elevationAnimation->setDuration(duration);
        m_elevationAnimation->start();
    }
}

void MWidgetBase::animateColor(const QColor &targetColor, int duration)
{
    if (m_colorAnimation) {
        QColor currentColor = backgroundColor();
        m_colorAnimation->setStartValue(currentColor);
        m_colorAnimation->setEndValue(targetColor);
        m_colorAnimation->setDuration(duration);
        m_colorAnimation->start();
    }
}

void MWidgetBase::applyTheme()
{
    // 根据主题模式设置颜色
    if (m_themeMode == MThemeMode::Dark) {
        // 暗色主题颜色
        m_colorScheme.surface = QColor(0x1C, 0x1B, 0x1F);
        m_colorScheme.surfaceDim = QColor(0x14, 0x13, 0x17);
        m_colorScheme.surfaceContainer = QColor(0x29, 0x27, 0x2C);
        m_colorScheme.onSurface = QColor(0xE3, 0xE1, 0xE6);
        m_colorScheme.onSurfaceVariant = QColor(0xC4, 0xC4, 0xD0);
        m_colorScheme.outline = QColor(0x8E, 0x8E, 0x99);
        m_colorScheme.outlineVariant = QColor(0x42, 0x42, 0x4E);
    } else {
        // 亮色主题颜色（默认）
        m_colorScheme.surface = QColor(0xFB, 0xFC, 0xFF);
        m_colorScheme.surfaceDim = QColor(0xDA, 0xE0, 0xE7);
        m_colorScheme.surfaceContainer = QColor(0xF0, 0xF2, 0xF5);
        m_colorScheme.onSurface = QColor(0x1C, 0x1B, 0x1F);
        m_colorScheme.onSurfaceVariant = QColor(0x44, 0x44, 0x4F);
        m_colorScheme.outline = QColor(0x75, 0x75, 0x7F);
        m_colorScheme.outlineVariant = QColor(0xC4, 0xC4, 0xD0);
    }
    
    updateColors();
}

void MWidgetBase::updateColors()
{
    // 更新背景色
    QColor bgColor = getCurrentBackgroundColor();
    m_backgroundColor = bgColor;
    
    QPalette pal = palette();
    pal.setColor(QPalette::Window, bgColor);
    pal.setColor(QPalette::Base, bgColor);
    setPalette(pal);
    
    // 更新阴影颜色
    if (m_shadowEffect) {
        QColor shadowColor = m_colorScheme.shadow;
        shadowColor.setAlpha(m_elevation > 0 ? 40 + static_cast<int>(m_elevation * 5) : 0);
        m_shadowEffect->setColor(shadowColor);
    }
    
    update();
}

void MWidgetBase::showAsDialog()
{
    setWindowType(MWindowType::Dialog);
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    show();
}

void MWidgetBase::showAsBottomSheet()
{
    setWindowType(MWindowType::BottomSheet);
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    show();
}

void MWidgetBase::showFullScreen()
{
    setWindowType(MWindowType::FullScreen);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    QWidget::showFullScreen();
}

// === 绘制方法 ===

void MWidgetBase::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    
    // 获取绘制区域
    QRect contentRect = getContentRect();
    
    // 绘制背景
    drawBackground(&painter, contentRect);
    
    // 绘制阴影
    if (m_shadowEnabled) {
        drawShadow(&painter, contentRect);
    }
    
    // 绘制前景（子类实现）
    drawForeground(&painter, contentRect);
}

void MWidgetBase::drawBackground(QPainter *painter, const QRect &rect)
{
    QColor bgColor = getCurrentBackgroundColor();
    
    // 创建圆角矩形路径
    QPainterPath path;
    path.addRoundedRect(rect, m_cornerRadius, m_cornerRadius);
    
    // 绘制背景
    painter->fillPath(path, bgColor);
    
    // 如果是对话框或底部弹出，添加半透明遮罩
    if (m_windowType == MWindowType::Dialog || 
        m_windowType == MWindowType::BottomSheet) {
        // 绘制边框
        QPen pen;
        pen.setColor(m_colorScheme.outlineVariant);
        pen.setWidth(1);
        painter->setPen(pen);
        painter->drawPath(path);
    }
}

void MWidgetBase::drawForeground(QPainter *painter, const QRect &rect)
{
    // 子类可以重写此方法绘制自定义内容
    Q_UNUSED(painter);
    Q_UNUSED(rect);
}

void MWidgetBase::drawShadow(QPainter *painter, const QRect &rect)
{
    // 阴影由 QGraphicsDropShadowEffect 处理
    Q_UNUSED(painter);
    Q_UNUSED(rect);
}

// === 事件处理 ===

void MWidgetBase::enterEvent(QEnterEvent *event)
{
    Q_UNUSED(event);
    setHovered(true);
    QWidget::enterEvent(event);
}

void MWidgetBase::leaveEvent(QEvent *event)
{
    Q_UNUSED(event);
    setHovered(false);
    QWidget::leaveEvent(event);
}

void MWidgetBase::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        setPressed(true);
    }
    QWidget::mousePressEvent(event);
}

void MWidgetBase::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        setPressed(false);
    }
    QWidget::mouseReleaseEvent(event);
}

void MWidgetBase::focusInEvent(QFocusEvent *event)
{
    setFocused(true);
    QWidget::focusInEvent(event);
}

void MWidgetBase::focusOutEvent(QFocusEvent *event)
{
    setFocused(false);
    QWidget::focusOutEvent(event);
}

void MWidgetBase::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    updateShadowGeometry();
}

void MWidgetBase::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    updateShadowGeometry();
}

void MWidgetBase::hideEvent(QHideEvent *event)
{
    QWidget::hideEvent(event);
}

// === 虚函数实现 ===

void MWidgetBase::onHoverEnter()
{
    // 子类可以重写
}

void MWidgetBase::onHoverLeave()
{
    // 子类可以重写
}

void MWidgetBase::onPress()
{
    // 子类可以重写
}

void MWidgetBase::onRelease()
{
    // 子类可以重写
}

void MWidgetBase::onFocusIn()
{
    // 子类可以重写
}

void MWidgetBase::onFocusOut()
{
    // 子类可以重写
}

void MWidgetBase::onThemeChanged()
{
    // 子类可以重写
}

// === 辅助方法 ===

QRect MWidgetBase::getContentRect() const
{
    int margin = m_shadowEnabled ? SHADOW_MARGIN : 0;
    return rect().adjusted(margin, margin, -margin, -margin);
}

QRect MWidgetBase::getShadowRect() const
{
    return rect();
}

QGraphicsDropShadowEffect* MWidgetBase::createShadowEffect()
{
    auto effect = new QGraphicsDropShadowEffect(const_cast<MWidgetBase*>(this));
    effect->setBlurRadius(20);
    effect->setOffset(0, 4);
    return effect;
}

QColor MWidgetBase::getCurrentBackgroundColor() const
{
    if (m_windowType == MWindowType::Dialog || 
        m_windowType == MWindowType::BottomSheet) {
        return m_colorScheme.surfaceContainer;
    }
    return m_colorScheme.surface;
}

void MWidgetBase::updateShadowGeometry()
{
    if (!m_shadowEffect) return;
    
    // 根据窗口类型和高度调整阴影
    int blurRadius = 10 + static_cast<int>(m_elevation * 2);
    int offsetY = 2 + static_cast<int>(m_elevation * 0.5);
    
    m_shadowEffect->setBlurRadius(blurRadius);
    m_shadowEffect->setOffset(0, offsetY);
    
    // 调整阴影透明度
    int alpha = qMin(60, 20 + static_cast<int>(m_elevation * 3));
    QColor shadowColor = m_colorScheme.shadow;
    shadowColor.setAlpha(alpha);
    m_shadowEffect->setColor(shadowColor);
}

// === Q_PROPERTY 的实现 ===

QColor MWidgetBase::backgroundColor() const
{
    return m_backgroundColor;
}

void MWidgetBase::setBackgroundColor(const QColor &color)
{
    if (m_backgroundColor != color) {
        m_backgroundColor = color;
        QPalette pal = palette();
        pal.setColor(QPalette::Window, color);
        pal.setColor(QPalette::Base, color);
        setPalette(pal);
        update();
    }
}