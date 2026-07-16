#include "MBlurCard.h"

#include <QApplication>
#include <QScreen>
#include <QWindow>
#include <QPainter>
#include <QPainterPath>
#include <QStyleOption>
#include <QGraphicsBlurEffect>
#include <QGraphicsOpacityEffect>
#include <QDebug>
#include <QLayout>
#include <QVBoxLayout>
#include <QBitmap>
#include <QPainter>

MBlurCard::MBlurCard(QWidget* parent)
    : QWidget(parent)
    , m_blurEffect(new QGraphicsBlurEffect(this))
    , m_blurAnimation(new QPropertyAnimation(this))
    , m_updateTimer(new QTimer(this))
{
    // Setup widget flags
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_OpaquePaintEvent, false);
    setAttribute(Qt::WA_NoSystemBackground, true);
    setAttribute(Qt::WA_Hover, true);
    setFocusPolicy(Qt::NoFocus);
    setMouseTracking(true);
    
    // Setup size policy
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setMinimumSize(100, 80);

    // Setup layout for content
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(16, 16, 16, 16);
    layout->setSpacing(0);
    setLayout(layout);

    // Setup graphics scene
    setupGraphicsScene();

    // Setup blur animation
    m_blurAnimation->setTargetObject(m_blurEffect);
    m_blurAnimation->setPropertyName("blurRadius");
    m_blurAnimation->setDuration(300);
    m_blurAnimation->setEasingCurve(QEasingCurve::InOutQuad);

    // Setup real-time update timer
    m_updateTimer->setInterval(16); // ~60fps
    connect(m_updateTimer, &QTimer::timeout, this, &MBlurCard::updateBackground);

    // Setup mask widget
    m_maskWidget = new QWidget(this);
    m_maskWidget->setAttribute(Qt::WA_TranslucentBackground);
    m_maskWidget->setGeometry(rect());
    m_maskWidget->raise();

    // Setup overlay label (使用QLabel显示图片)
    m_overlayLabel = new QLabel(this);
    m_overlayLabel->setAttribute(Qt::WA_TranslucentBackground);
    m_overlayLabel->setGeometry(rect());
    m_overlayLabel->setAlignment(Qt::AlignCenter);
    m_overlayLabel->raise();
    
    // Get theme colors
    m_surfaceColor = MTheme::instance()->color().surface;
    m_surfaceColor.setAlphaF(m_overlayOpacity);

    // Connect theme changes
    connect(MTheme::instance(), &MTheme::colorSchemeChanged, this, [this]() {
        m_surfaceColor = MTheme::instance()->color().surface;
        m_surfaceColor.setAlphaF(m_overlayOpacity);
        updateOverlay();
    });

    // Start real-time updates
    startRealTimeUpdates();
}

MBlurCard::~MBlurCard()
{
    stopRealTimeUpdates();
    delete m_blurAnimation;
    delete m_updateTimer;
    delete m_scene;
}

void MBlurCard::setupGraphicsScene()
{
    // Create scene
    m_scene = new QGraphicsScene(this);
    m_scene->setBackgroundBrush(Qt::transparent);

    // Create background item with blur effect
    m_backgroundItem = new QGraphicsPixmapItem();
    m_backgroundItem->setGraphicsEffect(m_blurEffect);
    m_backgroundItem->setZValue(0);
    m_scene->addItem(m_backgroundItem);

    // Create view
    m_view = new QGraphicsView(m_scene, this);
    m_view->setAttribute(Qt::WA_TranslucentBackground);
    m_view->setStyleSheet("background: transparent; border: none;");
    m_view->setFrameShape(QFrame::NoFrame);
    m_view->setGeometry(rect());
    m_view->setRenderHint(QPainter::Antialiasing);
    m_view->setRenderHint(QPainter::SmoothPixmapTransform);
    m_view->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setAlignment(Qt::AlignCenter);
    
    // Make view transparent
    m_view->viewport()->setAttribute(Qt::WA_TranslucentBackground);
    
    // Lower view behind other widgets
    m_view->lower();
}

void MBlurCard::setBlurSource(QWidget* source)
{
    if (m_blurSource == source)
        return;

    m_blurSource = source;
    updateBackground();
    emit blurSourceChanged();
}

