#pragma once
#pragma execution_character_set("utf-8")
#include "window_dm90.h"
#include <QStandardPaths>
#include <QFontDatabase>
#include <QApplication>
#include "dm90_devicedatasource.h"
#include "header_class.h"
#include "./HID/utils/CRC.h"

#define DM90MODELCODE "P0KT03"

#define DM70MODELCODE "P0KT04"

#define MICOutputGain 0x25

#define MICMUTE 0x22

#define HPOutputGain 0x27

//混音
#define PCMIX 0xc8
//指向模式
#define POlARPATTERN 0xda
//电平
#define MICLEVEL 0xC9
//LED灯光秀
#define LEDSWITCH 0xCB


window_dm90::window_dm90(QWidget *parent) : basewindow(parent)
{
    QIcon appIcon(":/image/images/logo.ico");
    QApplication::setWindowIcon(appIcon);
    //QFontDatabase::addApplicationFont(":/image/MBS_400/SourceHanSansCN Medium.ttf");
    setFixedSize(1023*m_Magnification,737*m_Magnification);

    //创建延时的计时器
    m_DM90delayTimer = new QTimer(this);
    connect(m_DM90delayTimer, SIGNAL(timeout()), this, SLOT(delayHandleTimeout()));

    m_setting_btn = new QCheckBox(this);
    m_setting_btn->hide();

    createUI();
    connect(m_titleBar, SIGNAL(signalButtonCloseClicked()), this, SLOT(back_view_click()));
}

