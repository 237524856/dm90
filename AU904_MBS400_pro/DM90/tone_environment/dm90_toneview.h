#ifndef DM90_TONEVIEW_H
#define DM90_TONEVIEW_H

#include <QWidget>
#include "common.h"

class dm90_toneView : public QWidget
{
    Q_OBJECT
public:
    explicit dm90_toneView(QWidget *parent = nullptr);


    void createUI();

    //刷新UI界面的方法
    void refreshUI();

    //计时器延时发送数据
    void toneStarDelayTimerFun();


    QVector<MySlider *> m_toneSliders;

    QVector<QLabel *> m_showValueLabels;

    //使能按钮
    custom_checkbox *m_highEQ_btn;

    enum TONE_SLIDER_NAME{

        TONE_SLIDER,//
        DELAY_SLIDER,

        FREQMODU1_SLIDER,//频率调制1
        DeptModu1_SLIDER, //调制深度1

        FREQMODU2_SLIDER,//频率调制1
        DeptModu2_SLIDER, //调制深度1
    };

    //当前选中的滑动号码
    int m_currentSliderNum =-1;

    send_receive_massage_control m_commin_fun;

public slots:

    void tone_chanege_value(int);

    void tone_release_value(int);

    void reset_click();

    void confirm_click();

    //使能按钮的方法
    void isopen_enable_click(int);

signals:

    //点击确认按钮的信号
    void confirmClick(int);

};

#endif // DM90_TONEVIEW_H
