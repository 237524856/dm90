#ifndef PD400_LEFTVIEW_H
#define PD400_LEFTVIEW_H

#include <QWidget>
#include "qcheckbox.h"
#include "../MBS_400/qhorslider.h"
#include "../MBS_400/myslider.h"
#include "qpushbutton.h"
#include <QLabel>
#include "../common.h"
#include "../send_receive_massage_control.h"
#include "custom_button.h"
#include "custom_checkbox.h"
#include "sliderwidget.h"

class pd400_leftview : public QWidget
{
    Q_OBJECT
public:
    explicit pd400_leftview(QWidget *parent = nullptr);
    void createUI();

    custom_checkbox *m_mute_btn;
    QLabel *m_mic_volumecurrent_label;

    //监听比例
    SliderWidget *m_mic_slider;

    QLabel *m_yinse_btn_img;

    QLabel *m_mic_space_img;

    //远中近的图片
    QLabel *m_yuanzhongjin_bgimg;
    //电平的进度条
    QLabel *m_dianping_lab_front;
    QLabel *m_dianping_lab_gree_front;
    QLabel *m_dianping_lab_red_front;

    //远中近当前的值
    QString m_dis_current_number;
    //音色调节当前值
    QString m_tone_current_number;

    //蒙版
    QLabel *m_mengban_image;

    void change_language();

    //设置隐藏电平
    void setHideDianping();

    //刷新背景数据

    void refresh_ui(uint8_t mute,uint16_t mix,uint8_t yinse,uint8_t micjuli,uint8_t micVolume);

    //同步信息
    void sys_receive_data(uint8_t address,int value);

    //计时器
    QTimer *m_DM30Timer;
    //页面中滑动条当前的值
    int m_slider_current_value=0;
    int m_slider_current_number = 0;

    //slider是否正在滑动
//    bool m_phsilder_runing = false;
//    bool m_mixsilder_runing = false;

public slots:
    void mic_chanege_value(int);
    void mix_release_value(int);
    void headsetVolume_eq_chanege_value(int);
    void headsetVolume_release_value(int);
    void change_yinse();
    void change_yuanzhongjin();
    //静音功能
    void mute_btn_click(int);
    //计时器的方法
    void handleTimeout();


signals:


protected:
    QLabel *m_micmute_label;
    QLabel *m_micvolume_label;
    QLabel *m_jianting_label;
    QLabel *m_yinse_label;
    custom_button *m_dichen_btn;
    custom_button *m_ziran_btn;
    custom_button *m_mingliang_btn;
    custom_button *m_jingdian_btn;
    QLabel *m_mic_juli_label;

    QLabel *m_space_number;

    QVector<QPushButton *>yuanzhongjin_btns;

    send_receive_massage_control m_commin_fun;

};

#endif // PD400_LEFTVIEW_H