void window_dm90::createUI(){

    m_setting_btn->show();
    m_setting_btn->resize(20*m_Magnification,20*m_Magnification);
    m_setting_btn->move(this->width()-40*m_Magnification,70*m_Magnification);
    QString setting_btn_stys=QString("QCheckBox::indicator{width: %1px;height: %1px;color: rgb(255, 0, 0);}"
                                     "QCheckBox::indicator:unchecked{border-image:url(:/image/images/pd400_setting_btn_default);}"
                                     "QCheckBox::indicator:unchecked:hover{border-image:url(:/image/images/pd400_setting_btn_hover);}"
                                     "QCheckBox::indicator:checked{border-image:url(:/image/images/pd400_setting_btn_press);}").arg(20*m_Magnification).arg(40*m_Magnification).arg(m_Magnification);
    m_setting_btn->setStyleSheet(setting_btn_stys);
    connect(m_setting_btn,SIGNAL(stateChanged(int)),this,SLOT(setting_change(int)));

    QVector<QString> mainBtnImage = {":/image/images/DM90/dm90_micbtn",":/image/images/DM90/dm90_high",":/image/images/DM90/dm90_lightingBtn"};
    for (int i =0;i<3;i++) {
        custom_button *main_btn = new custom_button(this);
        if(i==0){
            main_btn->resize(16*m_Magnification,26*m_Magnification);

        }else if(i==1){
            main_btn->resize(26*m_Magnification,26*m_Magnification);
        }else{
            main_btn->resize(26*m_Magnification,26*m_Magnification);
        }
        main_btn->move(30*m_Magnification + (main_btn->width()+30*m_Magnification)*i,75*m_Magnification);
        main_btn->setCheckable(true);
        QString mainBtn_stys=QString("QPushButton{border-image:url(%1);}"
                                     "QPushButton:hover{border-image:url(%1);}"
                                     "QPushButton:pressed{border-image:url(%2);}"
                                     "QPushButton:checked{border-image:url(%2);}"
                                     "QPushButton:focus{outline: none;}").arg(mainBtnImage.at(i)).arg(mainBtnImage.at(i)+"_select");
        main_btn->setStyleSheet(mainBtn_stys);

        main_btn->setObjectName(QString::number(1+i,10));
        connect(main_btn,SIGNAL(clicked()),this,SLOT(changeView_click()));
        m_main_btns.push_back(main_btn);
    }

    m_windowsStackedWidget = new QStackedWidget(this);
    m_windowsStackedWidget->resize(this->width(),600*m_Magnification);
    m_windowsStackedWidget->move(0,110*m_Magnification);

    QWidget *micView = new QWidget();
    micView->resize(this->width(),m_windowsStackedWidget->height());
    micView->move(0,0);

    m_leftView = new dm90_leftview(micView);
    m_leftView->move(10*m_Magnification,0*m_Magnification);
    m_leftView->resize(70*m_Magnification,600*m_Magnification);
    m_leftView->setStyleSheet("background:rgb(17,17,17);border-radius: 15px;");
    m_leftView->createUI();

    m_midView = new dm90_midview(micView);
    m_midView->move(90*m_Magnification,0*m_Magnification);
    m_midView->resize(180*m_Magnification,600*m_Magnification);
    //m_midView->setStyleSheet("background:rgb(17,17,17);border-radius: 15px;");
    m_midView->createUI();

    //右边的视图
    m_rightView = new dm90_rightview(micView);
    m_rightView->move(280*m_Magnification,0*m_Magnification);
    m_rightView->resize(730*m_Magnification,600*m_Magnification);
    m_rightView->setStyleSheet("background:rgb(17,17,17);border-radius: 15px;");
    m_rightView->createUI();

    m_windowsStackedWidget->addWidget(micView);
    //高级页面
    m_highView = new dm90_highView(this);
    m_highView->move(0,0);
    m_highView->resize(this->width(),m_windowsStackedWidget->height());
    m_windowsStackedWidget->addWidget(m_highView);
    m_highView->createUI();

    //灯光页面
    m_lightView = new dm90_lightView(this);
    m_lightView->move(0,0);
    m_lightView->resize(this->width(),m_windowsStackedWidget->height());
    m_windowsStackedWidget->addWidget(m_lightView);
    m_lightView->createUI();

    QWidget *maskView = new QWidget(this);
    maskView->resize(this->width(),this->height()-64*m_Magnification);
    maskView->move(0,64*m_Magnification);
    maskView->hide();
    m_rightView->m_maskView = maskView;
    m_rightView->createShowSignalView();


    //默认麦克风界面打开
    for (int i =0;i<m_main_btns.count();i++) {
        custom_button *main_btn = m_main_btns.at(i);
        main_btn->setChecked(false);
    }
    QPushButton *btn = m_main_btns.at(0);
    btn->setChecked(true);

    //默认为本地声音的列表
    m_midView->m_localCustom_itemList->m_objectName = "localSoundViewCustomPreset";

    //设置的页面
    m_setting_view = new pd400_setting_view(this);
    m_setting_view->move(0,0);
    m_setting_view->resize(this->width(),this->height());
    m_setting_view->createUI();
    m_setting_view->hide();
    connect(m_setting_view,SIGNAL(check_version()),this,SLOT(check_version_click()));
    connect(m_setting_view,SIGNAL(setting_mouse_press()),this,SLOT(setting_mouse_click()));
    connect(m_setting_view,SIGNAL(reset_clickBlock()),this,SLOT(reset_action()));
    //设置按钮在最上层
    m_setting_btn->raise();

    m_loadingView = new custom_loadingview(this->width(),this->height()-64*m_Magnification,this);
    m_loadingView->move(0,64*m_Magnification);
    m_loadingView->hide();

    //增加页面中的各种页面于页面信号槽
    connect(m_rightView,SIGNAL(rightView_addLocaltItem(int,int)),m_midView,SLOT(localAddItem(int,int)));
    connect(m_rightView,SIGNAL(rightView_addEffectItem(int,int)),m_midView,SLOT(effectAddItem(int,int)));
    connect(m_midView,SIGNAL(refreshLocalSoundUI(bool)),m_rightView,SLOT(clickItemRefreshUI(bool)));
}

