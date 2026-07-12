#ifndef MWIDGETBASE_H
#define MWIDGETBASE_H

#include <QWidget>
#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QGraphicsDropShadowEffect>
#include <QTimer>
#include <QPalette>
#include <QDebug>
#include <QEnterEvent>

#ifdef QTMATERIAL_LIBRARY
    #ifdef _WIN32
        #ifdef QTMATERIAL_SHARED
            #define QTMATERIAL_EXPORT __declspec(dllexport)
        #else
            #define QTMATERIAL_EXPORT __declspec(dllimport)
        #endif
    #else
        #define QTMATERIAL_EXPORT __attribute__((visibility("default")))
    #endif
#else
    #define QTMATERIAL_EXPORT
#endif

// Material Design 3 颜色系统
struct MColorScheme {
    // Primary colors
    QColor primary = QColor(0x1A, 0x73, 0xE8);
    QColor onPrimary = QColor(0xFF, 0xFF, 0xFF);
    QColor primaryContainer = QColor(0xD2, 0xE3, 0xFC);
    QColor onPrimaryContainer = QColor(0x04, 0x1E, 0x49);
    
    // Secondary colors
    QColor secondary = QColor(0x62, 0x5B, 0x71);
    QColor onSecondary = QColor(0xFF, 0xFF, 0xFF);
    QColor secondaryContainer = QColor(0xE8, 0xDD, 0xF9);
    QColor onSecondaryContainer = QColor(0x1E, 0x19, 0x2B);
    
    // Surface colors
    QColor surface = QColor(0xFB, 0xFC, 0xFF);
    QColor surfaceDim = QColor(0xDA, 0xE0, 0xE7);
    QColor surfaceContainer = QColor(0xF0, 0xF2, 0xF5);
    QColor onSurface = QColor(0x1C, 0x1B, 0x1F);
    QColor onSurfaceVariant = QColor(0x44, 0x44, 0x4F);
    
    // Error colors
    QColor error = QColor(0xBA, 0x1A, 0x1A);
    QColor onError = QColor(0xFF, 0xFF, 0xFF);
    QColor errorContainer = QColor(0xFF, 0xDA, 0xD6);
    QColor onErrorContainer = QColor(0x41, 0x00, 0x05);
    
    // Outline
    QColor outline = QColor(0x75, 0x75, 0x7F);
    QColor outlineVariant = QColor(0xC4, 0xC4, 0xD0);
    
    // Shadow
    QColor shadow = QColor(0x00, 0x00, 0x00);
    
    // Scrim
    QColor scrim = QColor(0x00, 0x00, 0x00);
    
    // Surface Tint
    QColor surfaceTint = QColor(0x1A, 0x73, 0xE8);
};

// 窗口类型枚举
enum class MWindowType {
    Normal,
    Dialog,
    BottomSheet,
    FullScreen
};

// 主题模式
enum class MThemeMode {
    Light,
    Dark,
    System
};

class QTMATERIAL_EXPORT MWidgetBase : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(qreal elevation READ getElevation WRITE setElevation)
    Q_PROPERTY(qreal cornerRadius READ cornerRadius WRITE setCornerRadius)
    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor)

public:
    explicit MWidgetBase(QWidget *parent = nullptr);
    virtual ~MWidgetBase();

    // === 基础属性设置 ===
    void setWindowType(MWindowType type);
    MWindowType windowType() const { return m_windowType; }
    
    void setThemeMode(MThemeMode mode);
    MThemeMode themeMode() const { return m_themeMode; }
    
    // === 颜色系统 ===
    virtual void setColorScheme(const MColorScheme &scheme);
    MColorScheme colorScheme() const { return m_colorScheme; }
    
    // 获取特定颜色的便捷方法
    virtual QColor getPrimaryColor() const;
    virtual QColor getSurfaceColor() const;
    virtual QColor getOnSurfaceColor() const;
    virtual QColor getOnSurfaceVariant() const;
    virtual QColor getErrorColor() const;
    virtual QColor getOutlineColor() const;
    
    // === 形状属性 ===
    void setCornerRadius(qreal radius);
    qreal cornerRadius() const { return m_cornerRadius; }
    
    void setElevation(qreal elevation);
    qreal getElevation() const { return m_elevation; }
    
    // 设置是否显示阴影
    void setShadowEnabled(bool enabled);
    bool shadowEnabled() const { return m_shadowEnabled; }
    
    // === 状态管理 ===
    void setHovered(bool hovered);
    bool isHovered() const { return m_hovered; }
    
    void setPressed(bool pressed);
    bool isPressed() const { return m_pressed; }
    
    void setFocused(bool focused);
    bool isFocused() const { return m_focused; }
    
    // === 动画控制 ===
    virtual void animateElevation(qreal targetElevation, int duration = 200);
    virtual void animateColor(const QColor &targetColor, int duration = 200);
    
    // === 主题切换 ===
    virtual void applyTheme();
    virtual void updateColors();
    
    // === 窗口控制 ===
    virtual void showAsDialog();
    virtual void showAsBottomSheet();
    virtual void showFullScreen();
    
    // === 自定义绘制接口（供子类重写） ===
    virtual void drawBackground(QPainter *painter, const QRect &rect);
    virtual void drawForeground(QPainter *painter, const QRect &rect);
    virtual void drawShadow(QPainter *painter, const QRect &rect);
    
    // === 状态变化虚函数（供子类重写） ===
    virtual void onHoverEnter();
    virtual void onHoverLeave();
    virtual void onPress();
    virtual void onRelease();
    virtual void onFocusIn();
    virtual void onFocusOut();
    virtual void onThemeChanged();

    // === Q_PROPERTY 的 getter/setter ===
    QColor backgroundColor() const;
    void setBackgroundColor(const QColor &color);

protected:
    // Qt事件重载
    void paintEvent(QPaintEvent *event) override;
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void focusInEvent(QFocusEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void showEvent(QShowEvent *event) override;
    void hideEvent(QHideEvent *event) override;
    
    // 获取绘制区域（考虑阴影和边距）
    virtual QRect getContentRect() const;
    virtual QRect getShadowRect() const;
    
    // 创建阴影效果
    virtual QGraphicsDropShadowEffect* createShadowEffect();
    
    // 获取当前背景颜色
    virtual QColor getCurrentBackgroundColor() const;

private:
    void initWidget();
    void setupShadow();
    void setupAnimations();
    void updateShadowGeometry();

protected:
    // === 成员变量 ===
    MWindowType m_windowType = MWindowType::Normal;
    MThemeMode m_themeMode = MThemeMode::Light;
    MColorScheme m_colorScheme;
    
    qreal m_cornerRadius = 12.0;
    qreal m_elevation = 0.0;
    bool m_shadowEnabled = true;
    
    // 状态
    bool m_hovered = false;
    bool m_pressed = false;
    bool m_focused = false;
    
    // 阴影效果
    QGraphicsDropShadowEffect *m_shadowEffect = nullptr;
    
    // 动画
    QPropertyAnimation *m_elevationAnimation = nullptr;
    QPropertyAnimation *m_colorAnimation = nullptr;
    QParallelAnimationGroup *m_animationGroup = nullptr;
    
    // 边距（为阴影预留空间）
    static constexpr int SHADOW_MARGIN = 20;
    
    // 背景颜色
    QColor m_backgroundColor;
};

#endif // MWIDGETBASE_H