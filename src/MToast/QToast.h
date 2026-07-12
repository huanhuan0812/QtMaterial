#ifndef QTOAST_H
#define QTOAST_H

#include <QWidget>
#include <QLabel>
#include <QPropertyAnimation>
#include <QTimer>
#include <QGraphicsOpacityEffect>

// 导出宏定义
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

class QTMATERIAL_EXPORT QToast : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity)

public:
    enum ToastDuration {
        LENGTH_SHORT = 1500,
        LENGTH_LONG = 3000
    };

    enum ToastPosition {
        POSITION_BOTTOM,
        POSITION_CENTER,
        POSITION_TOP
    };

    explicit QToast(QWidget *parent = nullptr);
    ~QToast();

    void setText(const QString &text);
    void setDuration(int ms);
    void setPosition(ToastPosition position);
    void setBackgroundColor(const QColor &color);
    void setTextColor(const QColor &color);
    void setCornerRadius(int radius);
    void setMargins(int horizontal, int vertical);
    void setMaxWidth(int maxWidth);
    void setFontSize(int size);
    void setBottomMarginRatio(qreal ratio);
    void setMinBottomMargin(int minMargin);
    void show();
    
    static void makeToast(QWidget *parent, 
                         const QString &text, 
                         int duration = LENGTH_SHORT,
                         ToastPosition position = POSITION_BOTTOM);
    
    static void makeToast(QWidget *parent, 
                         const QString &text, 
                         int duration,
                         ToastPosition position,
                         int fontSize,
                         const QColor &backgroundColor = QColor(40, 40, 40, 230),
                         const QColor &textColor = Qt::white);

    void setOpacity(qreal opacity);
    qreal opacity() const;

protected:
    void paintEvent(QPaintEvent *event) override;
    void showEvent(QShowEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    bool nativeEvent(const QByteArray &eventType, void *message, qintptr *result) override;

private slots:
    void onAnimationFinished();

private:
    void initUI();
    void updateSize();
    void updatePosition();
    void setupPlatformSpecificTransparency();

private:
    QLabel *m_label;
    QPropertyAnimation *m_opacityAnimation;
    QTimer *m_hideTimer;
    QGraphicsOpacityEffect *m_opacityEffect;
    
    QString m_text;
    int m_duration;
    ToastPosition m_position;
    QColor m_backgroundColor;
    QColor m_textColor;
    int m_cornerRadius;
    int m_hMargin;
    int m_vMargin;
    int m_maxWidth;
    int m_fontSize;
    
    qreal m_bottomMarginRatio;
    int m_minBottomMargin;
    
    bool m_isShowing;
    bool m_isUpdating;
};

#endif // QTOAST_H