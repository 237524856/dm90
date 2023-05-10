/// 版权所有:  版权所有(C) 2017，摩罗志远
/// 内容备注:
/// 完成日期:  2021年4月13日
/// 版    本:  1.0
/// 作    者:  杨禹
/// 修改记录1:
/// 修改日期:
/// 版 本 号:
/// 修 改 人:
/// 修改内容:
/// 修改记录2:
/// 修改日期:
/// 版 本 号:
/// 修 改 人:
/// 修改内容:

#ifndef DEVOPERATOR_H
#define DEVOPERATOR_H

#include <qobject.h>
#include "HID/FirmwareUpgradeProtocol.h"
#include "HID/utils/DeviceManager.h"
#include "HID/utils/HidBase.h"
#include <QThread>
#include <QVector>
#include <QSemaphore>


//获取同步的信息
//typedef struct sys_receive_data
//{

//   //uint8_t report; //报文ID
//    union
//    {
//        //USB HID 一个包 （全速最多64byte, 高速可达1024byte）
//        uint8_t buff[11];
//        //协议的数据包结构
//        struct
//        {
//            uint8_t report;
//            uint8_t frame_header;   // 帧头
//            uint8_t frame_len_L;  // 帧长度
//            uint8_t frame_len_H;  // 帧长度
//            uint8_t cp_addr;  // 设备地址
//            uint8_t command;		// 命令编码
//            //数据
//            uint8_t param_addr[2];//数据的地址
//            uint8_t param_value[2];	//数据的值
//            uint8_t crc_L;			// CRC校验
//            uint8_t crc_H;			// CRC校验
//        };
//    };
//}my_sys_receive_data;
#define MonitorVol 0x00FC

//限制收发的信号量


class DevOperator : public QObject , public CFirmwareUpgradeProtocol
{
    Q_OBJECT
public:
    explicit DevOperator(QObject *parent = nullptr);
    ~DevOperator();

    bool getDeviceArrs();

    QVector<int> timeGetDeviceArrs();
    //开始连接设备
    bool starConnectDevice(int device_pid);

    //开始连接设备
    bool connectHID();

    bool mic_volume(uint8_t value);

    QString m_deviceName;

    //开机时，或者拔插时的背景参数
    QVector<mypc_random_write> pd400_get_bgdata;
    
    //同步信息的值
    mypc_random_write m_sys_receive_data;

    //DM90同步信息的值
    pc_receive_continuitywrite_data m_DM90syn_receive_data;

    //设备是否已经连接
//    bool m_pc_isconnect;
    //dfu的数据是否是正在发送
    bool m_is_dfu_sending;

    QList<QByteArray> m_dfu_dataList;

    QList<QByteArray> m_dfu_package;

    //升级固件的方法
    void send_updata_DFU_message(unsigned char *send_data,int pack_num);

    //一直接收数据
    void thread_receive_message();

    void DM90Thread_receive_message();

    //固件发送的包数
    int dfu_send_packNumber;
    //当前已发的固件包数
    int dfu_send_current_packNumber;

    //发送信息成功
    bool m_is_sendmssage_success;

    void myprinf_send(pc_random_read package);

    uint32_t m_pid;

    mypc_random_write m_modelCode_data;

    //获取DM90的全部背景数据
    QVector<pc_receive_continuitywrite_data> m_DM90Request_device_data;

    //获取PM481的全部背景数据
    QVector<mypc_random_write> m_PM481Request_device_datas;

    QSemaphore m_isreceiveSem;//线程同步的信号量


    bool m_starThread = false;


public:

    void disconnect();
    //获取的设备的MODELCODE
    QString getDeviceModelCodel();

    //设备重置的方法
    int resetFactorySetting();

    //同步设备的参数
    QVector<pc_receive_continuitywrite_data> getsyncDeviceData();

protected:

    //ICommProtocol协议的实现

    virtual void OnRecvData(const PACKDATA& data);

    virtual void OnConnectDevice(ICommDeviceMgmt* pCommDevMgmt);

    virtual void OnDisconnectDeviceFinish();

private:
    uint8_t m_addL =200;
    uint8_t m_addH =150;

signals:
    //设备连接成功
    void connectDevice();
    //断开设备
    void disconnectDevice();
    //收完数据发送通知
    void refresh_main_signal();
    //pd400接收完数据背景参数
    void pd400_refresh_main_signal();
    //同步信息的信号
    void pd400_sysreceive_datasignal();

    //DM90同步信息的信号
    void DM90_synreceive_datasignal();

    //进度条的当前值
    void pd400_progress_current_value(int);
    //当前操作被中断的错误
    void pd400_break_fail_mess(int);
    //判断当前是否为DFU模式
    void blockDfuStatus(int);
    //判断当前设备的MODELCODEL
    void blockModelCodel();

    //dm90------------------------
    void dm90_blockModelCodel(QString);
    //收完设备背景参数的数据发送通知刷新整个app
    void dm90_blockRefresh_mainUI();

public slots:

    void startSendMsg();

    void pd400_startSendMsg();


    //用于连续读取DM90,DM70数据的
    void dm90_startContinuityReaddMsg();

};

#endif // DEVOPERATOR_H
