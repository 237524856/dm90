#include "common.h"
mydataSource background_data_pars={0x32,0x00086432,50,100,100,100,100,100,100,100,100,100,100,0x007803e8};

send_receive_massage_control *m_massge_control =NULL;
common_funtion *m_common_fun = NULL;


QVector<uint16_t > adds_arrs={0x00E0,0x00FC,0x00FB,0x0087,0x0089,0x008B,0x008D,0x008F,0x0091,0x0093,0x0095,0x0097,0x0099,0x009C,0x009A,0x00FF};

QVector<PACKDATA > m_sendMsgs;

QVector<PACKDATA > m_receiveMsgs;

QVector<QStringList>m_slider_map;

uint8_t LR_Balance = 0;
uint8_t LimiterLevel = 0;
uint8_t DAC_Mute_LH_Enable = 0;
uint8_t Reserved = 0;

int8_t BassEQGain=0;
int8_t MidEQGain=0;
int8_t TrebleEQGain=0;

uint8_t LPF_Freq_low=0;
uint8_t LPF_Freq_high=0;
uint8_t HPF_Freq_low=0;
uint8_t HPF_Freq_high=0;

bool isget_background_par = false;


//pd400--------------------------

QVector<mypc_random_write> pd400_m_receiveMsgs;

QVector<pc_random_write> pd400_m_sendMsgs;

QVector<pc_continuity_read> m_dm90continuityReadDatas;

QVector<pc_continuity_write> m_dm90continuityWriteDatas;

QVector<pc_random_write_multiple> m_dm90random_Write_multipleDatas;

QVector<pc_continuity_read> pd400_m_receive_bgMsgs;

QVector<pc_random_read> m_receive_pcParMsgs;

QVector<pc_continuity_read> m_getModelCodelDatas;

int pd400_mic_value = 100;

QVector<int>yaxianqi_slider_values={40,0,0,1,1};

QVector<int>xianfuqi_slider_values = {40,0,0};

pc_random_write receive_radom_data = {};

pc_random_write send_radom_data = {};
//随机读
pc_random_read              receive_radom_bgdata={};
//连续读
pc_continuity_read          continuity_read_data={};
//连续写
pc_continuity_write         continuity_write_data={};
//随机读
pc_random_write_multiple    continuity_write_multiple_data={};

QVector<QCheckBox *>m_mute_arrs;
QVector<QLabel *>m_micMon_arrs;

QVector<SliderWidget *>m_pd400SynSlider_arrs;
QVector<MySlider *>m_synSlider_arrs;
QVector<MySlider *>m_synmicvolume_Slider_arrs;
QVector<MySlider *>pd400ComLimGainSliders;

int pd400_send_value= 1000000;
QMap<QString,int>DM30_send_value;
QMap<QString,int>PD400_send_value;
int pd400_diffsend_value= 1000000;
QMap<QString,int>DM30_different_value;
QMap<QString,int>PD400_different_value;

QString version_str;
//设备的版本号数字
uint16_t device_version_x = 0;
uint16_t device_version_y = 0;
uint16_t device_version_z = 0;

uint16_t dsp_version_x = 0;
uint16_t dsp_version_y = 0;
uint16_t dsp_version_z = 0;

QString version_DFU_str ="";
bool is_usb_connect =false;
bool is_modelconnect = false;

QString DFU_version = "1.0.00";

QVector<mypc_random_write>m_modelCodelName;

uint32_t currentProPid;

//当前项目的MODELCODEL
QString m_currentModelCodel = "";

int language_index =0;

bool testis_connect = false;
//pm481
bool is_pm481_usb_connect =false;

int m_current_model = 0;
int m_pd400current_model = 0;

QTimer *m_PM481Timer = NULL;

QTimer *m_DM90delayTimer = nullptr;
int Qwdget_timer_index = 1;

QTimer *m_MainTimer = nullptr;

int m_PM481slider_current_value = 0;
int m_PM481slider_current_number = 0;

bool m_DM30phsilder_runing = false;
bool m_DM30micsilder_runing = false;

bool m_PD400phsilder_runing = false;
bool m_PD400mixsilder_runing = false;
bool m_PD400gainsilder_runing = false;

int m_dm30downloaddfu_number = 0;

bool is_needto_updatadsp = false;
bool is_dm30updata_runing = false;

bool is_pd400needto_updatadsp = false;
bool is_pd400updata_runing = false;

bool is_show_device =false;

QString m_dm30file_dsp;
QString m_PD400file_dsp;

dm90_recoderview *com_recordView=NULL;

QStringList highView_customTempPreData;

QString m_appversion_str = "2.1.2";
double m_Magnification = 1.0;
