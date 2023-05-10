
#pragma once
#pragma execution_character_set("utf-8")
#ifndef WINDOW_PD400_H
#define WINDOW_PD400_H

enum pd400_progress_class{
  PD400MCU = 0,
  PD400DSP,
  ALONEPD400DSP
};
#include <thread>

#include <QMainWindow>
#include "../MBS_400/basewindow.h"
#include "../devoperator.h"

#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QTranslator>
#include <QTimer>
#include "common.h"
#include "common_funtion.h"
#include "pd400_leftview.h"
#include "pd400_rightview.h"
#include "pd400_setting_view.h"
#include "pd400_xianzhiqi_view.h"
#include "pd400_yasuoqi_view.h"
#include "pd400_check_updateview.h"
#include "pd400_progress_view.h"
#include "dfu_thread.h"
#include "../send_receive_massage_control.h"
#include "header_class.h"
#include "dfu_0206_view.h"

#define PD400MODELCODE "P0KT01"

class window_pd400 : public basewindow
{
    Q_OBJECT
public:
    explicit window_pd400(QWidget *parent = nullptr);
    ~window_pd400();
    //控制通讯协议的对象
    //DevOperator *m_devoperator;
    //线程
    QThread *m_childThread;
    //是否是打开基础还是高级
    bool is_open_status;

    //是否是恢复出厂设置的事件
    bool is_resetFaocClick;

    void clearDataSource();
    void create_pd400UI();

    QMenu* menu[10];
    QAction* act[10];
    QMenuBar* menuBar;
    QStatusBar* status;

    common_funtion m_co_fun;
    pd400_leftview *m_left_view;
    pd400_rightview *m_right_view;
    pd400_xianzhiqi_view *m_show_xianfuView;
    pd400_yasuoqi_view *m_show_comp;
    QLabel *m_product_label;
    //设置按钮
    custom_checkbox *m_setting_btn;

    //点击设置按钮弹出的视图
    pd400_setting_view *m_setting_view;
    pd400_check_updateView *m_check_view;

    //进度条的视图
    pd400_progress_view *m_progress_view;

    DFU_thread *m_dfu_thread;


    QList<QByteArray> m_file_datas;

    //计时器
    QTimer *m_pTimer;
    //限制点击高级，基础按钮的计时器
    QTimer *m_disableTimer;
    //升级DSP计时器
    QTimer *m_starDspTimer;

    //是否处在升级模式的计时器
    QTimer *m_isdfu_timer;

    //心跳的计时器
    QTimer *m_hearbeattimer;

    send_receive_massage_control m_commin_fun;

    //升级DSP功能
    pd400_progress_class m_progress_type;
    void dsp_updata();
    dfu_0206_view *m_dfu_view;

public:
    void starConnectHid();
    //更改语言
    void languge_click();

public slots:

    void open_basic();
    void open_high();
    void back_view_click();


    //void change_status(int);

    //检查设备的版本
    void check_version_click();
    //恢复原厂设置
    void reset_action();

    //插入USB
    void OnConnectDevice();
    //拔出USB
    void OnDisConnectDevice();

    void setting_change(int);

    void handleTimeout();  //计时器函数
    void disableHandleTimeout();  //计时器函数
    void starDspTimeout();
    void hearbeatTimeout();

    void isdfu_handleTimeout();

    void click_limit_comp_view(int);

    void refresh_main_UI();
    
    //同步信息的方法
    void receive_sys_data();

    void main_load_xianfuqi_data();

    void main_load_yasuoqi_data();
    //固件升级的槽
    void DFU_updata();
    //进度条当前的值
    void progress_value(int);

    //发送信息异常的处理
    void sendMessage_fail_hand(int);
    //设置页面随便点一处隐藏
    void setting_mouse_click();

    //获取modelCodel
    void maingetModelCodel();
    //判断当前得设备处在什么模式
    void isDfuStatus(int);
    //SLIDER是否能拖动
    void setSliderDisabled(bool);

    void DM30_timer_outfun();

    void device_disconnect_fun();

    //DSP升级
    void alone_updata_dsp();
    void dsp_kt0206_progress_val(int);
    void dsp_kt0206_finish(int status);


signals:
    //发送数据
    void sendMsgSignals();

    void pd400BackClick();

    void dfuReadFileData();

protected:
    QTranslator m_translator;
    custom_button *m_basic_btn;
    custom_button *m_high_btn;

    //当前选中的语言
    int m_current_languge_index;

    int m_current_model = 1;

    //从设备上获取到的背景参数
    QByteArray m_pd400_device_datas;


};

#endif // WINDOW_PD400_H
