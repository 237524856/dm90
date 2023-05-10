/// 版权所有:  版权所有(C) 2017，摩罗志远
/// 内容摘要:  操作设备所有功能放在这
/// 完成日期:  2021年4月20日
/// 版    本:  1.0
/// 作    者:
/// 修改记录1:
/// 修改日期:
/// 版 本 号:
/// 修 改 人:
/// 修改内容:
/// 修改记录2:
/// 修改日期:
/// 版 本 号:
/// 修 改 人:杨禹

#pragma once
#pragma execution_character_set("utf-8")
#include <stdio.h>
#include <stdlib.h>
#include "devoperator.h"
#include "HID/hidapi.h"
#include "HID/utils/DeviceManager.h"
#include "HID/utils/HidBase.h"
#include <QDebug>
#include <QDateTime>
#include "common.h"
#include <QEventLoop>
#include <QTime>
#include <QElapsedTimer>
#include "./HID/utils/CRC.h"
#include <QMessageBox>

ICommDeviceMgmt* device_manager = NULL;

#define MAX_STR 512
//背景参数的包数，如果打开了SN不需要+1否则+1
#define BG_PACK_NUM 31 +OPENSN
//获取背景参数的包数
#define DM90GETPACK 8

//命令编码
#define COMMAND_CONTINUTITY_WRITE 0x05
#define COMMAND_CONTINUTITY_READ 0x06
#define COMMAND_RANDOM_WRITE 0x03

DevOperator::DevOperator(QObject *parent) : QObject(parent)
{
    pd400_get_bgdata={};
    //m_pc_isconnect = false;
    dfu_send_current_packNumber=0;
    m_is_sendmssage_success = true;
    m_is_sendmssage_success = false;


}
DevOperator::~DevOperator(){

    delete device_manager;
}
bool DevOperator::getDeviceArrs(){

    if(device_manager) delete device_manager;

    device_manager = new CDeviceManager<CHidBase>();

    if(device_manager){
        bool ret = device_manager->Init();
        if(ret){
            m_pDevMgmt = device_manager;
            device_manager->SetProtocol(this);
            device_manager->SetDeviceArgc(0);
            device_manager->GetDevicesInfo();
            return false;
        }
    }
    //    CHidBase *hid_base;
    //    connect(hid_base,SIGNAL(clicked()),this,SLOT(preset_btn_click()));
    return false;
}

QVector<int> DevOperator::timeGetDeviceArrs(){

    return  device_manager->GetDevicesInfo();
}
//开始连接设备
bool DevOperator::starConnectDevice(int device_pid){


    return device_manager->ClickConnectDevice(device_pid);;
}

bool DevOperator::connectHID(){

    device_manager = new CDeviceManager<CHidBase>();
    if(device_manager){
        bool ret = device_manager->Init();
        if(ret){
            m_pDevMgmt = device_manager;
            device_manager->SetProtocol(this);
            device_manager->SetDeviceArgc(0);
            bool isconnect = device_manager->AutoConnectDevice();
            return isconnect;
        }
    }
    return false;
}

