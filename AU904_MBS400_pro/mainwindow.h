#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#pragma once

#include <QMainWindow>
#include <QVector>
#include "MBS_400/window_mbs400.h"
#include "PD400/window_pd400.h"
#include "PM481/window_pm481.h"
#include "DM90/window_dm90.h"
#include "titleBar/mainbasewindow.h"
#include "common_funtion.h"
#include <QComboBox>
#include "PD400/childthread.h"
#include "mypushbutton.h"
#include "common_newwork_class.h"
#include "mainsubview.h"

#define PD400PID    0x0100
#define PM481PID    0x0101
#define DM90PID     0x0102
#define DM70PID     0x0103
#define PD200PID    0x0104
#define DM30RGB     0x0106

//QT_BEGIN_NAMESPACE
//namespace Ui { class MainWindow; }
//QT_END_NAMESPACE

class MainWindow : public mainBasewindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void mainCreateUI();


    //动画计时器
    QTimer *m_animationTimer;
    //线程
    QThread *m_childThread;

    //项目的个数
    QVector<QPushButton *>m_pro_btns;

    //当前插入设备的个数
    QVector<QPushButton *>m_currentPro_btns;

    //各种产品主页面
    window_pm481 *m_pm481_view;

    window_pd400 *m_pd400_view;

    window_dm90 *m_dm90_view;

    window_pm481 *m_pd200x_view;

    common_funtion m_co_fun;

    int m_current_languge_index =0;

    //是否已经显示了设备
    bool is_show_device =false;


public:
    QLabel *m_NoconnectView;

    QWidget *m_setup_view;
    //背景底图
    //mypushbutton *m_main_bgimage;
    QLabel *m_main_bgimage;
    //设备的名字
    QVector<QLabel *>m_deviceTitleArrs;
    //当前设备的名字
    QVector<QLabel *>m_currentDeviceTitleArrs;

    //设置页面的LABEL
    QVector<QLabel *>m_setupLabels;

    common_funtion m_commClass;
    //软件升级
    common_newwork_class *m_newwork_update;

signals:
    //发送数据
    void sendMsgSignals();

protected slots:
    void change_view();
    void device_enter_view(QString);
    void device_leave_view(QString);

    void pd400_back();
    void pm481_back();
    void dm90_back();
    void handleTimeout();

    void animationTimeout();
    //语言切换
    //void on_sel_language(int);
    //设置按钮
    void setup_clicked();
    //分享按钮的事件
    void share_btn_click();
    //设置视图的返回事件
    void setup_back();

    void YYfinished();

    void YYstarted();

    void YYstateChanged();


public slots:
    //切换语言
    void change_language(int index);
    void updateVersion_click();

    //切换按钮的方法
    void leftBtn_clicked();
    void rightBtn_clicked();


private:
    QPushButton *m_setup_back_btn;

    QPushButton *m_setup_btn;
    //软件的版本号
    //QString m_version_str;

    QLabel *m_updateLabel;

    QPushButton *m_update_btn;

    QLabel *m_languageLabel;
    //语言选择框
    QComboBox *m_cbo_sex;
    //设备的数量
    QVector<int> m_deviceArrs;

    double m_animationscale = 1;
    int m_animationscale_label = 0;

    //分页的视图
    QStackedWidget *m_mainSuperView;

    //切换的左按钮
    QPushButton *m_left_btn;
    //切换的右按钮
    QPushButton *m_right_btn;

    mainSubView *m_mainView1;

    mainSubView *m_mainView2;

private:
//    Ui::MainWindow *ui;
    QTranslator m_translator;

    //切换语言
    void changelanguage_sharebtn();

    //分享按钮的数组
    QVector<QPushButton *>m_share_btns;


    //window_mbs400 m_mbs400_view;
    //window_pd400 m_pd400_view;
    //window_pm481 m_pd481_view;
};
#endif // MAINWINDOW_H
