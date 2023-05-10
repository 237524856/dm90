  #ifndef CUSTOM_ITEM_H
#define CUSTOM_ITEM_H

#include <QWidget>
#include <QVector>
#include <QLabel>
#include <QLineEdit>
#include <QMouseEvent>
#include <QPushButton>
#include "custom_lineeditview.h"

class custom_item : public QWidget
{
    Q_OBJECT
public:
    explicit custom_item(QWidget *parent = nullptr);

    void mousePressEvent(QMouseEvent *qevent);            //鼠标按下事件
    void mouseReleaseEvent(QMouseEvent *qevent);         //鼠标释放事件
    void mouseMoveEvent(QMouseEvent *qevent);             //鼠标移动事件

    void enterEvent(QEvent *);                      //进入QWidget瞬间事件
    void leaveEvent(QEvent *);                      //离开QWidget瞬间事件

    QVector<QPushButton*>backImgs;
    //ITEM的标题
    custom_lineEditView *m_item_title;

    //设置标题
    void setTitleName(const QString &str);

    void setClickSelect(bool isClick);



public:

    bool m_item_isSelect = false;

signals:

    void item_click(QString);

    //进入ITEM
    void item_enter(QString);

    void item_btnClick(QString,QString);

public slots:

    void itemBtn_click();

};

#endif // CUSTOM_ITEM_H