bool DevOperator::mic_volume(uint8_t value){


    mypc_random_write recv = {};

    send_radom_data = {};
    send_radom_data.frame_header = 0xc4;
    send_radom_data.frame_len_L = 0x0b;
    send_radom_data.command = 0x03;
    send_radom_data.param_addr[0] = 0x23;
    send_radom_data.param_addr[1] = 0x20;
    send_radom_data.param_value[0] = 0x20;

    uint16_t pd400_crc = CCRC::Crc16((uint8_t *)&send_radom_data, sizeof(send_radom_data)-2);
    send_radom_data.crc_L = (pd400_crc) & 0xFF;
    send_radom_data.crc_H = (pd400_crc >> 8) & 0xFF;
    pd400_m_sendMsgs.push_back(send_radom_data);
    return true;
}
//把数据放到数组中然后发送到设备中
void DevOperator::startSendMsg(){    
    int i = 0;
    while (1) {
        //i++;
        QThread::msleep(10);
        if(pd400_m_sendMsgs.count() > 0){
            PACKDATA package = m_sendMsgs.at(0);
            PACKDATA recv = {  };
            QThread::msleep(5);
            bool ret = send_msg_device((unsigned char*)&package,(unsigned char*)&recv);
            //qDebug()<<"ms------"<<ret;
            if(!ret){
                m_sendMsgs.pop_front();
                if(package.index[4] == 0x77){ isget_background_par = false;}else{
                    isget_background_par = true;
                }

                if(recv.index[4] == 0x76 && isget_background_par){
                    m_receiveMsgs.push_back(recv);
                    if(m_receiveMsgs.count()>=adds_arrs.count()-1){
                        emit refresh_main_signal();
                    }
                    QByteArray array_data;
                    array_data.append((char*)&recv, sizeof(recv));
                    uint8_t qq = recv.index[6];
                    qDebug()<<"receive_data------"<<array_data<<"------xx"<<qq;
                }
            }

        }
    }
}
//pd400的发数据
void DevOperator::pd400_startSendMsg(){

    while (m_starThread) {
        QThread::msleep(1);
        if(is_usb_connect){
            //固件升级
            if(m_dfu_dataList.count()>0){
                for (int i=0;i<m_dfu_dataList.count();i++) {
                    if(!is_usb_connect){
                        //                        emit pd400_break_fail_mess(1);
                        //                        m_dfu_dataList.clear();//清空DFU的数据
                        //                        break;
                    }
                    QByteArray send_data = m_dfu_dataList.at(i);
                    if(i==0)send_updata_DFU_message((unsigned char*)send_data.data(),i);
                    //分成多包发送
                    if(i ==1){
                        m_is_dfu_sending = true;
                        QElapsedTimer time;
                        time.start();
                        while(time.elapsed() < 200)  {//等待200MS接收数据
                            if(!m_is_dfu_sending){
                                QThread::msleep(2000);
                                break;
                            }
                        }
                    }else if(i ==3){
                        m_is_dfu_sending = true;
                        dfu_send_current_packNumber+=3;
                        QList<QByteArray> m_dfu_package;
                        m_dfu_package = m_dfu_dataList.mid(1,2);
                        for(int i=0;i<m_dfu_package.count();i++){

                            send_updata_DFU_message((unsigned char*)m_dfu_package.at(i).data(),i);
                        }
                        QElapsedTimer time;
                        time.start();
                        while(time.elapsed() < 200)  {//等待200MS接收数据
                            if(!m_is_dfu_sending){
                                break;
                            }
                        }
                    }else if((i-3)%5==0){
                        m_dfu_package.clear();
                        dfu_send_current_packNumber+=5;
                        m_dfu_package = m_dfu_dataList.mid(i-5,5);
                        for(int j=0;j<m_dfu_package.count();j++){
                            send_updata_DFU_message((unsigned char*)m_dfu_package.at(j).data(),j);
                        }
                        m_is_dfu_sending = true;
                        QElapsedTimer time;
                        time.start();
                        while(time.elapsed() < 200)  {//等待200MS接收数据
                            if(!m_is_dfu_sending){
                                break;
                            }
                        }
                        //超时升级失败
                        if(m_is_dfu_sending){
                            qDebug()<<"kkkk==========================";
                            emit pd400_break_fail_mess(1);

                            m_dfu_dataList.clear();//清空DFU的数据
                        }
                    }
                    //告诉进度条当前的值
                    emit pd400_progress_current_value(dfu_send_current_packNumber);
                    if(dfu_send_current_packNumber == m_dfu_dataList.count()){
                        m_dfu_dataList.clear();
                    }
                    //qDebug()<< m_dfu_dataList.count()<<"kkkk"<<i<<dfu_send_current_packNumber;
                }

                //判断最后的包数
                if((m_dfu_dataList.count()-3)%5!=0){
                    m_is_dfu_sending = true;
                    m_dfu_package = m_dfu_dataList.mid(m_dfu_dataList.count()-(m_dfu_dataList.count()-3)%5,(m_dfu_dataList.count()-3)%5);
                    for(int k=0;k<m_dfu_package.count();k++){
                        send_updata_DFU_message((unsigned char*)m_dfu_package.at(k).data(),k);
                        dfu_send_current_packNumber++;
                        //告诉进度条当前的值
                        emit pd400_progress_current_value(dfu_send_current_packNumber);
                        if(dfu_send_current_packNumber == m_dfu_dataList.count()){
                            m_dfu_dataList.clear();
                        }
                    }
                }else{
                    m_is_dfu_sending = true;
                    m_dfu_package = m_dfu_dataList.mid(m_dfu_dataList.count()-5,5);
                    for(int k=0;k<m_dfu_package.count();k++){
                        send_updata_DFU_message((unsigned char*)m_dfu_package.at(k).data(),k);
                        dfu_send_current_packNumber++;
                        //告诉进度条当前的值
                        emit pd400_progress_current_value(dfu_send_current_packNumber);
                        if(dfu_send_current_packNumber == m_dfu_dataList.count()){
                            m_dfu_dataList.clear();
                        }
                    }
                }
            }
            //除了升级固件，其他发送数据的方法
            if(pd400_m_sendMsgs.count() > 0){
                pc_random_write package = pd400_m_sendMsgs.at(0);
                mypc_random_write recv = { };
                QThread::msleep(5);
                int ret = pd400_SendAndReceive((unsigned char*)&package,recv,12);
                //处理错误信息
                qDebug()<<"ms------"<<ret<<pd400_m_sendMsgs.count();
                if(ret<0 || !is_usb_connect){
                    pd400_m_sendMsgs.clear();
                    qDebug()<<"kkkk======gggg===================="<<is_usb_connect;
                    emit pd400_break_fail_mess(ret);
                }else{
                    pd400_m_sendMsgs.pop_front();
                }
            }else{
                //获取背景参数时先获取DFU的状态，MODELCODE获取也再这里
                if(m_receive_pcParMsgs.count() > 0){

                    QThread::msleep(5);
                    pc_random_read package = m_receive_pcParMsgs.at(0);
                    mypc_random_write recv = { };
                    int ret = pd400_SendAndReceive((unsigned char*)&package,recv,12);
                    //qDebug()<<"send------"<<ret;
                    if(ret<0  || !is_usb_connect){
                        m_receive_pcParMsgs.clear();
                        qDebug()<<"kkkk===============kkkkggggg==========="<<ret<<is_usb_connect;
                        emit pd400_break_fail_mess(ret);
                    }else{

                        m_receive_pcParMsgs.pop_front();
                    }
                    QByteArray array_data;
                    array_data.append((char*)&package, sizeof(package));
                    QString str;
                    for (int k=0;k<9;k++) {
                        uint8_t a = array_data[k];
                        str += " "+QString::number(a,16);
                    }
                    //if(package.report==0x4b)
                    // qDebug()<<"send-----_message"<<str<<testint;
                }

                //读取背景参数的数据--------------------------------------------------------------------------
                if(pd400_m_receive_bgMsgs .count() > 0){
                    //qDebug()<<"mslllllll------"<<pd400_get_bgdata.count();
                    QByteArray array_data_front;
                    array_data_front.append((char*)&pd400_m_receive_bgMsgs.at(0), sizeof(continuity_read_data));

                    mypc_random_write recv = { };
                    int ret = pd400_SendAndReceive((unsigned char*)array_data_front.data(),recv,12);
                    if(ret<0 || !is_usb_connect){
                        pd400_m_receive_bgMsgs.clear();

                    }else{
                        pd400_m_receive_bgMsgs.pop_front();
                        m_isreceiveSem.acquire();//如果0，一直堵塞
                    }
                }
            }

        }else{//获取modele发送数据

            pd400_m_receive_bgMsgs.clear();
            if(m_getModelCodelDatas.count() > 0){
                //qDebug()<<"xxppppppxxxppppxxxpp"<<m_receive_pcParMsgs.count();
                QByteArray array_data_front;
                array_data_front.append((char*)&m_getModelCodelDatas.at(0), sizeof(continuity_read_data));
                mypc_random_write recv = { };
                int ret = pd400_SendAndReceive((unsigned char*)array_data_front.data(),recv,12);
                //qDebug()<<"send------"<<ret;
                if(!ret || !is_modelconnect){
                    m_getModelCodelDatas.clear();
                }else{
                    m_getModelCodelDatas.pop_front();
                }
            }
        }
    }
}

