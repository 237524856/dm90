#include "FirmwareUpgradeProtocol.h"
#include "HID/utils/CRC.h"
#include <QDebug>


//是否执行bin文件转数组的代码
#define CONVERT_BIN_TO_C_ARRAY 0

//最新固件的版本号
uint32_t LATEST_FIREWARE_VERSION = 114; //v1.1.4


//为了防止忘了修改，非调试版本发布时，强制禁止转换
#ifdef NDEBUG
#undef CONVERT_BIN_TO_C_ARRAY
#define CONVERT_BIN_TO_C_ARRAY 0
#endif

#if 0==CONVERT_BIN_TO_C_ARRAY
    #include "firemware_data.hpp"
#else
    //bin文件转数组的代码之前，还没有生成firemware_data.hpp文件，
    //为了可以编译通过，暂时定义一个假的数组
    const uint8_t firmware_data[] = { 0x0, };

    //将bin文件转为c++数组
    void convert_bin_to_c_array()
    {
        FILE* fp = fopen("..//docs//SK-PN32_V1.1.4.bin", "rb");
        if (fp)
        {
            fseek(fp, 0, SEEK_END);
            uint32_t size = ftell(fp);
            fseek(fp, 0, SEEK_SET);

            uint8_t* buff = new uint8_t[size + 1];
            char* str = new char[size * 8];
            if (!buff || !str)
            {
                fclose(fp);
                return;
            }

            size_t offset = 0;
            size_t read_size = 0;

            do {
                read_size = fread(&buff[offset], 1, 4096, fp);
                if (read_size > 0)
                {
                    offset += read_size;
                }
            } while (read_size > 0);

            fclose(fp);

            if (size != offset)
            {
                delete[] buff;
                delete[] str;
                return;
            }

            char str_tmp[64];
            strcpy(str, "const uint8_t firmware_data[] = {\n");

            for (int i = 0; i < offset; i++)
            {
                sprintf(str_tmp, "0x%02X,", buff[i]);
                strcat(str, str_tmp);

                if (i % 32 == 31)
                    strcat(str, "\n");
            }
            strcat(str, "\n};\n");

            fp = fopen("..\\src\\protocol\\firemware_data.hpp", "wt");
            if (fp)
            {
                fwrite(str, 1, strlen(str), fp);
                fclose(fp);
            }

            delete[] buff;
            delete[] str;

            assert(0); //bin文件已转换为数组，请将 CONVERT_BIN_TO_C_ARRAY 宏定义为0
        }
    }
#endif



//支持的设备
static DeviceInfo support_device[] =
{
    //VID  PID
    {"Maono AU-100", "", 0x2b53, 0x006A, NULL,NULL},
    {"PM481",         "", 0x352F, 0x0101,  NULL,NULL},
    {"MBS400","", 0x31B2, 0x0011,  NULL,NULL},
    {"PD400","", 0x352F, 0x0100,  NULL,NULL},
    {"DM90","", 0x352F, 0x0102,  NULL,NULL},
    {"DM70","", 0x352F, 0x0103,  NULL,NULL},
    {"PD200X","", 0x352F, 0x0104,  NULL,NULL},
    {"DM30RGB","", 0x352F, 0x0106,  NULL,NULL},
};





CFirmwareUpgradeProtocol::CFirmwareUpgradeProtocol(void)
    : CProtocol(10+1) //比文档中的协议的数据包大小要多加一个字节，因为首字节是报文ID
    , m_pDevMgmt(NULL)
{
#if CONVERT_BIN_TO_C_ARRAY
    convert_bin_to_c_array();
#endif

}

CFirmwareUpgradeProtocol::~CFirmwareUpgradeProtocol(void)
{
}


bool CFirmwareUpgradeProtocol::IsSupportDevic(const char* path, uint32_t param1, uint32_t param2)
{
    int n = sizeof(support_device) / sizeof(support_device[0]);
    for (int i = 0; i < n; i++)
    {
        const DeviceInfo& dev = support_device[i];
        if (param1 == dev.param1 && param2 == dev.param2)
            return true;
    }

    return false;
}

const DeviceInfo* CFirmwareUpgradeProtocol::GetSupportDevices()
{
    int n = sizeof(support_device) / sizeof(support_device[0]);
    if (n == 0)
        return NULL;

    DeviceInfo* head = &support_device[0];

    for (int i = 1; i < n; i++)
    {
        head->next = &support_device[i];
        head = head->next;
    }

    head->next = 0;
//    qDebug()<<"kkkhhhhxxxxx"<<*&support_device[0].param1<<*&support_device[0].param2;
//    return NULL;
    return &support_device[0];
}


