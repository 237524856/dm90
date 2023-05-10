#ifndef DM90_VOLUMEEQVIEW_H
#define DM90_VOLUMEEQVIEW_H

#include <QWidget>
#include <QLabel>
#include "common.h"
#include "MBS_400/qverslider.h"

class dm90_volumeEQView : public QWidget
{
    Q_OBJECT
public:
    explicit dm90_volumeEQView(QWidget *parent = nullptr);

    void createUI();

    //刷新整个UI界面
    void refreshUI();

public:

    QVector<QLabel *>m_voiceCurrentValue;


    //所以水平的进度条的集合
    QVector<MySlider *>m_horSliders;
    //所以垂直的进度条的集合
    QVector<SliderWidget *>m_verSliders;

    //声音均衡器的开关
    custom_checkbox *m_highEQ_btn;

    //计时器延时发送数据
    void VoluemeStarDelayTimerFun();

protected:

    send_receive_massage_control m_commin_fun;

    //当前选中的滑动号码
    int m_currentSliderNum =-1;

public slots:

     void reset_click();

     void confirm_click();

     //开关的方法
     void isopen_enable_click(int);

     //各种进度条的方法的
     void verticalSlider_setValue(int);

     void horVoiceEQ_chanege_value(int);
     void horVoiceEQ_release_value(int);

signals:

     void confirmClick(int);

};

#endif // DM90_VOLUMEEQVIEW_H