void window_dm90::starConnectHid(int pid){

    if(pm481_devoperator){

        pm481_devoperator->pd400_get_bgdata.clear();
        pm481_devoperator->m_starThread = true;
        connect(this,SIGNAL(sendMsgSignals()),pm481_devoperator,SLOT(dm90_startContinuityReaddMsg()));
        //开启线程
        emit sendMsgSignals();

        bool is_connect = pm481_devoperator->starConnectDevice(pid);

        // bool is_connect = pm481_devoperator->starConnectDevice(0x0102);
        if(is_connect){//USB连接成功
            //插入的当前设备的PID
            is_modelconnect = true;
            is_usb_connect =true;
            currentProPid = pid;
            //先获取MODELCOLDE
            QString device_modecode = pm481_devoperator->getDeviceModelCodel();
        }else{
            qDebug()<<"USB连接失败";
            return;
        }
        //页面之间通讯的信号槽
        connect(pm481_devoperator, SIGNAL(connectDevice()), this, SLOT(OnConnectDevice()));//插入USB
        connect(pm481_devoperator, SIGNAL(disconnectDevice()), this, SLOT(OnDisConnectDevice()));//拔出USB
        connect(pm481_devoperator,SIGNAL(pd400_sysreceive_datasignal()),this,SLOT(receive_sys_data()));
        connect(pm481_devoperator,SIGNAL(dm90_blockModelCodel(QString)),this,SLOT(getModelCodel(QString)));//获取MODELCODEL
        connect(pm481_devoperator,SIGNAL(dm90_blockRefresh_mainUI()),this,SLOT(dm90_receiveRefresh_mainUI()));
        connect(pm481_devoperator,SIGNAL(DM90_synreceive_datasignal()),this,SLOT(receive_syn_data()));
    }

    //开启接收的线程
    m_receiveMessage_threa = NULL;
    if(!m_receiveMessage_threa){
        m_receiveMessage_threa = new dm90_receiveMessage_thread();
        m_receiveMessage_threa->m_receive_data = true;
        m_receiveMessage_threa->m_device_class = pm481_devoperator;
        m_receiveMessage_threa->start();
    }

    //DM90和DM70显示不同的界面
    if(pid == 0x0102){//DM90

    }else{

        m_leftView->hide();
        m_midView->move(10*m_Magnification,0*m_Magnification);
        m_rightView->move(200*m_Magnification,0*m_Magnification);
        m_rightView->resize(800*m_Magnification,600*m_Magnification);
    }
}
//获取MODELCODEL
void window_dm90::getModelCodel(QString modelCodel){

    if(modelCodel==DM90MODELCODE || modelCodel==DM70MODELCODE){//获取到了相同的MODEL，可以获取设备的背景参数

        pm481_devoperator->getsyncDeviceData();
    }
}
//刷新整个界面
void window_dm90::dm90_receiveRefresh_mainUI(){

    qDebug()<<"xxxx000000000"<<pm481_devoperator->m_DM90Request_device_data.count();

    for (int i=0;i<pm481_devoperator->m_DM90Request_device_data.count();i++) {
        //去偷去尾
        QByteArray device_datas_pack;
        device_datas_pack.append((char*)&pm481_devoperator->m_DM90Request_device_data.at(i), sizeof(pm481_devoperator->m_DM90Request_device_data.at(i)));

        //最后一包只有21个数据
        if(i==pm481_devoperator->m_DM90Request_device_data.count()-1){
            device_datas_pack = device_datas_pack.mid(10,42);
        }else{
            device_datas_pack = device_datas_pack.mid(10,52);
        }
        m_device_datas.append(device_datas_pack);
    }
    GET_DEVICE_DATA = new pc_DM90_device_dataSource();
    memset(GET_DEVICE_DATA,0,sizeof(pc_DM90_device_dataSource));

    //初始化用户本地声音默认预设
    GET_LOCAL_DATA = new pc_localSound_dataSource();
    memset(GET_DEVICE_DATA,0,sizeof(pc_localSound_dataSource));

    //初始化用户效果声音默认预设
    GET_EFFECT_DATA = new pc_effectSound_dataSource();
    memset(GET_EFFECT_DATA,0,sizeof(pc_effectSound_dataSource));

    //赋值给结构体
    GET_DEVICE_DATA = (pc_DM90_device_dataSource*)m_device_datas.data();
    //刷新整个界面的UI
    refreshMainUI();
}

