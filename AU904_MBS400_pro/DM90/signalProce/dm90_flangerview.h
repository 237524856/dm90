#ifndef DM90_FLANGERVIEW_H
#define DM90_FLANGERVIEW_H

#include <QWidget>
#include "common.h"

class dm90_flangerView : public QWidget
{
    Q_OBJECT
public:
    explicit dm90_flangerView(QWidget *parent = nullptr);
    void createUI();

    //刷新界面
    void refreshUI();

    //计时器延时发送数据
    void flangerStarDelayTimerFun();


private:

    QVector<MySlider *>m_flangerSliders;

    QVector<QLabel *> m_showValueLabels;

    //使能按钮
    custom_checkbox *m_highEQ_btn;

    enum FLANGER_SLIDER_NAME{
        LEVEL_SLIDER,
        DEPTMODU_SLIDER,
        DELAY_SLIDER,
        FEEDRATE_SLIDER,
        FREQMODU_SLIDER,
    };

    //当前选中的滑动号码
    int m_currentSliderNum =-1;

    send_receive_massage_control m_commin_fun;


public slots:

    void flanger_chanege_value(int);

    void flanger_release_value(int);

    void reset_click();

    void confirm_click();

    //使能按钮的方法
    void isopen_enable_click(int);


signals:

    void confirmClick(int);

};

#endif // DM90_FLANGERVIEW_H
