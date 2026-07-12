// MElevatedButton.h
#ifndef MELEVATEDBUTTON_H
#define MELEVATEDBUTTON_H

#include "MButtonBase.h"

class QTMATERIAL_EXPORT MElevatedButton : public MButtonBase
{
    Q_OBJECT
    Q_PROPERTY(qreal elevation READ elevation WRITE setElevation)
    Q_PROPERTY(qreal pressedElevation READ pressedElevation WRITE setPressedElevation)

public:
    explicit MElevatedButton(QWidget *parent = nullptr);
    explicit MElevatedButton(const QString &text, QWidget *parent = nullptr);
    MElevatedButton(const QIcon &icon, const QString &text, QWidget *parent = nullptr);
    virtual ~MElevatedButton();

    // 阴影高度
    qreal elevation() const;
    void setElevation(qreal elevation);
    
    qreal pressedElevation() const;
    void setPressedElevation(qreal elevation);
    
    // 背景颜色
    QColor backgroundColor() const;
    void setBackgroundColor(const QColor &color);

protected:
    void paintBackground(QPainter &painter, const QRect &rect) override;
    void paintBorder(QPainter &painter, const QRect &rect) override;
    void paintShadow(QPainter &painter, const QRect &rect);
    QColor getCurrentTextColor() const override;
    QColor getCurrentIconColor() const override;

private:
    void updateElevation();
    QColor getContrastColor(const QColor &color) const;
    void paintElevatedShadow(QPainter &painter, const QRect &rect, qreal elevation);

    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
    
    qreal m_elevation = 2.0;           // 默认阴影高度
    qreal m_pressedElevation = 8.0;    // 按下时的阴影高度
    qreal m_currentElevation = 2.0;    // 当前阴影高度（用于动画）
    QColor m_backgroundColor;
    
    QPropertyAnimation *m_elevationAnimation;
    
private slots:
    void animateElevation(bool pressed);
};

#endif // MELEVATEDBUTTON_H