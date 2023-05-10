
#pragma once
#include "send_receive_massage_control.h"
#include "common.h"
#include <QDebug>
#include <QStandardPaths>
#include "HID/utils/CRC.h"

send_receive_massage_control::send_receive_massage_control(QWidget *parent) : QWidget(parent)
{

}
void send_receive_massage_control::receive_background_par(){
    //m_sendMsgs.clear();//请求之前先删除
    m_receiveMsgs.clear();
    //qDebug()<<"xxxxresfreUI";
    for (int i = 0;i<adds_arrs.count();i++) {
        PACKDATA send = {  };
        send.command = 0x4b;

        send.index[0] = adds_arrs.at(i) & 0xFF;
        send.index[1] = (adds_arrs.at(i)>> 8)&0xFF;

        send.index[4] = 0x76;
        send.index[5] = 0;
        send.index[6] = 0;
        send.index[7] = 0;
        send.index[8] = 0;
        send.index[9] = 0;

        m_sendMsgs.push_back(send);
    }

}
void send_receive_massage_control::save_for_file(){

    QString filename = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+"/yangyu.dat";
    QFile file(filename);
    if(file.open(QIODevice::WriteOnly)) //只写模式
    {
        QDataStream out(&file);
        out << m_slider_map;
        file.close();
        //refresh_user_list();
    }
}
void send_receive_massage_control::pd400_mute(int status){
    send_radom_data = {};
    send_radom_data.report = 0x4b;
    send_radom_data.frame_header = 0xc4;
    send_radom_data.frame_len_L = 0x0b;
    send_radom_data.command = 0x03;
    send_radom_data.param_addr[0] = PARAM_MIC_MUTE & 0xFF;
    send_radom_data.param_addr[1] = 0x20;
    if(status==0){
       send_radom_data.param_value[0] = 0;
    }else{
       send_radom_data.param_value[0] = 1;
    }
    uint16_t pd400_crc = CCRC::Crc16((uint8_t *)&send_radom_data, sizeof(send_radom_data)-2);
    send_radom_data.crc_L = (pd400_crc) & 0xFF;
    send_radom_data.crc_H = (pd400_crc >> 8) & 0xFF;
    if(!is_usb_connect)return;
    pd400_m_sendMsgs.push_back(send_radom_data);
    qDebug()<<"pd400_mute"<<status;
}
void send_receive_massage_control::pd400_mic_volume(int value){

//    if(!is_usb_connect)return;
    send_radom_data = {};
    send_radom_data.report = 0x4b;
    send_radom_data.frame_header = 0xc4;
    send_radom_data.frame_len_L = 0x0b;
    send_radom_data.command = 0x03;
    send_radom_data.param_addr[0] = 0x23;
    send_radom_data.param_addr[1] = 0x20;
    send_radom_data.param_value[0] = value;

    uint16_t pd400_crc = CCRC::Crc16((uint8_t *)&send_radom_data, sizeof(send_radom_data)-2);
    send_radom_data.crc_L = (pd400_crc) & 0xFF;
    send_radom_data.crc_H = (pd400_crc >> 8) & 0xFF;

}

