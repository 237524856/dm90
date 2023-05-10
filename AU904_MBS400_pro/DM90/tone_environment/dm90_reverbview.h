#ifndef DM90_REVERBVIEW_H
#define DM90_REVERBVIEW_H

#include <QObject>
#include <QWidget>
#include "common.h"

class dm90_reverbView : public QWidget
{
    Q_OBJECT
public:
    explicit dm90_reverbView(QWidget *parent = nullptr);
    void createUI();

    //刷新UI界面的方法
    void refreshUI();

    //计时器延时发送数据
    void reverbStarDelayTimerFun();

    QVector<MySlider *> m_reverbSliders;

    QVector<QLabel *> m_showValueLabels;

    //使能按钮
    custom_checkbox *m_highEQ_btn;

    enum REVERB_SLIDER_NAME{

        PREDELAY_SLIDER,//
        TIME_SLIDER,
        DRYLEVEL_SLIDER,//直达声音量
        DAMPING_SLIDER, //阻尼
        WETLEVEL_SLIDER,//混响音量
        EARLYLEVEL_SLIDER, //前期反射比例
        SPREAD_SLIDER, //扩散度
        TAILLEVEL_SLIDER, //后期反射声比例
        ROOMSIZE_SLIDER, //房间尺寸
    };

    //当前选中的滑动号码
    int m_currentSliderNum =-1;

    send_receive_massage_control m_commin_fun;
public slots:

    void reverb_chanege_value(int);

    void reverb_release_value(int);

    void reset_click();

    void confirm_click();

    //使能按钮的方法
    void isopen_enable_click(int);
signals:

    void confirmClick(int);

};

#endif // DM90_REVERBVIEW_H
