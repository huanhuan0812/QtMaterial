// MButton.h
#ifndef MBUTTON_H
#define MBUTTON_H

#include <QPushButton>
#include <QPainter>
#include <QPainterPath>
#include <QVariantAnimation>
#include <QParallelAnimationGroup>
#include <QSequentialAnimationGroup>
#include <QTimer>
#include <QGraphicsDropShadowEffect>
#include <QList>
#include "utils/MTheme.h"  // 添加 MTheme 头文件

class MButton : public QPushButton
{
    Q_OBJECT

public:
    enum Type {
        Elevated,
        Filled,
        FilledTonal,
        Outlined,
        Text
    };
    Q_ENUM(Type)

    explicit MButton(QWidget *parent = nullptr);
    ~MButton();
    
    // 属性设置
    void setButtonType(Type type);
    void setIcon(const QString &iconText);
    void setIconFont(const QFont &font);
    void setHorizontalPadding(qreal padding);
    void setVerticalPadding(qreal padding);
    void setSpacing(qreal spacing);
    
    // Getter
    Type buttonType() const { return m_type; }
    QString icon() const { return m_iconText; }
    QFont iconFont() const { return m_iconFont; }
    qreal horizontalPadding() const { return m_horizontalPadding; }
    qreal verticalPadding() const { return m_verticalPadding; }
    qreal spacing() const { return m_spacing; }

    // 尺寸提示
    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

protected:
    void paintEvent(QPaintEvent *event) override;
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void focusInEvent(QFocusEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void changeEvent(QEvent *event) override;
    bool event(QEvent *event) override;

private:
    // 涟漪数据结构
    struct RippleData {
        QPointF center;
        qreal radius = 0.0;
        qreal opacity = 0.0;
        bool isActive = false;
        bool isFadingOut = false;
        QVariantAnimation *radiusAnim = nullptr;
        QVariantAnimation *opacityAnim = nullptr;
        QParallelAnimationGroup *group = nullptr;
        QTimer *fadeTimer = nullptr;
    };
    
    // 状态
    struct State {
        bool hovered = false;
        bool pressed = false;
        bool focused = false;
        bool enabled = true;
    };

    // 私有方法
    void initShadowEffect();
    void updateShadowEffect();
    void createRipple(const QPointF &pos);
    void startRippleFadeOut(RippleData *ripple);
    void cleanupRipple(RippleData *ripple);
    void cleanupAllRipples();
    
    // 从 MTheme 获取颜色的方法
    QColor getContainerColor() const;
    QColor getContentColor() const;
    QColor getStateLayerColor() const;
    QColor getOutlineColor() const;
    QColor getShadowColor() const;
    qreal getElevation() const;
    qreal getStateLayerOpacity() const;
    
    // 从 MTheme 获取主题参数
    qreal getCornerRadius() const;
    qreal getRippleOpacity() const;
    int getRippleDuration() const;
    int getRippleFadeDuration() const;
    int getRippleFadeDelay() const;
    
    void drawBackground(QPainter *painter, const QRectF &rect);
    void drawStateLayer(QPainter *painter, const QRectF &rect);
    void drawOutline(QPainter *painter, const QRectF &rect);
    void drawContent(QPainter *painter, const QRectF &rect);
    void drawRipples(QPainter *painter, const QRectF &rect);
    void drawShadow(QPainter *painter, const QRectF &rect);
    void updateTheme();
    
    QRectF getContentRect(const QRectF &rect) const;
    qreal calculateContentWidth() const;
    qreal calculateContentHeight() const;

    // 成员变量
    Type m_type = Filled;
    QString m_iconText;
    QFont m_iconFont;
    qreal m_horizontalPadding = 24.0;
    qreal m_verticalPadding = 0.0;
    qreal m_spacing = 8.0;
    State m_state;

    // 阴影效果
    QGraphicsDropShadowEffect *m_shadowEffect = nullptr;
    qreal m_currentElevation = 0.0;

    // 涟漪效果 - 支持多个涟漪同时存在
    QList<RippleData*> m_ripples;
    RippleData *m_currentRipple = nullptr;
};

#endif // MBUTTON_H