#ifndef DM90_COMPRESSORVIEW_H
#define DM90_COMPRESSORVIEW_H

#include <QWidget>
#include "common.h"

class dm90_compressorView : public QWidget
{
    Q_OBJECT
public:
    explicit dm90_compressorView(QWidget *parent = nullptr);

    void createUI();

    //刷新UI界面的方法
    void refreshUI();

    //计时器延时发送数据
    void compressorStarDelayTimerFun();


private:

    QVector<MySlider *>m_compressSliders;

    QVector<QLabel *> m_showValueLabels;

    //使能按钮
    custom_checkbox *m_highEQ_btn;

    enum COMPRESS_SLIDER_NAME{

        COMPRESS_SLIDER,
        ATTACK_SLIDER,
        COMPRESS_RATE,
        COMPRESS_RELEASE

    };
    //当前选中的滑动号码
    int m_currentSliderNum =-1;

    send_receive_massage_control m_commin_fun;

public slots:

    //进度条的方法
    void compress_chanege_value(int);

    void compress_release_value(int);

    void reset_click();

    void confirm_click();

    //使能按钮的方法
    void isopen_enable_click(int);

signals:

    //点击确认按钮的信号
    void confirmClick(int);

};

#endif // DM90_COMPRESSORVIEW_H
