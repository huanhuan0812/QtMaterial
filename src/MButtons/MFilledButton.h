// MFilledButton.h
#ifndef MFILLEDBUTTON_H
#define MFILLEDBUTTON_H

#include "MButtonBase.h"

class QTMBUTTON_EXPORT MFilledButton : public MButtonBase
{
    Q_OBJECT
    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor)

public:
    explicit MFilledButton(QWidget *parent = nullptr);
    explicit MFilledButton(const QString &text, QWidget *parent = nullptr);
    MFilledButton(const QIcon &icon, const QString &text, QWidget *parent = nullptr);
    virtual ~MFilledButton();

    QColor backgroundColor() const;
    void setBackgroundColor(const QColor &color);

protected:
    void paintBackground(QPainter &painter, const QRect &rect) override;
    void paintBorder(QPainter &painter, const QRect &rect) override;
    QColor getCurrentTextColor() const override;
    QColor getCurrentIconColor() const override;

private:
    QColor getContrastColor(const QColor &color) const;
    
    QColor m_backgroundColor;
};

#endif // MFILLEDBUTTON_H