#ifndef MTOOLTIP_H
#define MTOOLTIP_H

#include <QWidget>
#include <QTimer>
#include <QPropertyAnimation>
#include <QLabel>
#include <QPainter>
#include <QPainterPath>
#include <QFont>
#include <QColor>

#include "utils/MTheme.h"

class MToolTip : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity)
    
public:
    explicit MToolTip(QWidget* parent = nullptr);
    ~MToolTip() override = default;
    
    // API - 与QML版本保持一致
    QString text() const { return m_text; }
    void setText(const QString& text);
    
    int timeout() const { return m_timeout; }
    void setTimeout(int ms);
    
    // 显示/隐藏控制
    Q_INVOKABLE void open();
    Q_INVOKABLE void close();
    
    // Q_PROPERTY getter/setter
    qreal opacity() const { return m_opacity; }
    void setOpacity(qreal opacity);
    
protected:
    void paintEvent(QPaintEvent* event) override;
    void showEvent(QShowEvent* event) override;
    void hideEvent(QHideEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    
private:
    void initUI();
    void updateSizeAndPosition();
    void updateThemeColors();
    
    // 主题辅助方法 - 与QML中的_colors, _typography, _shape对应
    QColor getSurfaceColor() const;
    QColor getOnSurfaceColor() const;
    QFont getTextFont() const;
    int getCornerRadius() const;
    
    // 数据成员
    QString m_text;
    int m_timeout;
    qreal m_opacity;
    
    // UI组件
    QLabel* m_label;
    
    // 动画
    QPropertyAnimation* m_showAnim;
    QPropertyAnimation* m_hideAnim;
    QTimer* m_hideTimer;
    
    // 主题
    MTheme* m_theme;
    
    // MD3常量 - 与QML对应
    static constexpr int PADDING_HORIZONTAL = 8;  // 8dp左右内边距
    static constexpr int PADDING_VERTICAL = 4;    // 4dp上下内边距
    static constexpr int CORNER_RADIUS = 4;       // MD3 Extra Small
    static constexpr int MAX_WIDTH_RATIO = 80;    // 最大宽度为父窗口的80%
    static constexpr int DEFAULT_TIMEOUT = 2500;  // 默认超时时间
    static constexpr int Z_INDEX = 999;           // 层级
    
    // 动画时长
    static constexpr int SHOW_DURATION = 200;
    static constexpr int HIDE_DURATION = 150;
};

#endif // MTOOLTIP_H