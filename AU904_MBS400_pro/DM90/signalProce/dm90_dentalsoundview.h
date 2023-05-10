#ifndef DM90_DENTALSOUNDVIEW_H
#define DM90_DENTALSOUNDVIEW_H

#include <QWidget>
#include "common.h"

class dm90_dentalSoundView : public QWidget
{
    Q_OBJECT
public:
    explicit dm90_dentalSoundView(QWidget *parent = nullptr);

    void createUI();

    //刷新界面
    void refreshUI();

    //计时器延时发送数据
    void dentalsoundStarDelayTimerFun();


private:

    QVector<MySlider *>m_detonatorSliders;

    QVector<QLabel *> m_showValueLabels;

    //使能按钮
    custom_checkbox *m_highEQ_btn;

    enum DENTALSOUND_SLIDER_NAME{

        DENTALSOUND_SLIDER,
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

    void dentalsound_chanege_value(int);

    void dentalsound_release_value(int);

     void reset_click();

     void confirm_click();

     //使能按钮的方法
     void isopen_enable_click(int);
signals:

     void confirmClick(int);

};

#endif // DM90_DENTALSOUNDVIEW_H
