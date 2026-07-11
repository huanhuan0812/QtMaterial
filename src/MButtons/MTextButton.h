// MTextButton.h
#ifndef MTEXTBUTTON_H
#define MTEXTBUTTON_H

#include "MButtonBase.h"

class QTMBUTTON_EXPORT MTextButton : public MButtonBase
{
    Q_OBJECT

public:
    explicit MTextButton(QWidget *parent = nullptr);
    explicit MTextButton(const QString &text, QWidget *parent = nullptr);
    MTextButton(const QIcon &icon, const QString &text, QWidget *parent = nullptr);
    virtual ~MTextButton();

protected:
    void paintBackground(QPainter &painter, const QRect &rect) override;
    void paintBorder(QPainter &painter, const QRect &rect) override;
    QColor getCurrentTextColor() const override;
    QColor getCurrentIconColor() const override;

private:
    QColor textColor() const;
};

#endif // MTEXTBUTTON_H