#pragma once

#include "qdialog.h"
#include <QPushButton>
#include <QLabel>
#include <QMouseEvent>
#include <QLineEdit>
class MsgBox :public QDialog
{
    Q_OBJECT
public:
    MsgBox(int style,QString text);
    ~MsgBox(void);
public:
    QPushButton *okBtn;
    QPushButton *cancleBtn;
    QPushButton *closeBtn;
    QPushButton * msgBtn;
    QLabel * titleLabel;
    QLabel *askLabel;
    QLineEdit *m_lenedit;
protected:
    QPoint move_point;                                    //移动的距离
    bool mouse_press;                                    //鼠标按下
    void mousePressEvent(QMouseEvent *qevent);            //鼠标按下事件
    void mouseReleaseEvent(QMouseEvent *qevent);         //鼠标释放事件
    void mouseMoveEvent(QMouseEvent *qevent);             //鼠标移动事件
public slots:
    void okBtn_press();
    void cancleBtn_press();
    void closeBtn_press();
};

