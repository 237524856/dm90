#ifndef PM481_LEFTVIEW_H
#define PM481_LEFTVIEW_H

#include <QWidget>
#include "qcheckbox.h"
#include <QLabel>
#include "qpushbutton.h"
#include "../send_receive_massage_control.h"
#include "common_funtion.h"
#include "../common.h"
#include "custom_button.h"
#include "custom_checkbox.h"



class pm481_leftview : public QWidget
{
    Q_OBJECT
public:
    explicit pm481_leftview(QWidget *parent = nullptr);
    void createUI(int);
    void setHideDianping();

    custom_checkbox *m_mute_btn;
    QLabel *m_mic_volumecurrent_label;
    QLabel *m_mic_gaincurrent_label;
    QLabel *m_mic_lightcurrent_label;

    QLabel *m_yinse_btn_img;
    //电平的进度条
    QLabel *m_dianping_lab_front;
    QLabel *m_dianping_lab_gree_front;
    QLabel *m_dianping_lab_red_front;

    //灯的亮度调节
    MySlider *m_light_slider;
    //最底下的三角形
    QCheckBox *m_bottom_triangle;
    //rgb开关按钮
    QLabel *m_RGB_btn_bgimage;
    common_funtion m_co_fun;
    //监听比例
    QLabel *m_mix_btn_bgimage;
    //基础的蒙版
    QLabel *m_basic_mask;
    //rgb灯的蒙版
    QLabel *m_rgb_mask;

    QLabel *m_fullrgb_mask;
    //颜色选择的集合
    QVector<custom_checkbox *>rgb_btns;
    //获取数据后同步到软件
    void refresh_ui(uint8_t mute,uint16_t HeadphoneVolume,uint8_t micGain,uint8_t yinse,uint8_t rgbSwitch,uint8_t rgbAdj,uint8_t rgbNumber,uint8_t mic_mon);
    //实时同步
    void sys_receive_data(uint8_t address,int value,uint8_t address2,int value2,int level_data);

    //RGB按钮
    QVector<QPushButton *>m_switch_btns;

    //当前选中了那个音色
   QString m_current_yinse_number;

   //silder正在滑动
//   bool m_phsilder_runing = false;
//   bool m_micsilder_runing = false;

   //计时器
   //QTimer *m_DM30Timer;
   //页面中滑动条当前的值
//   int m_slider_current_value=0;
//   int m_micgainslider_current_value=0;
//   int m_slider_current_number = 0;

public:
    void change_language();

signals:


public slots:

    void mute_btn_click(int);
    void change_yinse();
    void headsetVolume_eq_chanege_value(int);
    void headsetVolume_release_value(int);

    void micgain_eq_change_value(int);
    void micgain_release_value(int);

    void light_change_value(int);
    void light_release_value(int);

    void light_switch_click();
    void mix_switch_click();
    //颜色的选择
    void rgb_btn_click(bool);

    void handleTimeout();


protected:
    QLabel *m_micmute_label;
    QLabel *m_micvolume_label;
    QLabel *m_jianting_label;
    QLabel *m_yinse_label;
    custom_button *m_dichen_btn;
    custom_button *m_ziran_btn;
    custom_button *m_mingliang_btn;
    custom_button *m_jingdian_btn;

    send_receive_massage_control m_commin_fun;

    enum MaonoDeviceName{

        MaonoDM30 = 1,
        MaonoPD200X,
        MaonoDM30RGB,
    };

};

#endif // PM481_LEFTVIEW_H
