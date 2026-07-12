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
}

void MCard::initCard()
{
    setCornerRadius(12.0);
    setElevation(1.0);
    setShadowEnabled(true);
    
    setAttribute(Qt::WA_StyledBackground, true);
    setAttribute(Qt::WA_TranslucentBackground, true);
    setAttribute(Qt::WA_OpaquePaintEvent, false);
    
    setupLayout();
    updateCardAppearance();
    
    if (!m_titleText.isEmpty())
        setTitle(m_titleText);
        
    setAutoResize(true);
    updateMask();
}

void MCard::setupLayout()
{
    m_mainLayout = new QVBoxLayout(this);
    updateLayoutMargins();
    m_mainLayout->setSpacing(8);

    // ---- 头部 ----
    m_headerWidget = new QWidget(this);
    m_headerWidget->setAttribute(Qt::WA_TranslucentBackground);
    m_headerWidget->setStyleSheet("background: transparent;");
    m_headerWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    
    m_headerLayout = new QHBoxLayout(m_headerWidget);
    m_headerLayout->setContentsMargins(0, 0, 0, 0);
    m_headerLayout->setSpacing(8);

    QVBoxLayout *textLayout = new QVBoxLayout();
    textLayout->setSpacing(2);
    textLayout->setContentsMargins(0, 0, 0, 0);

    m_titleLabel = new QLabel(this);
    QFont titleFont = m_titleLabel->font();
    titleFont.setWeight(QFont::Medium);
    titleFont.setPointSize(10);
    m_titleLabel->setFont(titleFont);
    m_titleLabel->setWordWrap(true);
    m_titleLabel->setAttribute(Qt::WA_TranslucentBackground);
    m_titleLabel->setStyleSheet("background: transparent;");
    m_titleLabel->setContentsMargins(0, 0, 0, 0);
    textLayout->addWidget(m_titleLabel);

    m_subtitleLabel = new QLabel(this);
    QFont subtitleFont = m_subtitleLabel->font();
    subtitleFont.setPointSize(9);
    m_subtitleLabel->setFont(subtitleFont);
    m_subtitleLabel->setWordWrap(true);
    m_subtitleLabel->hide();
    m_subtitleLabel->setAttribute(Qt::WA_TranslucentBackground);
    m_subtitleLabel->setStyleSheet("background: transparent;");
    m_subtitleLabel->setContentsMargins(0, 0, 0, 0);
    textLayout->addWidget(m_subtitleLabel);

    m_headerLayout->addLayout(textLayout);
    m_headerLayout->addStretch();
    m_mainLayout->addWidget(m_headerWidget);

    // ---- 内容容器 ----
    m_contentContainer = new QWidget(this);
    m_contentContainer->setObjectName("contentContainer");
    m_contentContainer->setAttribute(Qt::WA_TranslucentBackground);
    m_contentContainer->setStyleSheet("background: transparent;");
    m_contentContainer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QVBoxLayout *contentLayout = new QVBoxLayout(m_contentContainer);
    contentLayout->setContentsMargins(0, 0, 0, 0);
    contentLayout->setSpacing(0);
    m_mainLayout->addWidget(m_contentContainer, 1);

    if (m_contentWidget) {
        m_contentWidget->setAttribute(Qt::WA_TranslucentBackground);
        m_contentWidget->setStyleSheet("background: transparent;");
        contentLayout->addWidget(m_contentWidget, 1);
    }
}

void MCard::updateMask()
{
    QRect contentRect = getContentRect();
    
    int extraMargin = 20;
    QRect maskRect = contentRect.adjusted(-extraMargin, -extraMargin, extraMargin, extraMargin);
    maskRect = maskRect.intersected(rect());
    
    if (maskRect.width() > 0 && maskRect.height() > 0) {
        QPainterPath path;
        qreal radius = qMin(m_cornerRadius + 2, qMin(maskRect.width(), maskRect.height()) / 2.0);
        path.addRoundedRect(maskRect, radius, radius);
        QRegion region = QRegion(path.toFillPolygon().toPolygon());
        setMask(region);
    }
}