void send_receive_massage_control::pd400_headset_volume(int value){

    send_radom_data = {};
    send_radom_data.report = 0x4b;
    send_radom_data.frame_header = 0xc4;
    send_radom_data.frame_len_L = 0x0b;
    send_radom_data.command = 0x03;
    send_radom_data.param_addr[0] = PARAM_HP_GAIN & 0xff;
    send_radom_data.param_addr[1] = 0x20;

    send_radom_data.param_value[0] = value & 0xFF;

    uint16_t pd400_crc = CCRC::Crc16((uint8_t *)&send_radom_data, sizeof(send_radom_data)-2);
    send_radom_data.crc_L = (pd400_crc) & 0xFF;
    send_radom_data.crc_H = (pd400_crc >> 8) & 0xFF;
}
void send_receive_massage_control::pd400_monitor_mix(int value){

    send_radom_data = {};
    send_radom_data.report = 0x4b;
    send_radom_data.frame_header = 0xc4;
    send_radom_data.frame_len_L = 0x0b;
    send_radom_data.command = 0x03;
    send_radom_data.param_addr[0] = PARAM_LR_MONITOR & 0xff;
    send_radom_data.param_addr[1] = 0x20;

    //qDebug()<<"value"<<value-100;
    int changevalue = (value-100)*10+2000;
    send_radom_data.param_value[0] = changevalue & 0xFF;
    send_radom_data.param_value[1] = (changevalue>>8) & 0xFF;

    uint16_t pd400_crc = CCRC::Crc16((uint8_t *)&send_radom_data, sizeof(send_radom_data)-2);
    send_radom_data.crc_L = (pd400_crc) & 0xFF;
    send_radom_data.crc_H = (pd400_crc >> 8) & 0xFF;
}
void send_receive_massage_control::monitor_mix_switch(int value){
    send_radom_data = {};
    send_radom_data.report = 0x4b;
    send_radom_data.frame_header = 0xc4;
    send_radom_data.frame_len_L = 0x0b;
    send_radom_data.command = 0x03;
    send_radom_data.param_addr[0] = PARAM_LR_MONITOR & 0xff;
    send_radom_data.param_addr[1] = 0x20;

    //qDebug()<<"value"<<value-100;
    int changevalue = (value-100)*10+2000;
    send_radom_data.param_value[0] = changevalue & 0xFF;
    send_radom_data.param_value[1] = (changevalue>>8) & 0xFF;

    uint16_t pd400_crc = CCRC::Crc16((uint8_t *)&send_radom_data, sizeof(send_radom_data)-2);
    send_radom_data.crc_L = (pd400_crc) & 0xFF;
    send_radom_data.crc_H = (pd400_crc >> 8) & 0xFF;
    if(!is_usb_connect)return;
    pd400_m_sendMsgs.push_back(send_radom_data);
}
void send_receive_massage_control::pd400_yinse(int yinse_status){

    send_radom_data = {};
    send_radom_data.report = 0x4b;
    send_radom_data.frame_header = 0xc4;
    send_radom_data.frame_len_L = 0x0b;
    send_radom_data.command = 0x03;
    send_radom_data.param_addr[0] = PARAM_MIC_TONE & 0xff;
    send_radom_data.param_addr[1] = 0x20;
    send_radom_data.param_value[0] = yinse_status;
    uint16_t pd400_crc = CCRC::Crc16((uint8_t *)&send_radom_data, sizeof(send_radom_data)-2);
    send_radom_data.crc_L = (pd400_crc) & 0xFF;
    send_radom_data.crc_H = (pd400_crc >> 8) & 0xFF;
    if(!is_usb_connect)return;
    pd400_m_sendMsgs.push_back(send_radom_data);
    qDebug()<<"pd400_yinse"<<yinse_status;

}
void send_receive_massage_control::pd400_yuanzhongjin(int yuanzhongjin_status){

    send_radom_data = {};
    send_radom_data.report = 0x4b;
    send_radom_data.frame_header = 0xc4;
    send_radom_data.frame_len_L = 0x0b;
    send_radom_data.command = 0x03;
    send_radom_data.param_addr[0] = PARAM_MIC_DISTANCE & 0xff;
    send_radom_data.param_addr[1] = 0x20;
    send_radom_data.param_value[0] = yuanzhongjin_status;
    uint16_t pd400_crc = CCRC::Crc16((uint8_t *)&send_radom_data, sizeof(send_radom_data)-2);
    send_radom_data.crc_L = (pd400_crc) & 0xFF;
    send_radom_data.crc_H = (pd400_crc >> 8) & 0xFF;
    if(!is_usb_connect)return;
    pd400_m_sendMsgs.push_back(send_radom_data);
    qDebug()<<"pd400_yinse"<<yuanzhongjin_status;
}
void send_receive_massage_control::pd400_mic(int mic_value){

    send_radom_data = {};
    send_radom_data.report = 0x4b;
    send_radom_data.frame_header = 0xc4;
    send_radom_data.frame_len_L = 0x0b;
    send_radom_data.command = 0x03;
    send_radom_data.param_addr[0] = PARAM_MIC_PEQ & 0xff;
    send_radom_data.param_addr[1] = 0x20;
    send_radom_data.param_value[0] = mic_value;
    uint16_t pd400_crc = CCRC::Crc16((uint8_t *)&send_radom_data, sizeof(send_radom_data)-2);
    send_radom_data.crc_L = (pd400_crc) & 0xFF;
    send_radom_data.crc_H = (pd400_crc >> 8) & 0xFF;
    if(!is_usb_connect)return;
    pd400_m_sendMsgs.push_back(send_radom_data);
    qDebug()<<"pd400_yinse"<<mic_value;
}
void send_receive_massage_control::pd400_mic_gain(int mic_gain_value){


    send_radom_data = {};
    send_radom_data.report = 0x4b;
    send_radom_data.frame_header = 0xc4;
    send_radom_data.frame_len_L = 0x0b;
    send_radom_data.command = 0x03;
    send_radom_data.param_addr[0] = PARAM_MIC_GAIN & 0xff;
    send_radom_data.param_addr[1] = 0x20;

    send_radom_data.param_value[0] = mic_gain_value;

    uint16_t pd400_crc = CCRC::Crc16((uint8_t *)&send_radom_data, sizeof(send_radom_data)-2);
    send_radom_data.crc_L = (pd400_crc) & 0xFF;
    send_radom_data.crc_H = (pd400_crc >> 8) & 0xFF;
};
//使能限制器
void send_receive_massage_control::pd400_enble_limiter(int status){

    send_radom_data = {};
    send_radom_data.report = 0x4b;
    send_radom_data.frame_header = 0xc4;
    send_radom_data.frame_len_L = 0x0b;
    send_radom_data.command = 0x03;
    send_radom_data.param_addr[0] = PARAM_MIC_LIMIT_START & 0xff;
    send_radom_data.param_addr[1] = 0x20;

    send_radom_data.param_value[0] = status;

    uint16_t pd400_crc = CCRC::Crc16((uint8_t *)&send_radom_data, sizeof(send_radom_data)-2);
    send_radom_data.crc_L = (pd400_crc) & 0xFF;
    send_radom_data.crc_H = (pd400_crc >> 8) & 0xFF;
    if(!is_usb_connect)return;
    pd400_m_sendMsgs.push_back(send_radom_data);
}
void send_receive_massage_control::pd400_limiter(int xianzhiqi_value){
    send_radom_data = {};
    send_radom_data.report = 0x4b;
    send_radom_data.frame_header = 0xc4;
    send_radom_data.frame_len_L = 0x0b;
    send_radom_data.command = 0x03;
    send_radom_data.param_addr[0] = (PARAM_MIC_LIMIT_START & 0xff)+ 1;
    send_radom_data.param_addr[1] = 0x20;

    int changevalue = (xianzhiqi_value-40)*10+2000;
    send_radom_data.param_value[0] = changevalue & 0xFF;
    send_radom_data.param_value[1] = (changevalue>>8) & 0xFF;

    uint16_t pd400_crc = CCRC::Crc16((uint8_t *)&send_radom_data, sizeof(send_radom_data)-2);
    send_radom_data.crc_L = (pd400_crc) & 0xFF;
    send_radom_data.crc_H = (pd400_crc >> 8) & 0xFF;
}
void send_receive_massage_control::pd400_attack_limiter(int limiter_attack_value){

    send_radom_data = {};
    send_radom_data.report = 0x4b;
    send_radom_data.frame_header = 0xc4;
    send_radom_data.frame_len_L = 0x0b;
    send_radom_data.command = 0x03;
    send_radom_data.param_addr[0] = (PARAM_MIC_LIMIT_START & 0xff) + 2;
    send_radom_data.param_addr[1] = 0x20;

    send_radom_data.param_value[0] = limiter_attack_value;

    uint16_t pd400_crc = CCRC::Crc16((uint8_t *)&send_radom_data, sizeof(send_radom_data)-2);
    send_radom_data.crc_L = (pd400_crc) & 0xFF;
    send_radom_data.crc_H = (pd400_crc >> 8) & 0xFF;
}
void send_receive_massage_control::pd400_release_limiter(int limiter_release_value){

    send_radom_data = {};
    send_radom_data.report = 0x4b;
    send_radom_data.frame_header = 0xc4;
    send_radom_data.frame_len_L = 0x0b;
    send_radom_data.command = 0x03;
    send_radom_data.param_addr[0] = (PARAM_MIC_LIMIT_START & 0xff) + 3;
    send_radom_data.param_addr[1] = 0x20;

    send_radom_data.param_value[0] = limiter_release_value & 0xFF;
    send_radom_data.param_value[1] = (limiter_release_value >>8) & 0xFF;

    uint16_t pd400_crc = CCRC::Crc16((uint8_t *)&send_radom_data, sizeof(send_radom_data)-2);
    send_radom_data.crc_L = (pd400_crc) & 0xFF;
    send_radom_data.crc_H = (pd400_crc >> 8) & 0xFF;
}

