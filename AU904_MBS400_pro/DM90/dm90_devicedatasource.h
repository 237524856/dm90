#ifndef DM90_DEVICEDATASOURCE_H
#define DM90_DEVICEDATASOURCE_H

#include <QObject>



#define DM90PARAM_MODEL_CODE              (0x0000)
#define DM90PARAM_MODEL_NAME              (0x0003)
#define DM90PARAM_FW_REV_X                (0x000B)
#define DM90PARAM_FW_REV_Y                (0x000C)
#define DM90PARAM_FW_REV_Z                (0x000D)
#define DM90PARAM_FW_REV_BUILD            (0x000E)
#define DM90PARAM_FW_REV_TYPE             (0x000F)
#define DM90PARAM_DSP1_FW_REV_X           (0x0010)
#define DM90PARAM_DSP2_FW_REV_X           (0x0013)
#define DM90PARAM_USB_VID                 (0x0016)
#define DM90PARAM_USB_PID                 (0x0017)
#define DM90PARAM_DATE_YEAR               (0x0018)
#define DM90PARAM_DATE_MONTH              (0x0019)
#define DM90PARAM_DATE_DAY                (0x001A)
#define DM90PARAM_TIME_HOUR               (0x001B)
#define DM90PARAM_TIME_MINUTE             (0x001C)
#define DM90PARAM_TIME_SECOND             (0x001D)
#define DM90PARAM_SN_START                (0x001E)
#define DM90PARAM_SN_END                  (0x003D)

/* 配置数据 - 参数地址定义 */
#define DM90PARAM_CONFIG_START            (0x1000)
#define DM90PARAM_CONFIG_END              (0x100D)

#define DM90PARAM_HEART_BEAT              (0x1000)
#define DM90PARAM_DEVICE_ADDR             (0x1001)
#define DM90PARAM_PRESET_NUMBER           (0x1002)
#define DM90PARAM_STARTUP_LOAD_NO         (0x1003)
#define DM90PARAM_SAVE                    (0x1004)
#define DM90PARAM_LOAD                    (0x1005)
#define DM90PARAM_DELETE                  (0x1006)
#define DM90PARAM_COMM_ERR_ACK            (0x1007)
#define DM90PARAM_DFU                     (0x1008)
#define DM90PARAM_SYSTEM_REINIT           (0x1009)
#define DM90PARAM_REC_FACTORY             (0x100A)
#define DM90PARAM_SYNC_FLAG               (0x100B)
#define DM90PARAM_SYS_ERR                 (0x100C)
#define DM90PARAM_SYS_ERR2                (0x100D)



/* 预设 - 参数地址定义，每一类型的DSP功能的首和尾参数地址 */
#define DM90PARAM_PRESET_START            (0x2000)
#define DM90PARAM_PRESET_END              (0x20D2)

#define DM90PARAM_CHECKSUM                (0x2000)

#define DM90PARAM_PRESET_VALID_FLAG       (0x2001)

#define DM90PARAM_PRESET_NAME_START       (0x2002)
#define DM90PARAM_PRESET_NAME_END         (0x2021)

#define DM90PARAM_MUTE_START              (0x2022)
#define DM90PARAM_MUTE_END                (0x2023)

#define DM90PARAM_GAIN_START              (0x2024)
#define DM90PARAM_GAIN_END                (0x2027)

#define DM90PARAM_MIC_GATE_START          (0x2028)
#define DM90PARAM_MIC_GATE_END            (0x202C)

#define DM90PARAM_MIC_HPF_START           (0x202D)
#define DM90PARAM_MIC_HPF_END             (0x202E)

#define DM90PARAM_MIC_PEQ_START           (0x202F)
#define DM90PARAM_MIC_PEQ_END             (0x2056)

#define DM90PARAM_USB_PEQ_START           (0x2057)
#define DM90PARAM_USB_PEQ_END             (0x2088)

#define DM90PARAM_MIC_DEESS_START         (0x2089)
#define DM90PARAM_MIC_DEESS_END           (0x2090)

#define DM90PARAM_MIC_DEPOP_START         (0x2091)
#define DM90PARAM_MIC_DEPOP_END           (0x2098)

#define DM90PARAM_MIC_PITCH_START         (0x2099)
#define DM90PARAM_MIC_PITCH_END           (0x209A)

#define DM90PARAM_MIC_FLAN_START          (0x209B)
#define DM90PARAM_MIC_FLAN_END            (0x20A0)

