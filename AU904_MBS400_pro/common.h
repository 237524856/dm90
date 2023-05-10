#ifndef COMMON_H
#define COMMON_H
#include<QVector>
#include <QMap>
#include "msgbox.h"
#include <QFileDialog>
#include <QApplication>
#include "qcheckbox.h"
#include <QFile>
#include "HID/utils/CommunicateInterface.h"
#include "send_receive_massage_control.h"
#include "common_funtion.h"
#include "MBS_400/myslider.h"
#include <QMap>
#include "DM90/dm90_recoderview.h"
#include "DM90/dm90_devicedatasource.h"
#include "PM481/pm481_devicedatasource.h"
#include "PD400/sliderwidget.h"


//音量的寄存器地址
#define MonitorVol 0x00FC

//指定的某一位数值为1
#define setbit(x,y)  x|=(1<<y)
//指定的某一位数值为0
#define clrbit(x,y)  x&=~(1<<y)
//指定的某一位数取反
#define reversebit(x,y)  x^=(1<<y)
//获取的某一位的值
#define getbit(x,y)   ((x) >> (y)&1)

//如果打开为9 关闭为0
#define OPENSN 9

#define GET_DEVICE_DATA dm90_deviceDataSource::getInstance()->m_DM90_device_dataSource

#define GET_LOCAL_DATA dm90_deviceDataSource::getInstance()->m_localSound_dataSource

#define GET_EFFECT_DATA dm90_deviceDataSource::getInstance()->m_effect_dataSource

#define GET_PM481_DEVICE_DATA pm481_deviceDatasource::getInstance()->m_PM481_device_dataSource

//#pragma pack(1)
typedef struct dataSource{

    int8_t data_monitorVol;
    int32_t data_LR_lim_loud_mute;
    int32_t data_bass_mid_hid;
    int8_t data_eq0;
    int8_t data_eq1;
    int8_t data_eq2;
    int8_t data_eq3;
    int8_t data_eq4;
    int8_t data_eq5;
    int8_t data_eq6;
    int8_t data_eq7;
    int8_t data_eq8;
    int8_t data_eq9;
    int32_t LPF_HPF_Freq;


} mydataSource;



extern mydataSource background_data_pars;

extern send_receive_massage_control *m_massge_control;
extern common_funtion *m_common_fun;

//所有的寄存器的地址
extern QVector<uint16_t > adds_arrs;

//发送给设备的数组

extern QVector<PACKDATA > m_sendMsgs;

//接收数据的数组
extern QVector<PACKDATA > m_receiveMsgs;


extern QVector<QStringList>m_slider_map;


//发送给设备的各种结构数组
extern uint8_t LR_Balance;
extern uint8_t LimiterLevel;
extern uint8_t DAC_Mute_LH_Enable;
extern uint8_t Reserved;

//低音、中音、高音
extern int8_t BassEQGain;
extern int8_t MidEQGain;
extern int8_t TrebleEQGain;

//高级中的滤波器
extern uint8_t LPF_Freq_low;
extern uint8_t LPF_Freq_high;
extern uint8_t HPF_Freq_low;
extern uint8_t HPF_Freq_high;

//是否获取背景参数
extern bool isget_background_par;


//pd400-----------------------------------------------------

extern int pd400_mic_value;

extern QVector<int>yaxianqi_slider_values;

extern QVector<int>xianfuqi_slider_values;


//随机往设备写入数据======只有一包数据
typedef struct random_write
{
   uint8_t report; //报文ID
    union
    {
        //USB HID 一个包 （全速最多64byte, 高速可达1024byte）
        uint8_t buff[11];
        //协议的数据包结构
        struct
        {
            uint8_t frame_header;   // 帧头
            uint8_t frame_len_L;  // 帧长度
            uint8_t frame_len_H;  // 帧长度
            uint8_t cp_addr;  // 设备地址
            uint8_t command;		// 命令编码
            //数据
            uint8_t param_addr[2];//数据的地址
            uint8_t param_value[2];	//数据的值
            uint8_t crc_L;			// CRC校验
            uint8_t crc_H;			// CRC校验
        };
    };
}pc_random_write;

