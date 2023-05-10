#ifndef DM90_DETONATORVIEW_H
#define DM90_DETONATORVIEW_H

#include <QWidget>
#include "common.h"

class dm90_detonatorView : public QWidget
{
    Q_OBJECT
public:
    explicit dm90_detonatorView(QWidget *parent = nullptr);

    void createUI();
    //刷新界面
    void refreshUI();

    //计时器延时发送数据
    void detonatorStarDelayTimerFun();

private:

    QVector<MySlider *>m_detonatorSliders;

    QVector<QLabel *> m_showValueLabels;
    //使能按钮
    custom_checkbox *m_highEQ_btn;

    enum DETONATOR_SLIDER_NAME{

        DETONATOR_SLIDER,
        RELEASE_SLIDER,
        FREQ_SLIDER,
        MINGAIN_SLIDER,
        Q_SLIDER,
        DETECTPERIOD_SLIDER, //检测时间
        ATTACK_SLIDER

    };

    //当前选中的滑动号码
    int m_currentSliderNum =-1;

    send_receive_massage_control m_commin_fun;


public slots:

    //进度条的方法
    void detonator_chanege_value(int);

    void detonator_release_value(int);


     void reset_click();

     void confirm_click();

     void isopen_enable_click(int);

signals:

     //点击确认按钮的信号
     void confirmClick(int);

};

#endif // DM90_DETONATORVIEW_H