//接收设备的同步信息
void window_dm90::receive_syn_data(){

    if(GET_DEVICE_DATA->dm90_PolarPattern>10){
       qDebug()<<"xxxx000000000bbbbbb"<<GET_DEVICE_DATA->dm90_PolarPattern;
       return;
    }

    if(pm481_devoperator->m_DM90syn_receive_data.param_addr[0]==MICMUTE){

        GET_DEVICE_DATA->dm90_micMute = pm481_devoperator->m_DM90syn_receive_data.param_num[0];
        m_rightView->refreshUI();
    }
    else if(pm481_devoperator->m_DM90syn_receive_data.param_addr[0]==MICOutputGain){

        GET_DEVICE_DATA->dm90_micOutputGain = pm481_devoperator->m_DM90syn_receive_data.param_num[1]<<8 | pm481_devoperator->m_DM90syn_receive_data.param_num[0];
        m_rightView->refreshUI();
    }else if(pm481_devoperator->m_DM90syn_receive_data.param_addr[0]==HPOutputGain){

        GET_DEVICE_DATA->dm90_HpOutputGain = pm481_devoperator->m_DM90syn_receive_data.param_num[1]<<8 | pm481_devoperator->m_DM90syn_receive_data.param_num[0];
        m_highView->refresh();
    }else if(pm481_devoperator->m_DM90syn_receive_data.param_addr[0]==PCMIX){
        GET_DEVICE_DATA->dm90_pc_mix = pm481_devoperator->m_DM90syn_receive_data.param_num[1]<<8 | pm481_devoperator->m_DM90syn_receive_data.param_num[0];
        m_highView->refresh();
    }else if(pm481_devoperator->m_DM90syn_receive_data.param_addr[0]==POlARPATTERN){
        GET_DEVICE_DATA->dm90_PolarPattern = pm481_devoperator->m_DM90syn_receive_data.param_num[1]<<8 | pm481_devoperator->m_DM90syn_receive_data.param_num[0];
        m_leftView->refreshUI();
    }else if(pm481_devoperator->m_DM90syn_receive_data.param_addr[0]==MICLEVEL){

        int micInput =pm481_devoperator->m_DM90syn_receive_data.param_num[1]<<8 | pm481_devoperator->m_DM90syn_receive_data.param_num[0];
        int micOutput =pm481_devoperator->m_DM90syn_receive_data.param_value[3]<<8 | pm481_devoperator->m_DM90syn_receive_data.param_value[2];
        m_rightView->refreshLevel(micInput,micOutput);
    }else if(pm481_devoperator->m_DM90syn_receive_data.param_addr[0]==LEDSWITCH){//灯光秀
        int switchStatus = pm481_devoperator->m_DM90syn_receive_data.param_num[0];
        int ledModel = pm481_devoperator->m_DM90syn_receive_data.param_value[2];
        int H = pm481_devoperator->m_DM90syn_receive_data.param_value[7]<<8 | pm481_devoperator->m_DM90syn_receive_data.param_value[6];
        int S = pm481_devoperator->m_DM90syn_receive_data.param_value[11]<<8 | pm481_devoperator->m_DM90syn_receive_data.param_value[10];
        int V = pm481_devoperator->m_DM90syn_receive_data.param_value[15]<<8 | pm481_devoperator->m_DM90syn_receive_data.param_value[14];
        m_lightView->refreshColorItem(switchStatus,ledModel,H,S,V);
    }

}
//开始刷新其他的页面
void window_dm90::refreshMainUI(){

    //刷新设置页面
    QString setupdevice_name;
    for (int i =0;i<4; i++) {
        setupdevice_name += QChar((GET_DEVICE_DATA->dm90_DeviceName[i])&0xff);
        setupdevice_name += QChar((GET_DEVICE_DATA->dm90_DeviceName[i]>>8)&0xff);
    }
    m_setting_view->m_device_name_label->setText(tr("产品型号: ")+setupdevice_name);

    //获取版本号
    uint16_t version_x=device_version_x= GET_DEVICE_DATA->dm90_FirmwareRev[0];
    uint16_t version_y=device_version_y = GET_DEVICE_DATA->dm90_FirmwareRev[1];
    uint16_t version_z=device_version_z = GET_DEVICE_DATA->dm90_FirmwareRev[2];
    version_str =QString("V")+QString::number(version_x,10) +"."+QString::number(version_y,10)+"."+QString::number(version_z,10);
    m_setting_view->m_version_label->setText(tr("固件版本: ")+version_str);

    //DSP的版本号
    uint16_t DSP_x=dsp_version_x  = GET_DEVICE_DATA->dm90_DSP1Rev[0];
    uint16_t DSP_y=dsp_version_y  = GET_DEVICE_DATA->dm90_DSP1Rev[1];
    uint16_t DSP_z=dsp_version_z  = GET_DEVICE_DATA->dm90_DSP1Rev[2];
    QString dsp_str = QString("V")+QString::number(DSP_x,10) +"."+QString::number(DSP_y,10)+"."+QString::number(DSP_z,10);
    m_setting_view->m_DSP_label->setText(tr("DSP版本: ")+dsp_str);

    //设备SN号
    QString SN_str_name="";
    for (int i=0;i<25;i++) {
        if(isascii((GET_DEVICE_DATA->dm90_Device_SN[i])&0xff))
            SN_str_name +=  QChar((GET_DEVICE_DATA->dm90_Device_SN[i])&0xff);
        if(isascii((GET_DEVICE_DATA->dm90_Device_SN[i]>>8)&0xff))
            SN_str_name += QChar((GET_DEVICE_DATA->dm90_Device_SN[i]>>8)&0xff);
    }
    m_setting_view->m_SN_label->setText(tr("设备SN: ")+SN_str_name);

    m_leftView->refreshUI();

    m_rightView->refreshUI();

    m_highView->refresh();
    m_lightView->refresh();
    //qDebug()<<"指向===="<<GET_DEVICE_DATA->dm90_micMute<<GET_DEVICE_DATA->dm90_micInputGain<<GET_DEVICE_DATA->dm90_PolarPattern;

}
void window_dm90::receive_sys_data(){

    QByteArray array_data;
    array_data.append((char*)&pm481_devoperator->m_sys_receive_data, sizeof(pm481_devoperator->m_sys_receive_data));
    uint8_t addr = pm481_devoperator->m_sys_receive_data.param_addr[0];
    uint16_t value =(((pm481_devoperator->m_sys_receive_data.param_value[1] & 0xff)<<8) | (pm481_devoperator->m_sys_receive_data.param_value[0] & 0xff)) &0xffff;
    qDebug()<<"dm90----"<<addr<<value;
}

