// MToggleButton.h
#ifndef MTOGGLEBUTTON_H
#define MTOGGLEBUTTON_H

#include "MButtonBase.h"

class QTMATERIAL_EXPORT MToggleButton : public MButtonBase
{
    Q_OBJECT
    Q_PROPERTY(bool checked READ isChecked WRITE setChecked NOTIFY toggled)
    Q_PROPERTY(QColor checkedColor READ checkedColor WRITE setCheckedColor)
    Q_PROPERTY(QColor uncheckedColor READ uncheckedColor WRITE setUncheckedColor)
    Q_PROPERTY(ToggleStyle toggleStyle READ toggleStyle WRITE setToggleStyle)

public:
    // 切换按钮样式
    enum ToggleStyle {
        StyleFilled,      // 填充样式（类似 Filled Button）
        StyleOutlined,    // 描边样式（类似 Outlined Button）
        StyleText         // 文本样式（类似 Text Button）
    };
    Q_ENUM(ToggleStyle)

    explicit MToggleButton(QWidget *parent = nullptr);
    explicit MToggleButton(const QString &text, QWidget *parent = nullptr);
    MToggleButton(const QIcon &icon, const QString &text, QWidget *parent = nullptr);
    virtual ~MToggleButton();

    // 开关状态
    bool isChecked() const;
    void setChecked(bool checked);
    
    // 颜色配置
    QColor checkedColor() const;
    void setCheckedColor(const QColor &color);
    
    QColor uncheckedColor() const;
    void setUncheckedColor(const QColor &color);
    
    // 样式配置
    ToggleStyle toggleStyle() const;
    void setToggleStyle(ToggleStyle style);
    
    // 图标配置（选中/未选中不同图标）
    void setCheckedIcon(const QIcon &icon);
    void setUncheckedIcon(const QIcon &icon);
    
signals:
    void toggled(bool checked);

protected:
    void paintBackground(QPainter &painter, const QRect &rect) override;
    void paintBorder(QPainter &painter, const QRect &rect) override;
    QColor getCurrentTextColor() const override;
    QColor getCurrentIconColor() const override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintIcon(QPainter &painter);

private:
    void updateStyle();
    QColor getCurrentBackgroundColor() const;
    
    bool m_checked = false;
    QColor m_checkedColor;
    QColor m_uncheckedColor;
    ToggleStyle m_toggleStyle = StyleOutlined;
    
    QIcon m_checkedIcon;
    QIcon m_uncheckedIcon;
    bool m_hasCustomIcons = false;

    void paintEvent(QPaintEvent *event) override;
};

#endif // MTOGGLEBUTTON_H