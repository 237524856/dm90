#ifndef SEND_RECEIVE_MASSAGE_CONTROL_H
#define SEND_RECEIVE_MASSAGE_CONTROL_H

#include <QWidget>




#define PARAM_MIC_MUTE                (0x2022)

#define PARAM_MIC_GAIN                (0x2023)
#define PARAM_MIC_GAIN_END            (0x2024)

#define PARAM_MIC_PEQ                 (0x2025)

#define PARAM_MIC_COMP_START          (0x2026)
#define PARAM_MIC_COMP_END            (0x202B)

#define PARAM_MIC_LIMIT_START         (0x202C)
#define PARAM_MIC_LIMIT_END           (0x202F)

#define PARAM_HP_GAIN                 (0x2030)

#define PARAM_LR_MONITOR              (0x2031)

#define PARAM_MIC_TONE                (0x2032)

#define PARAM_MIC_DISTANCE            (0x2033)

#define PARAM_MIC_SIG_LEVEL           (0x2034)

#define PARAM_USER_MODE               (0x2035)

#define PARAM_RGB_SHOW_ONOFF          (0x2036)

#define PARAM_RGB_BRIGHT              (0x2037)

#define PARAM_RGB_SHOW_MODE           (0x2038)


class send_receive_massage_control : public QWidget
{
    Q_OBJECT
public:
    explicit send_receive_massage_control(QWidget *parent = nullptr);

    void receive_background_par();

    void save_for_file();


    //pd400的方法
    void pd400_mute(int);
    //监听比例
    void pd400_monitor_mix(int);
    //监听比例事件
    void monitor_mix_switch(int);
    //耳机音量
    void pd400_headset_volume(int);

    //麦克风的音量
    void pd400_mic_volume(int);

    //音色调节
    void pd400_yinse(int);

    //远中进的调节
    void pd400_yuanzhongjin(int);

    //均衡器
    void pd400_mic(int);

    //麦克风增益
    void pd400_mic_gain(int);

    //限制器
    void pd400_limiter(int);
    //限制器启动时间
    void pd400_attack_limiter(int);
    //限制器释放时间
    void pd400_release_limiter(int);    
    //使能限制器
    void pd400_enble_limiter(int);

    //限制器重置
    void pd400_limiter_reset();

    //压缩器
    void pd400_comp(int);
    //最大增益
    void pd400_maxgain_comp(int);
    //压缩启动时间
    void pd400_attack_comp(int);
    //压缩释放时间
    void pd400_release_comp(int);
    //压缩器压缩比
    void pd400_rate_comp(int);

    //使能压缩器
    void pd400_enable_comp(int);

    //压缩器重置
    void pd400_comp_reset();

    //rgb灯光的开关
    void rgb_switch(int);
    //rgb灯光调节
    void rgb_adjust(int);
    //颜色的选择
    void rgb_color_select(int);

    //切换基础高级模式
    void change_basic_high_Model(int);

    //判断是否为DUF模式
    void is_dfuStatus();
    //获取所有的背景参数
    void bg_datasource();

    //获取当前设备的MODEL CODEL
    void getModelCode();

    //基础近
    void set_basic_near();
    //基础中
    void set_basic_mid();
    //基础远
    void set_basic_far();
    //高级
    void set_high(int gain_value);
    //保存GAIN
    void save_gain_adv();
    //DM30设备的基础
    void DM30_set_basic();
    //DM30设备的高级
    void DM30_set_high();
    //同步软件数据的标志
    void sys_Software_tag(int);
    //心跳包
    void hearbeat_fun();
    //恢复出厂设置
    void resetFactorySetting();

    //添加DM90的收发数据的方法======================================
    //连续读数据
    void sendMessageForDevice(int address,int number,int value);

    //dm90连续发送多个参数
    void sendCotiMessageForDevice(int frame_len,int address,int number,const QVector<int>);

    //随机写多个
    void sendRandomMessageForDevice(int frame_len,const QVector<int>,const QVector<int>);


signals:

};

#endif // SEND_RECEIVE_MASSAGE_CONTROL_H