#define DM90PARAM_MIC_CHOR_START          (0x20A1)
#define DM90PARAM_MIC_CHOR_END            (0x20A7)

#define DM90PARAM_MIC_REVERB_START        (0x20A8)
#define DM90PARAM_MIC_REVERB_END          (0x20B6)

#define DM90PARAM_MIC_ECHO_START          (0x20B7)
#define DM90PARAM_MIC_ECHO_END            (0x20BC)

#define DM90PARAM_MIC_COMP_START          (0x20BD)
#define DM90PARAM_MIC_COMP_END            (0x20C1)

#define DM90PARAM_MIC_LIMIT_START         (0x20C2)
#define DM90PARAM_MIC_LIMIT_END           (0x20C5)

#define DM90PARAM_MIC_TONE_START          (0x20C6)
#define DM90PARAM_MIC_TONE_END            (0x20C7)

#define DM90PARAM_LR_MONITOR              (0x20C8)

#define DM90PARAM_MIC_SIG_LEVEL           (0x20C9)

#define DM90PARAM_HP_SIG_LEVEL            (0x20CA)

#define DM90PARAM_RGB_SHOW_START          (0x20CB)
#define DM90PARAM_RGB_SHOW_END            (0x20D1)

#define DM90PARAM_MIC_POL_PAT             (0x20D2)

typedef struct usbplayEQ{

    uint16_t Enable;
    uint16_t Type;
    uint16_t Freq;
    uint16_t Gain;
    uint16_t Q;

} dm90_usbPalyEQ;

typedef struct micEQ{

    uint16_t Enable;
    uint16_t Type;
    uint16_t Freq;
    uint16_t Gain;
    uint16_t Q;

} dm90_micEQ;


