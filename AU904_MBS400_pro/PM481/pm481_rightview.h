#ifndef PM481_RIGHTVIEW_H
#define PM481_RIGHTVIEW_H

#include <QWidget>
#include "qcheckbox.h"
#include <QLabel>
#include "qpushbutton.h"
#include "send_receive_massage_control.h"
#include "../MBS_400/myslider.h"
#include "custom_button.h"
#include "custom_checkbox.h"

class pm481_rightview : public QWidget
{
    Q_OBJECT
public:
    explicit pm481_rightview(QWidget *parent = nullptr);

    void createUI();
    void setHideDianping();

    custom_checkbox *m_mute_btn;
    QLabel *m_mic_volumecurrent_label;
    QLabel *m_mic_gaincurrent_label;

    QLabel *m_EQ_bgimage;
    //电平的进度条
    QLabel *m_dianping_lab_front;
    QLabel *m_dianping_lab_gree_front;
    QLabel *m_dianping_lab_red_front;

    //2个使能按钮
    QVector<QCheckBox *>enable_btns;
    //eq的label
    QVector<QString>m_eq_titles;
    QLabel *m_eqtitle_lal;
    QString m_current_btn_select;

    //高级的蒙版
    QLabel *m_high_mask;

    QVector<QLabel *>m_comp_limiter_lables;

    //slider的集合
    QVector<MySlider *>sliders;

    //获取数据后同步到软件
    void refresh_ui(uint8_t mute,uint16_t HeadphoneVolume,uint8_t micGain,uint8_t eqValue,
                    uint8_t limiter_status,int limiter_value,uint8_t comper_status,int comper_value,uint8_t mic_mon);
    //实时同步
    void sys_receive_data(uint8_t address,int value,int level_data);
    //计时器
//    QTimer *m_DM30Timer;
    //页面中滑动条当前的值
//    int m_slider_current_value=0;
//    int m_slider_current_number = 0;
//    bool m_phsilder_runing = false;
//    bool m_micsilder_runing = false;

signals:

public slots:
    void mute_btn_click(int);
    void change_eq();
    void headsetVolume_eq_chanege_value(int);
    void headsetVolume_release_value(int);

    void micgain_eq_chanege_value(int);
    void micgain_release_value(int);

    void change_limiter_comp(int);
    void limiter_comp_release_value(int);

    void isopen_btn_click(int);

    void handleTimeout();
    void mix_switch_click();

protected:
    QLabel *m_micmute_label;
    QLabel *m_micvolume_label;
    QLabel *m_mix_btn_bgimage;
    QLabel *m_jianting_label;
    QLabel *m_yinse_label;
    QLabel *m_junheng_label;
    QVector<QLabel *>xianfuyaxian_lbs;

    send_receive_massage_control m_commin_fun;
};

#endif // PM481_RIGHTVIEW_H
