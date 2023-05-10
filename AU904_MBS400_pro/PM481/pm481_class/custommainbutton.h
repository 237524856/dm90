#ifndef CUSTOMMAINBUTTON_H
#define CUSTOMMAINBUTTON_H

#include <QWidget>
#include <QPushButton>
#include <QMouseEvent>

class customMainButton : public QPushButton
{
    Q_OBJECT
public:
    explicit customMainButton(QWidget *parent = nullptr);

    void mousePressEvent(QMouseEvent *qevent);            //鼠标按下事件
    void mouseReleaseEvent(QMouseEvent *qevent);         //鼠标释放事件
    void mouseMoveEvent(QMouseEvent *qevent);             //鼠标移动事件

    void enterEvent(QEvent *);                      //进入QWidget瞬间事件
    void leaveEvent(QEvent *);                      //离开QWidget瞬间事件

signals:

    void custom_click();
    void custom_enter(QString);
    void custom_leave(QString);

};

#endif // CUSTOMMAINBUTTON_H
