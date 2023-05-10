#ifndef DM90_LIMITERVIEW_H
#define DM90_LIMITERVIEW_H

#include <QWidget>
#include "common.h"

class dm90_limiterView : public QWidget
{
    Q_OBJECT
public:
    explicit dm90_limiterView(QWidget *parent = nullptr);

    void createUI();

    //刷新UI界面的方法
    void refreshUI();

    //计时器延时发送数据
    void limiterStarDelayTimerFun();

private:

    QVector<MySlider *>m_limiterSliders;

    QVector<QLabel *> m_showValueLabels;

    //使能按钮
    custom_checkbox *m_highEQ_btn;

    QVector<QString> m_attack_arrs;//需要展示得枚举数字

    QVector<QString> m_release_arrs;//需要展示得枚举数字

    enum LIMITER_SLIDER_NAME{

        LIMITER_SLIDER,
        LIMITER_ATTACK,
        LIMITER_RELEASE

    };

    //当前选中的滑动号码
    int m_currentSliderNum =-1;

    send_receive_massage_control m_commin_fun;

public slots:


    void limiter_chanege_value(int);

    void limiter_release_value(int);

     void reset_click();

     void confirm_click();

     //使能按钮的方法
     void isopen_enable_click(int);
signals:

     //点击确认按钮的信号
     void confirmClick(int);
};

#endif // DM90_LIMITERVIEW_H
