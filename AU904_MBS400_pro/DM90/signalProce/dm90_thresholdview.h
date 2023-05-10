#ifndef DM90_THRESHOLDVIEW_H
#define DM90_THRESHOLDVIEW_H

#include <QWidget>
#include "common.h"

class dm90_thresholdview : public QWidget
{
    Q_OBJECT
public:
    explicit dm90_thresholdview(QWidget *parent = nullptr);

    void createUI();

    //刷新UI界面的方法
    void refreshUI();

    //计时器延时发送数据
    void thresholdStarDelayTimerFun();

    void mousePressEvent(QMouseEvent *qevent);            //鼠标按下事件
    //void mouseReleaseEvent(QMouseEvent *qevent);         //鼠标释放事件
    //void mouseMoveEvent(QMouseEvent *qevent);             //鼠标移动事件

protected:

    QVector<MySlider *> m_threshouldSliders;

    QVector<QString> m_showLabelNumber_arrs;//需要展示得枚举数字

    QVector<QLabel *> m_showValueLabels;
    //使能按钮
    custom_checkbox *m_highEQ_btn;

    enum SLIDER_NAME{

        THRESHOLD_SLIDER,//
        HOLD_SLIDER,
        ATTACK_SLIDER,//启动时间
        RELEASE_SLIDER //恢复时间

    };


    //当前选中的滑动号码
    int m_currentSliderNum =-1;

    send_receive_massage_control m_commin_fun;

public slots:


    void threshould_chanege_value(int);

    void threshould_release_value(int);

     void reset_click();

     void confirm_click();

     //使能按钮的方法
     void isopen_enable_click(int);
signals:

     void confirmClick(int);

};

#endif // DM90_THRESHOLDVIEW_H