void send_receive_massage_control::pd400_limiter_reset(){

    for (int i=0;i<3;i++) {

        send_radom_data = {};
        send_radom_data.report = 0x4b;
        send_radom_data.frame_header = 0xc4;
        send_radom_data.frame_len_L = 0x0b;
        send_radom_data.command = 0x03;

        send_radom_data.param_addr[1] = 0x20;
        send_radom_data.param_addr[0] = (PARAM_MIC_LIMIT_START & 0xff) +1 +i;
        if(i==0){
            //send_radom_data.param_addr[0] = 0x2b;
            int changevalue = 2000;
            send_radom_data.param_value[0] = changevalue & 0xFF;
            send_radom_data.param_value[1] = (changevalue>>8) & 0xFF;
        }else if(i==2){
            //send_radom_data.param_addr[0] = 0x2c;
            send_radom_data.param_value[0] = 200;
        }else{
            //send_radom_data.param_addr[0] = 0x2d;
            send_radom_data.param_value[0] = 100;
        }
        uint16_t pd400_crc = CCRC::Crc16((uint8_t *)&send_radom_data, sizeof(send_radom_data)-2);
        send_radom_data.crc_L = (pd400_crc) & 0xFF;
        send_radom_data.crc_H = (pd400_crc >> 8) & 0xFF;
        if(!is_usb_connect)return;
        pd400_m_sendMsgs.push_back(send_radom_data);
    }

}
void send_receive_massage_control::pd400_comp(int comp_value){

    send_radom_data = {};
    send_radom_data.report = 0x4b;
    send_radom_data.frame_header = 0xc4;
    send_radom_data.frame_len_L = 0x0b;
    send_radom_data.command = 0x03;
    send_radom_data.param_addr[0] = (PARAM_MIC_COMP_START & 0xff) +1;
    send_radom_data.param_addr[1] = 0x20;

    int changevalue = (comp_value-80)*10+2000;
    send_radom_data.param_value[0] = changevalue & 0xFF;
    send_radom_data.param_value[1] = (changevalue>>8) & 0xFF;

    uint16_t pd400_crc = CCRC::Crc16((uint8_t *)&send_radom_data, sizeof(send_radom_data)-2);
    send_radom_data.crc_L = (pd400_crc) & 0xFF;
    send_radom_data.crc_H = (pd400_crc >> 8) & 0xFF;
}
void send_receive_massage_control::pd400_maxgain_comp(int maxgain){

    send_radom_data = {};
    send_radom_data.report = 0x4b;
    send_radom_data.frame_header = 0xc4;
    send_radom_data.frame_len_L = 0x0b;
    send_radom_data.command = 0x03;
    send_radom_data.param_addr[0] = (PARAM_MIC_COMP_START & 0xff) +2;
    send_radom_data.param_addr[1] = 0x20;

    int changevalue = maxgain*10+2000;
    send_radom_data.param_value[0] = changevalue & 0xFF;
    send_radom_data.param_value[1] = (changevalue>>8) & 0xFF;

    uint16_t pd400_crc = CCRC::Crc16((uint8_t *)&send_radom_data, sizeof(send_radom_data)-2);
    send_radom_data.crc_L = (pd400_crc) & 0xFF;
    send_radom_data.crc_H = (pd400_crc >> 8) & 0xFF;
}
void send_receive_massage_control::pd400_attack_comp(int comp_attack_value){
    send_radom_data = {};
    send_radom_data.report = 0x4b;
    send_radom_data.frame_header = 0xc4;
    send_radom_data.frame_len_L = 0x0b;
    send_radom_data.command = 0x03;
    send_radom_data.param_addr[0] = (PARAM_MIC_COMP_START & 0xff) +3;
    send_radom_data.param_addr[1] = 0x20;

    send_radom_data.param_value[0] = comp_attack_value;

    uint16_t pd400_crc = CCRC::Crc16((uint8_t *)&send_radom_data, sizeof(send_radom_data)-2);
    send_radom_data.crc_L = (pd400_crc) & 0xFF;
    send_radom_data.crc_H = (pd400_crc >> 8) & 0xFF;
}
void send_receive_massage_control::pd400_release_comp(int comp_release_value){

    send_radom_data = {};
    send_radom_data.report = 0x4b;
    send_radom_data.frame_header = 0xc4;
    send_radom_data.frame_len_L = 0x0b;
    send_radom_data.command = 0x03;
    send_radom_data.param_addr[0] = (PARAM_MIC_COMP_START & 0xff) +4;
    send_radom_data.param_addr[1] = 0x20;

    send_radom_data.param_value[0] = comp_release_value & 0xFF;
    send_radom_data.param_value[1] = (comp_release_value >>8) & 0xFF;

    uint16_t pd400_crc = CCRC::Crc16((uint8_t *)&send_radom_data, sizeof(send_radom_data)-2);
    send_radom_data.crc_L = (pd400_crc) & 0xFF;
    send_radom_data.crc_H = (pd400_crc >> 8) & 0xFF;
}
void send_receive_massage_control:: pd400_rate_comp(int comp_rate_value){

    send_radom_data = {};
    send_radom_data.report = 0x4b;
    send_radom_data.frame_header = 0xc4;
    send_radom_data.frame_len_L = 0x0b;
    send_radom_data.command = 0x03;
    send_radom_data.param_addr[0] = PARAM_MIC_COMP_END & 0xff;
    send_radom_data.param_addr[1] = 0x20;

    send_radom_data.param_value[0] = comp_rate_value;

    uint16_t pd400_crc = CCRC::Crc16((uint8_t *)&send_radom_data, sizeof(send_radom_data)-2);
    send_radom_data.crc_L = (pd400_crc) & 0xFF;
    send_radom_data.crc_H = (pd400_crc >> 8) & 0xFF;
}

