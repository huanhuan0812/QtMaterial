#include "MCard.h"
#include <QPainter>
#include <QPainterPath>
#include <QStyleOption>
#include <QDebug>

MCard::MCard(QWidget *parent)
    : MWidgetBase(parent)
{
    initCard();
}

MCard::MCard(const QString &title, QWidget *parent)
    : MWidgetBase(parent)
    , m_titleText(title)
{
    initCard();
}

MCard::~MCard()
{
    // 清理工作由Qt父子机制处理
}

void MCard::initCard()
{
    // 设置默认属性
    setCornerRadius(12.0);
    setElevation(1.0);
    setShadowEnabled(true);
    
    // 设置布局
    setupLayout();
    
    // 设置默认样式
    updateCardAppearance();
    
    // 初始化标题
    if (!m_titleText.isEmpty()) {
        setTitle(m_titleText);
    }
    
    // 默认自适应
    setAutoResize(true);
}

void MCard::setupLayout()
{
    // 主布局
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setContentsMargins(16, 12, 16, 12);
    m_mainLayout->setSpacing(8);
    
    // 头部布局
    m_headerWidget = new QWidget(this);
    m_headerWidget->setObjectName("headerWidget");
    m_headerLayout = new QHBoxLayout(m_headerWidget);
    m_headerLayout->setContentsMargins(0, 0, 0, 0);
    m_headerLayout->setSpacing(12);
    
    // 标题和副标题的垂直布局
    QVBoxLayout *textLayout = new QVBoxLayout();
    textLayout->setSpacing(2);
    textLayout->setContentsMargins(0, 0, 0, 0);
    
    // 标题标签
    m_titleLabel = new QLabel(this);
    QFont titleFont = m_titleLabel->font();
    titleFont.setWeight(QFont::Medium);
    titleFont.setPointSize(10);
    m_titleLabel->setFont(titleFont);
    m_titleLabel->setWordWrap(true);
    textLayout->addWidget(m_titleLabel);
    
    // 副标题标签
    m_subtitleLabel = new QLabel(this);
    QFont subtitleFont = m_subtitleLabel->font();
    subtitleFont.setPointSize(9);
    m_subtitleLabel->setFont(subtitleFont);
    m_subtitleLabel->setWordWrap(true);
    m_subtitleLabel->hide();
    textLayout->addWidget(m_subtitleLabel);
    
    m_headerLayout->addLayout(textLayout, 1);
    m_headerLayout->addStretch();
    
    // 添加到主布局
    m_mainLayout->addWidget(m_headerWidget);
    
    // 内容容器
    m_contentContainer = new QWidget(this);
    m_contentContainer->setObjectName("contentContainer");
    QVBoxLayout *contentLayout = new QVBoxLayout(m_contentContainer);
    contentLayout->setContentsMargins(0, 4, 0, 0);
    contentLayout->setSpacing(0);
    m_mainLayout->addWidget(m_contentContainer);
    
    // 如果有内容控件，添加到容器
    if (m_contentWidget) {
        contentLayout->addWidget(m_contentWidget);
    }
}

void MCard::setAutoResize(bool autoResize)
{
    m_autoResize = autoResize;
    updateSizePolicy();
    updateGeometry();
}

void MCard::updateSizePolicy()
{
    if (m_autoResize) {
        // 自适应模式：根据内容调整大小
        setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        // 清除固定大小
        setFixedSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
    } else {
        // 手动控制模式
        setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    }
}

QSize MCard::sizeHint() const
{
    if (m_autoResize) {
        // 计算内容大小
        QSize contentSize = contentSizeHint();
        
        // 加上边距
        int margin = m_shadowEnabled ? SHADOW_MARGIN : 0;
        contentSize += QSize(margin * 2, margin * 2);
        
        // 加上内边距
        contentSize += QSize(32, 24); // 左右16，上下12
        
        return contentSize;
    }
    
    return QWidget::sizeHint();
}

QSize MCard::minimumSizeHint() const
{
    if (m_autoResize) {
        QSize minSize = sizeHint();
        // 设置最小尺寸
        minSize.setWidth(qMax(100, minSize.width()));
        minSize.setHeight(qMax(60, minSize.height()));
        return minSize;
    }
    
    return QWidget::minimumSizeHint();
}

