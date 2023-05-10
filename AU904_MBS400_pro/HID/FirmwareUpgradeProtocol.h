/********************************************************************
    created:	2018/04/14
    created:	14:4:2018   14:42
    filename: 	FirmwareUpgradeProtocol.h
    author:		hzf
    purpose:	设备通讯协议的实现
*********************************************************************/

#include "HID/utils/Protocol.h"
#include <QVector>

class CFirmwareUpgradeProtocol : public CProtocol
{
public:
    CFirmwareUpgradeProtocol(void);
    ~CFirmwareUpgradeProtocol(void);

    /**
     @brief 查询设备信息
     @param[out] recv:设备信息，需要根据协议解析
     @return  bool_t
     @sa
     @retval 成功返回TRUE，失败返回FALSE
     @note
     @author hzf
    */
    bool GetDeviceInfo(PACKDATA& recv);

    /**
     @brief 切换到正常模式
     @return  bool_t
     @sa
     @retval
     @note 如果当前是固件升级模式，则切换到正常模式
     @author hzf
    */
    bool SwitchToNormalMode();


    /**
     @brief 切换到固件更新模式
     @return  bool_t
     @sa
     @retval
     @note 如果当前是正常模式，则切换到固件更新模式
     @author hzf
    */
    bool SwitchToUpgradeMode();

    /**
     @brief 写设备的flash
     @param[in] ver:要写入固件的版本
     @param[in] offset:写入的偏移地址
     @param[in] pData:固件数据
     @param[in] dwDataBytes:固件数据的字节数
     @param[out] nProgress:写入进度,范围是0至100
     @return  bool_t
     @sa
     @retval 成功返回TRUE，失败返回FALSE
     @note
     @author hzf
    */
    bool WriteFlash(uint32_t& nProgress);
    /**
     @brief 读取设备的flash
     @param[in] offset:读取数据的偏移地址
     @param[in] pData:存读取数据的buff
     @param[in] dataBytes:存读取数据的buff的字节数
     @return  bool_t
     @sa
     @retval 成功返回TRUE，失败返回FALSE
     @note
     @author hzf
    */
    bool ReadFlash(uint32_t offset, uint8_t* pData, uint32_t dataBytes);


    /**
     @brief 打开或关闭APP
     @param[in] bOpen:TRUE--打开APP, FALSE--关闭APP
     @return  bool_t
     @sa
     @retval 成功返回TRUE，失败返回FALSE
     @note 连接设备后，必须首先OpenApp(TRUE), 之后才可以通讯
     @author hzf
    */
    bool OpenApp(bool bOpen);

    /**
      @brief 获取当前的设备的运行模式
      @return  int
      @sa
      @retval 0--正常模式， 1--固件升级模式,  2--混合模式（共用一个vid pid）   -1--未连接设备
      @note
      @author hzf
    */
    int GetDeviceType();
    //发送信息给设备
    bool send_msg_device(unsigned char * send_msg,unsigned char *receive_msg);

    int pd400_SendAndReceive(unsigned char *send, mypc_random_write &recv,uint32_t senddata_length);

    bool pd400_dfu_SendMess(unsigned char *send, mypc_random_write &recv, int n_number, uint32_t dwWaitTime);

    bool Kpd400_Receive(mypc_random_write &recv, int nTimes, uint32_t dwWaitTime);

    bool DM90_SendMessage_device(unsigned char *send, pc_receive_continuitywrite_data &recv, int n_number, uint32_t dwWaitTime);

    bool DM90_Receive(pc_receive_continuitywrite_data &recv, int nTimes, uint32_t dwWaitTime);

public:
    //ICommProtocol协议的实现

    /**
     @brief 查询协议是否支持指定的设备
     @param[in] path 设备路径
     @param[in] param1  参数1，如设备vid
     @param[in] param2  参数2，如设备pid
     @return  bool_t 支持返回TRUE，不支持返回FALSE
     @note
     @author hzf
    */
    virtual bool IsSupportDevic(const char* path, uint32_t param1, uint32_t param2);

    /**
     @brief 获取支持的设备
     @return  const DeviceInfo* 返回的是设备的链表，无设备返回NULL
     @note
     @author hzf
    */
    virtual const DeviceInfo* GetSupportDevices();

    /**
     @brief 验证应答数据包
     @param[in] package -- 要验证的应答包
     @param[in] command -- 期待应答的命令编码，例如：下发0x02的命令，期待应答的命令编码是0x03
     @return  bool_t 验证成功返回TRUE, 失败返回FALSE
     @note
     @author hzf
     */
    virtual bool CheckReply(PACKDATA& package, uint8_t command);


protected:
    ICommDeviceMgmt* m_pDevMgmt;
};


