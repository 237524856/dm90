#ifndef WINDOW_MBS400_H
#define WINDOW_MBS400_H

#include <QMainWindow>
#include <thread>
#include "basewindow.h"
#include "mbs400_back_leftview.h"
#include "mbs400_back_midview.h"
#include "mbs400_back_rightview.h"
#include "../devoperator.h"

class window_mbs400 : public basewindow
{
    Q_OBJECT
public:
    explicit window_mbs400(QWidget *parent = nullptr);
    ~window_mbs400();

    QLabel *m_product_label;
    //控制通讯协议的对象
    DevOperator *m_devoperator;
    //线程
    QThread *m_childThread;   

private:
    mbs400_back_midview *m_mid_view;
    mbs400_back_leftview *m_left_view;
    mbs400_back_rightview *m_right_view;
    //void initTitleBar();
signals:
    //发送数据
    void sendMsgSignals();
public slots:
    //插入USB
    void OnConnectDevice();
    //拔出USB
    void OnDisConnectDevice();
    //点击了用户定义的列表，刷新数据
    void refresh_user_massage(int);

private slots:
    void receive_highswith_status(bool);
    void refresh_user_customaudio(void);
    void refresh_main_UI();

};

#endif // WINDOW_MBS400_H