//随机从设备读取数据======只有一包数据
typedef struct random_read
{
   uint8_t report; //报文ID
    union
    {
        //USB HID 一个包 （全速最多64byte, 高速可达1024byte）
        uint8_t buff[9];

        //协议的数据包结构
        struct
        {
            uint8_t frame_header;   // 帧头
            uint8_t frame_len_L;  // 帧长度
            uint8_t frame_len_H;  // 帧长度
            uint8_t cp_addr;  // 设备地址
            uint8_t command;		// 命令编码
            //数据
            uint8_t param_addr[2];//数据的地址
            uint8_t crc_L;			// CRC校验
            uint8_t crc_H;			// CRC校验
        };
    };
}pc_random_read;


//连续往设备写入数据
typedef struct continuity_write
{
    uint8_t report;
    union
    {
        //USB HID 一个包 （全速最多64byte, 高速可达1024byte）
        uint8_t buff[64];
        //协议的数据包结构
        struct
        {

            uint8_t frame_header;   // 帧头
            uint8_t frame_len_L;  // 帧长度 L
            uint8_t frame_len_H;  // 帧长度 H
            uint8_t cp_addr;  // 设备地址
            uint8_t command;		// 命令编码
            //数据部分----------------
            uint8_t param_addr[2];//数据的地址
            uint8_t param_num[2];//数据的长度
            uint8_t param_value[2];	//数据的值
            uint8_t crc_L;			// CRC校验
            uint8_t crc_H;
            uint8_t extend_number[51];	//数据的值
        };
    };
}pc_continuity_write;


//随机写的地址和值
typedef struct write_mulitple{

    uint8_t addressL;
    uint8_t addressH;
    uint8_t valueL;
    uint8_t valueH;

} dm90_addrAndValue;


//随机往设备写入N包数据
typedef struct random_write_multiple
{
    uint8_t report;
    union
    {
        //USB HID 一个包 （全速最多64byte, 高速可达1024byte）
        uint8_t buff[64];
        //协议的数据包结构
        struct
        {
            uint8_t frame_header;   // 帧头
            uint8_t frame_len_L;  // 帧长度 L
            uint8_t frame_len_H;  // 帧长度 H
            uint8_t cp_addr;  // 设备地址
            uint8_t command;		// 命令编码
            //数据部分----------------
            dm90_addrAndValue addrAndValue[14];

            uint8_t crc_L;			// CRC校验
            uint8_t crc_H;
            uint8_t extend_number;	//数据的值
        };
    };
}pc_random_write_multiple;



//连续从设备读取数据
typedef struct continuity_read
{
   uint8_t report; //报文ID
    union
    {
        //USB HID 一个包 （全速最多64byte, 高速可达1024byte）
        uint8_t buff[64];

        //协议的数据包结构
        struct
        {
            uint8_t frame_header;   // 帧头
            uint8_t frame_len_L;  // 帧长度
            uint8_t frame_len_H;  // 帧长度
            uint8_t cp_addr;  // 设备地址
            uint8_t command;		// 命令编码
            //数据
            uint8_t param_addr[2];//数据的地址
            uint8_t param_number[2];//读取得值的个数
            uint8_t crc_L;			// CRC校验
            uint8_t crc_H;			// CRC校验
            uint8_t extend_number[53];  // 扩展的字符
        };
    };
}pc_continuity_read;


//pd400发送给设备的数组

extern QVector<mypc_random_write> pd400_m_receiveMsgs;

extern QVector<pc_random_write> pd400_m_sendMsgs;

//DM90连续读取数组的队列
extern QVector<pc_continuity_read> m_dm90continuityReadDatas;