void send_receive_massage_control::pd400_enable_comp(int status){
    send_radom_data = {};
    send_radom_data.report = 0x4b;
    send_radom_data.frame_header = 0xc4;
    send_radom_data.frame_len_L = 0x0b;
    send_radom_data.command = 0x03;
    send_radom_data.param_addr[0] = PARAM_MIC_COMP_START & 0xff;
    send_radom_data.param_addr[1] = 0x20;

    send_radom_data.param_value[0] = status;

    uint16_t pd400_crc = CCRC::Crc16((uint8_t *)&send_radom_data, sizeof(send_radom_data)-2);
    send_radom_data.crc_L = (pd400_crc) & 0xFF;
    send_radom_data.crc_H = (pd400_crc >> 8) & 0xFF;
    if(!is_usb_connect)return;
    pd400_m_sendMsgs.push_back(send_radom_data);
}
void send_receive_massage_control::pd400_comp_reset(){

    for (int i=0;i<5;i++) {
        send_radom_data = {};
        send_radom_data.report = 0x4b;
        send_radom_data.frame_header = 0xc4;
        send_radom_data.frame_len_L = 0x0b;
        send_radom_data.command = 0x03;

        send_radom_data.param_addr[1] = 0x20;
        send_radom_data.param_addr[0] = (PARAM_MIC_COMP_START & 0xff) + 1 + i;
        if(i==0){
            //send_radom_data.param_addr[0] = 0x26;
            int changevalue = 2000;
            send_radom_data.param_value[0] = changevalue & 0xFF;
            send_radom_data.param_value[1] = (changevalue>>8) & 0xFF;
        }else if(i==2){
            //send_radom_data.param_addr[0] = 0x27;
            send_radom_data.param_value[0] = 100;
        }else if(i==3){
            //send_radom_data.param_addr[0] = 0x28;
            send_radom_data.param_value[0] = 200;
        }else if(i==4){
            //send_radom_data.param_addr[0] = 0x29;
            send_radom_data.param_value[0] = 1;
        }else{
            int changevalue = 2010;
            send_radom_data.param_value[0] = changevalue & 0xFF;
            send_radom_data.param_value[1] = (changevalue>>8) & 0xFF;
        }
        uint16_t pd400_crc = CCRC::Crc16((uint8_t *)&send_radom_data, sizeof(send_radom_data)-2);
        send_radom_data.crc_L = (pd400_crc) & 0xFF;
        send_radom_data.crc_H = (pd400_crc >> 8) & 0xFF;
        if(!is_usb_connect)return;
        pd400_m_sendMsgs.push_back(send_radom_data);
    }
}

