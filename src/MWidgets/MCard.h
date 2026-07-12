#ifndef MCARD_H
#define MCARD_H

#include "MWidgetBase.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>

class MCard : public MWidgetBase
{
    Q_OBJECT
    Q_PROPERTY(bool checkable READ isCheckable WRITE setCheckable)
    Q_PROPERTY(bool checked READ isChecked WRITE setChecked NOTIFY checkedChanged)
    Q_PROPERTY(bool clickable READ isClickable WRITE setClickable)

public:
    explicit MCard(QWidget *parent = nullptr);
    explicit MCard(const QString &title, QWidget *parent = nullptr);
    virtual ~MCard();

    // === 内容设置 ===
    void setTitle(const QString &title);
    QString title() const { return m_titleText; }

    void setSubtitle(const QString &subtitle);
    QString subtitle() const { return m_subtitleText; }

    void setContentWidget(QWidget *widget);
    QWidget* contentWidget() const { return m_contentWidget; }

    // === 自适应控制 ===
    void setAutoResize(bool autoResize);
    bool autoResize() const { return m_autoResize; }
    
    // 获取内容推荐大小
    QSize contentSizeHint() const;

    // === 交互模式 ===
    void setCheckable(bool checkable);
    bool isCheckable() const { return m_checkable; }

    void setChecked(bool checked);
    bool isChecked() const { return m_checked; }

    void setClickable(bool clickable);
    bool isClickable() const { return m_clickable; }

    // === 卡片样式 ===
    void setOutlined(bool outlined);
    bool isOutlined() const { return m_outlined; }

    void setElevated(bool elevated);
    bool isElevated() const { return m_elevated; }

    // === 尺寸控制 ===
    void setFixedWidth(int width);
    void setFixedHeight(int height);
    void setMaximumWidth(int width);
    void setMaximumHeight(int height);
    void setMinimumWidth(int width);
    void setMinimumHeight(int height);

signals:
    void clicked();
    void pressed();
    void released();
    void checkedChanged(bool checked);
    void entered();
    void left();

protected:
    // 重写基类方法
    void drawForeground(QPainter *painter, const QRect &rect) override;
    void onHoverEnter() override;
    void onHoverLeave() override;
    void onPress() override;
    void onRelease() override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;

    // 更新卡片外观
    virtual void updateCardAppearance();
    
    // 重新实现大小提示
    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

private:
    void initCard();
    void setupLayout();
    void updateStyle();
    void updateSizePolicy();

private slots:
    void onContentClicked();

private:
    // === UI组件 ===
    QVBoxLayout *m_mainLayout = nullptr;
    QHBoxLayout *m_headerLayout = nullptr;
    QWidget *m_headerWidget = nullptr;
    
    QLabel *m_titleLabel = nullptr;
    QLabel *m_subtitleLabel = nullptr;
    
    QWidget *m_contentWidget = nullptr;
    QWidget *m_contentContainer = nullptr;

    // === 内容数据 ===
    QString m_titleText;
    QString m_subtitleText;
    
    // === 交互状态 ===
    bool m_checkable = false;
    bool m_checked = false;
    bool m_clickable = true;
    
    // === 样式选项 ===
    bool m_outlined = false;
    bool m_elevated = false;
    
    // === 自适应 ===
    bool m_autoResize = true;
};

#endif // MCARD_H