QSize MCard::contentSizeHint() const
{
    QSize size(0, 0);
    
    // 计算标题大小
    if (!m_titleText.isEmpty()) {
        QSize titleSize = m_titleLabel->sizeHint();
        size.setWidth(qMax(size.width(), titleSize.width()));
        size.setHeight(size.height() + titleSize.height());
    }
    
    // 计算副标题大小
    if (!m_subtitleText.isEmpty()) {
        QSize subtitleSize = m_subtitleLabel->sizeHint();
        size.setWidth(qMax(size.width(), subtitleSize.width()));
        size.setHeight(size.height() + subtitleSize.height() + 2);
    }
    
    // 计算内容控件大小
    if (m_contentWidget) {
        QSize contentSize = m_contentWidget->sizeHint();
        size.setWidth(qMax(size.width(), contentSize.width()));
        size.setHeight(size.height() + contentSize.height() + 4);
    }
    
    // 如果没有内容，给一个默认大小
    if (size.isEmpty()) {
        size = QSize(150, 60);
    }
    
    return size;
}

void MCard::setTitle(const QString &title)
{
    m_titleText = title;
    m_titleLabel->setText(title);
    m_titleLabel->setVisible(!title.isEmpty());
    updateCardAppearance();
    if (m_autoResize) {
        updateGeometry();
    }
}

void MCard::setSubtitle(const QString &subtitle)
{
    m_subtitleText = subtitle;
    m_subtitleLabel->setText(subtitle);
    m_subtitleLabel->setVisible(!subtitle.isEmpty());
    updateCardAppearance();
    if (m_autoResize) {
        updateGeometry();
    }
}

void MCard::setContentWidget(QWidget *widget)
{
    if (m_contentWidget) {
        m_contentContainer->layout()->removeWidget(m_contentWidget);
        m_contentWidget->deleteLater();
    }
    
    m_contentWidget = widget;
    if (widget) {
        widget->setParent(m_contentContainer);
        static_cast<QVBoxLayout*>(m_contentContainer->layout())->addWidget(widget);
        // 监听内容变化
        widget->installEventFilter(this);
    }
    updateCardAppearance();
    if (m_autoResize) {
        updateGeometry();
    }
}

void MCard::setCheckable(bool checkable)
{
    m_checkable = checkable;
    if (!checkable) {
        setChecked(false);
    }
    updateCardAppearance();
}

void MCard::setChecked(bool checked)
{
    if (m_checkable && m_checked != checked) {
        m_checked = checked;
        updateCardAppearance();
        emit checkedChanged(checked);
    }
}

void MCard::setClickable(bool clickable)
{
    m_clickable = clickable;
    setCursor(clickable ? Qt::PointingHandCursor : Qt::ArrowCursor);
    updateCardAppearance();
}

void MCard::setOutlined(bool outlined)
{
    m_outlined = outlined;
    if (outlined) {
        setElevation(0);
        setShadowEnabled(false);
    } else {
        setElevation(1.0);
        setShadowEnabled(true);
    }
    updateCardAppearance();
}

void MCard::setElevated(bool elevated)
{
    m_elevated = elevated;
    if (elevated) {
        setElevation(4.0);
        setShadowEnabled(true);
        setOutlined(false);
    } else {
        setElevation(1.0);
    }
    updateCardAppearance();
}

void MCard::setFixedWidth(int width)
{
    m_autoResize = false;
    QWidget::setFixedWidth(width);
}

void MCard::setFixedHeight(int height)
{
    m_autoResize = false;
    QWidget::setFixedHeight(height);
}

void MCard::setMaximumWidth(int width)
{
    m_autoResize = false;
    QWidget::setMaximumWidth(width);
}

void MCard::setMaximumHeight(int height)
{
    m_autoResize = false;
    QWidget::setMaximumHeight(height);
}

void MCard::setMinimumWidth(int width)
{
    m_autoResize = false;
    QWidget::setMinimumWidth(width);
}

void MCard::setMinimumHeight(int height)
{
    m_autoResize = false;
    QWidget::setMinimumHeight(height);
}

