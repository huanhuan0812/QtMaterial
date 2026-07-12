// MButtonBase.h
#ifndef MBUTTONBASE_H
#define MBUTTONBASE_H

#include <QPushButton>
#include <QColor>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>

#ifdef QTMATERIAL_LIBRARY
#  define QTMATERIAL_EXPORT Q_DECL_EXPORT
#else
#  define QTMATERIAL_EXPORT Q_DECL_IMPORT
#endif

class QTMATERIAL_EXPORT MButtonBase : public QPushButton
{
    Q_OBJECT

    // 颜色属性
    Q_PROPERTY(QColor primaryColor READ primaryColor WRITE setPrimaryColor)
    Q_PROPERTY(QColor rippleColor READ rippleColor WRITE setRippleColor)
    
    // 尺寸属性
    Q_PROPERTY(int cornerRadius READ cornerRadius WRITE setCornerRadius)
    
    // 视觉效果开关
    Q_PROPERTY(bool rippleEnabled READ isRippleEnabled WRITE setRippleEnabled)
    Q_PROPERTY(bool stateLayerEnabled READ isStateLayerEnabled WRITE setStateLayerEnabled)
    Q_PROPERTY(bool focusIndicatorEnabled READ isFocusIndicatorEnabled WRITE setFocusIndicatorEnabled)
    Q_PROPERTY(bool hoverElevationEnabled READ isHoverElevationEnabled WRITE setHoverElevationEnabled)
    
    // 动画属性
    Q_PROPERTY(int animationDuration READ animationDuration WRITE setAnimationDuration)
    
    // 内部动画属性
    Q_PROPERTY(qreal rippleOpacity READ rippleOpacity WRITE setRippleOpacity)
    Q_PROPERTY(qreal rippleRadius READ rippleRadius WRITE setRippleRadius)
    Q_PROPERTY(qreal stateLayerOpacity READ stateLayerOpacity WRITE setStateLayerOpacity)
    Q_PROPERTY(qreal focusIndicatorOpacity READ focusIndicatorOpacity WRITE setFocusIndicatorOpacity)
    Q_PROPERTY(qreal elevationOffset READ elevationOffset WRITE setElevationOffset)

public:
    explicit MButtonBase(QWidget *parent = nullptr);
    explicit MButtonBase(const QString &text, QWidget *parent = nullptr);
    MButtonBase(const QIcon &icon, const QString &text, QWidget *parent = nullptr);
    virtual ~MButtonBase();

    // 颜色访问器
    QColor primaryColor() const;
    void setPrimaryColor(const QColor &color);
    
    QColor rippleColor() const;
    void setRippleColor(const QColor &color);

    // 尺寸访问器
    int cornerRadius() const;
    void setCornerRadius(int radius);

    // 视觉效果开关
    bool isRippleEnabled() const;
    void setRippleEnabled(bool enabled);

    bool isStateLayerEnabled() const;
    void setStateLayerEnabled(bool enabled);

    bool isFocusIndicatorEnabled() const;
    void setFocusIndicatorEnabled(bool enabled);

    bool isHoverElevationEnabled() const;
    void setHoverElevationEnabled(bool enabled);

    // 动画时长
    int animationDuration() const;
    void setAnimationDuration(int ms);

    // 内部动画属性访问器
    qreal rippleOpacity() const;
    void setRippleOpacity(qreal opacity);

    qreal rippleRadius() const;
    void setRippleRadius(qreal radius);

    qreal stateLayerOpacity() const;
    void setStateLayerOpacity(qreal opacity);

    qreal focusIndicatorOpacity() const;
    void setFocusIndicatorOpacity(qreal opacity);

    qreal elevationOffset() const;
    void setElevationOffset(qreal offset);

    // 状态访问器
    void setCheckable(bool checkable);
    bool isCheckable() const;

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void focusInEvent(QFocusEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

    // 子类需要实现的虚函数
    virtual void paintBackground(QPainter &painter, const QRect &rect) = 0;
    virtual void paintBorder(QPainter &painter, const QRect &rect) = 0;
    virtual QColor getCurrentTextColor() const = 0;
    virtual QColor getCurrentIconColor() const = 0;
    
    // 公共绘制辅助函数
    void paintRipple(QPainter &painter, const QRect &rect);
    void paintStateLayer(QPainter &painter, const QRect &rect);
    void paintFocusIndicator(QPainter &painter, const QRect &rect);
    void paintHoverElevation(QPainter &painter, const QRect &rect);
    void paintLabel(QPainter &painter);
    
    // 动画控制
    void startRippleAnimation(const QPointF &center);
    void startStateLayerAnimation(bool visible);
    void startFocusIndicatorAnimation(bool visible);
    void startHoverElevationAnimation(bool hovered);
    
    // 状态访问
    bool isHovered() const { return m_hovered; }
    bool isPressed() const { return m_pressed; }
    const QPointF& rippleCenter() const { return m_rippleCenter; }

    //toggle checkable state
    QColor getContrastColor(const QColor &color) const;

private:
    void setupAnimations();
    
    // 颜色
    QColor m_primaryColor;
    QColor m_rippleColor;
    
    // 尺寸
    int m_cornerRadius = 20;
    int m_animationDuration = 300;
    
    // 视觉效果开关
    bool m_rippleEnabled = true;
    bool m_stateLayerEnabled = true;
    bool m_focusIndicatorEnabled = true;
    bool m_hoverElevationEnabled = true;
    
    // 动画状态
    qreal m_rippleOpacity = 0.0;
    qreal m_rippleRadius = 0.0;
    qreal m_stateLayerOpacity = 0.0;
    qreal m_focusIndicatorOpacity = 0.0;
    qreal m_elevationOffset = 0.0;
    
    QPointF m_rippleCenter;
    QPointF m_mousePos;
    bool m_hovered = false;
    bool m_pressed = false;

    // 状态属性
    bool m_checkable = false;
    
    // 动画对象
    QPropertyAnimation *m_rippleOpacityAnimation;
    QPropertyAnimation *m_rippleRadiusAnimation;
    QParallelAnimationGroup *m_rippleAnimationGroup;
    
    QPropertyAnimation *m_stateLayerAnimation;
    QPropertyAnimation *m_focusIndicatorAnimation;
    QPropertyAnimation *m_hoverElevationAnimation;
};

#endif // MBUTTONBASE_H