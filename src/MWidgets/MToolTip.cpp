#include "MToolTip.h"
#include <QScreen>
#include <QApplication>
#include <QStyleOption>
#include <QTimer>
#include <QPropertyAnimation>
#include <QPainterPath>
#include <QDebug>

MToolTip::MToolTip(QWidget* parent)
    : QWidget(parent)
    , m_timeout(DEFAULT_TIMEOUT)
    , m_opacity(0.0)
    , m_theme(MTheme::instance())
{
    // 窗口标志 - 类似QML的z:999和透明背景
    setWindowFlags(Qt::ToolTip | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_ShowWithoutActivating);
    setAttribute(Qt::WA_DeleteOnClose);
    
    setFocusPolicy(Qt::NoFocus);
    
    // 初始化UI
    initUI();
    
    // 初始化动画 - 对应QML的NumberAnimation
    m_showAnim = new QPropertyAnimation(this, "opacity", this);
    m_showAnim->setDuration(SHOW_DURATION);
    m_showAnim->setEasingCurve(QEasingCurve::OutQuad);
    m_showAnim->setEndValue(1.0);
    
    m_hideAnim = new QPropertyAnimation(this, "opacity", this);
    m_hideAnim->setDuration(HIDE_DURATION);
    m_hideAnim->setEasingCurve(QEasingCurve::InQuad);
    m_hideAnim->setEndValue(0.0);
    connect(m_hideAnim, &QPropertyAnimation::finished, this, &QWidget::hide);
    
    // 自动隐藏定时器 - 对应QML的Timer
    m_hideTimer = new QTimer(this);
    m_hideTimer->setSingleShot(true);
    connect(m_hideTimer, &QTimer::timeout, this, &MToolTip::close);
    
    // 初始状态
    setVisible(false);
}

void MToolTip::initUI()
{
    m_label = new QLabel(this);
    m_label->setAlignment(Qt::AlignCenter);
    m_label->setWordWrap(true);
    m_label->setText(m_text);
    
    // 应用主题样式
    updateThemeColors();
    
    // 应用字体
    QFont font = getTextFont();
    m_label->setFont(font);
}

void MToolTip::setText(const QString& text)
{
    m_text = text;
    if (m_label) {
        m_label->setText(text);
    }
    updateSizeAndPosition();
    update();
}

void MToolTip::setTimeout(int ms)
{
    m_timeout = qMax(0, ms);
}

void MToolTip::open()
{
    if (m_text.isEmpty()) {
        qWarning() << "MToolTip: Cannot open with empty text";
        return;
    }
    
    // 停止所有正在进行的动画和定时器
    m_showAnim->stop();
    m_hideAnim->stop();
    m_hideTimer->stop();
    
    // 更新位置和大小
    updateSizeAndPosition();
    
    // 确保主题颜色最新
    updateThemeColors();
    
    // 显示并播放淡入动画 - 对应QML的showAnim.restart()
    setOpacity(0.0);
    show();
    m_showAnim->start();
    
    // 启动自动隐藏定时器 - 对应QML的hideTimer
    if (m_timeout > 0) {
        m_hideTimer->setInterval(m_timeout);
        m_hideTimer->start();
    }
}

void MToolTip::close()
{
    if (isVisible()) {
        m_hideTimer->stop();
        m_showAnim->stop();
        // 播放淡出动画 - 对应QML的hideAnim.restart()
        m_hideAnim->start();
    }
}

void MToolTip::setOpacity(qreal opacity)
{
    m_opacity = qBound(0.0, opacity, 1.0);
    update(); // 触发重绘
}

void MToolTip::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event)
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    // 应用透明度 - 对应QML的opacity属性
    painter.setOpacity(m_opacity);
    
    // 绘制背景圆角矩形 - 对应QML的Rectangle
    QPainterPath path;
    QRectF rect = this->rect();
    int radius = getCornerRadius();
    path.addRoundedRect(rect, radius, radius);
    
    // 填充背景色 - 对应QML的color: _colors.inverseSurface
    QColor bgColor = getSurfaceColor();
    bgColor.setAlphaF(0.9); // 对应QML的opacity: 0.9
    painter.fillPath(path, bgColor);
    
    // 绘制边框或阴影（可选）
    // 这里保持简单，与QML版本一致
}

void MToolTip::showEvent(QShowEvent* event)
{
    QWidget::showEvent(event);
    raise(); // 确保在最上层 - 对应QML的z:999
}