// === 绘制方法 ===

void MCard::drawForeground(QPainter *painter, const QRect &rect)
{
    if (!m_outlined) {
        return;
    }
    
    // 绘制边框
    QPainterPath path;
    path.addRoundedRect(rect, cornerRadius(), cornerRadius());
    
    QPen pen;
    if (m_checked) {
        pen.setColor(getPrimaryColor());
        pen.setWidth(2);
    } else if (m_hovered) {
        pen.setColor(getOutlineColor());
        pen.setWidth(1);
    } else {
        pen.setColor(getOutlineColor());
        pen.setWidth(1);
        pen.setStyle(Qt::DotLine);
    }
    painter->setPen(pen);
    painter->setBrush(Qt::NoBrush);
    painter->drawPath(path);
}

// === 事件处理 ===

void MCard::onHoverEnter()
{
    MWidgetBase::onHoverEnter();
    emit entered();
    
    if (m_clickable && !m_outlined) {
        if (m_elevated) {
            animateElevation(6.0, 150);
        } else {
            animateElevation(2.0, 150);
        }
    }
    updateCardAppearance();
}

void MCard::onHoverLeave()
{
    MWidgetBase::onHoverLeave();
    emit left();
    
    if (m_clickable && !m_outlined) {
        if (m_elevated) {
            animateElevation(4.0, 150);
        } else {
            animateElevation(1.0, 150);
        }
    }
    updateCardAppearance();
}

void MCard::onPress()
{
    MWidgetBase::onPress();
    emit pressed();
    
    if (m_clickable) {
        if (m_elevated) {
            animateElevation(2.0, 100);
        } else {
            animateElevation(0.0, 100);
        }
    }
    updateCardAppearance();
}

void MCard::onRelease()
{
    MWidgetBase::onRelease();
    emit released();
    
    if (m_clickable) {
        if (m_elevated && m_hovered) {
            animateElevation(6.0, 150);
        } else if (m_elevated) {
            animateElevation(4.0, 150);
        } else if (m_hovered && !m_outlined) {
            animateElevation(2.0, 150);
        } else {
            animateElevation(1.0, 150);
        }
    }
    updateCardAppearance();
}

void MCard::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && m_clickable) {
        setPressed(true);
        emit pressed();
    }
    MWidgetBase::mousePressEvent(event);
}

void MCard::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && m_clickable) {
        setPressed(false);
        emit released();
        
        if (rect().contains(event->pos())) {
            emit clicked();
            
            if (m_checkable) {
                setChecked(!m_checked);
            }
        }
    }
    MWidgetBase::mouseReleaseEvent(event);
}

void MCard::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (m_clickable) {
        emit clicked();
    }
    MWidgetBase::mouseDoubleClickEvent(event);
}

// === 辅助方法 ===

void MCard::updateCardAppearance()
{
    updateStyle();
    update();
}

void MCard::updateStyle()
{
    // 更新标题颜色
    QColor textColor = m_checked ? getPrimaryColor() : getOnSurfaceColor();
    QPalette titlePal = m_titleLabel->palette();
    titlePal.setColor(QPalette::WindowText, textColor);
    m_titleLabel->setPalette(titlePal);
    
    // 更新副标题颜色
    QPalette subtitlePal = m_subtitleLabel->palette();
    subtitlePal.setColor(QPalette::WindowText, getOnSurfaceVariant());
    m_subtitleLabel->setPalette(subtitlePal);
    
    // 更新背景色
    if (m_checked && m_checkable) {
        QColor checkedColor = getPrimaryColor();
        checkedColor.setAlpha(30);
        setBackgroundColor(checkedColor);
    } else if (m_hovered && m_clickable && !m_outlined) {
        QColor hoverColor = getOnSurfaceColor();
        hoverColor.setAlpha(20);
        setBackgroundColor(hoverColor);
    } else {
        setBackgroundColor(getCurrentBackgroundColor());
    }
}

void MCard::onContentClicked()
{
    if (m_clickable) {
        emit clicked();
        if (m_checkable) {
            setChecked(!m_checked);
        }
    }
}