void MBlurCard::setBlurAmount(qreal amount)
{
    amount = qBound(0.0, amount, static_cast<qreal>(m_blurMax));
    
    if (qFuzzyCompare(m_blurAmount, amount))
        return;

    m_blurAmount = amount;
    m_blurEffect->setBlurRadius(m_blurAmount);
    emit blurAmountChanged();
}

void MBlurCard::setDragable(bool dragable)
{
    if (m_dragable == dragable)
        return;

    m_dragable = dragable;
    emit dragableChanged();
}

void MBlurCard::setBlurMax(int max)
{
    if (m_blurMax == max)
        return;

    m_blurMax = qMax(1, max);
    setBlurAmount(m_blurAmount);
    emit blurMaxChanged();
}

void MBlurCard::setBorderRadius(int radius)
{
    if (m_borderRadius == radius)
        return;

    m_borderRadius = qMax(0, radius);
    updateMask();
    updateOverlay();
    update();
    emit borderRadiusChanged();
}

void MBlurCard::setBorderColor(const QColor& color)
{
    if (m_borderColor == color)
        return;

    m_borderColor = color;
    updateOverlay();
    emit borderColorChanged();
}

void MBlurCard::setBorderWidth(qreal width)
{
    if (qFuzzyCompare(m_borderWidth, width))
        return;

    m_borderWidth = qMax(0.0, width);
    updateLayoutMargins();
    updateOverlay();
    emit borderWidthChanged();
}

void MBlurCard::setOverlayOpacity(qreal opacity)
{
    opacity = qBound(0.0, opacity, 1.0);
    
    if (qFuzzyCompare(m_overlayOpacity, opacity))
        return;

    m_overlayOpacity = opacity;
    m_surfaceColor.setAlphaF(m_overlayOpacity);
    updateOverlay();
    emit overlayOpacityChanged();
}

void MBlurCard::setContentWidget(QWidget* widget)
{
    if (m_contentWidget) {
        layout()->removeWidget(m_contentWidget);
        m_contentWidget->deleteLater();
    }

    m_contentWidget = widget;

    if (m_contentWidget) {
        layout()->addWidget(m_contentWidget);
        m_contentWidget->setParent(this);
        m_contentWidget->setAttribute(Qt::WA_TranslucentBackground);
        m_contentWidget->raise();
    }
}

void MBlurCard::animateBlur(qreal targetAmount, int duration)
{
    targetAmount = qBound(0.0, targetAmount, static_cast<qreal>(m_blurMax));
    
    m_blurAnimation->setEndValue(targetAmount);
    m_blurAnimation->setDuration(duration);
    m_blurAnimation->start();
    
    m_blurAmount = targetAmount;
}

void MBlurCard::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event)
    // 透明背景，由子组件处理绘制
}

void MBlurCard::resizeEvent(QResizeEvent* event)
{
    Q_UNUSED(event)
    
    QRect rect = this->rect();
    
    // Update view
    m_view->setGeometry(rect);
    m_maskWidget->setGeometry(rect);
    m_overlayLabel->setGeometry(rect);
    
    // Update background size
    if (m_backgroundItem) {
        m_backgroundItem->setPos(0, 0);
    }
    
    // Update mask
    updateMask();
    updateOverlay();
    updateLayoutMargins();
}

void MBlurCard::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        m_isPressed = true;
        
        if (m_dragable) {
            m_isDragging = true;
            m_dragStartPos = event->globalPos();
            m_widgetStartPos = pos();
            emit dragStarted();
        }
        update();
    }
    QWidget::mousePressEvent(event);
}

void MBlurCard::mouseMoveEvent(QMouseEvent* event)
{
    if (m_isDragging && m_dragable) {
        QPoint delta = event->globalPos() - m_dragStartPos;
        move(m_widgetStartPos + delta);
    }
    QWidget::mouseMoveEvent(event);
}

void MBlurCard::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        m_isPressed = false;
        
        if (m_isDragging) {
            m_isDragging = false;
            emit dragEnded();
            
            QPoint delta = event->globalPos() - m_dragStartPos;
            if (delta.manhattanLength() < 5) {
                emit clicked();
            }
        } else {
            emit clicked();
        }
        update();
    }
    QWidget::mouseReleaseEvent(event);
}

void MBlurCard::showEvent(QShowEvent* event)
{
    QWidget::showEvent(event);
    startRealTimeUpdates();
    updateBackground();
}

