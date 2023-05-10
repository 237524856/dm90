
#ifndef MYSLIDER_H
#define MYSLIDER_H

#include <QWidget>
#include <QMouseEvent>
#include <QSlider>
#include <QTimer>
#include<QKeyEvent>

class MySlider : public QSlider
{
    Q_OBJECT
public:
    MySlider(QWidget *parent = 0);

    QTimer *m_slider_timer;

    //按钮禁止点击的事件
    void setSliderDisable(bool);
    //~MySlider();
Q_SIGNALS:
    void sliderReleasedAt(int);//当鼠标释放时，抛出包含鼠标X坐标位置信息的信号

public Q_SLOTS:

public slots:
     void OnMouseLeave();


signals:

    void sliderCustomChangeValue(int);

    //void setValue(int value);
protected:  //mouse
    virtual void mousePressEvent(QMouseEvent *event) override;  //单击
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;    
    virtual void keyReleaseEvent(QKeyEvent *event) override;
    virtual void wheelEvent(QWheelEvent *event) override;
    // virtual bool event(QEvent *event) override;
    bool m_isMoving;
    int m_value;
    bool m_mousePress;
    double m_pos;




};

#endif
// MYSLIDER_H
