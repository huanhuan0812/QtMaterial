#ifndef MCARD_H
#define MCARD_H

#include <QWidget>
#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent>
#include <QGraphicsDropShadowEffect>
#include <QLayout>
#include <QStyleOption>

#include "utils/MTheme.h"

class MCard : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(Type type READ type WRITE setType)
    Q_PROPERTY(int radius READ radius WRITE setRadius)
    Q_PROPERTY(int padding READ padding WRITE setPadding)
    Q_PROPERTY(QColor containerColor READ containerColor WRITE setContainerColor)
    Q_PROPERTY(QColor outlineColor READ outlineColor WRITE setOutlineColor)
    Q_PROPERTY(int elevationLevel READ elevationLevel WRITE setElevationLevel)
    Q_PROPERTY(bool hovered READ hovered WRITE setHovered)
    Q_PROPERTY(bool pressed READ pressed WRITE setPressed)

public:
    enum class Type {
        Elevated,
        Filled,
        Outlined
    };
    Q_ENUM(Type)

    explicit MCard(QWidget* parent = nullptr);
    ~MCard() override = default;

    // Type
    Type type() const { return m_type; }
    void setType(Type type);

    // Radius
    int radius() const { return m_radius; }
    void setRadius(int radius);

    // Padding
    int padding() const { return m_padding; }
    void setPadding(int padding);

    // Colors
    QColor containerColor() const { return m_containerColor; }
    void setContainerColor(const QColor& color);

    QColor outlineColor() const { return m_outlineColor; }
    void setOutlineColor(const QColor& color);

    // Elevation
    int elevationLevel() const { return m_elevationLevel; }
    void setElevationLevel(int level);

    // States
    bool hovered() const { return m_hovered; }
    void setHovered(bool hovered);

    bool pressed() const { return m_pressed; }
    void setPressed(bool pressed);

    // Content management
    void setContentWidget(QWidget* widget);
    QWidget* contentWidget() const { return m_contentWidget; }

signals:
    void clicked();
    void typeChanged();
    void radiusChanged();
    void paddingChanged();
    void containerColorChanged();
    void outlineColorChanged();
    void elevationLevelChanged();
    void hoveredChanged();
    void pressedChanged();

protected:
    void paintEvent(QPaintEvent* event) override;
    void enterEvent(QEnterEvent* event) override;
    void leaveEvent(QEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    bool event(QEvent* event) override;

private:
    void updateStyle();
    void updateShadow();
    QColor getDefaultContainerColor() const;
    QColor getDefaultOutlineColor() const;
    int getDefaultElevationLevel() const;
    void updateContentMargins();

private:
    Type m_type = Type::Elevated;
    int m_radius = 12;
    int m_padding = 16;
    QColor m_containerColor;
    QColor m_outlineColor;
    int m_elevationLevel = -1;  // -1 means auto-calculate from type
    bool m_hovered = false;
    bool m_pressed = false;

    QWidget* m_contentWidget = nullptr;
    QGraphicsDropShadowEffect* m_shadowEffect = nullptr;

    // Cache for performance
    QPainterPath m_cachedPath;
};

#endif // MCARD_H