void MCard::resizeEvent(QResizeEvent *event)
{
    MWidgetBase::resizeEvent(event);
    updateMask();
}

void MCard::setCornerRadius(qreal radius)
{
    MWidgetBase::setCornerRadius(radius);
    updateMask();
}

void MCard::setShadowEnabled(bool enabled)
{
    MWidgetBase::setShadowEnabled(enabled);
    updateLayoutMargins();
    updateMask();
}

void MCard::updateLayoutMargins()
{
    if (m_mainLayout) {
        int shadowMargin = m_shadowEnabled ? SHADOW_MARGIN : 0;
        m_mainLayout->setContentsMargins(
            shadowMargin + 0,
            shadowMargin + 0,
            shadowMargin + 16,
            shadowMargin + 12
        );
    }
}

// ---------- 其余方法 ----------

void MCard::setAutoResize(bool autoResize)
{
    m_autoResize = autoResize;
    updateSizePolicy();
    updateGeometry();
}

void MCard::updateSizePolicy()
{
    if (m_autoResize) {
        setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        setFixedSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
        // 【修改】确保最小高度为 MIN_CARD_HEIGHT (80)
        setMinimumSize(MIN_CARD_WIDTH, MIN_CARD_HEIGHT);
    } else {
        setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    }
}

QSize MCard::sizeHint() const
{
    if (m_autoResize) {
        QSize contentSize = contentSizeHint();
        int shadowMargin = m_shadowEnabled ? SHADOW_MARGIN : 0;
        contentSize += QSize(shadowMargin * 2 + 16, shadowMargin * 2 + 12);
        
        // 【修改】强制应用最小尺寸限制，高度至少为 MIN_CARD_HEIGHT (80)
        contentSize.setWidth(qMax(MIN_CARD_WIDTH, contentSize.width()));
        contentSize.setHeight(qMax(MIN_CARD_HEIGHT, contentSize.height()));
        return contentSize;
    }
    return QWidget::sizeHint();
}

QSize MCard::minimumSizeHint() const
{
    if (m_autoResize)
        // 【修改】最小高度为 MIN_CARD_HEIGHT (80)
        return QSize(MIN_CARD_WIDTH, MIN_CARD_HEIGHT);
    return QWidget::minimumSizeHint();
}

QSize MCard::contentSizeHint() const
{
    QSize size(0, 0);
    if (!m_titleText.isEmpty()) {
        QSize titleSize = m_titleLabel->sizeHint();
        size.setWidth(qMax(size.width(), titleSize.width()));
        size.setHeight(size.height() + titleSize.height());
    }
    if (!m_subtitleText.isEmpty()) {
        QSize subtitleSize = m_subtitleLabel->sizeHint();
        size.setWidth(qMax(size.width(), subtitleSize.width()));
        size.setHeight(size.height() + subtitleSize.height() + 2);
    }
    if (m_contentWidget) {
        QSize contentSize = m_contentWidget->sizeHint();
        size.setWidth(qMax(size.width(), contentSize.width()));
        size.setHeight(size.height() + contentSize.height() + 8);
    }
    
    if (size.isEmpty())
        size = QSize(150, 80);
    
    // 【修改】确保内容大小至少为最小尺寸
    size.setWidth(qMax(MIN_CARD_WIDTH, size.width()));
    size.setHeight(qMax(MIN_CARD_HEIGHT, size.height()));
    return size;
}

void MCard::setTitle(const QString &title)
{
    m_titleText = title;
    m_titleLabel->setText(title);
    m_titleLabel->setVisible(!title.isEmpty());
    updateCardAppearance();
    if (m_autoResize) updateGeometry();
}

void MCard::setSubtitle(const QString &subtitle)
{
    m_subtitleText = subtitle;
    m_subtitleLabel->setText(subtitle);
    m_subtitleLabel->setVisible(!subtitle.isEmpty());
    updateCardAppearance();
    if (m_autoResize) updateGeometry();
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
        widget->setAttribute(Qt::WA_TranslucentBackground);
        widget->setStyleSheet("background: transparent;");
        widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        static_cast<QVBoxLayout*>(m_contentContainer->layout())->addWidget(widget, 1);
        widget->installEventFilter(this);
    }
    updateCardAppearance();
    if (m_autoResize) updateGeometry();
}

