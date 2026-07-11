#include "QToast.h"
#include <QPainter>
#include <QPainterPath>
#include <QScreen>
#include <QApplication>
#include <QGuiApplication>
#include <QWindow>
#include <QDebug>

#ifdef Q_OS_WIN
#include <windows.h>
#endif

#ifdef Q_OS_MAC
#import <objc/objc-runtime.h>
#endif

QToast::QToast(QWidget *parent)
    : QWidget(parent)
    , m_duration(LENGTH_SHORT)
    , m_position(POSITION_BOTTOM)
    , m_backgroundColor(40, 40, 40, 230)
    , m_textColor(Qt::white)
    , m_cornerRadius(25)
    , m_hMargin(40)
    , m_vMargin(15)
    , m_maxWidth(0)
    , m_fontSize(14)
    , m_bottomMarginRatio(0.08)
    , m_minBottomMargin(0)
    , m_isShowing(false)
    , m_isUpdating(false)
{
    initUI();
}

QToast::~QToast()
{
}

void QToast::initUI()
{
    // 设置窗口标志 - 关键修改
    Qt::WindowFlags flags = Qt::FramelessWindowHint | 
                            Qt::Tool |                    // 使用 Tool 代替 Popup
                            Qt::WindowStaysOnTopHint;
    
#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
    // Qt 5.15+ 原生支持输入穿透
    flags |= Qt::WindowTransparentForInput;
#endif
    
    setWindowFlags(flags);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_ShowWithoutActivating);
    setFocusPolicy(Qt::NoFocus);  // 不获取焦点
    
    // 对于 Qt 5.14 及以下，WA_TransparentForMouseEvents 作为辅助
#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
    setAttribute(Qt::WA_TransparentForMouseEvents);
#endif
    
    // 创建标签
    m_label = new QLabel(this);
    m_label->setAlignment(Qt::AlignCenter);
    m_label->setWordWrap(true);
    
    QString styleSheet = QString("QLabel { color: %1; font-size: %2px; }")
                        .arg(m_textColor.name())
                        .arg(m_fontSize);
    m_label->setStyleSheet(styleSheet);
    
    // 透明度效果
    m_opacityEffect = new QGraphicsOpacityEffect(this);
    m_opacityEffect->setOpacity(0.0);
    setGraphicsEffect(m_opacityEffect);
    
    // 透明度动画
    m_opacityAnimation = new QPropertyAnimation(this, "opacity");
    m_opacityAnimation->setDuration(300);
    m_opacityAnimation->setStartValue(0.0);
    m_opacityAnimation->setEndValue(1.0);
    
    // 隐藏定时器
    m_hideTimer = new QTimer(this);
    m_hideTimer->setSingleShot(true);
    
    connect(m_opacityAnimation, &QPropertyAnimation::finished, 
            this, &QToast::onAnimationFinished);
    connect(m_hideTimer, &QTimer::timeout, this, [this]() {
        if (m_isShowing) {
            m_opacityAnimation->setStartValue(1.0);
            m_opacityAnimation->setEndValue(0.0);
            m_opacityAnimation->start();
        }
    });
    
    // 设置平台特定的穿透支持
    setupPlatformSpecificTransparency();
    
    // 默认不显示
    hide();
}

void QToast::setupPlatformSpecificTransparency()
{
#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
    // 对于旧版本 Qt，需要额外的平台特定处理
    // 这将在 nativeEvent 中处理
#endif
}

bool QToast::nativeEvent(const QByteArray &eventType, void *message, qintptr *result)
{
#ifdef Q_OS_WIN
    // Windows 平台：让鼠标事件穿透
    if (eventType == "windows_generic_MSG") {
        MSG *msg = static_cast<MSG *>(message);
        if (msg->message == WM_NCHITTEST) {
            // 返回 HTTRANSPARENT 让点击穿透
            *result = HTTRANSPARENT;
            return true;
        }
    }
#elif defined(Q_OS_MAC)
    // macOS 平台：对于 Qt 5.15+ 使用 Qt::WindowTransparentForInput 已经足够
    // 这里不需要额外的处理
    Q_UNUSED(eventType);
    Q_UNUSED(message);
    Q_UNUSED(result);
#endif
    return QWidget::nativeEvent(eventType, message, result);
}

void QToast::setText(const QString &text)
{
    m_text = text;
    m_label->setText(text);
    updateSize();
}

void QToast::updateSize()
{
    if (m_isUpdating || m_text.isEmpty()) {
        return;
    }
    
    m_isUpdating = true;
    
    QScreen *screen = QGuiApplication::primaryScreen();
    if (!screen) {
        m_isUpdating = false;
        return;
    }
    
    int screenWidth = screen->availableGeometry().width();
    int screenHeight = screen->availableGeometry().height();
    
    int maxWidth;
    if (m_maxWidth > 0) {
        maxWidth = qMin(m_maxWidth, screenWidth - 40);
    } else {
        maxWidth = qMin(static_cast<int>(screenWidth * 0.8), 600);
        maxWidth = qMax(maxWidth, 200);
        maxWidth = qMin(maxWidth, screenWidth - 40);
    }
    
    QFont font = m_label->font();
    font.setPointSize(m_fontSize);
    m_label->setFont(font);
    
    QFontMetrics fm(font);
    QRect textRect = fm.boundingRect(QRect(0, 0, maxWidth, 2000), 
                                     Qt::TextWordWrap | Qt::AlignCenter, 
                                     m_text);
    
    int textWidth = textRect.width();
    int textHeight = textRect.height();
    
    if (textWidth < maxWidth) {
        QRect exactRect = fm.boundingRect(QRect(0, 0, textWidth + 10, 2000), 
                                         Qt::TextWordWrap | Qt::AlignCenter, 
                                         m_text);
        textWidth = exactRect.width();
        textHeight = exactRect.height();
    }
    
    textWidth = qMin(textWidth, maxWidth);
    
    if (textWidth < 50) {
        textWidth = 50;
        textHeight = 30;
    }
    
    int finalWidth = textWidth + m_hMargin * 2;
    int finalHeight = textHeight + m_vMargin * 2;
    
    finalWidth = qMax(finalWidth, 100);
    finalHeight = qMax(finalHeight, 40);
    
    finalWidth = qMin(finalWidth, screenWidth - 20);
    finalHeight = qMin(finalHeight, screenHeight - 100);
    
    int labelX = (finalWidth - textWidth) / 2;
    int labelY = (finalHeight - textHeight) / 2;
    m_label->setGeometry(labelX, labelY, textWidth, textHeight);
    
    resize(finalWidth, finalHeight);
    
    m_isUpdating = false;
}