void send_receive_massage_control::rgb_switch(int status){

    send_radom_data = {};
    send_radom_data.report = 0x4b;
    send_radom_data.frame_header = 0xc4;
    send_radom_data.frame_len_L = 0x0b;
    send_radom_data.command = 0x03;
    send_radom_data.param_addr[0] = PARAM_RGB_SHOW_ONOFF& 0xff;
    send_radom_data.param_addr[1] = 0x20;
    if(status==0){
       send_radom_data.param_value[0] = 0;
    }else{
       send_radom_data.param_value[0] = 1;
    }
    uint16_t pd400_crc = CCRC::Crc16((uint8_t *)&send_radom_data, sizeof(send_radom_data)-2);
    send_radom_data.crc_L = (pd400_crc) & 0xFF;
    send_radom_data.crc_H = (pd400_crc >> 8) & 0xFF;
    if(!is_usb_connect)return;
    pd400_m_sendMsgs.push_back(send_radom_data);
}
void send_receive_massage_control::rgb_adjust(int adjValue){

    send_radom_data = {};
    send_radom_data.report = 0x4b;
    send_radom_data.frame_header = 0xc4;
    send_radom_data.frame_len_L = 0x0b;
    send_radom_data.command = 0x03;
    send_radom_data.param_addr[0] = PARAM_RGB_BRIGHT & 0xff;
    send_radom_data.param_addr[1] = 0x20;

    send_radom_data.param_value[0] = adjValue;

    uint16_t pd400_crc = CCRC::Crc16((uint8_t *)&send_radom_data, sizeof(send_radom_data)-2);
    send_radom_data.crc_L = (pd400_crc) & 0xFF;
    send_radom_data.crc_H = (pd400_crc >> 8) & 0xFF;
}
void send_receive_massage_control::rgb_color_select(int color_index){

    send_radom_data = {};
    send_radom_data.report = 0x4b;
    send_radom_data.frame_header = 0xc4;
    send_radom_data.frame_len_L = 0x0b;
    send_radom_data.command = 0x03;
    send_radom_data.param_addr[0] = PARAM_RGB_SHOW_MODE & 0xFF;
    send_radom_data.param_addr[1] = 0x20;

    send_radom_data.param_value[0] = color_index;

    uint16_t pd400_crc = CCRC::Crc16((uint8_t *)&send_radom_data, sizeof(send_radom_data)-2);
    send_radom_data.crc_L = (pd400_crc) & 0xFF;
    send_radom_data.crc_H = (pd400_crc >> 8) & 0xFF;
    pd400_m_sendMsgs.push_back(send_radom_data);
}
void send_receive_massage_control::change_basic_high_Model(int modelStatus){

    send_radom_data = {};
    send_radom_data.report = 0x4b;
    send_radom_data.frame_header = 0xc4;
    send_radom_data.frame_len_L = 0x0b;
    send_radom_data.command = 0x03;
    send_radom_data.param_addr[0] = PARAM_USER_MODE & 0xff;
    send_radom_data.param_addr[1] = 0x20;
    if(modelStatus==0){
       send_radom_data.param_value[0] = 0;
    }else{
       send_radom_data.param_value[0] = 1;
    }
    uint16_t pd400_crc = CCRC::Crc16((uint8_t *)&send_radom_data, sizeof(send_radom_data)-2);
    send_radom_data.crc_L = (pd400_crc) & 0xFF;
    send_radom_data.crc_H = (pd400_crc >> 8) & 0xFF;
    if(!is_usb_connect)return;
    pd400_m_sendMsgs.push_back(send_radom_data);
}