//线程一直接收收到的信息=========
void DevOperator::thread_receive_message(){
    mypc_random_write pc_recv = { };
    bool pc_ret = Kpd400_Receive(pc_recv,3,64);
    //处理错误信息
    QByteArray array_data;
    array_data.append((char*)&pc_recv, sizeof(pc_recv));
    QString str;
    for (int k=0;k<64;k++) {
        uint8_t a = array_data[k];
        str += " "+QString::number(a,16);
    }

    if(pc_recv.command == 0x0b){
        if(pc_recv.param_value[0]==1||pc_recv.param_value[0]==4||pc_recv.param_value[0]==2){//固件升级失败的逻辑
            m_dfu_dataList.clear();
            m_dfu_package.clear();
            emit pd400_break_fail_mess(1);
            qDebug()<<"失败================================";
        }else if(pc_recv.param_value[0]==5){
            m_dfu_dataList.clear();
            m_dfu_package.clear();
            emit pd400_break_fail_mess(2);
        }
    }
    //获取DFU的状态
    if(pc_recv.param_addr[0] == 0x08 && pc_recv.param_addr[1] == 0x10){
        emit blockDfuStatus(pc_recv.param_value[0]);
    }else if(pc_recv.command == 0x06 && pc_recv.param_addr[0] ==0x00){//获取model code

        m_modelCodelName.push_back(pc_recv);

        emit blockModelCodel();

    }else{//获取背景参数
        if(pc_ret){
            if(pc_recv.command == 0x06 && pc_recv.param_addr[0] !=0x00){//连续读数据
                m_PM481Request_device_datas.push_back(pc_recv);
                m_isreceiveSem.release();
                if(pc_recv.param_addr[0]==0x22&&pc_recv.param_addr[1]==0x20)
                    emit pd400_refresh_main_signal();
            }

            //同步设备自动返回信息
            m_sys_receive_data = pc_recv;
            uint8_t addr1 = m_sys_receive_data.param_addr[0];
            uint8_t addr2 = m_sys_receive_data.param_addr[1];
            if(addr1 ==0x22 && addr2 ==0x20){//静音
                if(pd400_get_bgdata.count()>1){
                    pd400_get_bgdata.replace(0,m_sys_receive_data);
                }
            }
            if(m_PM481Request_device_datas.count()>=3){//信息同步
                emit pd400_sysreceive_datasignal();
            }else{

            }
        }
    }
    if(m_dfu_dataList.count()>0){

        if(pc_ret)m_is_dfu_sending=false;
    }

    //点击了重启按钮


    //return  true;
}