bool CFirmwareUpgradeProtocol::CheckReply(PACKDATA& package, uint8_t command)
{
    // 验证命令编码
//    if (package.command != command)
//    {
//        //log_error("CheckReply error: reply cmd 0x%02x != 0x%02x", package.command, command);
//        return false;
//    }

    // 验证应答标志
//    if (package.reply_flag != REPLY_OK)
//    {
//        //log_error("CheckReply error: reply flag 0x%02x != 0x%02x  cmd=0x%02x", package.reply_flag, REPLY_OK, package.command);
//        return false;
//    }

    // 验证应答包的CRC，m_nPackLen-1-1，除去首字节报文ID和最后一个字节存放CRC值
//    uint8_t crc = CCRC::Crc8(package.buff, m_nPackLen - 1 -1);
//    if (crc != package.buff[m_nPackLen - 1 - 1])
//    {
//        //log_error("CheckReply error: reply crc 0x%02x != 0x%02x  cmd=0x%02x", package.buff[m_nPackLen-1-1], crc, package.command);
//        return false;
//    }	
    return true;
}

bool CFirmwareUpgradeProtocol::OpenApp(bool bOpen)
{
    if (!m_pDevMgmt)
    {
        return false;
    }

    PACKDATA recv = {  };
    PACKDATA send = {  };

    send.command = 0x4b;
    send.index[0] = 0x7e;
    send.index[1] = 0x70;
    send.index[4] = 0x77;
    send.index[6] = 0x60;
//    send.data[0] = bOpen ? 1 : 0; //1—连接设备    0—断开连接
//    send.data[1] = 2; //0：APP和控制面板   1：机架软件    2：固件升级面板
    bool ret = m_pDevMgmt->SendAndReceive((unsigned char*)&send, (unsigned char*)&recv, 3, 500);
    return ret;
}


int CFirmwareUpgradeProtocol::GetDeviceType()
{
    if (!m_pDevMgmt || !m_pDevMgmt->IsConnectDevice())
        return -1;

    const ICommunicateDevice* pDev = m_pDevMgmt->GetDevice();

    // const char* szPath = m_pDevMgmt->GetDevice()->GetDevicePath();
    // if (!szPath)
    //     return -1;

    // uint16_t vid = 0, pid = 0;
    // if (!GetVidPidFromStr(szPath, vid, pid))
    //     return -1;

    uint32_t vid = pDev->GetDeviceParam1();
    uint32_t pid = pDev->GetDeviceParam2();

    if (vid == 0x2b53)
    {
        //固件升级和正常模式是同一个vid pid的时候
        if (pid == 0x0002)
        {
            return 1;
        }
        else if (pid == 0x006A)
        {
            return 0;
        }
    }

    return -1;
}



bool CFirmwareUpgradeProtocol::GetDeviceInfo(PACKDATA &recv)
{
    if (!m_pDevMgmt)
    {
        return false;
    }
    
    PACKDATA send = { };

    send.command = 0x32;

    if (m_pDevMgmt->SendAndReceive((unsigned char*)&send, (unsigned char*)&recv, 3, 200))
    {
        return true;
    }

    return false;
}


bool CFirmwareUpgradeProtocol::SwitchToUpgradeMode()
{
    if (m_pDevMgmt)
    {
        PACKDATA send = {  };
        PACKDATA recv = {  };

        send.command = 0x3a;
        send.buff[4] = 1; //1:更新      0:不更新
        strcpy((char*)(&send.buff[5]), "12ab901386"); 

        bool ret = m_pDevMgmt->SendAndReceive((unsigned char*)&send, (unsigned char*)&recv, 3, 500);
        if (ret)
        {
            //1：校验成功，继续升级（旧版本不会校验，默认返回与下发一致，即1）    0：校验不成功，停止升级
            if (recv.buff[4] != 1)
            {
                return false;
            }

            return true;
        }
    }

    return false;
}

bool CFirmwareUpgradeProtocol::SwitchToNormalMode()
{
    //if (m_pDevMgmt)
    //{
    //    PACKDATA send = { 0 };
    //    send.command = 0x30;
    //    return m_pDevMgmt->SendNotReceive(send);
    //}

    return false;
}

