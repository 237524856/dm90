#ifndef DM90_RIGHTVIEW_H
#define DM90_RIGHTVIEW_H

#include <QWidget>

#include <QStackedWidget>
#include "common.h"
#include "../MBS_400/myslider.h"
#include "custom_button.h"
#include "custom_checkbox.h"
#include "signalProce/dm90_thresholdview.h"
#include "signalProce/dm90_detonatorview.h"
#include "signalProce/dm90_compressorview.h"
#include "signalProce/dm90_limiterview.h"
#include "signalProce/dm90_dentalsoundview.h"
#include "signalProce/dm90_flangerview.h"
#include "signalProce/dm90_volumeeqview.h"
#include "tone_environment/dm90_toneview.h"
#include "tone_environment/dm90_reverbview.h"
#include "tone_environment/dm90_delayview.h"
#include "dm90_recoderview.h"
#include "MBS_400/qverslider.h"

class dm90_rightview : public QWidget
{
    Q_OBJECT
public:
    explicit dm90_rightview(QWidget *parent = nullptr);

    //创建UI
    void createUI();

    //刷新UI
    void refreshUI();

    //刷新电平,Input,Output
    void refreshLevel(int,int);

    //开启延时计时器
    void starDelayTimerFun();

public:

    void createShowSignalView();



public:

    //所以水平的进度条的集合
    QVector<MySlider *>m_horSliders;
    //所以垂直的进度条的集合
    QVector<SliderWidget *>m_verSliders;

    //所有开关按钮的集合
    QVector<custom_checkbox *>open_close_btns;

    dm90_thresholdview      *m_thresholdView;
    dm90_detonatorView      *m_detonatorView;
    dm90_compressorView     *m_compressorView;
    dm90_limiterView        *m_limiterView;
    dm90_dentalSoundView    *m_dentalsoundView;
    dm90_flangerView        *m_flangerView;
    dm90_volumeEQView       *m_volumeEqView;

    //效果器得详情
    dm90_toneView           *m_toneView;
    dm90_reverbView         *m_reverbView;//混响
    dm90_delayView          *m_delayView;

    QWidget *m_maskView;

    dm90_recoderview *m_recorderView;

    enum SIHNALINDEX{
        THRESHOLD = 1,
        DETONATOR,//去爆音
        COMPRESSOR,
        LIMITER,
        DENTALSOUND,//齿音消除
        FLANGER,
    };

    enum LOCALEFFECT{
        TONE =100,
        REVERB,
        DELAY
    };

    //水平进度条
    enum CURRENTHSLIDER{

        INPUTSLIDER = 300,
        OUTPUTSLIDER,
        EQHPSLIDER,//高通滤波器
        THRESHOLDSLIDER,//门限
        DETONATORSLIDER,//去爆音器
        COMPRESSORSLIDER,
        LIMITERSLIDER,
        DENTALSOUNDSLIDER,//齿音消除
        REVERBSLIDER,//混音
        DELAYSLIDER,//延时

    };

    //MIC声音均衡器EQ
    enum MICEQ{
        LOW_EQ = 0,
        MID_EQ,
        HIGH_EQ
    };

    //显示当前的值的数据
    QVector<QLabel *>m_currentValues;

    custom_checkbox *m_mute_btn;

    //声音均衡器的当前的值
    QVector<QLabel *>m_voiceCurrentValue;


protected slots:

    //横向滑动条的事件
    void rightViewSlider_chanege_value(int);

    void rightViewSlider_release_value(int);

    void verticalSlider_setValue(int);
    //void verticalSlider_released();


    //静音事件
    void mute_btn_click(int);


    void eq_singsClick();
    //效果器的切换
    void effectClick();

    void select_btn_click();

    void isopen_btn_click(int);

    void signalselect_btn_click();

    //voice EQ
    void voiceEQshow_btn_click();

    //显示环境得详情
    void tone_environment_btn_click();

    //各种状态的开关
    void isOpenCloseBtn_click(int);

    //高通滤波器
    void highpassBtn_click(int);

    //各种视图中确定按钮的槽
    void view_confirmClick(int);


    //点击本地声音更新预设
    void clickItemRefreshUI(bool);




public slots:
    //本地声音，效果的切换
    void receiveResult(int);

protected:

    void createSignalView();

    //设备的负数转为实际的参数
    //double loadingChangeNegativeNumber(int);

    //实际的负参数上传到设备中
    double uploadChangeNegativeNumber(int);

    send_receive_massage_control m_commin_fun;

    //高通滤波器的转换公式
    int changexfor_freq(int x);

    int changeFreqfor_X(int freq);

    void setHideDianping();

    bool m_isList_click = false;

private:

    //输入增益的电平的进度条
    QLabel *m_inputDianping_moveLab;
    //输出增益的电平的进度条
    QLabel *m_outputDianping_moveLab;

    QLabel *m_titleStatus;

    QLabel *m_effectTitleStatus;

    QWidget *m_eqView;

    QWidget *m_signalView;
    //均衡器，信号处理器
    QStackedWidget *m_pStackedWidget;
    //本地声音，效果
    QStackedWidget *m_localEffectView;

    QStackedWidget *m_effectStackedWidget;

    //当前选中的滑动号码
    int m_currentSliderNum =-1;

signals:

    //控件增加本地音效ITEM
    void rightView_addLocaltItem(int,int);

    //点击控件增加效果ITEM
    void rightView_addEffectItem(int,int);


};

#endif // DM90_RIGHTVIEW_H