void MToolTip::hideEvent(QHideEvent* event)
{
    QWidget::hideEvent(event);
}

void MToolTip::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);
    // 更新label位置
    if (m_label) {
        QRect rect = this->rect();
        m_label->setGeometry(
            PADDING_HORIZONTAL, 
            PADDING_VERTICAL,
            rect.width() - PADDING_HORIZONTAL * 2,
            rect.height() - PADDING_VERTICAL * 2
        );
    }
}

void MToolTip::updateSizeAndPosition()
{
    if (!parentWidget()) {
        qWarning() << "MToolTip: No parent widget set";
        return;
    }
    
    // 获取父窗口几何信息
    QRect parentRect = parentWidget()->rect();
    QPoint parentCenter = parentRect.center();
    
    // 更新label大小约束 - 对应QML的width: Math.min(implicitWidth, parent.width - 32)
    if (m_label) {
        int maxWidth = parentRect.width() * MAX_WIDTH_RATIO / 100;
        int maxHeight = parentRect.height() * MAX_WIDTH_RATIO / 100;
        m_label->setMaximumWidth(maxWidth);
        m_label->setMaximumHeight(maxHeight);
        m_label->adjustSize();
    }
    
    // 计算tooltip大小 - 对应QML的implicitWidth/Height
    int labelWidth = m_label ? m_label->sizeHint().width() : 0;
    int labelHeight = m_label ? m_label->sizeHint().height() : 0;
    
    int tooltipWidth = labelWidth + PADDING_HORIZONTAL * 2;
    int tooltipHeight = labelHeight + PADDING_VERTICAL * 2;
    
    // 确保最小尺寸
    tooltipWidth = qMax(tooltipWidth, 50);
    tooltipHeight = qMax(tooltipHeight, 30);
    
    // 计算位置 - 对应QML的x: (parent.width - width) / 2, y: (parent.height - height) / 2
    QPoint globalPos = parentWidget()->mapToGlobal(parentCenter);
    QRect screenRect = QApplication::primaryScreen()->availableGeometry();
    
    int x = globalPos.x() - tooltipWidth / 2;
    int y = globalPos.y() - tooltipHeight / 2;
    
    // 调整位置以保持在屏幕内
    const int margin = 10;
    x = qMax(screenRect.left() + margin, qMin(x, screenRect.right() - tooltipWidth - margin));
    y = qMax(screenRect.top() + margin, qMin(y, screenRect.bottom() - tooltipHeight - margin));
    
    // 设置几何
    setGeometry(x, y, tooltipWidth, tooltipHeight);
}

void MToolTip::updateThemeColors()
{
    if (!m_label || !m_theme) {
        return;
    }
    
    // 更新文本颜色 - 对应QML的color: _colors.inverseOnSurface
    QPalette pal = m_label->palette();
    pal.setColor(QPalette::WindowText, getOnSurfaceColor());
    m_label->setPalette(pal);
    
    // 更新字体 - 对应QML的font.*
    QFont font = getTextFont();
    m_label->setFont(font);
}

// ----- 主题辅助方法 -----

QColor MToolTip::getSurfaceColor() const
{
    if (m_theme) {
        return m_theme->color().inverseSurface;
    }
    // 回退颜色 - MD3暗色主题的默认值
    return QColor(0x1C, 0x1B, 0x1F);
}

QColor MToolTip::getOnSurfaceColor() const
{
    if (m_theme) {
        return m_theme->color().inverseOnSurface;
    }
    // 回退颜色
    return QColor(0xE6, 0xE1, 0xE5);
}

QFont MToolTip::getTextFont() const
{
    QFont font;
    if (m_theme) {
        // 使用bodySmall排版 - 对应QML的_typography.bodySmall
        auto bodySmall = m_theme->typography().bodySmall;
        font.setFamily(bodySmall.family);
        font.setPixelSize(bodySmall.size);
        font.setWeight(static_cast<QFont::Weight>(bodySmall.weight));
        font.setLetterSpacing(QFont::AbsoluteSpacing, bodySmall.tracking);
    } else {
        // 回退字体
        font.setFamily("Roboto, sans-serif");
        font.setPixelSize(12);
        font.setWeight(QFont::Normal);
    }
    return font;
}

int MToolTip::getCornerRadius() const
{
    if (m_theme) {
        // 使用cornerExtraSmall - 对应QML的radius: 4 // MD3 Extra Small
        return m_theme->shape().cornerExtraSmall;
    }
    return CORNER_RADIUS;
}