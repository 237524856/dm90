#ifndef CUSTOM_CHECKBOX_H
#define CUSTOM_CHECKBOX_H

#include <QWidget>
#include <QCheckBox>

class custom_checkbox : public QCheckBox
{
    Q_OBJECT
public:
    explicit custom_checkbox(QWidget *parent = nullptr);


protected:

    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
signals:

};

#endif // CUSTOM_CHECKBOX_H