typedef struct DM90_device_dataSource{

    //设备的名字
    uint16_t dm90_DeviceName[8];

    //mcu固件版本号
    uint16_t dm90_FirmwareRev[5];

    //DSP1 Rev
    uint16_t dm90_DSP1Rev[3];

    //DSP2 Rev 还没有使用
    uint16_t dm90_DSP2Rev[3];

    //VID PID
    uint16_t dm90_USBID[2];

    //生产日期
    uint16_t dm90_data[3];

    //生产时间
    uint16_t dm90_time[3];

    //生产时间+sn号
    uint16_t dm90_Device_SN[25];

    //各种需要到的参数
    uint16_t dm90_micMute;

    uint16_t dm90_usbPlayMute;

    //增益
    uint16_t dm90_micInputGain;

    uint16_t dm90_micOutputGain;

    uint16_t dm90_usbPlayGain;

    uint16_t dm90_HpOutputGain;

    //门限
    uint16_t dm90_threshold_Enable;

    uint16_t dm90_threshold_value;

    uint16_t dm90_threshold_Attack;

    uint16_t dm90_threshold_Release;

    uint16_t dm90_threshold_Hold;

    //高通滤波器
    uint16_t dm90_HPF_Enable;

    uint16_t dm90_HPF_Freq;

    //Mic 参量均衡器的8段EQ
    dm90_micEQ dm90micEQdatasource[8];

    //10段EQ,UsbPlay 参量均衡器
    dm90_usbPalyEQ dm90UsbPalyEQdatasource[10];

    //齿音消除器
    uint16_t dm90_Dental_Enable;

    uint16_t dm90_Dental_Threshold;

    uint16_t dm90_Dental_Freq;

    uint16_t dm90_Dental_Q;

    uint16_t dm90_Dental_Attack;

    uint16_t dm90_Dental_Release;

    uint16_t dm90_Dental_MinGain;

    uint16_t dm90_Dental_DetectPeriod;

    //去爆音器
    uint16_t dm90_Detonator_Enable;

    uint16_t dm90_Detonator_Threshold;

    uint16_t dm90_Detonator_Freq;

    uint16_t dm90_Detonator_Q;

    uint16_t dm90_Detonator_Attack;

    uint16_t dm90_Detonator_Release;

    uint16_t dm90_Detonator_MinGain;

    uint16_t dm90_Detonator_DetectPeriod;

    //语音变调
    uint16_t dm90_VoiceSandhi_Enable;

    uint16_t dm90_VoiceSandhi_Rate;

    //镶边器/移相器
    uint16_t dm90_flanger_Enable;

    uint16_t dm90_flanger_Level;

    uint16_t dm90_flanger_Delay;

    uint16_t dm90_flanger_FreqModu;

    int16_t dm90_flanger_DeptModu;

    int16_t dm90_flanger_FeedRate;

    //和声
    uint16_t dm90_harmony_Enable;

    uint16_t dm90_harmony_Level;

    uint16_t dm90_harmony_Delay;

    uint16_t dm90_harmony_FreqModu1;

    uint16_t dm90_harmony_DeptModu1;

    uint16_t dm90_harmony_FreqModu2;

    uint16_t dm90_harmony_DeptModu2;

    //混响(Reverb)
    uint16_t dm90_Reverb_Mode;

    uint16_t dm90_Reverb_Enable;

    uint16_t dm90_Reverb_PreDelay;

    uint16_t dm90_Reverb_DryLevel;

    uint16_t dm90_Reverb_WetLevel;

    uint16_t dm90_Reverb_Spread;

    uint16_t dm90_Reverb_RoomSize;

    uint16_t dm90_Reverb_Time;

    uint16_t dm90_Reverb_Damping;

    uint16_t dm90_Reverb_EarlyLevel;

    uint16_t dm90_Reverb_TailLevel;

    uint16_t dm90_Reverb_LpfEn;

    uint16_t dm90_Reverb_LpfBw;

    uint16_t dm90_Reverb_HpfEn;

    uint16_t dm90_Reverb_HpfBw;

    //回声(Echo) 延迟的一种算法
    uint16_t dm90_Echo_Mode;

    uint16_t dm90_Echo_Enable;

    uint16_t dm90_Echo_Level;

    uint16_t dm90_Echo_Feedback;

    uint16_t dm90_Echo_Delay;

    uint16_t dm90_Echo_MaxDelay;

    //压缩器(Comp)
    uint16_t dm90_Comp_Enable;

    uint16_t dm90_Comp_Threshold;

    uint16_t dm90_Comp_Attack;

    uint16_t dm90_Comp_Release;

    uint16_t dm90_Comp_Rate;

    //限幅器(Limit)
    uint16_t dm90_Limit_Enable;

    uint16_t dm90_Limit_Threshold;

    uint16_t dm90_Limit_Attack;

    uint16_t dm90_Limit_Release;

    //音调
    uint16_t dm90_tone_lowGain;

    uint16_t dm90_tone_highGain;

    //监听混音比例
    uint16_t dm90_pc_mix;

    //MIC电平指示
    uint16_t dm90_Input_Level;

    uint16_t dm90_Output_Level;

    //LED灯光秀
    uint16_t dm90_LED_OnOff;

    uint16_t dm90_LED_Mode;

    //固定模式
    uint16_t dm90_FIX_H;

    uint16_t dm90_FIX_S;

    uint16_t dm90_FIX_V;

    //循坏模式
    uint16_t dm90_LOOP_TimeCnt;

    uint16_t dm90_LOOP_Rate;

    uint16_t dm90_LOOP_H;

    uint16_t dm90_LOOP_S;

    uint16_t dm90_LOOP_V;

    //呼吸模式
    uint16_t dm90_breathe_TimeCnt;

    uint16_t dm90_breathe_Rate;

    uint16_t dm90_breathe_H;

    uint16_t dm90_breathe_S;

    uint16_t dm90_breathe_V;

    //增加颜色
    uint16_t dm90_appendNumHSV;

    uint16_t dm90_appendH1;

    uint16_t dm90_appendS1;

    uint16_t dm90_appendV1;

    uint16_t dm90_appendH2;

    uint16_t dm90_appendS2;

    uint16_t dm90_appendV2;

    //MIC指向模式
    uint16_t dm90_PolarPattern;

}pc_DM90_device_dataSource;