//DM90数据的发送，可用于连续包的发送---同时读取或者写入多条数据
void DevOperator::dm90_startContinuityReaddMsg(){

    while (m_starThread) {
        QThread::msleep(1);
        if(is_usb_connect){
            if(m_dm90continuityReadDatas.count()>0){//读取设备的数据
                //QThread::msleep(50);
                pc_receive_continuitywrite_data recv = { };

                QByteArray array_data_front;
                array_data_front.append((char*)&m_dm90continuityReadDatas.at(0), sizeof(continuity_read_data));
                bool ret = DM90_SendMessage_device((unsigned char*)array_data_front.data(),recv,3,200);
                QThread::msleep(100);
                if(!ret || !is_usb_connect){
                    m_dm90continuityReadDatas.clear();
                    emit pd400_break_fail_mess(ret);
                }else{
                    m_dm90continuityReadDatas.pop_front();
                    m_isreceiveSem.acquire();//如果0，一直堵塞
                }
            }else if(m_dm90continuityWriteDatas.count()>0){//连续写入数据

                pc_receive_continuitywrite_data recv = { };
                QByteArray array_data_front;
                array_data_front.append((char*)&m_dm90continuityWriteDatas.at(0), sizeof(continuity_read_data));
                bool ret = DM90_SendMessage_device((unsigned char*)array_data_front.data(),recv,3,200);
                QThread::msleep(100);
                if(!ret || !is_usb_connect){
                    m_dm90continuityWriteDatas.clear();
                    emit pd400_break_fail_mess(ret);
                }else{
                    m_dm90continuityWriteDatas.pop_front();
                    //m_isreceiveSem.acquire();//如果0，一直堵塞
                }
            }else if(m_dm90random_Write_multipleDatas.count()>0){//随机写入数据

                pc_receive_continuitywrite_data recv = { };
                QByteArray array_data_front;
                array_data_front.append((char*)&m_dm90random_Write_multipleDatas.at(0), sizeof(continuity_read_data));
                bool ret = DM90_SendMessage_device((unsigned char*)array_data_front.data(),recv,3,200);
                if(!ret || !is_usb_connect){
                    m_dm90random_Write_multipleDatas.clear();
                    emit pd400_break_fail_mess(ret);
                }else{
                    m_dm90random_Write_multipleDatas.pop_front();
                    //m_isreceiveSem.acquire();//如果0，一直堵塞
                }

            }

        }
    }
}
//dm90设备一直接收数据 =============(目前只用于DM90项目******************************)
void DevOperator::DM90Thread_receive_message(){

    pc_receive_continuitywrite_data pc_recv = { };
    bool pc_ret = DM90_Receive(pc_recv,3,64);

    if(pc_ret){
        //解析MODELCODEL,首先判断MODEL有无问题
        if(pc_recv.command==COMMAND_CONTINUTITY_READ){

            if(pc_recv.param_addr[0]==0&&pc_recv.param_addr[1]==0){//获取MODElcode数据
                QString modelcodel_str = "";
                for (int i=0;i<6;i++) {
                    modelcodel_str +=  QChar(pc_recv.param_value[i]);
                }
                emit dm90_blockModelCodel(modelcodel_str);//发送MODELCodel数据给主界面
                m_isreceiveSem.release();

                //qDebug()<<"接收到的数据==="<<modelcodel_str;
            }else if(((pc_recv.param_addr[0]==0x22||pc_recv.param_addr[0]==0x3c||pc_recv.param_addr[0]==0x56||pc_recv.param_addr[0]==0x70
                       ||pc_recv.param_addr[0]==0x8a||pc_recv.param_addr[0]==0xa4||pc_recv.param_addr[0]==0xbe||pc_recv.param_addr[0]==0xd8)&&pc_recv.param_addr[1]==0x20) ||
                     (pc_recv.param_addr[1]==0x00 &&(pc_recv.param_addr[0]==0x03||pc_recv.param_addr[0]==0x1d) )){ //接收设备的背景参数
                m_isreceiveSem.release();
                m_DM90Request_device_data.push_back(pc_recv);
                //如果有七包数组则返回主界面刷新整个APP
                qDebug()<<"接收到的数据==="<<m_DM90Request_device_data.count();
                if(m_DM90Request_device_data.count()>=10){
                    emit dm90_blockRefresh_mainUI();
                }
            }
        }
      if(pc_recv.command==COMMAND_RANDOM_WRITE && m_DM90Request_device_data.count()>=10 && pc_recv.param_addr[1] ==0x20){
          m_DM90syn_receive_data = pc_recv;
          emit DM90_synreceive_datasignal();
      }
    }


    /**********打印信息***************************/
    if(pc_ret){
        //收到的数据各种判断
        QByteArray array_data;
        array_data.append((char*)&pc_recv, sizeof(pc_recv));
        QString str;
        for (int k=0;k<64;k++) {
            uint8_t a = array_data[k];
            str += " "+QString::number(a,16);
        }
        //qDebug()<<"接收到的数据==="<<str;


    }

}

