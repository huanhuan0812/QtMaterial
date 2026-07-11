// MOutlinedButton.h
#ifndef MOUTLINEDBUTTON_H
#define MOUTLINEDBUTTON_H

#include "MButtonBase.h"

class QTMBUTTON_EXPORT MOutlinedButton : public MButtonBase
{
    Q_OBJECT
    Q_PROPERTY(qreal borderWidth READ borderWidth WRITE setBorderWidth)

public:
    explicit MOutlinedButton(QWidget *parent = nullptr);
    explicit MOutlinedButton(const QString &text, QWidget *parent = nullptr);
    MOutlinedButton(const QIcon &icon, const QString &text, QWidget *parent = nullptr);
    virtual ~MOutlinedButton();

    qreal borderWidth() const;
    void setBorderWidth(qreal width);

protected:
    void paintBackground(QPainter &painter, const QRect &rect) override;
    void paintBorder(QPainter &painter, const QRect &rect) override;
    QColor getCurrentTextColor() const override;
    QColor getCurrentIconColor() const override;

private:
    QColor outlineColor() const;
    
    qreal m_borderWidth = 1.0;
};

#endif // MOUTLINEDBUTTON_H