//切换麦克风，高级，灯光的视图
void window_dm90::changeView_click(){

    for (int i =0;i<m_main_btns.count();i++) {
        custom_button *main_btn = m_main_btns.at(i);
        main_btn->setChecked(false);
    }

    QPushButton *btn = static_cast<QPushButton *>(sender());
    btn->setChecked(true);
    int btn_number = btn->objectName().toInt();
    m_windowsStackedWidget->setCurrentIndex(btn_number-1);

    Qwdget_timer_index = btn_number;

    if(btn_number==1){//麦克风


    }else if(btn_number==2){//高级页面

        m_highView->m_usercustom_itemList->m_objectName = "highViewCustomPreset";
        m_highView->m_usercustom_itemList->refreshUI();

    }else if(btn_number==3){//灯光页面

        //m_lightView->listRefreshColor();
    }
    //发送信息
    qDebug()<<"xxxxxkkkk"<<btn_number;

}

//计时器的方法=================
void window_dm90::delayHandleTimeout(){

    m_rightView->starDelayTimerFun();
    m_rightView->m_volumeEqView->VoluemeStarDelayTimerFun();
    m_rightView->m_thresholdView->thresholdStarDelayTimerFun();
    m_rightView->m_detonatorView->detonatorStarDelayTimerFun();//去爆音延时器
    m_rightView->m_compressorView->compressorStarDelayTimerFun();
    m_rightView->m_limiterView->limiterStarDelayTimerFun();
    m_rightView->m_dentalsoundView->dentalsoundStarDelayTimerFun();//齿音消除器
    m_rightView->m_flangerView->flangerStarDelayTimerFun();//镶边器
    m_rightView->m_toneView->toneStarDelayTimerFun();//音调
    m_rightView->m_reverbView->reverbStarDelayTimerFun();//混响
    m_rightView->m_delayView->delayStarDelayTimerFun();//延迟

    m_lightView->lightStarDelayTimerFun();
    m_highView->highStarDelayTimerFun();
}
void window_dm90::DM90_addContinuityReadData(const pc_continuity_read &data){


}