//获取MODELCODE的方法
QString DevOperator::getDeviceModelCodel(){

    continuity_read_data = {};
    continuity_read_data.report = 0x4b;
    continuity_read_data.frame_header = 0xc4;
    continuity_read_data.frame_len_L = 0x0b;
    continuity_read_data.command = 0x06;
    continuity_read_data.param_addr[0] = 0x00;
    continuity_read_data.param_addr[1] = 0x00;
    continuity_read_data.param_number[0] = 0x03;
    uint16_t pd400_crc = CCRC::Crc16((uint8_t *)&continuity_read_data, sizeof(continuity_read_data)-2-53);//有效数据得长度
    continuity_read_data.crc_L = (pd400_crc) & 0xFF;
    continuity_read_data.crc_H = (pd400_crc >> 8) & 0xFF;

    m_dm90continuityReadDatas.push_back(continuity_read_data);
    //    QByteArray array_data_front;
    //    array_data_front.append((char*)&continuity_read_data, sizeof(continuity_read_data));
    //    pc_receive_continuitywrite_data recv = { };
    //    bool ret = DM90_SendMessage_device((unsigned char*)array_data_front.data(),recv,3,200);
    //    if(ret){
    //        //解析MODELCODEL
    //        QString modelcodel_str;
    //        for (int i=0;i<6;i++) {
    //            modelcodel_str +=  QChar(recv.param_value[i]);
    //        }
    //        return modelcodel_str;
    //    }else{

    //        return "error";
    //    }
    return "error";
}