QVector<uint16_t>default_near_values={0,2000,2010,100,200,1,0,2000,100,200,100,50};
void send_receive_massage_control::set_basic_near(){

    default_near_values.replace(default_near_values.count()-1,pd400_mic_value);
    for (int i =0;i<default_near_values.count();i++) {
        send_radom_data = {};
        send_radom_data.report = 0x4b;
        send_radom_data.frame_header = 0xc4;
        send_radom_data.frame_len_L = 0x0b;
        send_radom_data.command = 0x03;
        send_radom_data.param_addr[0] = (PARAM_MIC_COMP_START & 0xff)+ i;
        if(i==10){
            send_radom_data.param_addr[0] = (PARAM_MIC_GAIN & 0xff);
        }else if(i==11){
            send_radom_data.param_addr[0] = (PARAM_MIC_GAIN_END & 0xff);
        }
        send_radom_data.param_addr[1] = 0x20;

        send_radom_data.param_value[0] = default_near_values.at(i)&0xff;
        send_radom_data.param_value[1] = (default_near_values.at(i) >>8)&0xff;

        uint16_t pd400_crc = CCRC::Crc16((uint8_t *)&send_radom_data, sizeof(send_radom_data)-2);
        send_radom_data.crc_L = (pd400_crc) & 0xFF;
        send_radom_data.crc_H = (pd400_crc >> 8) & 0xFF;
        if(!is_usb_connect)return;
        pd400_m_sendMsgs.push_back(send_radom_data);
    }
}
void send_receive_massage_control::set_basic_mid(){

}
QVector<uint16_t>default_far_values={0,1800,2120,10,2000,2,0,2000,100,200,100,50};
void send_receive_massage_control::set_basic_far(){

    default_far_values.replace(default_far_values.count()-1,pd400_mic_value);
    for (int i =0;i<default_far_values.count();i++) {
        send_radom_data = {};
        send_radom_data.report = 0x4b;
        send_radom_data.frame_header = 0xc4;
        send_radom_data.frame_len_L = 0x0b;
        send_radom_data.command = 0x03;
        send_radom_data.param_addr[0] = (PARAM_MIC_COMP_START & 0xff)+ i;
        if(i==10){
            send_radom_data.param_addr[0] = (PARAM_MIC_GAIN & 0xff);
        }else if(i==11){
            send_radom_data.param_addr[0] = (PARAM_MIC_GAIN_END & 0xff);
        }
        send_radom_data.param_addr[1] = 0x20;

        send_radom_data.param_value[0] = default_far_values.at(i)&0xff;
        send_radom_data.param_value[1] = (default_far_values.at(i) >>8)&0xff;

        uint16_t pd400_crc = CCRC::Crc16((uint8_t *)&send_radom_data, sizeof(send_radom_data)-2);
        send_radom_data.crc_L = (pd400_crc) & 0xFF;
        send_radom_data.crc_H = (pd400_crc >> 8) & 0xFF;
        if(!is_usb_connect)return;
        pd400_m_sendMsgs.push_back(send_radom_data);
    }
}
QVector<uint16_t>default_high_values={0,2000,2010,100,200,1,0,2000,100,200,50};
void send_receive_massage_control::set_high(int gain_value){
    default_high_values.replace(default_high_values.count()-1,gain_value);

    for (int i =0;i<11;i++) {
        send_radom_data = {};
        send_radom_data.report = 0x4b;
        send_radom_data.frame_header = 0xc4;
        send_radom_data.frame_len_L = 0x0b;
        send_radom_data.command = 0x03;
        send_radom_data.param_addr[0] = (PARAM_MIC_COMP_START & 0xff)+ i;
        if(i==10)send_radom_data.param_addr[0] = (PARAM_MIC_GAIN & 0xff);
        send_radom_data.param_addr[1] = 0x20;

        send_radom_data.param_value[0] = default_high_values.at(i)&0xff;
        send_radom_data.param_value[1] = (default_high_values.at(i) >>8)&0xff;

        uint16_t pd400_crc = CCRC::Crc16((uint8_t *)&send_radom_data, sizeof(send_radom_data)-2);
        send_radom_data.crc_L = (pd400_crc) & 0xFF;
        send_radom_data.crc_H = (pd400_crc >> 8) & 0xFF;
        if(!is_usb_connect)return;
        pd400_m_sendMsgs.push_back(send_radom_data);
    }

}
void send_receive_massage_control::save_gain_adv(){

    receive_radom_bgdata = {};
    receive_radom_bgdata.report = 0x4b;
    receive_radom_bgdata.frame_header = 0xc4;
    receive_radom_bgdata.frame_len_L = 0x09;
    receive_radom_bgdata.command = 0x04;

    receive_radom_bgdata.param_addr[0] = 0x24;
    receive_radom_bgdata.param_addr[1] = 0x20;

    uint16_t pd400_crc = CCRC::Crc16((uint8_t *)&receive_radom_bgdata, sizeof(receive_radom_bgdata)-2);
    receive_radom_bgdata.crc_L = (pd400_crc) & 0xFF;
    receive_radom_bgdata.crc_H = (pd400_crc >> 8) & 0xFF;

    m_receive_pcParMsgs.push_back(receive_radom_bgdata);
}
//DM30设备的基础
QVector<uint16_t>default_DM30high_values={0,2000,0,2000};
QVector<uint8_t>default_DM30high_addrs={0x26,0x27,0x2c,0x2d};
void send_receive_massage_control::DM30_set_basic(){
    for (int i =0;i<4;i++) {
        send_radom_data = {};
        send_radom_data.report = 0x4b;
        send_radom_data.frame_header = 0xc4;
        send_radom_data.frame_len_L = 0x0b;
        send_radom_data.command = 0x03;

         send_radom_data.param_addr[0] = default_DM30high_addrs.at(i);
        send_radom_data.param_addr[1] = 0x20;
        send_radom_data.param_value[0] = default_DM30high_values.at(i)&0xff;
        send_radom_data.param_value[1] = (default_DM30high_values.at(i) >>8)&0xff;

        uint16_t pd400_crc = CCRC::Crc16((uint8_t *)&send_radom_data, sizeof(send_radom_data)-2);
        send_radom_data.crc_L = (pd400_crc) & 0xFF;
        send_radom_data.crc_H = (pd400_crc >> 8) & 0xFF;
        if(!is_usb_connect)return;
        pd400_m_sendMsgs.push_back(send_radom_data);
    }
}
//DM30设备的高级
void send_receive_massage_control::DM30_set_high(){

}
void send_receive_massage_control::is_dfuStatus(){

    receive_radom_bgdata = {};
    receive_radom_bgdata.report = 0x4b;
    receive_radom_bgdata.frame_header = 0xc4;
    receive_radom_bgdata.frame_len_L = 0x09;
    receive_radom_bgdata.command = 0x04;

    receive_radom_bgdata.param_addr[0] = 0x08;
    receive_radom_bgdata.param_addr[1] = 0x10;

    uint16_t pd400_crc = CCRC::Crc16((uint8_t *)&receive_radom_bgdata, sizeof(receive_radom_bgdata)-2);
    receive_radom_bgdata.crc_L = (pd400_crc) & 0xFF;
    receive_radom_bgdata.crc_H = (pd400_crc >> 8) & 0xFF;
    if(!is_usb_connect)return;
    m_receive_pcParMsgs.push_back(receive_radom_bgdata);
}
//同步软件数据的标志
void send_receive_massage_control::sys_Software_tag(int tag){

    send_radom_data = {};
    send_radom_data.report = 0x4b;
    send_radom_data.frame_header = 0xc4;
    send_radom_data.frame_len_L = 0x0b;
    send_radom_data.command = 0x03;
    send_radom_data.param_addr[0] = 0x0b;
    send_radom_data.param_addr[1] = 0x10;

    send_radom_data.param_value[0] = tag;

    uint16_t pd400_crc = CCRC::Crc16((uint8_t *)&send_radom_data, sizeof(send_radom_data)-2);
    send_radom_data.crc_L = (pd400_crc) & 0xFF;
    send_radom_data.crc_H = (pd400_crc >> 8) & 0xFF;
    if(!is_usb_connect)return;
    pd400_m_sendMsgs.push_back(send_radom_data);
}
//心跳包
void send_receive_massage_control::hearbeat_fun(){

    receive_radom_bgdata = {};
    receive_radom_bgdata.report = 0x4b;
    receive_radom_bgdata.frame_header = 0xc4;
    receive_radom_bgdata.frame_len_L = 0x09;
    receive_radom_bgdata.command = 0x04;

    receive_radom_bgdata.param_addr[0] = 0x00;
    receive_radom_bgdata.param_addr[1] = 0x10;

    uint16_t pd400_crc = CCRC::Crc16((uint8_t *)&receive_radom_bgdata, sizeof(receive_radom_bgdata)-2);
    receive_radom_bgdata.crc_L = (pd400_crc) & 0xFF;
    receive_radom_bgdata.crc_H = (pd400_crc >> 8) & 0xFF;
    if(!is_usb_connect)return;
    m_receive_pcParMsgs.push_back(receive_radom_bgdata);
}
void send_receive_massage_control::resetFactorySetting(){

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
    if(!is_usb_connect)return;
    pd400_m_sendMsgs.push_back(send_radom_data);
}
//获取背景参数
void send_receive_massage_control::bg_datasource(){


    //增加设备信息
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
        pd400_m_receive_bgMsgs.push_back(continuity_read_data);
    }

    continuity_read_data = {};
    continuity_read_data.report = 0x4b;
    continuity_read_data.frame_header = 0xc4;
    continuity_read_data.frame_len_L = 0x0b;
    continuity_read_data.command = 0x06;
    continuity_read_data.param_addr[0] = 0x22;
    continuity_read_data.param_addr[1] = 0x20;
    continuity_read_data.param_number[0] = 23;

    uint16_t pd400_crc = CCRC::Crc16((uint8_t *)&continuity_read_data, sizeof(continuity_read_data)-2-53);//有效数据得长度
    continuity_read_data.crc_L = (pd400_crc) & 0xFF;
    continuity_read_data.crc_H = (pd400_crc >> 8) & 0xFF;
    pd400_m_receive_bgMsgs.push_back(continuity_read_data);
    //增加版本号
