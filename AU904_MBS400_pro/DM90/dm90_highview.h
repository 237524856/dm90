#ifndef DM90_HIGHVIEW_H
#define DM90_HIGHVIEW_H

#include <QWidget>
#include "common.h"
#include "custom_button.h"
#include "dm90_higheqview.h"
#include <QStackedWidget>
#include "customView/custom_listview.h"
#include "customView/custom_itemlistview.h"

class dm90_highView : public QWidget
{
    Q_OBJECT
public:
    explicit dm90_highView(QWidget *parent = nullptr);

    void createUI();

    //刷新当前的UI
    void refresh();

    //计时器发送数据
    void highStarDelayTimerFun();

    QVector<MySlider *> m_highHorSliders;

    QVector<SliderWidget *> m_mixhighlowSliders;

    //当前页面的当前值
    QVector<QLabel *>m_currentValueLabels;

    enum OUTPUT_SLIDER_NAME{

        HPOUTPUT_SLIDER,//耳机输出
        MIX_SLIDER,//监听
        MICHIGHGAIN_SLIDER,//高音
        MICLOWGAIN_SLIDER,//低音
    };
    //默认预设列表
    custom_listView *m_user_listView;

    custom_itemListView *m_usercustom_itemList;


public slots:

    void output_chanege_value(int);

//    void output_release_value(int);

//    void highLow_chanege_value(int);
//    void highLow_release_value(int);

    //监听、高音、低音
    void mic_chanege_value(int);
    void mix_release_value(int);

    //是否打开高级EQ
    void disable_usbplay(int);

    //EQ视图的回调
    void eqViewSlider_value(int);
    //EQ视图的回调的释放
    void eqViewSliderRelease_value(int);

    //点击默认预设
    void user_listView_click(QString,QString,int);
    //点击自定义预设
    void custom_listView_click(QString,QString,int);

    //自定义预设后面的按钮
    void custom_preset_click();

protected:

    send_receive_massage_control m_commin_fun;

    QStackedWidget *m_highEQStackedWidget;

    dm90_highEQView *m_EQView;

    common_funtion m_commClass;


    //当前选中的滑动号码
    int m_HcurrentSliderNum =-1;

    //高级均衡武器选中的SLIDER
    int m_eqViewSliderNumber=0;

    //dm90_highEQView *m_EQView;

    QListWidget *m_listWidget_default;

    //默认，自定义LIST选中的下标
    //int m_allItemesIndex=0;

    //自定义当前ITEME的text
    QString m_defaultCurrentItemText = "原声";




signals:

};

#endif // DM90_HIGHVIEW_H