void QToast::updatePosition()
{
    QScreen *screen = QGuiApplication::primaryScreen();
    if (!screen) {
        return;
    }
    
    QRect screenGeometry = screen->availableGeometry();
    int x = (screenGeometry.width() - width()) / 2;
    int y;
    
    switch (m_position) {
    case POSITION_TOP:
        y = static_cast<int>(screenGeometry.height() * 0.05);
        break;
    case POSITION_CENTER:
        y = (screenGeometry.height() - height()) / 2;
        break;
    case POSITION_BOTTOM:
    default: {
        int maxMargin = static_cast<int>(screenGeometry.height() * 0.3);
        int minMargin = m_minBottomMargin;
        int bottomMargin = static_cast<int>(screenGeometry.height() * m_bottomMarginRatio);
        bottomMargin = qBound(minMargin, bottomMargin, maxMargin);
        y = screenGeometry.height() - height() - bottomMargin;
        break;
    }
    }
    
    x = qMax(10, x);
    y = qMax(10, y);
    
    move(x, y);
}

void QToast::setDuration(int ms)
{
    m_duration = ms;
}

void QToast::setPosition(ToastPosition position)
{
    m_position = position;
    if (m_isShowing) {
        updatePosition();
    }
}

void QToast::setBackgroundColor(const QColor &color)
{
    m_backgroundColor = color;
    update();
}

void QToast::setTextColor(const QColor &color)
{
    m_textColor = color;
    QString styleSheet = QString("QLabel { color: %1; font-size: %2px; }")
                        .arg(color.name())
                        .arg(m_fontSize);
    m_label->setStyleSheet(styleSheet);
}

void QToast::setCornerRadius(int radius)
{
    m_cornerRadius = radius;
    update();
}

void QToast::setMargins(int horizontal, int vertical)
{
    m_hMargin = horizontal;
    m_vMargin = vertical;
    updateSize();
}

void QToast::setMaxWidth(int maxWidth)
{
    m_maxWidth = maxWidth;
    updateSize();
}

void QToast::setFontSize(int size)
{
    m_fontSize = size;
    QString styleSheet = QString("QLabel { color: %1; font-size: %2px; }")
                        .arg(m_textColor.name())
                        .arg(m_fontSize);
    m_label->setStyleSheet(styleSheet);
    updateSize();
}

void QToast::setBottomMarginRatio(qreal ratio)
{
    m_bottomMarginRatio = qBound(0.0, ratio, 0.5);
    if (m_isShowing) {
        updatePosition();
    }
}

void QToast::setMinBottomMargin(int minMargin)
{
    m_minBottomMargin = qMax(0, minMargin);
    if (m_isShowing) {
        updatePosition();
    }
}

void QToast::show()
{
    if (m_isShowing) {
        return;
    }
    
    updateSize();
    updatePosition();
    
    m_isShowing = true;
    QWidget::show();
    
    m_opacityAnimation->setStartValue(0.0);
    m_opacityAnimation->setEndValue(1.0);
    m_opacityAnimation->start();
    
    m_hideTimer->start(m_duration);
}

void QToast::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    QPainterPath path;
    path.addRoundedRect(rect(), m_cornerRadius, m_cornerRadius);
    
    painter.fillPath(path, m_backgroundColor);
}

void QToast::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    m_opacityEffect->setOpacity(0.0);
}

void QToast::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    update();
}

void QToast::onAnimationFinished()
{
    if (m_opacityAnimation->endValue() == 0.0) {
        hide();
        m_isShowing = false;
        QTimer::singleShot(0, this, &QObject::deleteLater);
    }
}

qreal QToast::opacity() const
{
    return m_opacityEffect ? m_opacityEffect->opacity() : 0.0;
}

void QToast::setOpacity(qreal opacity)
{
    if (m_opacityEffect) {
        m_opacityEffect->setOpacity(opacity);
    }
}

void QToast::makeToast(QWidget *parent, 
                       const QString &text, 
                       int duration,
                       ToastPosition position)
{
    QToast *toast = new QToast(parent);
    toast->setText(text);
    toast->setDuration(duration);
    toast->setPosition(position);
    toast->show();
}

void QToast::makeToast(QWidget *parent, 
                       const QString &text, 
                       int duration,
                       ToastPosition position,
                       int fontSize,
                       const QColor &backgroundColor,
                       const QColor &textColor)
{
    QToast *toast = new QToast(parent);
    toast->setText(text);
    toast->setDuration(duration);
    toast->setPosition(position);
    toast->setFontSize(fontSize);
    toast->setBackgroundColor(backgroundColor);
    toast->setTextColor(textColor);
    toast->show();
}