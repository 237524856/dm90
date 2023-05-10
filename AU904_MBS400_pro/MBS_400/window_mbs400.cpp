
#pragma once
#pragma execution_character_set("utf-8")
#include "window_mbs400.h"
#include <QPushButton>
#include <QApplication>
//#include "QDesktopWidget"
#include <QFontDatabase>
#include <QDebug>
#include <QStandardPaths>
#include "../common.h"

window_mbs400::window_mbs400(QWidget *parent) : basewindow (parent)
{

    //判断是否是第一次打开程序
    bool is_frist_open = m_common_fun->is_frist_open();
    if(!is_frist_open){//第一次打开
        QString filename = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+"/yangyu.dat";
        QFile file(filename);
        //读之前先要取出来
        if(file.open(QIODevice::WriteOnly)){
            //只写模式
            QDataStream out(&file);
            out << "";
            file.close();
        }
    }
    QFontDatabase::addApplicationFont(":/image/MBS_400/SourceHanSansCN Medium.ttf");
//    this->setMaximumSize(1023,737);  //设置窗体最大尺寸
//    this->setMinimumSize(1023,737);  //设置窗体最小尺寸
//    QDesktopWidget* desktopWidget = QApplication::desktop();
//    QRect deskRect = desktopWidget->availableGeometry();
//    double availableScreenX = deskRect.width();
//    double availableScreenY = deskRect.height();

//    Qt::WindowFlags flags = 0;
//    flags |= Qt::WindowMinMaxButtonsHint;
//    setWindowFlags(flags);
    setFixedSize(1023,737);

    //this->setWindowFlags(Qt::FramelessWindowHint);

    QLabel *imageLogo = new QLabel(this);
    imageLogo->resize(154*m_Magnification ,49*m_Magnification );
    imageLogo->move(10,500*m_Magnification );
    imageLogo->setPixmap(QPixmap(":/image/images/mbs400_maono.png"));
    imageLogo->setScaledContents(true);

    QPushButton *output_btn = new QPushButton(this);
    output_btn->resize(20,20);
    output_btn->move(20,75);
    output_btn->setStyleSheet("border-image:url(:/image/images/mbs400_output_btn.png)");

    QPushButton *sys_btn = new QPushButton(this);
    sys_btn->resize(100,25);
    sys_btn->move(904,72);
    sys_btn->setStyleSheet("QPushButton{border-image:url(:/image/images/mbs400_sys_btn.png) 0 100 0 0;}"
                           "QPushButton:pressed{border-image:url(:/image/images/mbs400_sys_btn.png) 0 0 0 100;}");

    //left view
    m_left_view = new mbs400_back_leftview(this);
    m_left_view->resize(170,593);
    m_left_view->move(8,100);
    m_left_view->show();
    m_left_view->new_createUI();

    m_mid_view = new mbs400_back_midview(this);
    m_mid_view->resize(346,593);
    m_mid_view->move(187,100);
    m_mid_view->show();
    m_mid_view->createUI();

    //right view
    m_right_view = new mbs400_back_rightview(this);
    m_right_view->resize(471,593);
    m_right_view->move(543,100);
    m_right_view->show();
    QVector<int>right_silder_values = {10,20,30,50,60,70,80,90,0,100};
    for (int i = 0;i<right_silder_values.count();i++) {
        m_right_view->m_right_sliderValues.push_back(right_silder_values.at(i));
    }
    m_right_view->new_createUI();
    //接收高级按钮的信号
    connect(m_right_view,SIGNAL(switch_status(bool)),this,SLOT(receive_highswith_status(bool)));
    // 初始化标题栏;
    //initTitleBar();

    //bottom button

    QVector<QString> share_btn_names = { "QPushButton{border-image:url(:/image/images/mbs400_fb_btn.png) 0 44 0 0;}"
                                         "QPushButton:hover{border-image:url(:/image/images/mbs400_fb_btn.png) 0 22 0 22;}"
                                         "QPushButton:pressed{border-image:url(:/image/images/mbs400_fb_btn.png) 0 0 0 44;}",
                                         "QPushButton{border-image:url(:/image/images/mbs400_tt_btn.png) 0 44 0 0;}"
                                         "QPushButton:hover{border-image:url(:/image/images/mbs400_tt_btn.png) 0 22 0 22;}"
                                         "QPushButton:pressed{border-image:url(:/image/images/mbs400_tt_btn.png) 0 0 0 44;}",
                                         "QPushButton{border-image:url(:/image/images/mbs400_ins_btn.png) 0 44 0 0;}"
                                         "QPushButton:hover{border-image:url(:/image/images/mbs400_ins_btn.png) 0 22 0 22;}"
                                         "QPushButton:pressed{border-image:url(:/image/images/mbs400_ins_btn.png) 0 0 0 44;}"};

    for (int i = 0;i< share_btn_names.count();i++) {

        QPushButton *share_buttom = new QPushButton(this);
        share_buttom->resize(22,22);
        share_buttom->move(20 + 48*i,this->height()-35);
        const QString str = share_btn_names.at(i);
        share_buttom->setStyleSheet(str);

    }

    m_product_label =new QLabel(this);
    m_product_label->resize(300,40);
    m_product_label->move(566,this->height()-40);
    m_product_label->setText("请插入USB设备");
    QFont font;
    font.setFamily("Source Han Sans CN Medium");
    font.setPixelSize(13);
    m_product_label->setFont(font);
    QPalette pa;
    QColor niceBlue(255, 0, 0);
    pa.setColor(QPalette::WindowText,niceBlue);
    m_product_label->setPalette(pa);

    QPushButton *upgrade_btn = new QPushButton(this);
    upgrade_btn->resize(100,26);
    upgrade_btn->move(this->width()-142,this->height()-35);
    upgrade_btn->setStyleSheet("border-image:url(:/image/images/mbs400_upgrade_btn.png);color:white;font-size:13px");
    upgrade_btn->setText("    固件升级");
    upgrade_btn->setFont(font);


    //收发数据，连接是否成功
    m_devoperator = new DevOperator();
    m_childThread = new QThread();
    //多线程
    m_devoperator->moveToThread(m_childThread);
    m_childThread->start();
    //使用信号-槽机制，发出开始指令
    connect(this,SIGNAL(sendMsgSignals()),m_devoperator,SLOT(startSendMsg()));

    //开启线程
    emit sendMsgSignals();

    bool is_connect = m_devoperator->connectHID();
    if(is_connect){//USB连接成功
        QPalette pa;
        pa.setColor(QPalette::WindowText,QColor(255, 255, 255));
        m_product_label->setPalette(pa);
        //获取背景参数
        isget_background_par = true;
        m_massge_control->receive_background_par();

        uint16_t version_num = 0x12;
        QString version_str = QString::number(version_num,16);
        QString version_name = version_str.mid(0,1) + "."+version_str.mid(1,1);
        QString base_name = "产品型号 MBS-400   |   固件版本：V";
        m_product_label->setText(base_name+version_name);

    }else{//USB连接失败
        qDebug()<<"拔出USB";
        m_product_label->setText("请插入USB设备");
        QPalette pa;
        pa.setColor(QPalette::WindowText,QColor(255, 0, 0));

        m_product_label->setPalette(pa);
    }
    //页面之间通讯的信号槽
    connect(m_devoperator, SIGNAL(connectDevice()), this, SLOT(OnConnectDevice()));
    connect(m_devoperator, SIGNAL(disconnectDevice()), this, SLOT(OnDisConnectDevice()));
    connect(m_right_view,SIGNAL(user_message()),this,SLOT(refresh_user_customaudio()));
    connect(m_left_view,SIGNAL(click_userlist_row(int)),this,SLOT(refresh_user_massage(int)));
    connect(m_devoperator,SIGNAL(refresh_main_signal()),this,SLOT(refresh_main_UI()));
}
window_mbs400::~window_mbs400(){
    delete m_mid_view;
    delete m_left_view;
    delete m_right_view;
    delete m_devoperator;
    m_childThread->quit();
    m_childThread->wait();
    delete m_childThread;
}


