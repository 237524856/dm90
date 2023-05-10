#include "pm481sendreceiveclass.h"
#include "../common.h"
#include "../HID/utils/CRC.h"

pm481SendReceiveClass::pm481SendReceiveClass(QObject *parent) : QObject(parent)
{

}
void pm481SendReceiveClass::bg_datasource(){

    for (int i=0;i<21+3;i++) {

        receive_radom_bgdata = {};
        receive_radom_bgdata.report = 0x4b;
        receive_radom_bgdata.frame_header = 0xc4;
        receive_radom_bgdata.frame_len_L = 0x09;
        receive_radom_bgdata.command = 0x04;
        if(i<21){
            receive_radom_bgdata.param_addr[0] = 0x22+i;
            receive_radom_bgdata.param_addr[1] = 0x20;
        }else{
            //版本号
            receive_radom_bgdata.param_addr[0] = 0x0b+(i-16);
            receive_radom_bgdata.param_addr[1] = 0x00;
        }
        uint16_t pd400_crc = CCRC::Crc16((uint8_t *)&receive_radom_bgdata, sizeof(receive_radom_bgdata)-2);
        receive_radom_bgdata.crc_L = (pd400_crc) & 0xFF;
        receive_radom_bgdata.crc_H = (pd400_crc >> 8) & 0xFF;
        if(!is_pm481_usb_connect)return;
        //pd400_m_receive_bgMsgs.push_back(receive_radom_bgdata);
    }
}
