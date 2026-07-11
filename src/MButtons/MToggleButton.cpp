// MToggleButton.cpp
#include "MToggleButton.h"
#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent>
#include <QStyleOptionButton>

MToggleButton::MToggleButton(QWidget *parent)
    : MButtonBase(parent)
    , m_checkedColor(primaryColor())
    , m_uncheckedColor(palette().color(QPalette::ButtonText))
{
    setCheckable(true);
    updateStyle();
}

MToggleButton::MToggleButton(const QString &text, QWidget *parent)
    : MToggleButton(parent)
{
    setText(text);
}

MToggleButton::MToggleButton(const QIcon &icon, const QString &text, QWidget *parent)
    : MToggleButton(text, parent)
{
    setIcon(icon);
}

MToggleButton::~MToggleButton()
{
}

bool MToggleButton::isChecked() const { return m_checked; }
void MToggleButton::setChecked(bool checked) {
    if (m_checked != checked) {
        m_checked = checked;
        updateStyle();
        update();
        emit toggled(m_checked);
    }
}

QColor MToggleButton::checkedColor() const { return m_checkedColor; }
void MToggleButton::setCheckedColor(const QColor &color) {
    if (m_checkedColor != color) {
        m_checkedColor = color;
        update();
    }
}

QColor MToggleButton::uncheckedColor() const { return m_uncheckedColor; }
void MToggleButton::setUncheckedColor(const QColor &color) {
    if (m_uncheckedColor != color) {
        m_uncheckedColor = color;
        update();
    }
}

MToggleButton::ToggleStyle MToggleButton::toggleStyle() const { return m_toggleStyle; }
void MToggleButton::setToggleStyle(ToggleStyle style) {
    if (m_toggleStyle != style) {
        m_toggleStyle = style;
        updateStyle();
        update();
    }
}

void MToggleButton::setCheckedIcon(const QIcon &icon) {
    m_checkedIcon = icon;
    m_hasCustomIcons = true;
    update();
}

void MToggleButton::setUncheckedIcon(const QIcon &icon) {
    m_uncheckedIcon = icon;
    m_hasCustomIcons = true;
    update();
}

void MToggleButton::paintBackground(QPainter &painter, const QRect &rect)
{
    if (m_toggleStyle == StyleFilled) {
        QColor bgColor = getCurrentBackgroundColor();
        if (!isEnabled()) {
            bgColor.setAlphaF(0.12);
        }
        
        QPainterPath backgroundPath;
        backgroundPath.addRoundedRect(rect, cornerRadius(), cornerRadius());
        painter.fillPath(backgroundPath, bgColor);
    } else if (m_toggleStyle == StyleOutlined) {
        // 背景透明，不绘制
    } else if (m_toggleStyle == StyleText) {
        // 背景透明，不绘制
    }
}

void MToggleButton::paintBorder(QPainter &painter, const QRect &rect)
{
    if (m_toggleStyle == StyleOutlined && isEnabled()) {
        QColor borderColor = m_checked ? m_checkedColor : m_uncheckedColor;
        if (!m_checked && !isHovered() && !isPressed()) {
            borderColor.setAlphaF(0.38);
        }
        
        QPen borderPen(borderColor, 1.5);
        painter.setPen(borderPen);
        painter.setBrush(Qt::NoBrush);
        
        QRectF borderRect = QRectF(rect).adjusted(0.5, 0.5, -0.5, -0.5);
        painter.drawRoundedRect(borderRect, cornerRadius(), cornerRadius());
    }
}

QColor MToggleButton::getCurrentTextColor() const
{
    if (!isEnabled()) {
        QColor disabledColor = m_uncheckedColor;
        disabledColor.setAlphaF(0.38);
        return disabledColor;
    }
    
    if (m_toggleStyle == StyleFilled) {
        return getContrastColor(getCurrentBackgroundColor());
    } else {
        return m_checked ? m_checkedColor : m_uncheckedColor;
    }
}

QColor MToggleButton::getCurrentIconColor() const
{
    return getCurrentTextColor();
}

void MToggleButton::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && rect().contains(event->pos())) {
        setChecked(!m_checked);
    }
    MButtonBase::mouseReleaseEvent(event);
}

void MToggleButton::paintIcon(QPainter &painter)
{
    if (!m_hasCustomIcons) {
        MButtonBase::paintLabel(painter);
        return;
    }
    
    QIcon currentIcon = m_checked ? m_checkedIcon : m_uncheckedIcon;
    if (currentIcon.isNull()) {
        MButtonBase::paintLabel(painter);
        return;
    }
    
    // 绘制自定义图标
    QRect iconRect(0, 0, height() * 0.6, height() * 0.6);
    iconRect.moveCenter(rect().center());
    
    if (!text().isEmpty()) {
        // 如果还有文字，图标在左边
        int textWidth = fontMetrics().horizontalAdvance(text());
        int totalWidth = iconRect.width() + 8 + textWidth;
        iconRect.moveLeft((width() - totalWidth) / 2);
    }
    
    QPixmap pixmap = currentIcon.pixmap(iconRect.size());
    painter.drawPixmap(iconRect, pixmap);
    
    // 绘制文字
    if (!text().isEmpty()) {
        QRect textRect = rect();
        textRect.setLeft(iconRect.right() + 8);
        textRect.setRight(textRect.right() - 8);
        
        QStyleOptionButton option;
        initStyleOption(&option);
        option.rect = textRect;
        option.palette.setColor(QPalette::ButtonText, getCurrentTextColor());
        
        style()->drawControl(QStyle::CE_PushButtonLabel, &option, &painter, this);
    }
}

void MToggleButton::updateStyle()
{
    if (m_toggleStyle == StyleFilled) {
        setStateLayerEnabled(true);
        setFocusIndicatorEnabled(true);
        setHoverElevationEnabled(true);
    } else if (m_toggleStyle == StyleOutlined) {
        setStateLayerEnabled(true);
        setFocusIndicatorEnabled(true);
        setHoverElevationEnabled(false);
    } else if (m_toggleStyle == StyleText) {
        setStateLayerEnabled(true);
        setFocusIndicatorEnabled(true);
        setHoverElevationEnabled(false);
    }
}

QColor MToggleButton::getCurrentBackgroundColor() const
{
    if (!isEnabled()) {
        QColor disabledBg = m_uncheckedColor;
        disabledBg.setAlphaF(0.12);
        return disabledBg;
    }
    
    return m_checked ? m_checkedColor : m_uncheckedColor;
}

// 重写 paintEvent 以支持自定义图标
void MToggleButton::paintEvent(QPaintEvent *event)
{
    if (m_hasCustomIcons) {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing, true);
        
        const QRect rect = this->rect();
        
        paintHoverElevation(painter, rect);
        paintBackground(painter, rect);
        paintStateLayer(painter, rect);
        paintBorder(painter, rect);
        paintFocusIndicator(painter, rect);
        paintRipple(painter, rect);
        paintIcon(painter);
    } else {
        MButtonBase::paintEvent(event);
    }
}