void MBlurCard::hideEvent(QHideEvent* event)
{
    QWidget::hideEvent(event);
    stopRealTimeUpdates();
}

bool MBlurCard::event(QEvent* event)
{
    switch (event->type()) {
        case QEvent::HoverEnter:
            m_isHovered = true;
            update();
            break;
        case QEvent::HoverLeave:
            m_isHovered = false;
            update();
            break;
        default:
            break;
    }
    return QWidget::event(event);
}

void MBlurCard::updateBackground()
{
    if (!isVisible() || !m_blurSource || !m_scene) {
        return;
    }

    // Capture background
    QPixmap captured = captureBackground();
    
    if (!captured.isNull()) {
        // Update background item
        m_backgroundItem->setPixmap(captured);
        m_backgroundItem->update();
        m_scene->update();
    }
}

void MBlurCard::updateMask()
{
    if (!m_maskWidget)
        return;

    // Create rounded rectangle mask using QPainterPath
    QBitmap mask(width(), height());
    mask.fill(Qt::color0);
    
    QPainter painter(&mask);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(Qt::color1);
    painter.setPen(Qt::NoPen);
    
    QPainterPath path;
    path.addRoundedRect(rect(), m_borderRadius, m_borderRadius);
    painter.drawPath(path);
    painter.end();
    
    m_maskWidget->setMask(mask);
}

void MBlurCard::updateOverlay()
{
    if (!m_overlayLabel)
        return;

    // Create overlay pixmap with rounded corners and border
    QPixmap overlayPixmap(size());
    overlayPixmap.fill(Qt::transparent);
    
    QPainter painter(&overlayPixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    
    // Draw rounded rect path
    QPainterPath path;
    path.addRoundedRect(rect(), m_borderRadius, m_borderRadius);
    
    // Draw overlay color (surface with opacity)
    painter.setBrush(m_surfaceColor);
    painter.setPen(Qt::NoPen);
    painter.drawPath(path);
    
    // Draw border
    if (m_borderWidth > 0 && m_borderColor.alpha() > 0) {
        QPen pen(m_borderColor, m_borderWidth);
        pen.setJoinStyle(Qt::RoundJoin);
        painter.setPen(pen);
        painter.setBrush(Qt::NoBrush);
        painter.drawPath(path);
    }
    
    painter.end();
    
    // Set pixmap to label
    m_overlayLabel->setPixmap(overlayPixmap);
    m_overlayLabel->setScaledContents(true);
}

void MBlurCard::updateMaskPath()
{
    m_maskPath = QPainterPath();
    m_maskPath.addRoundedRect(rect(), m_borderRadius, m_borderRadius);
}

void MBlurCard::updateLayoutMargins()
{
    if (layout()) {
        int margin = static_cast<int>(m_borderWidth) + 12;
        layout()->setContentsMargins(margin, margin, margin, margin);
    }
}

QPixmap MBlurCard::captureBackground() const
{
    if (!m_blurSource || !m_blurSource->isVisible()) {
        return QPixmap();
    }

    // Get the position of this widget relative to the blur source
    QPoint sourcePos = m_blurSource->mapToGlobal(QPoint(0, 0));
    QPoint thisPos = mapToGlobal(QPoint(0, 0));
    
    // Calculate capture rectangle
    QRect captureRect(
        thisPos.x() - sourcePos.x(),
        thisPos.y() - sourcePos.y(),
        width(),
        height()
    );

    // Capture from the source widget
    QPixmap captured = m_blurSource->grab(captureRect);
    
    // Fallback: capture from screen
    if (captured.isNull()) {
        QScreen* screen = QApplication::primaryScreen();
        if (screen) {
            captured = screen->grabWindow(
                0,  // 0 for entire desktop
                mapToGlobal(QPoint(0, 0)).x(),
                mapToGlobal(QPoint(0, 0)).y(),
                width(),
                height()
            );
        }
    }

    return captured;
}

void MBlurCard::startRealTimeUpdates()
{
    if (m_realtimeEnabled && m_updateTimer && !m_updateTimer->isActive()) {
        m_updateTimer->start();
    }
}

void MBlurCard::stopRealTimeUpdates()
{
    if (m_updateTimer && m_updateTimer->isActive()) {
        m_updateTimer->stop();
    }
}