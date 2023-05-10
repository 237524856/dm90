#ifndef CUSTOM_BUTTON_H
#define CUSTOM_BUTTON_H

#include <QWidget>
#include <QPushButton>

class custom_button : public QPushButton
{
    Q_OBJECT
public:
    explicit custom_button(QWidget *parent = nullptr);


protected:

    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
signals:

};

#endif // CUSTOM_BUTTON_H
