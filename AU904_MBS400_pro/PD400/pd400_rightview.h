#ifndef PD400_RIGHTVIEW_H
#define PD400_RIGHTVIEW_H

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

class pd400_rightview : public QWidget
{
    Q_OBJECT
public:
    explicit pd400_rightview(QWidget *parent = nullptr);
    void createUI();

    custom_checkbox *m_mute_btn;

    SliderWidget *m_mic_slider;

    QLabel *m_EQ_bgimage;
    //电平
    QLabel *m_dianping_lab_front;
    QLabel *m_dianping_lab_gree_front;
    QLabel *m_dianping_lab_red_front;

    QLabel *m_mic_volumecurrent_label;

    //蒙版
    QLabel *m_mengban_image;

    QVector<int>mic_slider_values;  
    //slider的当前值集合
    QVector<QLabel *>m_current_labels;
    //slider的集合
    QVector<MySlider *>sliders;
    //2个使能按钮
    QVector<QCheckBox *>enable_btns;

    QVector<QPushButton *>select_btns;

    //eq的label
    QVector<QString>m_eq_titles;
    QLabel *m_eqtitle_lal;
    QString m_current_btn_select;

    //设置隐藏电平
    void setHideDianping();

    //刷新界面
    void refresh_ui(int mute,int mix,int eq,int gain,int limit,int comp,int limit_enable,int com_enable,uint8_t micVolume);
    //同步信息
    void sys_receive_data(uint8_t address,int value);
   //切换语言
   void change_language();

   //加载限幅器数据
   void load_xianfuqi_data();
   //加载压缩器数据
   void load_yasuoqi_data();

   //计时器
   QTimer *m_DM30Timer;
   //页面中滑动条当前的值
   int m_slider_current_value=0;
   int m_slider_current_number = 0;

public slots:
    void change_eq();
    void headsetVolume_eq_chanege_value(int);
    void headsetVolume_release_value(int);
    void mic_eq_chanege_value(int);
    void slider_release_value(int);
    void hunyin_chanege_value(int);
    void hunyin_release_value(int);
    void select_btn_click();
    void isopen_btn_click(int);

    void mute_btn_click(int);

    void headsetVolume_eq_press_value();

    void handleTimeout();


signals:
    //弹出限制器或者压缩器的视图
    void show_limitOrcomp_view(int);

    void DM30_timer_out();


protected:
    QLabel *m_micmute_label;
    QLabel *m_micvolume_label;
    QLabel *m_jianting_label;
    QLabel *m_junheng_label;
    QLabel *m_miceq_label;
    QVector<QLabel *>xianfuyaxian_lbs;
    send_receive_massage_control m_commin_fun;
    //virtual bool eventFilter(QObject *obj, QEvent *event);

};

#endif // PD400_RIGHTVIEW_H
