#ifndef DM90_DELAYVIEW_H
#define DM90_DELAYVIEW_H

#include <QWidget>
#include "common.h"

class dm90_delayView : public QWidget
{
    Q_OBJECT
public:
    explicit dm90_delayView(QWidget *parent = nullptr);

    void createUI();

    //刷新UI界面的方法
    void refreshUI();

    //计时器延时发送数据
    void delayStarDelayTimerFun();

    QVector<MySlider *> m_delaySliders;

    QVector<QLabel *> m_showValueLabels;

    //使能按钮
    custom_checkbox *m_highEQ_btn;

    enum DELAY_SLIDER_NAME{

        LEVEL_SLIDER,//
        DELAY_SLIDER,
        FEEDBACK_SLIDER,//
        MAXDELAY,
    };

    //当前选中的滑动号码
    int m_currentSliderNum =-1;

    send_receive_massage_control m_commin_fun;

protected:

    //可变的最大的值
    QLabel *m_changeMaxLabel;

public slots:

    void delay_chanege_value(int);

    void delay_release_value(int);

    void reset_click();

    void confirm_click();

    //使能按钮的方法
    void isopen_enable_click(int);

signals:

    void confirmClick(int);

};

#endif // DM90_DELAYVIEW_H
