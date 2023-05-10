#ifndef YY_CUSTOMBUTTON_H
#define YY_CUSTOMBUTTON_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>


class YY_customButton : public QPushButton
{
    Q_OBJECT
public:
    explicit YY_customButton(QWidget *parent = nullptr);

    void setUI(QString,QString);

    //
    QLabel *m_topImage;

    QLabel *m_content;

signals:

};

#endif // YY_CUSTOMBUTTON_H
