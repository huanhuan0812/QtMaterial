#ifndef MBLURCARD_H
#define MBLURCARD_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QGraphicsBlurEffect>
#include <QPropertyAnimation>
#include <QTimer>
#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QLabel>

#include "utils/MTheme.h"

class MBlurCard : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QWidget* blurSource READ blurSource WRITE setBlurSource)
    Q_PROPERTY(qreal blurAmount READ blurAmount WRITE setBlurAmount)
    Q_PROPERTY(bool dragable READ dragable WRITE setDragable)
    Q_PROPERTY(int blurMax READ blurMax WRITE setBlurMax)
    Q_PROPERTY(int borderRadius READ borderRadius WRITE setBorderRadius)
    Q_PROPERTY(QColor borderColor READ borderColor WRITE setBorderColor)
    Q_PROPERTY(qreal borderWidth READ borderWidth WRITE setBorderWidth)
    Q_PROPERTY(qreal overlayOpacity READ overlayOpacity WRITE setOverlayOpacity)

public:
    explicit MBlurCard(QWidget* parent = nullptr);
    ~MBlurCard() override;

    // Getters
    QWidget* blurSource() const { return m_blurSource; }
    qreal blurAmount() const { return m_blurAmount; }
    bool dragable() const { return m_dragable; }
    int blurMax() const { return m_blurMax; }
    int borderRadius() const { return m_borderRadius; }
    QColor borderColor() const { return m_borderColor; }
    qreal borderWidth() const { return m_borderWidth; }
    qreal overlayOpacity() const { return m_overlayOpacity; }

    // Setters
    void setBlurSource(QWidget* source);
    void setBlurAmount(qreal amount);
    void setDragable(bool dragable);
    void setBlurMax(int max);
    void setBorderRadius(int radius);
    void setBorderColor(const QColor& color);
    void setBorderWidth(qreal width);
    void setOverlayOpacity(qreal opacity);

    // Content management
    void setContentWidget(QWidget* widget);
    QWidget* contentWidget() const { return m_contentWidget; }

    // Animation
    void animateBlur(qreal targetAmount, int duration = 300);

signals:
    void blurSourceChanged();
    void blurAmountChanged();
    void dragableChanged();
    void blurMaxChanged();
    void borderRadiusChanged();
    void borderColorChanged();
    void borderWidthChanged();
    void overlayOpacityChanged();
    void clicked();
    void dragStarted();
    void dragEnded();

protected:
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void showEvent(QShowEvent* event) override;
    void hideEvent(QHideEvent* event) override;
    bool event(QEvent* event) override;

private:
    void setupGraphicsScene();
    void updateBackground();
    void updateMask();
    void updateOverlay();
    void updateMaskPath();
    void updateLayoutMargins();
    QPixmap captureBackground() const;
    void startRealTimeUpdates();
    void stopRealTimeUpdates();

private:
    // Core properties
    QWidget* m_blurSource = nullptr;
    qreal m_blurAmount = 20.0;
    bool m_dragable = false;
    int m_blurMax = 64;
    int m_borderRadius = 24;
    QColor m_borderColor = Qt::transparent;
    qreal m_borderWidth = 0.0;
    qreal m_overlayOpacity = 0.2;

    // Graphics components
    QGraphicsScene* m_scene = nullptr;
    QGraphicsView* m_view = nullptr;
    QGraphicsPixmapItem* m_backgroundItem = nullptr;
    QGraphicsBlurEffect* m_blurEffect = nullptr;
    
    // Mask layer
    QWidget* m_maskWidget = nullptr;
    QPainterPath m_maskPath;

    // Overlay (使用QLabel来显示绘制好的图片)
    QLabel* m_overlayLabel = nullptr;
    QColor m_surfaceColor;

    // Content
    QWidget* m_contentWidget = nullptr;

    // Animation
    QPropertyAnimation* m_blurAnimation = nullptr;

    // Real-time update timer
    QTimer* m_updateTimer = nullptr;
    bool m_realtimeEnabled = true;

    // Drag state
    bool m_isDragging = false;
    QPoint m_dragStartPos;
    QPoint m_widgetStartPos;

    // Hover/Press state
    bool m_isHovered = false;
    bool m_isPressed = false;

    // Cache
    QRect m_cachedRect;
};

#endif // MBLURCARD_H