//插入USB
void window_dm90::OnConnectDevice(){

    qDebug()<<"usb插入------";
}
//拔出USB
void window_dm90::OnDisConnectDevice(){

    qDebug()<<"usb拔出------";
    pm481_devoperator->m_starThread = false;
    m_receiveMessage_threa->m_receive_data = false;
    device_disconnect_fun();
    emit Dm90BackClick();
    this->close();
    delete this;
}

void window_dm90::device_disconnect_fun(){

    is_usb_connect =false;
    is_modelconnect = false;
    pm481_devoperator->m_isreceiveSem.release();
    //清空背景参数的数组
    pm481_devoperator->m_DM90Request_device_data.clear();
    m_device_datas.clear();
    m_modelCodelName.clear();

    pd400_m_sendMsgs.clear();
    m_receive_pcParMsgs.clear();
    m_modelCodelName.clear();
}

void window_dm90::back_view_click(){

    pm481_devoperator->m_starThread = false;
    m_receiveMessage_threa->m_receive_data = false;
    device_disconnect_fun();
    emit Dm90BackClick();

#if defined(Q_OS_WIN32)
    this->close();
    delete this;
    pm481_devoperator->disconnect();
#else
    pm481_devoperator->disconnect();
#endif
}

void window_dm90::setting_change(int status){

    if(status){
        m_setting_view->show();
        m_rightView->m_recorderView->hide();
    }else{
        m_setting_view->hide();
        m_rightView->m_recorderView->show();
    }
}
void window_dm90::setting_mouse_click(){

    m_setting_btn->setCheckState(Qt::Unchecked);
    m_setting_view->hide();
}

void window_dm90::check_version_click(){
    m_setting_btn->setCheckState(Qt::Unchecked);
    m_setting_view->hide();
    m_rightView->m_recorderView->show();

}
//恢复出厂设置
void window_dm90::reset_action(){
    m_setting_btn->setCheckState(Qt::Unchecked);
    m_setting_view->hide();

    //恢复出厂设置
    is_usb_connect = true;

    QVector<int>addrs;
    QVector<int>value;

    addrs.push_back(0x100A);
    value.push_back(1);
    m_commin_fun.sendRandomMessageForDevice(0x0b,addrs,value);

//    delete GET_DEVICE_DATA;
//    GET_DEVICE_DATA =NULL;

//    delete GET_LOCAL_DATA;
//    GET_LOCAL_DATA =NULL;

//    delete GET_EFFECT_DATA;
//    GET_EFFECT_DATA =NULL;

    QThread::msleep(500);
    device_disconnect_fun();

    is_modelconnect = true;
    is_usb_connect =true;
    QString device_modecode = pm481_devoperator->getDeviceModelCodel();

}

