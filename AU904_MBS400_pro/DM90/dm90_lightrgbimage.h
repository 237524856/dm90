#ifndef DM90_LIGHTRGBIMAGE_H
#define DM90_LIGHTRGBIMAGE_H

#include <QWidget>
#include <QMouseEvent>
#include <QLabel>
#include <QPushButton>

class dm90_lightRGBimage : public QWidget
{
    Q_OBJECT
public:
    explicit dm90_lightRGBimage(QWidget *parent = nullptr);

    void createUI();

    void mousePressEvent(QMouseEvent *qevent);            //鼠标按下事件
    void mouseReleaseEvent(QMouseEvent *qevent);         //鼠标释放事件
    void mouseMoveEvent(QMouseEvent *qevent);             //鼠标移动事件
#if defined(Q_OS_WIN32)
    void enterEvent(QEvent *);                      //进入QWidget瞬间事件
#else
   void enterEvent(QEnterEvent *);                      //进入QWidget瞬间事件
#endif

   void leaveEvent(QEvent *);                      //离开QWidget瞬间事件

   void paintEvent(QPaintEvent *event);

signals:

   void custom_click(int,int,int);
   void custom_enter();
   void custom_leave();

};

#endif // DM90_LIGHTRGBIMAGE_H
