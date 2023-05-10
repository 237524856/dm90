#ifndef PM481_DEVICEDATASOURCE_H
#define PM481_DEVICEDATASOURCE_H

#include <QObject>




typedef struct PM481_device_dataSource{

    //设备的名字
    uint16_t PM481_DeviceName[8];

    //mcu固件版本号
    uint16_t PM481_FirmwareRev[5];

    //DSP1 Rev
    uint16_t PM481_DSP1Rev[3];

    //DSP2 Rev 还没有使用
    uint16_t PM481_DSP2Rev[3];

    //VID PID
    uint16_t PM481_USBID[2];

    //生产日期
    uint16_t PM481_data[3];

    //生产时间
    uint16_t PM481_time[3];

    //生产时间
    uint16_t PM481_Device_SN[25];

    //MIC静音
    uint16_t PM481_micMute;

    //MIC增益(Gain)
    uint16_t PM481_micBaseGain;

    uint16_t PM481_micBaseAdv;

    //MIC均衡器模式
    uint16_t PM481_micMode;

    //压缩器
    uint16_t PM481_compEnable;
    uint16_t PM481_compThreshold;
    uint16_t PM481_compMaxGain;
    uint16_t PM481_compAttack;
    uint16_t PM481_compRelease;
    uint16_t PM481_compRate;

    //限幅器
    uint16_t PM481_limitEnable;
    uint16_t PM481_limitThreshold;
    uint16_t PM481_limitAttack;
    uint16_t PM481_limitRelease;

    //耳机音量
    uint16_t PM481_HPGain;

    //监听混音比例
    uint16_t PM481_mix;

    //音色调节
    uint16_t PM481_Tone;

    //MIC使用距离
    uint16_t PM481_Dis;

    //MIC电平指示
    uint16_t PM481_Level;

    //用户模式
    uint16_t PM481_UserMode;

    //LED灯光秀开关
    uint16_t PM481_LedShowOnOff;

    //LED灯光亮度
    uint16_t PM481_LedBright;

    //LED灯光秀模式
    uint16_t PM481_LedShowMode;


}pc_PM481_device_dataSource;


class pm481_deviceDatasource : public QObject
{
    Q_OBJECT

public:
    //单例模式
    static pm481_deviceDatasource *getInstance();

public:

    pc_PM481_device_dataSource *m_PM481_device_dataSource;
    //设备的负数转为实际的参数
    double loadingChangeNegativeNumber(int load_number){

        double load_number_value = (double)(load_number-2000)/10;

        return QString::number(load_number_value,'f',1).toDouble();
    }

private:

    static pm481_deviceDatasource *self;



signals:

};

#endif // PM481_DEVICEDATASOURCE_H
