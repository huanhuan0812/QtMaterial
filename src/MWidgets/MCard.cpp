#include "MCard.h"

#include <QApplication>
#include <QPainter>
#include <QPainterPath>
#include <QStyle>
#include <QVBoxLayout>
#include <QColor>

MCard::MCard(QWidget* parent)
    : QWidget(parent)
    , m_shadowEffect(new QGraphicsDropShadowEffect(this))
{
    // Setup widget flags
    setAttribute(Qt::WA_Hover, true);
    setFocusPolicy(Qt::NoFocus);

    // Setup shadow effect
    m_shadowEffect->setColor(Qt::transparent);
    m_shadowEffect->setBlurRadius(0);
    m_shadowEffect->setOffset(0, 0);
    setGraphicsEffect(m_shadowEffect);

    // Setup layout for content
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(m_padding, m_padding, m_padding, m_padding);
    layout->setSpacing(0);
    setLayout(layout);

    // Set default colors based on theme
    m_containerColor = getDefaultContainerColor();
    m_outlineColor = getDefaultOutlineColor();
    m_elevationLevel = getDefaultElevationLevel();

    // Setup size policy
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // Initial shadow update
    updateShadow();

    // Ensure we repaint when theme changes
    connect(MTheme::instance(), &MTheme::colorSchemeChanged, this, [this]() {
        if (m_type != Type::Outlined) {
            m_containerColor = getDefaultContainerColor();
        }
        update();
    });
}

void MCard::setType(Type type)
{
    if (m_type == type)
        return;

    m_type = type;
    m_containerColor = getDefaultContainerColor();
    m_outlineColor = getDefaultOutlineColor();
    m_elevationLevel = getDefaultElevationLevel();
    updateShadow();
    update();
    emit typeChanged();
}

void MCard::setRadius(int radius)
{
    if (m_radius == radius)
        return;

    m_radius = radius;
    m_cachedPath = QPainterPath();
    updateShadow();
    update();
    emit radiusChanged();
}

void MCard::setPadding(int padding)
{
    if (m_padding == padding)
        return;

    m_padding = padding;
    updateContentMargins();
    emit paddingChanged();
}

void MCard::setContainerColor(const QColor& color)
{
    if (m_containerColor == color)
        return;

    m_containerColor = color;
    update();
    emit containerColorChanged();
}

void MCard::setOutlineColor(const QColor& color)
{
    if (m_outlineColor == color)
        return;

    m_outlineColor = color;
    update();
    emit outlineColorChanged();
}

void MCard::setElevationLevel(int level)
{
    if (m_elevationLevel == level)
        return;

    m_elevationLevel = level;
    updateShadow();
    emit elevationLevelChanged();
}

void MCard::setHovered(bool hovered)
{
    if (m_hovered == hovered)
        return;

    m_hovered = hovered;
    update();
    emit hoveredChanged();
}

void MCard::setPressed(bool pressed)
{
    if (m_pressed == pressed)
        return;

    m_pressed = pressed;
    update();
    emit pressedChanged();
}

void MCard::setContentWidget(QWidget* widget)
{
    if (m_contentWidget) {
        layout()->removeWidget(m_contentWidget);
        m_contentWidget->deleteLater();
    }

    m_contentWidget = widget;

    if (m_contentWidget) {
        layout()->addWidget(m_contentWidget);
        m_contentWidget->setParent(this);
    }
}

void MCard::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event)

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Create rounded rect path
    QRectF rect = QRectF(0, 0, width(), height());
    
    if (m_cachedPath.isEmpty()) {
        m_cachedPath = QPainterPath();
        m_cachedPath.addRoundedRect(rect, m_radius, m_radius);
    }

    // Draw background
    painter.fillPath(m_cachedPath, m_containerColor);

    // Draw outline for Outlined type
    if (m_type == Type::Outlined) {
        QPen pen(m_outlineColor, 1);
        pen.setJoinStyle(Qt::RoundJoin);
        painter.setPen(pen);
        painter.drawPath(m_cachedPath);
    }

    // Draw state layer (hover/pressed overlay)
    qreal stateOpacity = 0.0;
    if (m_pressed) {
        stateOpacity = MTheme::instance()->state().pressedStateLayerOpacity;
    } else if (m_hovered) {
        stateOpacity = MTheme::instance()->state().hoverStateLayerOpacity;
    }

    if (stateOpacity > 0.0) {
        QColor stateColor = Qt::black;
        stateColor.setAlphaF(stateOpacity);
        painter.fillPath(m_cachedPath, stateColor);
    }
}

void MCard::enterEvent(QEnterEvent* event)
{
    Q_UNUSED(event)
    setHovered(true);
    QWidget::enterEvent(event);
}

void MCard::leaveEvent(QEvent* event)
{
    Q_UNUSED(event)
    setHovered(false);
    QWidget::leaveEvent(event);
}

void MCard::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        setPressed(true);
    }
    QWidget::mousePressEvent(event);
}

void MCard::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton && m_pressed) {
        setPressed(false);
        
        // Check if click was inside the widget
        if (rect().contains(event->pos())) {
            emit clicked();
        }
    }
    QWidget::mouseReleaseEvent(event);
}

void MCard::resizeEvent(QResizeEvent* event)
{
    Q_UNUSED(event)
    m_cachedPath = QPainterPath();
    updateShadow();
    QWidget::resizeEvent(event);
}

bool MCard::event(QEvent* event)
{
    if (event->type() == QEvent::HoverLeave) {
        setHovered(false);
    } else if (event->type() == QEvent::HoverEnter) {
        setHovered(true);
    }
    return QWidget::event(event);
}

void MCard::updateStyle()
{
    update();
}

void MCard::updateShadow()
{
    int level = (m_elevationLevel >= 0) ? m_elevationLevel : getDefaultElevationLevel();
    
    if (level <= 0) {
        m_shadowEffect->setEnabled(false);
        return;
    }

    m_shadowEffect->setEnabled(true);
    
    // Calculate shadow parameters based on elevation level
    qreal blurRadius = level * 0.2;
    qreal verticalOffset = level * 1.2;
    qreal opacity = 0.2 + (level * 0.02);
    
    // Clamp values
    blurRadius = qMin(qreal(20.0), qMax(qreal(0.0), blurRadius));
    verticalOffset = qMin(qreal(20.0), qMax(qreal(0.0), verticalOffset));
    opacity = qMin(qreal(0.8), qMax(qreal(0.0), opacity));
    
    m_shadowEffect->setBlurRadius(blurRadius);
    m_shadowEffect->setOffset(0, verticalOffset);
    m_shadowEffect->setColor(QColor(MTheme::instance()->color().shadow));
    m_shadowEffect->setEnabled(true);
}

QColor MCard::getDefaultContainerColor() const
{
    const auto& color = MTheme::instance()->color();
    
    if (!isEnabled()) {
        return color.surfaceVariant;
    }
    
    switch (m_type) {
        case Type::Elevated:
            return color.surfaceContainerLow;
        case Type::Filled:
            return color.surfaceContainerHighest;
        case Type::Outlined:
            return color.surface;
        default:
            return color.surfaceContainerLow;
    }
}

QColor MCard::getDefaultOutlineColor() const
{
    return MTheme::instance()->color().outline;
}

int MCard::getDefaultElevationLevel() const
{
    if (m_type == MCard::Type::Elevated) {
        return 3;
    }
    return 0;
}

void MCard::updateContentMargins()
{
    if (layout()) {
        layout()->setContentsMargins(m_padding, m_padding, m_padding, m_padding);
    }
}