bool CFirmwareUpgradeProtocol::WriteFlash(uint32_t& nProgress)
{
//    nProgress = 0;

//    if (!m_pDevMgmt )
//    {
//        return false;
//    }

//    const uint8_t* pData = firmware_data;
//    uint32_t dataBytes = sizeof(firmware_data);

//    bool bRet = false;
//    PACKDATA recv = {  };
//    PACKDATA data = {  };

//    data.command = 0x26;
//    (*(uint32_t*)& data.data[0]) = LATEST_FIREWARE_VERSION;
//    (*(uint32_t*)& data.data[4]) = 0x40000;
//    (*(uint32_t*)& data.data[8]) = dataBytes;
//    (*(uint32_t*)& data.data[12]) = CCRC::Crc32(pData, dataBytes); //crc32

//    bRet = m_pDevMgmt->SendAndReceive(data, recv, 3, 3 * 60 * 1000);
//    if (!bRet)
//    {
//        return false;
//    }

//    //总包数
//    uint32_t n = (dataBytes + 59 - 1) / 59;
//    uint32_t nPos = 0;
//    uint32_t i = 0;
//    uint32_t nRemain = 0;

//    while (nPos < dataBytes)
//    {
//        memset(&data, 0, sizeof(data));

//        //命令编码
//        data.command = 0x28;

//        //包序号
//        data.index[0] = (uint8_t)(((uint16_t)i) >> 8);
//        data.index[1] = (uint8_t)(i);

//        //数据
//        nRemain = dataBytes - nPos;
//        if (nRemain >= 59)
//        {
//            memcpy(data.data, &pData[nPos], 59);
//            nPos += 59;
//        }
//        else
//        {
//            memcpy(data.data, &pData[nPos], nRemain);
//            nPos += nRemain;
//        }

//        bRet = m_pDevMgmt->SendAndReceive(data, recv, 3, 16000);
//        if (!bRet)
//        {
//            return false;
//        }

//        i++;
//        nProgress = i * 100 / n;
//    }

    return true;
}

bool CFirmwareUpgradeProtocol::ReadFlash(uint32_t offset, uint8_t* pData, uint32_t dataBytes)
{
    if (!m_pDevMgmt || !pData)
    {
        return false;
    }

    bool bRet = false;
    PACKDATA recv = {  };
    PACKDATA data = {  };

    //总包数
    int n = (dataBytes + 59 - 1) / 59;
    uint32_t nPos = 0;
    int i = 0;

//    while (nPos < dataBytes)
//    {
//        memset(&data, 0, sizeof(data));

        //命令编码
//        data.command = 0x34;

//        //包序号
//        data.index[0] = (uint8_t)(((uint16_t)i) >> 8);
//        data.index[1] = (uint8_t)(i);

//        data.data[0] = (uint8_t)(dataBytes - nPos > 59 ? 59 : dataBytes - nPos);
//        (*(uint32_t*)& data.data[1]) = offset + nPos;

//        bRet =  my_view->test();->SendAndReceive(data, recv, 3, 16000);
//        if (bRet)
//        {
//            //数据
//            if (nPos < dataBytes - 59)
//            {
//                memcpy(&pData[nPos], recv.data, 59);
//                nPos += 59;
//            }
//            else
//            {
//                memcpy(&pData[nPos], recv.data, dataBytes - nPos);
//                nPos += (dataBytes - nPos);
//            }
//        }
//        else
//        {
//            return false;
//        }

//        i++;
//    }
    return true;
}

bool CFirmwareUpgradeProtocol::send_msg_device(unsigned char *send_msg,unsigned char *receive_msg){


//    PACKDATA recv = {  };
  //  uint8_t crc = CCRC::Crc8(send_msg.buff, m_nPackLen - 1 -1);

    bool ret = m_pDevMgmt->SendAndReceive(send_msg, receive_msg, 3, 500);


    return ret;
}
int CFirmwareUpgradeProtocol::pd400_SendAndReceive(unsigned char *send, mypc_random_write &recv,uint32_t senddata_length){

    int ret = m_pDevMgmt->pd400_SendAndReceive(send, recv, 3, senddata_length);
    return ret;
}

bool CFirmwareUpgradeProtocol::pd400_dfu_SendMess(unsigned char *send, mypc_random_write &recv, int n_number, uint32_t dwWaitTime){
    bool ret = m_pDevMgmt->pd400_dfu_SendMess(send,recv,n_number,dwWaitTime);
    return ret;
}

bool CFirmwareUpgradeProtocol::DM90_SendMessage_device(unsigned char *send, pc_receive_continuitywrite_data &recv, int n_number, uint32_t dwWaitTime){
    bool ret = m_pDevMgmt->DM90_SendMessage(send,recv,n_number,dwWaitTime);
    return ret;
}

bool CFirmwareUpgradeProtocol::Kpd400_Receive(mypc_random_write &recv, int nTimes, uint32_t dwWaitTime){

    bool ret = m_pDevMgmt->pd400_Receive(recv, 3, 64);

    return ret;
}
bool CFirmwareUpgradeProtocol::DM90_Receive(pc_receive_continuitywrite_data &recv, int nTimes, uint32_t dwWaitTime){

    bool ret = m_pDevMgmt->DM90_ReceiveMessage(recv, 3, dwWaitTime);

    return ret;
}