int DevOperator::resetFactorySetting(){

    send_radom_data = {};
    send_radom_data.report = 0x4b;
    send_radom_data.frame_header = 0xc4;
    send_radom_data.frame_len_L = 0x0b;
    send_radom_data.command = 0x03;
    send_radom_data.param_addr[0] = 0x0A;
    send_radom_data.param_addr[1] = 0x10;

    send_radom_data.param_value[0] = 1;

    uint16_t pd400_crc = CCRC::Crc16((uint8_t *)&send_radom_data, sizeof(send_radom_data)-2);
    send_radom_data.crc_L = (pd400_crc) & 0xFF;
    send_radom_data.crc_H = (pd400_crc >> 8) & 0xFF;

    mypc_random_write recv = { };

    int ret = pd400_SendAndReceive((unsigned char*)&send_radom_data,recv,12);

    return ret;
}
//DM90获取背景参数的方法--------------------------
QVector<pc_receive_continuitywrite_data> DevOperator::getsyncDeviceData(){

    //增加设备信息****************************
    for (int i=0;i<2;i++) {

        continuity_read_data = {};
        continuity_read_data.report = 0x4b;
        continuity_read_data.frame_header = 0xc4;
        continuity_read_data.frame_len_L = 0x0b;
        continuity_read_data.command = 0x06;
        continuity_read_data.param_addr[0] = 0x03+i*26;
        continuity_read_data.param_addr[1] = 0x00;
        continuity_read_data.param_number[0] = 0x1A;

        uint16_t pd400_crc = CCRC::Crc16((uint8_t *)&continuity_read_data, sizeof(continuity_read_data)-2-53);//有效数据得长度
        continuity_read_data.crc_L = (pd400_crc) & 0xFF;
        continuity_read_data.crc_H = (pd400_crc >> 8) & 0xFF;
        m_dm90continuityReadDatas.push_back(continuity_read_data);
    }

    //需要发送的获取的设备的背景参数
    //QVector<QByteArray>request_device_data;
    //接收设备返回的数据
    QVector<pc_receive_continuitywrite_data> receive_device_datas;
    for(int i=0;i<DM90GETPACK;i++){
        continuity_read_data = {};
        continuity_read_data.report = 0x4b;
        continuity_read_data.frame_header = 0xc4;
        continuity_read_data.frame_len_L = 0x0b;
        continuity_read_data.command = 0x06;
        continuity_read_data.param_addr[0] = 0x22+i*26;
        continuity_read_data.param_addr[1] = 0x20;
        if(i==7){
            if(currentProPid==0x0102){
               continuity_read_data.param_number[0] = 0x1A-23;
            }else if(currentProPid==0x0103){
               continuity_read_data.param_number[0] = 0x1A-23+7;
            }

        }else{
            continuity_read_data.param_number[0] = 0x1A;
        }

        uint16_t pd400_crc = CCRC::Crc16((uint8_t *)&continuity_read_data, sizeof(continuity_read_data)-2-53);//有效数据得长度
        continuity_read_data.crc_L = (pd400_crc) & 0xFF;
        continuity_read_data.crc_H = (pd400_crc >> 8) & 0xFF;
        //        QByteArray array_data_front;
        //        array_data_front.append((char*)&continuity_read_data, sizeof(continuity_read_data));
        m_dm90continuityReadDatas.push_back(continuity_read_data);
    }

    //    while (request_device_data.count()>0) {

    //        QThread::msleep(15);
    //        pc_receive_continuitywrite_data recv = { };
    //        bool ret = DM90_SendMessage_device((unsigned char*)request_device_data.at(0).data(),recv,3,200);
    //        if(ret){//发送成功，清除当前的数据
    //            receive_device_datas.push_back(recv);
    //            request_device_data.pop_front();
    //        }else{//获取失败

    //            return receive_device_datas;
    //        }
    //    }

    return receive_device_datas;
}