void MCard::setCheckable(bool checkable)
{
    m_checkable = checkable;
    if (!checkable) setChecked(false);
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

// === 尺寸控制 ===

void MCard::setFixedWidth(int width)
{
    m_autoResize = false;
    width = qMax(MIN_CARD_WIDTH, width);
    QWidget::setFixedWidth(width);
}

void MCard::setFixedHeight(int height)
{
    m_autoResize = false;
    // 【修改】强制不能低于 MIN_CARD_HEIGHT (80)
    height = qMax(MIN_CARD_HEIGHT, height);
    QWidget::setFixedHeight(height);
}

void MCard::setMaximumWidth(int width)
{
    m_autoResize = false;
    width = qMax(MIN_CARD_WIDTH, width);
    QWidget::setMaximumWidth(width);
}

void MCard::setMaximumHeight(int height)
{
    m_autoResize = false;
    // 【修改】强制不能低于 MIN_CARD_HEIGHT (80)
    height = qMax(MIN_CARD_HEIGHT, height);
    QWidget::setMaximumHeight(height);
}

void MCard::setMinimumWidth(int width)
{
    m_autoResize = false;
    width = qMax(MIN_CARD_WIDTH, width);
    QWidget::setMinimumWidth(width);
}

void MCard::setMinimumHeight(int height)
{
    m_autoResize = false;
    // 【修改】强制不能低于 MIN_CARD_HEIGHT (80)
    height = qMax(MIN_CARD_HEIGHT, height);
    QWidget::setMinimumHeight(height);
}

// ---------- 绘制 ----------
void MCard::drawForeground(QPainter *painter, const QRect &rect)
{
    if (!m_outlined) return;
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

// ---------- 事件 ----------
void MCard::onHoverEnter()
{
    MWidgetBase::onHoverEnter();
    emit entered();
    if (m_clickable && !m_outlined) {
        if (m_elevated) animateElevation(6.0, 150);
        else animateElevation(2.0, 150);
    }
    updateCardAppearance();
}

void MCard::onHoverLeave()
{
    MWidgetBase::onHoverLeave();
    emit left();
    if (m_clickable && !m_outlined) {
        if (m_elevated) animateElevation(4.0, 150);
        else animateElevation(1.0, 150);
    }
    updateCardAppearance();
}

void MCard::onPress()
{
    MWidgetBase::onPress();
    emit pressed();
    if (m_clickable) {
        if (m_elevated) animateElevation(2.0, 100);
        else animateElevation(0.0, 100);
    }
    updateCardAppearance();
}

void MCard::onRelease()
{
    MWidgetBase::onRelease();
    emit released();
    if (m_clickable) {
        if (m_elevated && m_hovered) animateElevation(6.0, 150);
        else if (m_elevated) animateElevation(4.0, 150);
        else if (m_hovered && !m_outlined) animateElevation(2.0, 150);
        else animateElevation(1.0, 150);
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
            if (m_checkable) setChecked(!m_checked);
        }
    }
    MWidgetBase::mouseReleaseEvent(event);
}

void MCard::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (m_clickable) emit clicked();
    MWidgetBase::mouseDoubleClickEvent(event);
}

// ---------- 样式更新 ----------
void MCard::updateCardAppearance()
{
    updateStyle();
    update();
}

void MCard::updateStyle()
{
    QColor textColor = m_checked ? getPrimaryColor() : getOnSurfaceColor();
    QPalette titlePal = m_titleLabel->palette();
    titlePal.setColor(QPalette::WindowText, textColor);
    m_titleLabel->setPalette(titlePal);

    QPalette subtitlePal = m_subtitleLabel->palette();
    subtitlePal.setColor(QPalette::WindowText, getOnSurfaceVariant());
    m_subtitleLabel->setPalette(subtitlePal);

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
        if (m_checkable) setChecked(!m_checked);
    }
}