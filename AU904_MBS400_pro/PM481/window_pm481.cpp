#pragma once
#pragma execution_character_set("utf-8")

#include "window_pm481.h"
#include <QApplication>
//#include "QDesktopWidget"
#include <QFontDatabase>
#include <QThread>
#include <QDebug>
#include <QTreeWidget>
#include <QButtonGroup>
#include <QCheckBox>
#include <QDesktopServices>
#include <QMessageBox>
#include "../header_class.h"
#include "../mainwindow.h"
#include <QMap>
#include <QStandardPaths>
#include "networking_class.h"
#include <ctype.h>

window_pm481::window_pm481(QWidget *parent) : basewindow(parent)
{
    QIcon appIcon(":/image/images/logo.ico");
    QApplication::setWindowIcon(appIcon);
    //QFontDatabase::addApplicationFont(":/image/MBS_400/SourceHanSansCN Medium.ttf");
    setFixedSize(1023*m_Magnification,737*m_Magnification);
    is_needto_updatadsp=false;
    //    m_file_dsp="pp";
    //pm481_createUI();
    //连接设备
    //开始接收数据的线程
    //使用信号-槽机制，发出开始指令
    //    starConnectHid();
    m_setting_btn = new QCheckBox(this);
    m_setting_btn->hide();
    m_basic_btn = new custom_button(this);
    m_basic_btn->hide();
    m_high_btn = new custom_button(this);
    m_high_btn->hide();
    m_product_label =new QLabel(this);

    connect(m_titleBar, SIGNAL(signalButtonCloseClicked()), this, SLOT(back_click()));

    m_disableTimer = new QTimer(this);
    connect(m_disableTimer, SIGNAL(timeout()), this, SLOT(disableHandleTimeout()));


    m_PM481Timer = new QTimer(this);
    connect(m_PM481Timer, SIGNAL(timeout()), this, SLOT(sendLastHandleTimeout()));

    m_dfu_view = new dfu_0206_view();
    connect(m_dfu_view, SIGNAL(dsp_progress_value(int)), this, SLOT(dsp_kt0206_progress_val(int)));
    connect(m_dfu_view, SIGNAL(dsp_finish_progress(int)), this, SLOT(dsp_kt0206_finish(int)));

    m_starDspTimer = new QTimer(this);
    connect(m_starDspTimer, SIGNAL(timeout()), this, SLOT(starDspTimeout()));

    m_hearbeattimer = new QTimer(this);
    connect(m_hearbeattimer, SIGNAL(timeout()), this, SLOT(hearbeatTimeout()));
}
void window_pm481::starConnectHid(){

    if(pm481_devoperator){
        pm481_devoperator->m_starThread = true;
        connect(this,SIGNAL(sendMsgSignals()),pm481_devoperator,SLOT(pd400_startSendMsg()));
        //开启线程
        emit sendMsgSignals();
        if(pm481_devoperator->starConnectDevice(currentProPid)){
            is_modelconnect = true;

            //获取modelcodel
            //QThread::msleep(300);
            m_commin_fun.getModelCode();

            qDebug()<<"pm481USB连接成功";

        }else{
            is_usb_connect =false;
            qDebug()<<"pm481USB连接失败";
        }
        //数据跟页面之间通讯的信号槽
        connect(pm481_devoperator, SIGNAL(connectDevice()), this, SLOT(OnConnectDevice()));
        connect(pm481_devoperator, SIGNAL(disconnectDevice()), this, SLOT(OnDisConnectDevice()));
        connect(pm481_devoperator,SIGNAL(pd400_refresh_main_signal()),this,SLOT(refresh_main_UI()));
        connect(pm481_devoperator,SIGNAL(pd400_sysreceive_datasignal()),this,SLOT(receive_sys_data()));
        //进度条
        connect(pm481_devoperator,SIGNAL(pd400_progress_current_value(int)),this,SLOT(progress_value(int)));
        connect(pm481_devoperator,SIGNAL(pd400_break_fail_mess(int)),this,SLOT(sendMessage_fail_hand(int)));
        connect(pm481_devoperator,SIGNAL(blockDfuStatus(int)),this,SLOT(isDfuStatus(int)));
        connect(pm481_devoperator,SIGNAL(blockModelCodel()),this,SLOT(maingetModelCodel()));
    }
    m_MainTimer->stop();
    //发送数据的计时器
    m_pTimer = new QTimer(this);
    connect(m_pTimer, SIGNAL(timeout()), this, SLOT(handleTimeout()));
    m_pTimer->start(200);

    m_dfu_thread = NULL;
    if(!m_dfu_thread){
        m_dfu_thread = new DFU_thread();
        m_dfu_thread->isReceiveData = true;
        m_dfu_thread->m_ret = true;
        m_dfu_thread->m_dfu_mess = pm481_devoperator;
        m_dfu_thread->start();
    }
}
void window_pm481::clearDataSource(){
    m_mute_arrs.clear();
    m_synmicvolume_Slider_arrs.clear();
    m_synSlider_arrs.clear();
    m_micMon_arrs.clear();

}
void window_pm481::pm481_createUI(int deviceIndex){

    m_setting_btn->show();
    m_basic_btn->show();
    m_high_btn->show();

    clearDataSource();
//    QButtonGroup *m_buttonGroup = new QButtonGroup();
//    m_buttonGroup->setExclusive(true);

    //  m_basic_btn = new QPushButton(this);
    m_basic_btn->resize(80*m_Magnification,26*m_Magnification);
    m_basic_btn->move(15*m_Magnification,67*m_Magnification);
    m_basic_btn->setText(tr("基础"));
    QString basic_btn_stys=QString("QPushButton{border-image:url(:/image/images/pd400_ba_hi_btn.png);font-family:'Source Han Sans CN Medium';color:white;font-size:%1px;}"
                                   "QPushButton:hover{border-image:url(:/image/images/pd400_ba_hi_btn.png);font-family:'Source Han Sans CN Medium';color:rgb(54,54,54);font-size:%1px;}"
                                   "QPushButton:pressed{border-image:url(:/image/images/pd400_ba_hi_lig_btn.png);}"
                                   "QPushButton:checked{border-image:url(:/image/images/pd400_ba_hi_lig_btn.png);}"
                                   "QPushButton:focus{outline: none;}").arg(15*m_Magnification);
    m_basic_btn->setStyleSheet(basic_btn_stys);
    m_basic_btn->setCheckable(true);
    //m_basic_btn->setChecked(true);
    connect(m_basic_btn,SIGNAL(clicked()),this,SLOT(open_basic()));
    //    m_buttonGroup->addButton(m_basic_btn,0);

    //    m_high_btn = new QPushButton(this);

    m_high_btn->resize(80*m_Magnification,26*m_Magnification);
    m_high_btn->move(98*m_Magnification,67*m_Magnification);
    m_high_btn->setText(tr("高级"));
    m_high_btn->setStyleSheet(basic_btn_stys);
    m_high_btn->setCheckable(true);
    connect(m_high_btn,SIGNAL(clicked()),this,SLOT(open_high()));
    //    m_buttonGroup->addButton(m_high_btn,1);
    //    m_buttonGroup->buttonClicked(m_high_btn);
    //    connect(m_buttonGroup,SIGNAL(buttonClicked(int)),this,SLOT(change_status(int)));

    m_basic_btn->setDisabled(true);
    m_high_btn->setDisabled(true);

    //    m_setting_btn = new QCheckBox(this);
    m_setting_btn->resize(20*m_Magnification,20*m_Magnification);
    m_setting_btn->move(this->width()-40*m_Magnification,70*m_Magnification);
    QString setting_btn_stys=QString("QCheckBox::indicator{width: %1px;height: %1px;color: rgb(255, 0, 0);}"
                                     "QCheckBox::indicator:unchecked{border-image:url(:/image/images/pd400_setting_btn_default);}"
                                     "QCheckBox::indicator:unchecked:hover{border-image:url(:/image/images/pd400_setting_btn_hover);}"
                                     "QCheckBox::indicator:checked{border-image:url(:/image/images/pd400_setting_btn_press);}").arg(20*m_Magnification).arg(40*m_Magnification).arg(m_Magnification);
    m_setting_btn->setStyleSheet(setting_btn_stys);
    connect(m_setting_btn,SIGNAL(stateChanged(int)),this,SLOT(setting_change(int)));
    //m_setting_btn->setDisabled(true);
    //左边的视图
    m_left_view = new pm481_leftview(this);
    m_left_view->move(15*m_Magnification,96*m_Magnification);
    m_left_view->resize(496*m_Magnification,604*m_Magnification);
    m_left_view->createUI(deviceIndex);
    m_left_view->show();
    //右边的视图
    m_right_view = new pm481_rightview(this);
    m_right_view->move(518*m_Magnification,96*m_Magnification);
    m_right_view->resize(496*m_Magnification,604*m_Magnification);
    m_right_view->createUI();
    m_right_view->show();

    //设置的页面
    m_setting_view = new pd400_setting_view(this);
    m_setting_view->move(0,0);
    m_setting_view->resize(this->width(),this->height());
    m_setting_view->createUI();
    m_setting_view->hide();
    connect(m_setting_view,SIGNAL(change_languge(int)),this,SLOT(languge_click(int)));
    connect(m_setting_view,SIGNAL(check_version()),this,SLOT(check_version_click()));
    connect(m_setting_view,SIGNAL(setting_mouse_press()),this,SLOT(setting_mouse_click()));
    connect(m_setting_view,SIGNAL(reset_clickBlock()),this,SLOT(reset_action()));
    //设置按钮在最上层
    m_setting_btn->raise();

    m_check_view = new pd400_check_updateView(this);
    m_check_view->move(0,0);
    m_check_view->resize(this->width(),this->height());
    m_check_view->createUI();
    m_check_view->hide();
    connect(m_check_view,SIGNAL(updata_DFU()),this,SLOT(DFU_updata()));
    connect(m_check_view,SIGNAL(finish_download_dspfile()),this,SLOT(alone_updata_dsp()));

    //进度条的页面
    m_progress_view = new pd400_progress_view(this);
    m_progress_view->move(0,0);
    m_progress_view->resize(this->width(),this->height());
    m_progress_view->createUI();
    m_progress_view->hide();

    //  m_product_label =new QLabel(this);
    m_product_label->resize(650*m_Magnification,40*m_Magnification);
    m_product_label->move(216*m_Magnification,this->height()-40*m_Magnification);
    m_product_label->setText(tr("请插入USB设备"));
    QString product_label_stys=QString("font-family:'Source Han Sans CN Medium';color:red;font-size:%1px;").arg(15*m_Magnification);
    m_product_label->setStyleSheet(product_label_stys);
    m_product_label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
}
//刷新整个UI界面
void window_pm481::refresh_main_UI(){


    //先去掉相同得部分
    if(pm481_devoperator->m_PM481Request_device_datas.count()>=3){

        for(int i=0;i<pm481_devoperator->m_PM481Request_device_datas.size();i++){
            for(int j=i+1;j<pm481_devoperator->m_PM481Request_device_datas.size();j++){
                QByteArray array_data_front;
                array_data_front.append((char*)&pm481_devoperator->m_PM481Request_device_datas[j], sizeof(pm481_devoperator->m_PM481Request_device_datas[j]));
                QByteArray array_data_back;
                array_data_back.append((char*)&pm481_devoperator->m_PM481Request_device_datas[i], sizeof(pm481_devoperator->m_PM481Request_device_datas[i]));
                if(array_data_front==array_data_back){
                    pm481_devoperator->m_PM481Request_device_datas.remove(j);
                    j--;
                }
            }
        }
    }


    //更新数据到对象中
    for (int i=0;i<pm481_devoperator->m_PM481Request_device_datas.count();i++) {
        //去偷去尾
        QByteArray device_datas_pack;
        device_datas_pack.append((char*)&pm481_devoperator->m_PM481Request_device_datas.at(i), sizeof(pm481_devoperator->m_PM481Request_device_datas.at(i)));


        //最后一包只有21个数据
//        if(i==pm481_devoperator->m_DM90Request_device_data.count()-1){
//            device_datas_pack = device_datas_pack.mid(10,42);
//        }else{
            device_datas_pack = device_datas_pack.mid(10,52);
//        }
        m_pm481_device_datas.append(device_datas_pack);
    }

    GET_PM481_DEVICE_DATA = new pc_PM481_device_dataSource();
    memset(GET_PM481_DEVICE_DATA,0,sizeof(pc_PM481_device_dataSource));
    //赋值给结构体
    GET_PM481_DEVICE_DATA = (pc_PM481_device_dataSource*)m_pm481_device_datas.data();
    //刷新整个界面的UI
    qDebug()<<"xxxxx"<<GET_PM481_DEVICE_DATA->PM481_micBaseGain;


    m_setting_btn->setDisabled(false);


    QString device_name  = tr("已连接");
    device_name += " ";
    QString setupdevice_name;
    for (int i =0;i<4; i++) {
        device_name += QChar((GET_PM481_DEVICE_DATA->PM481_DeviceName[i])&0xff);
        device_name += QChar((GET_PM481_DEVICE_DATA->PM481_DeviceName[i]>>8)&0xff);

        setupdevice_name += QChar((GET_PM481_DEVICE_DATA->PM481_DeviceName[i])&0xff);
        setupdevice_name += QChar((GET_PM481_DEVICE_DATA->PM481_DeviceName[i]>>8)&0xff);
    }
    device_name += tr("设备");
    m_setting_view->m_device_name_label->setText(tr("产品型号: ")+setupdevice_name);
    m_product_label->setText(device_name);
    QString product_label_stys=QString("font-family:'Source Han Sans CN Medium';color:white;font-size:%1px;").arg(15*m_Magnification);
    m_product_label->setStyleSheet(product_label_stys);


    //开启各种东西
    m_basic_btn->setDisabled(false);
    m_high_btn->setDisabled(false);
    setSliderDisabled(false);

    for (int i=0;i<m_left_view->m_switch_btns.count();i++) {

        QPushButton *btn = m_left_view->m_switch_btns.at(i);
        btn->setDisabled(false);
    }

    //MCU的版本号
    uint16_t version_x=device_version_x = GET_PM481_DEVICE_DATA->PM481_FirmwareRev[0];
    uint16_t version_y=device_version_y = GET_PM481_DEVICE_DATA->PM481_FirmwareRev[1];
    uint16_t version_z=device_version_z = GET_PM481_DEVICE_DATA->PM481_FirmwareRev[2];
    version_str =QString("V")+QString::number(version_x,10) +"."+QString::number(version_y,10)+"."+QString::number(version_z,10);
    m_setting_view->m_version_label->setText(tr("固件版本: ")+version_str);


    //DSP的版本号
    uint16_t DSP_x=dsp_version_x = GET_PM481_DEVICE_DATA->PM481_DSP1Rev[0];
    uint16_t DSP_y=dsp_version_y = GET_PM481_DEVICE_DATA->PM481_DSP1Rev[1];
    uint16_t DSP_z=dsp_version_z = GET_PM481_DEVICE_DATA->PM481_DSP1Rev[2];

    QString dsp_str = QString("V")+QString::number(DSP_x,10) +"."+QString::number(DSP_y,10)+"."+QString::number(DSP_z,10);
    qDebug()<<"ddddsp====="<<dsp_str;
    m_setting_view->m_DSP_label->setText(tr("DSP版本: ")+dsp_str);

    //设备SN号
    QString SN_str_name="";
    for (int i=0;i<25;i++) {
        if(isascii((GET_PM481_DEVICE_DATA->PM481_Device_SN[i])&0xff))
        SN_str_name +=  QChar((GET_PM481_DEVICE_DATA->PM481_Device_SN[i])&0xff);
        if(isascii((GET_PM481_DEVICE_DATA->PM481_Device_SN[i]>>8)&0xff))
        SN_str_name += QChar((GET_PM481_DEVICE_DATA->PM481_Device_SN[i]>>8)&0xff);
    }
    m_setting_view->m_SN_label->setText(tr("设备SN: ")+SN_str_name);

    //静音
    uint8_t mute =  GET_PM481_DEVICE_DATA->PM481_micMute;
    //耳机音量
    uint8_t headphone_volume = GET_PM481_DEVICE_DATA->PM481_HPGain;
    //监听混响比例
    uint16_t mix_value = GET_PM481_DEVICE_DATA->PM481_mix;
    uint8_t micMon = (mix_value-2000) /10;
    //增益
    uint8_t gain = GET_PM481_DEVICE_DATA->PM481_micBaseGain;
    uint8_t yinse = GET_PM481_DEVICE_DATA->PM481_Tone;
    //用户模式
    uint8_t user_model =  GET_PM481_DEVICE_DATA->PM481_UserMode;
    uint8_t rgb_switch =  GET_PM481_DEVICE_DATA->PM481_LedShowOnOff;
    uint8_t rgb_adj =     GET_PM481_DEVICE_DATA->PM481_LedBright;
    uint8_t rgb_number =  GET_PM481_DEVICE_DATA->PM481_LedShowMode;

    //均衡器
    uint8_t eq_value =  GET_PM481_DEVICE_DATA->PM481_micMode;
    //限制器
    uint16_t limit = GET_PM481_DEVICE_DATA->PM481_limitThreshold;
    //压缩器
    uint16_t comp = GET_PM481_DEVICE_DATA->PM481_compThreshold;

    //2个使能
    uint8_t limit_enable = GET_PM481_DEVICE_DATA->PM481_limitEnable;
    uint8_t comp_enable = GET_PM481_DEVICE_DATA->PM481_compEnable;

    m_current_model = user_model + 1;
    //用户模式
    if(user_model==0){//基础模式
        m_basic_btn->blockSignals(true);
        //m_basic_btn->setCheckable(true);
        m_basic_btn->setChecked(true);
        m_basic_btn->blockSignals(false);
        //显示蒙版
        m_left_view ->m_basic_mask->hide();
        m_right_view->m_high_mask->show();

    }else{//高级模式
        m_high_btn->blockSignals(true);
        //m_high_btn->setCheckable(true);
        m_high_btn->setChecked(true);
        m_high_btn->blockSignals(false);
        //显示蒙版
        m_left_view ->m_basic_mask->show();
        m_right_view->m_high_mask->hide();
    }

    m_left_view->m_fullrgb_mask->hide();
    m_left_view->refresh_ui(mute,headphone_volume,gain,yinse,rgb_switch,rgb_adj,rgb_number,micMon);
    m_right_view->refresh_ui(mute,headphone_volume,gain,eq_value,limit_enable,limit,comp_enable,comp,micMon);

    //发送同步标志
    m_commin_fun.sys_Software_tag(0);

#if defined(Q_OS_WIN32)
    //开启心跳
    //m_hearbeattimer->start(700);
#else

#endif

    is_resetFaocClick = false;


}
//
void window_pm481::receive_sys_data(){

    if(is_usb_connect){
        QByteArray array_data;
        array_data.append((char*)&pm481_devoperator->m_sys_receive_data, sizeof(pm481_devoperator->m_sys_receive_data));
        uint8_t addr = pm481_devoperator->m_sys_receive_data.param_addr[0];
        uint16_t currentValue =(((pm481_devoperator->m_sys_receive_data.param_value[1] & 0xff)<<8) | (pm481_devoperator->m_sys_receive_data.param_value[0] & 0xff)) &0xffff;
        //防止返回的当前值跟调试的值冲突
        // send_radom_data.command = 0x04;
        if(pm481_devoperator->m_sys_receive_data.command == 0x03){
            //pd400_send_value = currentValue;
            // qDebug()<<"打印数据------"<<pm481_devoperator->m_sys_receive_data.param_addr[1]<<pm481_devoperator->m_sys_receive_data.param_value[0];
            uint16_t level_value =(((pm481_devoperator->m_sys_receive_data.param_value[1])<<8) | (pm481_devoperator->m_sys_receive_data.param_value[0])) &0xffff;
            //麦克风监听
            m_left_view->sys_receive_data(pm481_devoperator->m_sys_receive_data.param_addr[0],pm481_devoperator->m_sys_receive_data.param_value[0],
                    pm481_devoperator->m_sys_receive_data.crc_L,pm481_devoperator->m_sys_receive_data.pars[0],level_value);
            m_right_view->sys_receive_data(pm481_devoperator->m_sys_receive_data.param_addr[0],pm481_devoperator->m_sys_receive_data.param_value[0],level_value);
        }
    }
}
//基础
void window_pm481::open_basic(){
    m_high_btn->setChecked(false);
    m_basic_btn->setChecked(true);
    m_right_view->m_high_mask->show();
    m_left_view->m_basic_mask->hide();
    if(m_current_model ==1)return;
    m_current_model = 1;
    m_commin_fun.change_basic_high_Model(0);

    //重置限制器，压缩器
    m_commin_fun.DM30_set_basic();
    for (int i=0;i<m_right_view->enable_btns.count();i++) {
        m_right_view->enable_btns.at(i)->blockSignals(true);
        m_right_view->enable_btns.at(i)->setCheckState(Qt::Unchecked);
        m_right_view->enable_btns.at(i)->blockSignals(false);
    }
    for (int i=0;i<2;i++) {
        m_right_view->sliders.at(i)->blockSignals(true);
        if(i==0){
            m_right_view->sliders.at(i)->setValue(40);
        }else{
            m_right_view->sliders.at(i)->setValue(80);
        }
        QString slider_stys=QString("QSlider::groove:horizontal{background: rgba(0, 0, 0,100);height:%2px;border: 0px solid #FFFFFF;border-image:url(:/image/images/pd400_right_slider_bgimg.png);}"
                                    "QSlider::handle:horizontal{width:%1px;height:%1px;margin-top: -%2px;margin-left: 0px;margin-bottom: -%2px;margin-right: 0px;border-image:url(:/image/images/pd400_round.png);}"
                                    "QSlider::sub-page:horizontal{background-color:rgba(0,0,0,100);border-radius: %2px;border-image:url(:/image/images/pd400_right_addslider_disabled.png);}").arg(12*m_Magnification).arg(4*m_Magnification);
        m_right_view->sliders.at(i)->setStyleSheet(slider_stys);
        m_right_view->sliders.at(i)->blockSignals(false);
        m_right_view->m_comp_limiter_lables.at(i)->setText("0dB");
    }
    //音色调节
    if(m_left_view->m_current_yinse_number=="dichen"){
        m_commin_fun.pd400_yinse(0);
    }else if(m_left_view->m_current_yinse_number=="ziran"){
        m_commin_fun.pd400_yinse(1);
    }else if(m_left_view->m_current_yinse_number=="mingliang"){
        m_commin_fun.pd400_yinse(2);
    }else{
        m_commin_fun.pd400_yinse(3);
    }
    m_high_btn->setDisabled(true);
    m_disableTimer->stop();
    m_disableTimer->start(300);
}
//高级
void window_pm481::open_high(){

    m_high_btn->setChecked(true);
    m_basic_btn->setChecked(false);
    m_right_view->m_high_mask->hide();
    m_left_view->m_basic_mask->show();
    if(m_current_model ==2)return;
    m_current_model = 2;
    m_commin_fun.change_basic_high_Model(1);
    //发送均衡器模式的命令
    int eq_current_number = m_right_view->m_current_btn_select.toInt()-200;
    m_commin_fun.pd400_mic(eq_current_number);
    //重置限制器，压缩器
    m_commin_fun.DM30_set_basic();
    for (int i=0;i<m_right_view->enable_btns.count();i++) {
        m_right_view->enable_btns.at(i)->blockSignals(true);
        m_right_view->enable_btns.at(i)->setCheckState(Qt::Unchecked);
        m_right_view->enable_btns.at(i)->blockSignals(false);
    }
    for (int i=0;i<2;i++) {
        m_right_view->sliders.at(i)->blockSignals(true);
        if(i==0){
            m_right_view->sliders.at(i)->setValue(40);
        }else{
            m_right_view->sliders.at(i)->setValue(80);
        }
        m_right_view->sliders.at(i)->blockSignals(false);
        m_right_view->sliders.at(i)->setSliderDisable(true);
        m_right_view->m_comp_limiter_lables.at(i)->setText("0dB");
    }
    m_basic_btn->setDisabled(true);
    m_disableTimer->stop();
    m_disableTimer->start(300);
}
void window_pm481::setting_change(int status){

    if(status){
        m_setting_view->show();
    }else{
        m_setting_view->hide();
    }
}
void window_pm481::languge_click(int){

}
//设置页面的检查更新，确认固件升级===============================================================
void window_pm481::check_version_click(){

    m_setting_btn->setCheckState(Qt::Unchecked);
    m_setting_view->hide();

    //判断是那个设备，根据不同得设备下载不同地址的文件

    QString MCU_ver_str;
    QString MCU_file_num;
    QUrl MCU_url;

    QString DSP_ver_str;
    QString DSP_file_num;
    QUrl DSP_url;

    if(currentProPid==0x0101){//DM30
        //mcu部分
        MCU_ver_str = "http://159.75.118.131/upinfo_test.aspx?proj_name=DM30";
        MCU_file_num = "/DM30_";
        MCU_url = QUrl("http://159.75.118.131/dnfile_test.aspx?f_sn=17");

        //dsp部分
        DSP_ver_str = "http://159.75.118.131/upinfo_test.aspx?proj_name=DM30_dsp";
        DSP_file_num = "/DM30_dsp";
        DSP_url = QUrl("http://159.75.118.131/dnfile_test.aspx?f_sn=15");

    }else if(currentProPid==0x0104){//PD200X
        //mcu部分
        MCU_ver_str = "http://159.75.118.131/upinfo_test.aspx?proj_name=PD200X";
        MCU_file_num = "/PD200X_";
        MCU_url = QUrl("http://159.75.118.131/dnfile_test.aspx?f_sn=27");

        //dsp部分
        DSP_ver_str = "http://159.75.118.131/upinfo_test.aspx?proj_name=PD200X_dsp";
        DSP_file_num = "/PD200X_dsp";
        DSP_url = QUrl("http://159.75.118.131/dnfile_test.aspx?f_sn=28");
    }else if(currentProPid==0x0106){//DM30 RGB
        //mcu部分
        MCU_ver_str = "http://159.75.118.131/upinfo_test.aspx?proj_name=DM30RGB";
        MCU_file_num = "/DM30RGB_";
        MCU_url = QUrl("http://159.75.118.131/dnfile_test.aspx?f_sn=29");

        //dsp部分
        DSP_ver_str = "http://159.75.118.131/upinfo_test.aspx?proj_name=DM30RGB_DSP";
        DSP_file_num = "/DM30RGB_dsp";
        DSP_url = QUrl("http://159.75.118.131/dnfile_test.aspx?f_sn=30");
    }

    //首先获取网络请求下来的数据，解析BIN文件和设备的版本号对比
    m_dm30downloaddfu_number = 0;
    m_check_view->m_model_codel = DM30;
    is_needto_updatadsp = false;
    is_dm30updata_runing = false;
    //先判断是否需要下载
    QString ver_str = networking_class::getInstance()->network_updateversion(MCU_ver_str);
    QStringList ver_arrs = ver_str.split(".");
    if(ver_arrs.count()>=3){
        int net_dsp_version = ver_arrs.at(0).toInt()*1000+ver_arrs.at(1).toInt()*100+ver_arrs.at(2).toInt()-1000;
        int dsp_version = device_version_x*1000+device_version_y*100+device_version_z;
        if(net_dsp_version > dsp_version){//下载升级
            QString file = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+MCU_file_num+ver_str+".bin";
            m_check_view->set_bindfu_file(file);
            m_check_view->download_bin_file(QUrl(MCU_url),file);
            is_dm30updata_runing =true;
            m_progress_type = MCU;
        }else{//判断DSP固件的版本是否需要升级
            m_dm30downloaddfu_number = 100;
            //判断dsp的版本号
            QString ver_str = networking_class::getInstance()->network_updateversion(DSP_ver_str);
            QStringList ver_arrs = ver_str.split(".");
            if(ver_arrs.count()>=3){
                int net_dsp_version = ver_arrs.at(0).toInt()*10000+ver_arrs.at(1).toInt()*1000+ver_arrs.at(2).toInt();
                int dsp_version = dsp_version_x*10000+dsp_version_y*1000+dsp_version_z;
                if(net_dsp_version > dsp_version){//下载DSP固件
                    m_hearbeattimer->stop();
                    m_dm30file_dsp = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+DSP_file_num+ver_str+".bin";
                    //this->set_bindfu_file(file_dsp);
                    m_check_view->download_bin_file(DSP_url,m_dm30file_dsp);
                    //qDebug() <<"22222xxxxxxxxx"<<"下载完成";
                    is_needto_updatadsp = true;
                    is_dm30updata_runing =true;
                }else{
                    m_check_view->set_diff_status(2);
                }
            }
        }
        qDebug()<<"xx9999xx"<<net_dsp_version<<dsp_version;
    }

    // m_check_view->set_diff_status(2);
    //m_check_view->show();

    //为了适配除本机外其他电脑也能打开文件，暂无知道原因
//    emit dfuReadFileData();
}
void window_pm481::DFU_updata(){

    if(!is_usb_connect)return;
    m_hearbeattimer->stop();
    if(m_check_view->m_dataSourceList.count()<1)return;

    pm481_devoperator->dfu_send_packNumber = m_check_view->m_dataSourceList.count()-1;
    pm481_devoperator->dfu_send_current_packNumber = 0;
    m_file_datas.clear();

    qDebug()<< m_check_view->m_dataSourceList.count()<<"xxxllllll";
    //QList<QByteArray> my_list_data;
    // m_dfu_thread->m_dfu_dataList = m_check_view->m_dataSourceList;
    QByteArray byte_number;
    byte_number.append(63, char(0));
    byte_number.replace(0,11,m_check_view->m_dataSourceList.at(0));
    byte_number.insert(0,0X4b);
    m_file_datas.push_back(byte_number);

    for (int i=1;i<m_check_view->m_dataSourceList.count();i++) {

        QByteArray send_data = m_check_view->m_dataSourceList.at(i);
        //分成多包发送
        //是否需要补0
        int add_data = send_data.count()%63;
        if(add_data !=0){//后面补0

            for (int j =0;j<(63-add_data);j++) {
                send_data.append(static_cast<char>(0));
            }
        }
        for (int n=0;n<send_data.count()/63;n++) {
            QByteArray send_mess = send_data.mid(n*63,63);
            send_mess.insert(0,0X4b);

            //if(!pm481_devoperator->m_pc_isconnect)return;
            QString str;
            for (int k=0;k<64;k++) {
                uint8_t a = send_mess[k] & 0xff;
                str += " "+QString::number(a,16);
            }
            //qDebug()<<"send_message"<<str;
            m_file_datas.push_back(send_mess);
            //pm481_devoperator->send_updata_DFU_message((unsigned char*)send_mess.data());
        }
        //
    }
    pm481_devoperator->m_dfu_dataList.clear();
    pm481_devoperator->m_dfu_dataList = m_file_datas;

    //判断是否是DSP跟MCU一起升级
    if(is_needto_updatadsp){
        m_progress_view->m_progress_view->setMaximum(m_file_datas.count()+105);
        m_progress_view->m_progress_view->setValue(0);
    }else{
        m_progress_view->m_progress_view->setMaximum(m_file_datas.count());
        m_progress_view->m_progress_view->setValue(0);
    }

    m_progress_view->show();
}
void window_pm481::progress_value(int current_value){

    //m_progress_view->m_progress_view->setMaximum(m_file_datas.count()-1);


    switch (m_progress_type) {
    case MCU:{

        if(is_needto_updatadsp){
            m_progress_view->m_progress_view->setValue(current_value);
            int value =static_cast<int>((static_cast<double>(current_value) / static_cast<double>(m_file_datas.count()+105))*100) ;
            m_progress_view->m_current_value->setText(QString::number(value,10)+"%");
        }else{
            m_progress_view->m_progress_view->setValue(current_value);
            int value =static_cast<int>((static_cast<double>(current_value) / static_cast<double>(m_file_datas.count()))*100) ;
            //qDebug()<<"sxxxxxxxxxxxxxxxend_message"<<value<<current_value<<m_file_datas.count();
            m_progress_view->m_current_value->setText(QString::number(value,10)+"%");
        }
    }
        break;
    case DSP:{
        m_progress_view->m_progress_view->setValue(current_value);
        int value =static_cast<int>((static_cast<double>(current_value) / static_cast<double>(m_dfu_view->m_total_lengh))*100) ;
        qDebug()<<current_value <<m_dfu_view->m_total_lengh<<value;
        m_progress_view->m_current_value->setText(QString::number(value,10)+"%");
    }
        break;
    default:
    {

    }
    }
    //    if(current_value >=(m_file_datas.count()))
    //        m_progress_view->hide();
}
//固件或者成功了
void window_pm481::sendMessage_fail_hand(int status){

    m_hearbeattimer->stop();
    QString connect_str = tr("发送数据错误，错误码：");
    if(status==1){
        m_progress_view->hide();
        is_dm30updata_runing =false;
        //打开所有得蒙版
        device_disconnect_fun();
        m_product_label->setText(tr("请重新连接设备"));
        QString product_label_stys=QString("font-family:'Source Han Sans CN Medium';color:red;font-size:%1px;").arg(15*m_Magnification);
        m_product_label->setStyleSheet(product_label_stys);
        QMessageBox::information(NULL, tr("提示"), tr("升级失败"),QMessageBox::Yes);
        back_click();

    }else if(status==2){//mcu升级之后再升级DSP

        dsp_updata();
    }else{
        m_progress_view->hide();
        is_pd400updata_runing =false;
        //打开所有得蒙版
        device_disconnect_fun();

        if(is_resetFaocClick){
           m_product_label->setText(tr("已恢复出厂设置，请重新连接设备"));
        }else{
          m_product_label->setText(tr("设备连接失败,请重新连接"));
        }
        QString product_label_stys=QString("font-family:'Source Han Sans CN Medium';color:red;font-size:%1px;").arg(15*m_Magnification);
        m_product_label->setStyleSheet(product_label_stys);
        //QMessageBox::information(NULL, tr("提示"), connect_str + QString::number(status,10),QMessageBox::Yes);
    }
}
//DSP升级
void  window_pm481::dsp_updata(){

    qDebug()<<"xkxkxkxkxk"<<m_dm30file_dsp<<is_needto_updatadsp;
    //判断是否需要升级
    if(is_needto_updatadsp){//升级DSP
        QFile file(m_dm30file_dsp);
        if(!file.open(QIODevice::ReadOnly)){
            QMessageBox::information(NULL, "Title", "打开错误+"+file.errorString(),
                                     QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
            return;
        }
        m_starDspTimer->start(2000);

    }else{//不需要升级DSP

        //打开所有得蒙版
        device_disconnect_fun();
        m_product_label->setText(tr("请重新连接设备"));
        QString product_label_stys=QString("font-family:'Source Han Sans CN Medium';color:red;font-size:%1px;").arg(15*m_Magnification);
        m_product_label->setStyleSheet(product_label_stys);
        m_progress_view->hide();

        QMessageBox msg;
        msg.setWindowTitle(tr("提示"));
        msg.setText(tr("升级成功"));
        QPushButton *noButton = msg.addButton(tr("是"),QMessageBox::ActionRole);
        msg.addButton(QMessageBox::No);
        msg.button(QMessageBox::No)->setHidden(true);
        msg.setDefaultButton(noButton);
        msg.exec();

        QThread::msleep(1000);
        is_dm30updata_runing =false;
    }
}
void window_pm481::starDspTimeout(){

    if(pm481_devoperator->m_pid == currentProPid){
        m_dfu_view->m_pid = pm481_devoperator->m_pid;
        m_dfu_view->startNewExe(m_dm30file_dsp);
    }else{
        dsp_kt0206_finish(-1);
    }
    m_starDspTimer->stop();
}
//单独升级dsp
void window_pm481::alone_updata_dsp(){

    if(is_needto_updatadsp){//升级DSP
        QFile file(m_dm30file_dsp);
        if(!file.open(QIODevice::ReadOnly)){
            QMessageBox::information(NULL, "Title", "打开错误+"+file.errorString(),
                                     QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
            return;
        }
        m_progress_view->m_progress_view->setValue(0);
        m_progress_view->m_current_value->setText(QString::number(0,10)+"%");
        m_progress_type = DSP;
        m_progress_view->show();
        m_dfu_view->m_pid = pm481_devoperator->m_pid;
        m_dfu_view->startNewExe(m_dm30file_dsp);
    }
}
void window_pm481::dsp_kt0206_progress_val(int value){

    if(m_progress_type == MCU){
        progress_value(value + m_file_datas.count());
    }else{
        m_progress_view->m_progress_view->setMaximum(m_dfu_view->m_total_lengh);
        progress_value(value);
    }
}
void window_pm481::dsp_kt0206_finish(int status){

    m_progress_view->hide();

    //打开所有得蒙版
    device_disconnect_fun();
    m_product_label->setText(tr("请重新连接设备"));
    QString product_label_stys=QString("font-family:'Source Han Sans CN Medium';color:red;font-size:%1px;").arg(15*m_Magnification);
    m_product_label->setStyleSheet(product_label_stys);

    if(status ==1){//成功
        QMessageBox msg;
        msg.setWindowTitle(tr("提示"));
        msg.setText(tr("升级成功,请重新连接设备"));
        QPushButton *noButton = msg.addButton(tr("是"),QMessageBox::ActionRole);
        msg.addButton(QMessageBox::No);
        msg.button(QMessageBox::No)->setHidden(true);
        msg.setDefaultButton(noButton);
        msg.exec();
    }else{//失败
        QMessageBox msg;
        msg.setWindowTitle(tr("提示"));
        msg.setText(tr("升级失败"));
        QPushButton *noButton = msg.addButton(tr("是"),QMessageBox::ActionRole);
        msg.addButton(QMessageBox::No);
        msg.button(QMessageBox::No)->setHidden(true);
        msg.setDefaultButton(noButton);
        msg.exec();
    }
    //QThread::msleep(500);
    is_dm30updata_runing =false;
    back_click();

}
void window_pm481::open_basic_orhigh(){

}
void window_pm481::handleTimeout(){

    //qDebug()<<"------xxxxxxx酷酷酷酷酷"<<pd400_send_value<<pd400_diffsend_value<<send_radom_data.command<<send_radom_data.param_addr[0];
    QMap<QString,int>::iterator send_value_iter = DM30_send_value.find(QString::number(send_radom_data.param_addr[0],16));
    QMap<QString,int>::iterator different_value_iter = DM30_different_value.find(QString::number(send_radom_data.param_addr[0],16));
    //    if(pd400_diffsend_value != pd400_send_value){
    //        if(is_usb_connect)
    //            //当数据为自己写的时候才发送
    //            if(send_radom_data.command == 0x03){
    //                pd400_m_sendMsgs.push_back(send_radom_data);
    //                //qDebug()<<"xxxxxxx酷酷酷酷酷"<<pd400_send_value;
    //            }
    //    }
    //  pd400_diffsend_value = pd400_send_value;
    if(different_value_iter.value() != send_value_iter.value()){
        if(is_usb_connect)
            //当数据为自己写的时候才发送
            //qDebug()<<"xxxxxxx酷酷酷酷酷2222"<<pd400_send_value<<send_radom_data.command;
            if(send_radom_data.command == 0x03){
                pd400_m_sendMsgs.push_back(send_radom_data);
                //qDebug()<<"xxxxxxx酷酷酷酷酷"<<pd400_send_value;
            }
    }
    DM30_different_value.insert(QString::number(send_radom_data.param_addr[0],16),send_value_iter.value());
    //different_value_iter.value() = send_value_iter.value();
}

void window_pm481::sendLastHandleTimeout(){

    if(m_PM481slider_current_number==1){
        m_commin_fun.pd400_headset_volume(m_PM481slider_current_value);
        DM30_send_value.insert(QString::number(PARAM_HP_GAIN & 0xff,16),m_PM481slider_current_value);
    }else if(m_PM481slider_current_number==2){
        m_commin_fun.pd400_mic_gain(m_PM481slider_current_value);
        DM30_send_value.insert(QString::number(PARAM_MIC_GAIN & 0xff,16),m_PM481slider_current_value);
    }else if(m_PM481slider_current_number==3){
        m_commin_fun.rgb_adjust(m_PM481slider_current_value);
        DM30_send_value.insert(QString::number(PARAM_RGB_BRIGHT & 0xff,16),m_PM481slider_current_value);
    }else if(m_PM481slider_current_number==4){
        m_commin_fun.pd400_limiter(m_PM481slider_current_value);
        DM30_send_value.insert(QString::number((PARAM_MIC_LIMIT_START & 0xff)+ 1,16),m_PM481slider_current_value);
    }else if(m_PM481slider_current_number==5){
        m_commin_fun.pd400_comp(m_PM481slider_current_value);
        DM30_send_value.insert(QString::number((PARAM_MIC_COMP_START & 0xff) +1,16),m_PM481slider_current_value);
    }
    //    m_DM30micsilder_runing = false;
    //    m_DM30phsilder_runing = false;
    //qDebug()<<"xxjjjjxxxbbbbb"<<m_DM30micsilder_runing;
    m_PM481Timer->stop();
}
//限制高级，基础的按钮的方法
void window_pm481::disableHandleTimeout(){
    m_basic_btn->setDisabled(false);
    m_high_btn->setDisabled(false);
    m_disableTimer->stop();
}
void window_pm481::isDfuStatus(int isDfu){

    if(isDfu){//Dfu模式
        m_product_label->setText(tr("设备为升级模式"));
        QString product_label_stys=QString("font-family:'Source Han Sans CN Medium';color:white;font-size:%1px;").arg(15*m_Magnification);
        m_product_label->setStyleSheet(product_label_stys);

    }else{//APP模式下
        //发送同步标志
        m_commin_fun.sys_Software_tag(1);
        is_usb_connect =true;
//        pm481_devoperator->pd400_get_bgdata.clear();
//        if(pm481_devoperator->pd400_get_bgdata.count()<=0){
            m_commin_fun.bg_datasource();
//        }
    }
}
//获取modelCode
void window_pm481::maingetModelCodel(){

    //过滤一样的参数
    QString modelcodel_str;

    for (int i=0;i<6;i++) {
        modelcodel_str +=  QChar(m_modelCodelName.at(0).buff[10+i]);
    }

    if(modelcodel_str==m_currentModelCodel){//为当前的设备
        is_usb_connect =true;//连接成功
        //清空数组
        m_receive_pcParMsgs.clear();
        m_modelCodelName.clear();
        m_product_label->setText(tr("同步设备参数中..."));
        //判断DFU模式
        m_commin_fun.is_dfuStatus();
    }
}
//usb插入
void window_pm481::OnConnectDevice(){

    if(!is_dm30updata_runing){
        //QThread::msleep(1000);
        is_modelconnect = true;
        // is_usb_connect =true;
        //    pm481_devoperator->pd400_get_bgdata.clear();
        //    m_commin_fun.bg_datasource();

        //QVector<int> deviceArrs = pm481_devoperator->timeGetDeviceArrs();
        //qDebug()<<"isConnect---test----xxxxxxxxxxxxxxxxxx"<<pm481_devoperator->m_pid <<deviceArrs;
        pm481_devoperator->m_PM481Request_device_datas.clear();


        m_commin_fun.getModelCode();
        is_usb_connect =true;
    }

    qDebug()<<"usb插入------";
}

void window_pm481::device_disconnect_fun(){

    pm481_devoperator->m_starThread = false;
    m_dfu_thread->isReceiveData = false;
    m_disableTimer->stop();
    m_hearbeattimer->stop();
    is_usb_connect =false;
    is_modelconnect = false;

    //清空背景参数的数组
    pm481_devoperator->m_PM481Request_device_datas.clear();
    m_pm481_device_datas.clear();
    m_modelCodelName.clear();

    pd400_m_sendMsgs.clear();
    m_receive_pcParMsgs.clear();
    m_modelCodelName.clear();
    m_product_label->setText(tr("请插入USB设备"));
    QString product_label_stys=QString("font-family:'Source Han Sans CN Medium';color:red;font-size:%1px;").arg(15*m_Magnification);
    m_product_label->setStyleSheet(product_label_stys);

    m_right_view->m_high_mask->show();
    m_left_view->m_basic_mask->show();

    m_basic_btn->setDisabled(true);
    m_basic_btn->setChecked(false);
    m_high_btn->setDisabled(true);
    m_high_btn->setChecked(false);
    for (int i=0;i<m_left_view->m_switch_btns.count();i++) {

        QPushButton *btn = m_left_view->m_switch_btns.at(i);
        btn->setDisabled(true);
    }
    setSliderDisabled(true);

    m_setting_btn->setDisabled(true);
    //显示RGB的蒙版
    m_left_view->m_fullrgb_mask->show();

    pm481_devoperator->m_isreceiveSem.release();
}

//usb拔出
void window_pm481::OnDisConnectDevice(){
    qDebug()<<"usb拔出------";
    if(!is_dm30updata_runing){
        device_disconnect_fun();

        emit pm481BackClick();
        this->close();
        delete this;
#if defined(Q_OS_WIN32)
#else

#endif

    }else{

    }

    // m_closeTimer->start(2000);
}
void window_pm481::setting_mouse_click(){

    m_setting_btn->setCheckState(Qt::Unchecked);
    m_setting_view->hide();
}
void window_pm481::back_click(){

    device_disconnect_fun();
    emit pm481BackClick();


#if defined(Q_OS_WIN32)
    this->close();
    delete this;
    pm481_devoperator->disconnect();

#else

    pm481_devoperator->disconnect();
#endif

}

void window_pm481::setSliderDisabled(bool status){
    for (int i=0;i<m_synmicvolume_Slider_arrs.count();i++) {
        MySlider *mic_volue_slider = m_synmicvolume_Slider_arrs.at(i);
        mic_volue_slider->setDisabled(status);
    }
    for (int i=0;i<m_synSlider_arrs.count();i++) {
        MySlider *mic_gain_slider = m_synSlider_arrs.at(i);
        mic_gain_slider->setDisabled(status);
    }
}
//心跳包
void window_pm481::hearbeatTimeout(){

    //发送数据
    m_commin_fun.hearbeat_fun();

}
//恢复出厂设置
void window_pm481::reset_action(){

    m_hearbeattimer->stop();
    m_setting_btn->setCheckState(Qt::Unchecked);
    m_setting_view->hide();

    //恢复出厂设置
    pm481_devoperator->resetFactorySetting();


    //切换提示语 提示重新插入设备
    is_resetFaocClick =true;

    //蒙版开启
    m_setting_btn->setDisabled(true);
    //显示RGB的蒙版
    m_left_view->m_fullrgb_mask->show();
    m_right_view->m_high_mask->show();
    m_left_view->m_basic_mask->show();

    m_basic_btn->setDisabled(true);
    m_basic_btn->setChecked(false);
    m_high_btn->setDisabled(true);
    m_high_btn->setChecked(false);
    setSliderDisabled(true);

    m_disableTimer->stop();
    m_hearbeattimer->stop();
    is_usb_connect =false;
    is_modelconnect = false;

    //清空背景参数的数组
    pm481_devoperator->m_PM481Request_device_datas.clear();
    m_pm481_device_datas.clear();
    m_modelCodelName.clear();

    pd400_m_sendMsgs.clear();
    m_receive_pcParMsgs.clear();
    m_modelCodelName.clear();
    m_product_label->setText(tr("请插入USB设备"));
    QString product_label_stys=QString("font-family:'Source Han Sans CN Medium';color:red;font-size:%1px;").arg(15*m_Magnification);
    m_product_label->setStyleSheet(product_label_stys);

    m_right_view->m_high_mask->show();
    m_left_view->m_basic_mask->show();

    m_basic_btn->setDisabled(true);
    m_basic_btn->setChecked(false);
    m_high_btn->setDisabled(true);
    m_high_btn->setChecked(false);
    for (int i=0;i<m_left_view->m_switch_btns.count();i++) {

        QPushButton *btn = m_left_view->m_switch_btns.at(i);
        btn->setDisabled(true);
    }
    setSliderDisabled(true);

    m_setting_btn->setDisabled(true);
    //显示RGB的蒙版
    m_left_view->m_fullrgb_mask->show();

    pm481_devoperator->m_isreceiveSem.release();

    QThread::msleep(500);
    is_modelconnect = true;
    m_commin_fun.getModelCode();

//    m_product_label->setText(tr("已恢复出厂设置，请重新连接设备"));
//    QString product_label_stys=QString("font-family:'Source Han Sans CN Medium';color:red;font-size:%1px;").arg(15*m_Magnification);
//    m_product_label->setStyleSheet(product_label_stys);

}
window_pm481::~window_pm481(){

    m_PM481Timer->stop();
    delete m_PM481Timer;

    m_disableTimer->stop();
    delete m_disableTimer;

    m_starDspTimer->stop();
    delete m_starDspTimer;

    m_hearbeattimer->stop();
    delete m_hearbeattimer;


    m_pTimer->stop();
    delete m_pTimer;

    m_dfu_thread->m_ret = false;
    m_dfu_thread->quit();
    m_dfu_thread->wait();

    clearDataSource();

    delete m_left_view;
    delete m_right_view;

    delete m_setting_view;
    delete m_setting_btn;
    delete m_basic_btn;
    delete m_high_btn;
    delete m_product_label;
}