//    for(int k=0;k<3;k++) {
//        receive_radom_bgdata = {};
//        receive_radom_bgdata.report = 0x4b;
//        receive_radom_bgdata.frame_header = 0xc4;
//        receive_radom_bgdata.frame_len_L = 0x09;
//        receive_radom_bgdata.command = 0x04;
//        receive_radom_bgdata.param_addr[0] = 0x0b+(k&0xff);


//        uint16_t pd400_crc = CCRC::Crc16((uint8_t *)&receive_radom_bgdata, sizeof(receive_radom_bgdata)-2);
//        receive_radom_bgdata.crc_L = (pd400_crc) & 0xFF;
//        receive_radom_bgdata.crc_H = (pd400_crc >> 8) & 0xFF;
//        pd400_m_receive_bgMsgs.push_back(receive_radom_bgdata);
//    }
}
void send_receive_massage_control::getModelCode(){


//    m_getModelCodelDatas

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

    m_getModelCodelDatas.push_back(continuity_read_data);

//    for (int i=0;i<3;i++) {
//        receive_radom_bgdata = {};
//        receive_radom_bgdata.report = 0x4b;
//        receive_radom_bgdata.frame_header = 0xc4;
//        receive_radom_bgdata.frame_len_L = 0x09;
//        receive_radom_bgdata.command = 0x04;

//        receive_radom_bgdata.param_addr[0] = 0x00 +i;
//        receive_radom_bgdata.param_addr[1] = 0x00;

//        uint16_t pd400_crc = CCRC::Crc16((uint8_t *)&receive_radom_bgdata, sizeof(receive_radom_bgdata)-2);
//        receive_radom_bgdata.crc_L = (pd400_crc) & 0xFF;
//        receive_radom_bgdata.crc_H = (pd400_crc >> 8) & 0xFF;

//        m_receive_pcParMsgs.push_back(receive_radom_bgdata);
//    }
}