void DevOperator::send_updata_DFU_message(unsigned char *send_data,int pack_num){

    pd400_get_bgdata.clear();
    mypc_random_write recv = { };
    bool ret = pd400_dfu_SendMess(send_data,recv,pack_num,200);
    //qDebug()<<"dfu------"<<ret;
    if(!ret){
        //        qDebug()<<"发送成功------"<<ret;
    }
}
void DevOperator::OnRecvData(const PACKDATA& data)
{

}
void DevOperator::OnConnectDevice(ICommDeviceMgmt* pCommDevMgmt)//设备已经插入的方法
{
    const char* name = pCommDevMgmt->GetDevice()->GetDeviceName();

    m_pid = pCommDevMgmt->GetDevice()->GetDeviceParam2();

    //    m_deviceName = QString::fromUtf8(name);
    qDebug()<<"isConnect---test----xxxxxxxxxxxxxxxxxx"<<name<<m_pid;
    //OpenApp(true);
    emit connectDevice();
}
void DevOperator::OnDisconnectDeviceFinish()//拔掉的方法
{
    printf("NOconnect-----new------\n");
    qDebug()<<"NOconnect----xxxxxxxxxxxxxxxxxx";

    emit disconnectDevice();
}
void DevOperator::disconnect(){

    device_manager->DisconnectDevice();
}

void DevOperator::myprinf_send(pc_random_read package){
    QByteArray array_data;
    array_data.append((char*)&package, sizeof(package));
    QString str1="";

    for (int k=0;k<sizeof(package);k++) {
        uint8_t a = array_data.at(k);
        str1 += " "+QString::number(a,16);
    }
    //qDebug()<<"send------"<<str1;
}
