#ifndef WINDOW_DM90_H
#define WINDOW_DM90_H

#include <QMainWindow>
#include <QStackedWidget>
#include "../MBS_400/basewindow.h"
#include "common.h"
#include "dm90_leftview.h"
#include "dm90_midview.h"
#include "dm90_rightview.h"
#include "dm90_highview.h"
#include "dm90_lightview.h"
#include "dm90_receivemessage_thread.h"
#include "PD400/pd400_setting_view.h"
#include "customView/custom_loadingview.h"

class window_dm90 : public basewindow
{
    Q_OBJECT
public:
    explicit window_dm90(QWidget *parent = nullptr);

    dm90_leftview *m_leftView;
    dm90_midview *m_midView;
    dm90_rightview *m_rightView;

    //高级页面
    dm90_highView *m_highView;
    //灯光页面
    dm90_lightView *m_lightView;

    //加载的视图
    custom_loadingview *m_loadingView;
    //从设备上获取到的背景参数
    QByteArray m_device_datas;

    void createUI();

    enum SelectStatus{
       mic_click = 1,
       high_click,
       light_click
    };

public:

    void starConnectHid(int pid);

private:

    //获取完设备的背景参数刷新整个界面
    void refreshMainUI();

    void DM90_addContinuityReadData(const pc_continuity_read &data);

    send_receive_massage_control m_sendMessageClass;

    //设备断开连接后需要做的处理
    void device_disconnect_fun();

//    bool eventFilter(QObject *watched, QEvent *event);

private:

    //设置按钮
    QCheckBox *m_setting_btn;
    //点击设置按钮弹出的视图
    pd400_setting_view *m_setting_view;

    QStackedWidget *m_windowsStackedWidget;

    SelectStatus m_select_click;

    QVector<custom_button *>m_main_btns;
    //接收数据的线程
    dm90_receiveMessage_thread *m_receiveMessage_threa;

    send_receive_massage_control m_commin_fun;

public slots:

    void changeView_click();

    void back_view_click();

    //设备主动返回值
    void receive_sys_data();

    //插入USB
    void OnConnectDevice();
    //拔出USB
    void OnDisConnectDevice();

    //获取MODELCODE
    void getModelCodel(QString);

    //接收完所有完设备背景参数
    void dm90_receiveRefresh_mainUI();

    //接收同步的信息
    void receive_syn_data();

    //计时器延时的方法
    void delayHandleTimeout();

    //设置按钮的点击事件
    void setting_change(int);

    //设置视图点击空白处隐藏设置视图
    void setting_mouse_click();
    //设置视图点击检查版本
    void check_version_click();
    //设置按钮恢复出厂设置
    void reset_action();



signals:

   void Dm90BackClick();
   void changeTitle(int);
   //开启收发数据
   void sendMsgSignals();


};

#endif // WINDOW_DM90_H