//本地声音默认预设的结构体
typedef struct localSound_dataSource{

    //门限
    uint16_t dm90_threshold_Enable;

    uint16_t dm90_threshold_value;

    uint16_t dm90_threshold_Attack;

    uint16_t dm90_threshold_Release;

    uint16_t dm90_threshold_Hold;

    //高通滤波器
    uint16_t dm90_HPF_Enable;

    uint16_t dm90_HPF_Freq; //6

    //Mic 参量均衡器的8段EQ
    dm90_micEQ dm90micEQdatasource[8];

    //10段EQ,UsbPlay 参量均衡器
    //dm90_usbPalyEQ dm90UsbPalyEQdatasource[10];

    //齿音消除器
    uint16_t dm90_Dental_Enable;

    uint16_t dm90_Dental_Threshold;

    uint16_t dm90_Dental_Freq;

    uint16_t dm90_Dental_Q;

    uint16_t dm90_Dental_Attack;

    uint16_t dm90_Dental_Release;

    uint16_t dm90_Dental_MinGain;

    uint16_t dm90_Dental_DetectPeriod;

    //去爆音器
    uint16_t dm90_Detonator_Enable;

    uint16_t dm90_Detonator_Threshold;

    uint16_t dm90_Detonator_Freq;

    uint16_t dm90_Detonator_Q;

    uint16_t dm90_Detonator_Attack;

    uint16_t dm90_Detonator_Release;

    uint16_t dm90_Detonator_MinGain;

    uint16_t dm90_Detonator_DetectPeriod;

    //语音变调
    uint16_t dm90_VoiceSandhi_Enable;

    uint16_t dm90_VoiceSandhi_Rate;

    //镶边器/移相器
    uint16_t dm90_flanger_Enable;

    uint16_t dm90_flanger_Level;

    uint16_t dm90_flanger_Delay;

    uint16_t dm90_flanger_FreqModu;

    int16_t dm90_flanger_DeptModu;

    int16_t dm90_flanger_FeedRate;

    //压缩器(Comp)
    uint16_t dm90_Comp_Enable;

    uint16_t dm90_Comp_Threshold;

    uint16_t dm90_Comp_Attack;

    uint16_t dm90_Comp_Release;

    uint16_t dm90_Comp_Rate;

    //限幅器(Limit)
    uint16_t dm90_Limit_Enable;

    uint16_t dm90_Limit_Threshold;

    uint16_t dm90_Limit_Attack;

    uint16_t dm90_Limit_Release;

}pc_localSound_dataSource;

//效果声音默认预设的结构体
typedef struct effectSound_dataSource{
    //和声
    uint16_t dm90_harmony_Enable;

    uint16_t dm90_harmony_Level;

    uint16_t dm90_harmony_Delay;

    uint16_t dm90_harmony_FreqModu1;

    uint16_t dm90_harmony_DeptModu1;

    uint16_t dm90_harmony_FreqModu2;

    uint16_t dm90_harmony_DeptModu2;

    //混响(Reverb)
    uint16_t dm90_Reverb_Mode;

    uint16_t dm90_Reverb_Enable;

    uint16_t dm90_Reverb_PreDelay;

    uint16_t dm90_Reverb_DryLevel;

    uint16_t dm90_Reverb_WetLevel;

    uint16_t dm90_Reverb_Spread;

    uint16_t dm90_Reverb_RoomSize;

    uint16_t dm90_Reverb_Time;

    uint16_t dm90_Reverb_Damping;

    uint16_t dm90_Reverb_EarlyLevel;

    uint16_t dm90_Reverb_TailLevel;

    uint16_t dm90_Reverb_LpfEn;

    uint16_t dm90_Reverb_LpfBw;

    uint16_t dm90_Reverb_HpfEn;

    uint16_t dm90_Reverb_HpfBw;

    //回声(Echo) 延迟的一种算法
    uint16_t dm90_Echo_Mode;

    uint16_t dm90_Echo_Enable;

    uint16_t dm90_Echo_Level;

    uint16_t dm90_Echo_Feedback;

    uint16_t dm90_Echo_Delay;

    uint16_t dm90_Echo_MaxDelay;

}pc_effectSound_dataSource;


class dm90_deviceDataSource : public QObject
{
    Q_OBJECT
public:

    //单例模式
    static dm90_deviceDataSource *getInstance();


public:

    pc_DM90_device_dataSource *m_DM90_device_dataSource;

    //本地的声音预设
    pc_localSound_dataSource *m_localSound_dataSource;
    //效果的声音预设
    pc_effectSound_dataSource *m_effect_dataSource;

    //设备的负数转为实际的参数
    double loadingChangeNegativeNumber(int load_number){

        double load_number_value = (double)(load_number-2000)/10;

        return QString::number(load_number_value,'f',1).toDouble();
    }
    //实际参数转为设备需要的参数
    double loadingChangeDeviceNumber(double load_number){

        double load_number_value = load_number *10 +2000;
        return load_number_value;
    }


private:

    static dm90_deviceDataSource *self;


signals:

};

#endif // DM90_DEVICEDATASOURCE_H