//DM90连续写数组的队列
extern QVector<pc_continuity_write> m_dm90continuityWriteDatas;
//DM90随机写数组的队列（多包）
extern QVector<pc_random_write_multiple> m_dm90random_Write_multipleDatas;


//读取背景参数
extern QVector<pc_continuity_read> pd400_m_receive_bgMsgs;
//除背景参数外需要读取设备的数据数组
extern QVector<pc_random_read> m_receive_pcParMsgs;

//获取MODELCODEL数据
extern QVector<pc_continuity_read> m_getModelCodelDatas;

extern pc_random_write receive_radom_data;

extern pc_random_write send_radom_data;

//随机读数据
extern pc_random_read       receive_radom_bgdata;

//连续读数据
extern pc_continuity_read   continuity_read_data;
//连续写数据
extern pc_continuity_write   continuity_write_data;
//随机写入数据（多包）
extern pc_random_write_multiple  continuity_write_multiple_data;

//同步的静音按钮
extern QVector<QCheckBox *>m_mute_arrs;

//同步麦克风监听
extern QVector<QLabel *>m_micMon_arrs;

//同步的slider
extern QVector<SliderWidget *>m_pd400SynSlider_arrs;
extern QVector<MySlider *>m_synSlider_arrs;
extern QVector<MySlider *>m_synmicvolume_Slider_arrs;

extern QVector<MySlider *>pd400ComLimGainSliders;

//当前要发送的值
extern int pd400_send_value;
extern QMap<QString,int>DM30_send_value;
extern QMap<QString,int>PD400_send_value;
//已经发送的值
extern int pd400_diffsend_value;
extern QMap<QString,int>DM30_different_value;
extern QMap<QString,int>PD400_different_value;
//版本号
extern QString version_str;
//设备的版本号数字
extern uint16_t device_version_x;
extern uint16_t device_version_y;
extern uint16_t device_version_z;

extern uint16_t dsp_version_x;
extern uint16_t dsp_version_y;
extern uint16_t dsp_version_z;

extern QString version_DFU_str;

//判断USB是否连接
extern bool is_usb_connect;

//判断model
extern bool is_modelconnect;

//软件的版本号
extern QString DFU_version;

//当前设备的MODELCODEL
extern QVector<mypc_random_write>m_modelCodelName;

//当前项目的PID
extern uint32_t currentProPid;

//当前项目的MODELCODEL
extern QString m_currentModelCodel;

//当前语言
extern int language_index;

extern bool testis_connect;

//pm481的项目---------------------------------------------------------------------------
extern bool is_pm481_usb_connect;

extern int m_current_model;

extern int m_pd400current_model;

//发送最后一包的计时器
extern QTimer *m_PM481Timer;

//dm90需要延时获取数据发送得计时器
extern QTimer *m_DM90delayTimer;
//页面计时器的下标
extern int Qwdget_timer_index;

//主入口显示设备的计时器
extern QTimer *m_MainTimer;

extern int m_PM481slider_current_value;
extern int m_PM481slider_current_number;

extern bool m_DM30phsilder_runing;
extern bool m_DM30micsilder_runing;

extern bool m_PD400phsilder_runing;
extern bool m_PD400mixsilder_runing;
extern bool m_PD400gainsilder_runing;

//下载DFU文件的次数
extern int m_dm30downloaddfu_number;
//DSP是否需要升级
extern bool is_needto_updatadsp;

extern bool is_dm30updata_runing;

extern bool is_pd400needto_updatadsp;

extern bool is_pd400updata_runing;


extern QString m_dm30file_dsp;
extern QString m_PD400file_dsp;

//录音的视图
extern dm90_recoderview *com_recordView;

extern bool is_show_device;

//高级页面的自定义预设
extern QStringList highView_customTempPreData;


//上位机的版本号
extern QString m_appversion_str;
extern double m_Magnification;

#endif // COMMON_H