void window_mbs400::receive_highswith_status(bool is_open){

    if(is_open){
        m_mid_view->m_simple_mask_view->setVisible(true);
    }else{
        m_mid_view->m_simple_mask_view->setVisible(false);
    }

}
//刷新左视图用户的列表
void window_mbs400::refresh_user_customaudio(void){
    m_left_view->m_delete_datasource_index = -1;
    m_left_view->refresh_user_list();

}
//点击左视图刷新用户的信息
void window_mbs400::refresh_user_massage(int row){
    qDebug()<<"刷新了数据"<<row;
    m_right_view->refresh_data_forui(row);

}
//插入USB
void window_mbs400::OnConnectDevice(){

   qDebug()<<"插入USB";
   QPalette pa;
   pa.setColor(QPalette::WindowText,QColor(255, 255, 255));
   m_product_label->setPalette(pa);
   //获取所有的背景的参数
   isget_background_par = true;
   //QThread::msleep(500);
   //qDebug()<<"开始获取背景参数";
   m_massge_control->receive_background_par();
   uint16_t version_num = 0x12;
   QString version_str = QString::number(version_num,16);
   QString version_name = version_str.mid(0,1) + "."+version_str.mid(1,1);
   QString base_name = "产品型号 MBS-400   |   固件版本：V";
   m_product_label->setText(base_name+version_name);
}
//拔出USB
void window_mbs400::OnDisConnectDevice(){

    qDebug()<<"拔出USB";
    isget_background_par = false;
    m_product_label->setText("请插入USB设备");
    QPalette pa;
    pa.setColor(QPalette::WindowText,QColor(255, 0, 0));
    m_product_label->setPalette(pa);
}
//刷新所有的UI
void window_mbs400::refresh_main_UI(){
    //解析所有的背景参数
    m_mid_view->notice_refresh_ui();
    m_right_view->notice_refresh_ui();
    //刷新之后清空数据

//    qDebug()<<"刷新所有的UI"<<m_receiveMsgs[1].index[6];
}
