#ifndef WINDOW_PM481_H
#define WINDOW_PM481_H

#include <QMainWindow>
#include "../MBS_400/basewindow.h"
#include <QTimer>
#include "pm481_leftview.h"
#include "pm481_rightview.h"
#include "pm481sendreceiveclass.h"
#include "PD400/dfu_thread.h"
#include "PD400/pd400_setting_view.h"
#include "PD400/pd400_check_updateview.h"
#include "PD400/pd400_progress_view.h"
#include "dfu_0206_view.h"

//#define PM481MODELCODE "P0KT02"
enum progress_class{
  MCU = 0,
  DSP
};



class window_pm481 : public basewindow
{
    Q_OBJECT
public:
    explicit window_pm481(QWidget *parent = nullptr);

    ~window_pm481();

    //初始化之初清空全局的数据
    void clearDataSource();
    //计时器
    QTimer *m_pTimer;
    //限制点击高级，基础按钮的计时器
    QTimer *m_disableTimer;
    //关闭窗口计时器
    QTimer *m_starDspTimer;
    //判断是否是
    QTimer *m_DfuStatusTimer;

    //心跳的计时器
    QTimer *m_hearbeattimer;

    void pm481_createUI(int);

    //开始连接HID
    void starConnectHid();

    pm481_leftview *m_left_view;
    pm481_rightview *m_right_view;
    //显示是否连接的LABEL
    QLabel *m_product_label;

    //收发数据的类
    send_receive_massage_control m_commin_fun;
    //专门一直接收数据的线程
    DFU_thread *m_dfu_thread;

    //设置按钮
    QCheckBox *m_setting_btn;

    //点击设置按钮弹出的视图
    pd400_setting_view *m_setting_view;
    pd400_check_updateView *m_check_view;
    //进度条的视图
    pd400_progress_view *m_progress_view;

    //固件升级的数据
    QList<QByteArray> m_file_datas;

    //SLIDER是否能拖动
    void setSliderDisabled(bool);


    void device_disconnect_fun();

    //升级DSP功能
    progress_class m_progress_type;
//    bool is_updata_runing;//升级中
    //QString m_file_dsp;
    void dsp_updata();
    dfu_0206_view *m_dfu_view;

    bool is_resetFaocClick;

signals:

    //发送数据
    void sendMsgSignals();

    void pm481BackClick();

    void dfuReadFileData();

public slots:

    //插入USB
    void OnConnectDevice();
    //拔出USB
    void OnDisConnectDevice();
    //获取完背景参数，刷新主界面
    void refresh_main_UI();
    //实时同步设备返回的信息
    void receive_sys_data();

    void handleTimeout();  //计时器函数
    void disableHandleTimeout();  //计时器函数
    void sendLastHandleTimeout(); //最后一包的计时器
    void starDspTimeout();
    void hearbeatTimeout();

    void open_basic_orhigh();
    //void change_status(int);
    void open_basic();
    void open_high();
    //设置按钮的事件
    void setting_change(int);


    void languge_click(int);
    void check_version_click();
    //恢复原厂设置
    void reset_action();
    void setting_mouse_click();
    void DFU_updata();
    //进度条
    void progress_value(int);
    void sendMessage_fail_hand(int);
    void isDfuStatus(int);
    void maingetModelCodel();

    void back_click();
    //DSP升级
    void alone_updata_dsp();
    void dsp_kt0206_progress_val(int);
    void dsp_kt0206_finish(int status);

protected:

    custom_button *m_basic_btn;
    custom_button *m_high_btn;

    //从设备上获取到的背景参数
    QByteArray m_pm481_device_datas;



};

#endif // WINDOW_PM481_H