//添加DM90的收发数据的方法======================================
void send_receive_massage_control::sendMessageForDevice(int address,int number,int value){

    continuity_write_data = {};
    continuity_write_data.report = 0x4b;
    continuity_write_data.frame_header = 0xc4;
    continuity_write_data.frame_len_L = 0x0d;
    continuity_write_data.command = 0x05;
    continuity_write_data.param_addr[0] = address&0xff;
    continuity_write_data.param_addr[1] = 0x20;
    continuity_write_data.param_num[0] = number&0xff;
    continuity_write_data.param_value[0] = value&0xff;
    continuity_write_data.param_value[1] = (value >>8)&0xff;

    uint16_t pd400_crc = CCRC::Crc16((uint8_t *)&continuity_write_data, sizeof(continuity_write_data)-2-51);//有效数据得长度
    continuity_write_data.crc_L = (pd400_crc) & 0xFF;
    continuity_write_data.crc_H = (pd400_crc >> 8) & 0xFF;
    //添加到数组中
    m_dm90continuityWriteDatas.push_back(continuity_write_data);
}

//dm90连续发送多个参数
void send_receive_massage_control::sendCotiMessageForDevice(int frame_len,int address,int number,const QVector<int>values){

    continuity_write_data = {};
    continuity_write_data.report = 0x4b;
    continuity_write_data.frame_header = 0xc4;
    continuity_write_data.frame_len_L = frame_len;
    continuity_write_data.command = 0x05;
    continuity_write_data.param_addr[0] = address&0xff;
    continuity_write_data.param_addr[1] = (address>>8)&0xff;
    continuity_write_data.param_num[0] = number&0xff;
    //数据赋值部分============
    continuity_write_data.param_value[0] = values.at(0)&0xff;
    continuity_write_data.param_value[1] = (values.at(0) >>8)&0xff;
    continuity_write_data.crc_L= values.at(1)&0xff;
    continuity_write_data.crc_H= (values.at(1) >>8)&0xff;

    //继续赋值
    for (int i =0;i<(values.count()-2)*2;i++) {
        if(i%2==0){
            int defaultValue = values.at(i/2+2);
           continuity_write_data.extend_number[i]= defaultValue&0xff;
           continuity_write_data.extend_number[i+1]= (defaultValue>>8)&0xff;
        }
    }

    //检验位有两种情况，一种是数据刚好用完，一种是数据没用完
    if(values.count()<24+2){
        uint16_t pd400_crc = CCRC::Crc16((uint8_t *)&continuity_write_data, sizeof(continuity_write_data)-3-2*(26-values.count()));//有效数据得长度
        continuity_write_data.extend_number[(values.count()-2)*2] = (pd400_crc) & 0xFF;
        continuity_write_data.extend_number[(values.count()-2)*2+1] = (pd400_crc >> 8) & 0xFF;
    }else{
        uint16_t pd400_crc = CCRC::Crc16((uint8_t *)&continuity_write_data, sizeof(continuity_write_data)-3);//有效数据得长度
        continuity_write_data.extend_number[48] = (pd400_crc) & 0xFF;
        continuity_write_data.extend_number[49] = (pd400_crc >> 8) & 0xFF;
    }

    m_dm90continuityWriteDatas.push_back(continuity_write_data);
}

//dm90随机发送多个参数
void send_receive_massage_control::sendRandomMessageForDevice(int frame_len,const QVector<int>addrs,const QVector<int>values){

    continuity_write_multiple_data = {};
    continuity_write_multiple_data.report = 0x4b;
    continuity_write_multiple_data.frame_header = 0xc4;
    continuity_write_multiple_data.frame_len_L = frame_len;
    continuity_write_multiple_data.command = 0x03;
    for (int i=0;i<addrs.count();i++) {
       continuity_write_multiple_data.addrAndValue[i].addressL = addrs.at(i)&0xff;
       continuity_write_multiple_data.addrAndValue[i].addressH = (addrs.at(i)>>8)&0xff;
       //赋值
       continuity_write_multiple_data.addrAndValue[i].valueL = values.at(i)&0xff;
       continuity_write_multiple_data.addrAndValue[i].valueH = (values.at(i)>>8)&0xff;
    }
    if(addrs.count()<14){//检验位
        uint16_t pd400_crc = CCRC::Crc16((uint8_t *)&continuity_write_multiple_data, sizeof(continuity_write_multiple_data)-3-4*(14-addrs.count()));//有效数据得长度
        continuity_write_multiple_data.addrAndValue[addrs.count()].addressL = pd400_crc & 0xFF;
        continuity_write_multiple_data.addrAndValue[addrs.count()].addressH = (pd400_crc >> 8) & 0xFF;
    }else{
        uint16_t pd400_crc = CCRC::Crc16((uint8_t *)&continuity_write_multiple_data, sizeof(continuity_write_multiple_data)-2);//有效数据得长度
        continuity_write_multiple_data.crc_L = (pd400_crc) & 0xFF;
        continuity_write_multiple_data.crc_H = (pd400_crc >> 8) & 0xFF;
    }

    m_dm90random_Write_multipleDatas.push_back(continuity_write_multiple_data);

}
