
#if !defined(ICOMMUNICATEDEVICE_H__A171578B_017B_4E6A_B3CE_F01624B52945_)
#define ICOMMUNICATEDEVICE_H__A171578B_017B_4E6A_B3CE_F01624B52945_

#include <iostream>
#include <QVector>
/**
 * 数据包结构
 */
typedef struct PackData
{
   // uint8_t report; //报文ID

    union
    {
        //USB HID 一个包 （全速最多64byte, 高速可达1024byte）
        uint8_t buff[11];

        //协议的数据包结构
        struct
        {
            uint8_t command;		// 命令编码

            uint8_t index[10];	// 包序号

//            uint8_t reply_flag;		// 应答标志

//            uint8_t data[60];		// 数据域

            //uint8_t crc;			// CRC校验 用buff[max_send_bytes-1]作为crc校验字节
        };

    };
}PACKDATA;


//随机往设备写入数据
typedef struct myrandom_write
{

   //uint8_t report; //报文ID
    union
    {
        //USB HID 一个包 （全速最多64byte, 高速可达1024byte）
        uint8_t buff[64];
        //协议的数据包结构
        struct
        {
            uint8_t report;
            uint8_t frame_header;   // 帧头
            uint8_t frame_len_L;  // 帧长度
            uint8_t frame_len_H;  // 帧长度
            uint8_t cp_addr;  // 设备地址
            uint8_t command;		// 命令编码
            //数据
            uint8_t param_addr[2];//数据的地址
            uint8_t param_value[2];	//数据的值
            uint8_t crc_L;			// CRC校验
            uint8_t crc_H;
            uint8_t pars[52];// CRC校验
        };
    };
}mypc_random_write;


typedef struct receive_continuitywrite_data//接收设备返回的值
{

   //uint8_t report; //报文ID
    union
    {
        //USB HID 一个包 （全速最多64byte, 高速可达1024byte）
        uint8_t buff[64];
        //协议的数据包结构
        struct
        {
            uint8_t report;
            uint8_t frame_header;   // 帧头
            uint8_t frame_len_L;  // 帧长度 L
            uint8_t frame_len_H;  // 帧长度 H
            uint8_t cp_addr;  // 设备地址
            uint8_t command;		// 命令编码
            //数据部分----------------
            uint8_t param_addr[2];//数据的地址
            uint8_t param_num[2];//数据的长度,或者值
            uint8_t param_value[52];	//数据的值
            uint8_t crc_L;			// CRC校验
            uint8_t crc_H;
        };
    };
}pc_receive_continuitywrite_data;


/**
 * 设备信息
 */
typedef struct DeviceInfo_
{
    char name[64];  //设备名称
    char path[512]; //设备路径
    uint32_t param1;//设备参数1，如vid
    uint32_t param2;//设备参数2，如pid
    void*    param3;//设备参数3

    DeviceInfo_* next; //下一个设备，用于表示链表
}DeviceInfo;


/** 
 @brief 设备拔插事件通知回调函数
 @param[in] path 设备路径
 @param[in] removal 1--拔出设备  0--插入设备
 @param[in] param1  参数1，如设备vid
 @param[in] param2  参数2， 如设备pid
 @param[in] param3  参数3
 @return  void
*/
typedef void (*FnDeviceChangeCallback)(const char* path, int removal, uint32_t param1, uint32_t param2, void* param3);

typedef void (*pd400_FnDeviceChangeCallback)(const char* path, int removal, uint32_t param1, uint32_t param2);

/**
 * 通讯设备接口
 */
class ICommunicateDevice
{
public:
    virtual ~ICommunicateDevice(){}

    /**
     @brief 初始化
     @return  bool_t 成功返回TRUE，否则返回FALSE
     @note
     @author hzf
    */
    virtual bool Init() = 0;
    

    /**
     @brief 枚举设备
     @return  void* 返回设备的对象指针，需要用GetEnumInfo()获取设备信息。未找到设备，返回NULL。
     @note
     @author hzf
    */
    virtual void* EnumDevice(uint32_t param1, uint32_t param2, void* param3) = 0;

    /**
     @brief 获取枚举到的设备的信息
     @param[in] dev_obj 设备对象，由函数EnumDevice的返回值获得
     @param[out] path 设备路径
     @param[out] param1  参数1，如设备vid
     @param[out] param2  参数2，如设备pid
     @note
     @author hzf
    */
    virtual bool GetEnumInfo(void* dev_obj, char* dev_path, uint32_t &param1, uint32_t &param2)=0;

    /**
     @brief 获取设备的参数1，如设备vid
     @note
     @author hzf
    */
    virtual uint32_t GetDeviceParam1() const = 0;
    
    /**
     @brief 获取设备的参数2，如设备pid
     @note
     @author hzf
    */
    virtual uint32_t GetDeviceParam2() const = 0;

    /**
     @brief 判断是否已经打开设备
     @return  bool_t 已经打开设备返回TRUE，否则返回FALSE
     @note 
     @author hzf
    */
    virtual bool IsDeviceOpen()=0;

    /**
     @brief 获取设备名称
     @return  const char* 未打开设备返回NULL
     @note 
     @author hzf
    */
    virtual const char* GetDeviceName() const=0;

    /**
     @brief 打开设备
     @param[in] dev_path:设备的路径
     @param[out] param1  参数1，如设备vid
     @param[out] param2  参数2，如设备pid
     @param[in]  param3  打开设备可能需要的参数，没有参数可以为NULL
     @return  bool_t 失败返回FALSE,发送成功返回TRUE
     @note 
     @author hzf
    */
    virtual bool OpenDevice(const char* dev_path, uint32_t param1, uint32_t param2, void* param3)=0;
        
    /**
     @brief 关闭设备
     @return  void
     @sa   
     @retval 
     @note 
     @author hzf
    */
    virtual void CloseDevice()=0;
        
    /**
     @brief 将数据发送到已打开的设备
     @param[in] buff:要写入设备的数据
     @param[in] nBuffBytes:buff中数据的字节数, 要与GetWriteMaxLength()返回的字节数相等
     @return  bool_t 失败返回FALSE,但返回TRUE不表示发送成功
     @note 此函数是异步发送，不会等待返回结果
     @author hzf
    */
    virtual bool Write(unsigned char *buff, int nBuffBytes)=0;

    /**
     @brief 从已打开的设备接收数据
     @param[out] pBuff:接收数据的缓存，不能小于GetReadMaxLength()返回的字节数
     @param[in] dwWaitTime:等待超时的时间，单位毫秒，-1 for blocking wait.
     @return  bool_t 失败返回FALSE,发送成功返回TRUE
     @note 调用此函数后，要等到接收数据成功或等待超时，函数才会返回
     @author hzf
    */
    virtual bool Read(unsigned char *pBuff, int dwWaitTime)=0;

    virtual bool myRead(mypc_random_write *pBuff, int dwWaitTime)=0;

    virtual bool DM90Read(pc_receive_continuitywrite_data *pBuff, int dwWaitTime)=0;

    /**
     @brief 获取设备报文数据的长度
     @return  int 返回可读数据的字节数
     @note 调用此函数之前，需要先打开设备
     @author hzf
    */
    virtual int GetMaxReportLength()=0;


    /**
     @brief 获取设备路径
     @return  const char*
     @note
     @author hzf
    */
    virtual const char* GetDevicePath() const=0;

    /**
     @brief 注册设备拔插通知
     @return  void
     @note
     @author hzf
    */
    virtual void RegisterDeviceChangeNotification(FnDeviceChangeCallback fn, void* user_data)=0;


protected:
    ICommunicateDevice(){}
};


/** 
 * 通讯数据加密解密接口
 */
class ICommEncrypt
{
public:
    virtual ~ICommEncrypt() {}
    
    /**
     @brief 加密数据
     @param[in out] data:要加密的数据包
     @return  void
     @note 
     @author hzf
    */
    virtual void Encrypt(PACKDATA &data)=0;
    
    /**
     @brief 解密数据
     @param[in out] data:要解密的数据包
     @return  void
     @note 
     @author hzf
    */
    virtual void Decrypt(PACKDATA &data)=0;

protected:
    ICommEncrypt() {}
};

class ICommDeviceMgmt;

//协议接口
class ICommProtocol
{
public:
    virtual ~ICommProtocol() {}

    /**
     @brief 查询协议是否支持指定的设备
     @param[in] path 设备路径
     @param[in] param1  参数1，如设备vid
     @param[in] param2  参数2，如设备pid
     @return  bool_t 支持返回TRUE，不支持返回FALSE
     @note
     @author hzf
    */
    virtual bool IsSupportDevic(const char* path, uint32_t param1, uint32_t param2)=0;

    /**
     @brief 获取支持的设备
     @return  const DeviceInfo* 返回的是设备的链表，无设备返回NULL
     @note
     @author hzf
    */
    virtual const DeviceInfo* GetSupportDevices() = 0;

    /**
     @brief 验证应答数据包
     @param[in] package -- 要验证的应答包
     @param[in] command -- 期待应答的命令编码，例如：下发0x02的命令，期待应答的命令编码是0x03
     @return  bool_t 验证成功返回TRUE, 失败返回FALSE
     @note  
     @author hzf
     */
    virtual bool CheckReply(PACKDATA& package, uint8_t command)=0;
        
    
    /**
     @brief 计算数据包的CRC值
     @param[in out] data:数据包
     @return  void
     @note
     @author hzf
    */
    virtual void CalcCRC(PACKDATA &data)=0;


    /**
     @brief CRC值的验证
     @param[in out] data:数据包
     @return  void
     @note
     @author hzf
    */
    virtual bool maonoCalcCRC(mypc_random_write &data)=0;


    /**
     @brief 加密数据
     @param[in out] data:要加密的数据包
     @return  void
     @note 
     @author hzf
    */
    virtual void Encrypt(PACKDATA &data)=0;


    /**
     @brief 解密数据
     @param[in out] data:要解密的数据包
     @return  void
     @note 
     @author hzf
    */
    virtual void Decrypt(PACKDATA &data)=0;


    /**
     @brief 处理其他非监听的数据包
     @param[in] data:接收到的数据包
     @return  void
     @note 此函数被接收线程调用，不是在主线程中
     @author hzf
    */
    virtual void OnRecvData(const PACKDATA& data)=0;
            

    /**
     @brief 连接设备成功时的通知事件
     @param[in] pCommDevMgmt:通讯设备管理器对象
     @return  void
     @note 
     @author hzf
    */
    virtual void OnConnectDevice(ICommDeviceMgmt *pCommDevMgmt)=0;  

    
    /**
     @brief 断开设备连接时的通知事件，此时已经不能可以向设备发送数据
     @return  void
     @author hzf
    */
    virtual void OnDisconnectDeviceFinish()=0;

    /**
     @brief 设备连接状态的变更 
     @param[in] path 设备路径
     @param[in] removal 1--拔出设备  0--插入设备
     @param[in] param1  参数1，如设备vid
     @param[in] param2  参数2， 如设备pid
     @return  void
     @author hzf
     */
    //virtual void OnDeviceChange(const char* dev_path, int removal, uint32_t param1, uint32_t param2) = 0;
        

    /**
     @brief 获取数据包中有效数据的字节数
     @return  int
     @retval 返回数据包中有效数据的字节数
     @note 
     @author hzf
    */
    virtual int GetPackLength() = 0;

	/**
     @brief 设置数据包中有效数据的字节数
     @return  int nPackLength 数据包长度
     @retval 
     @note 
     @author hj
    */
    virtual void SetPackLength(int nPackLength) = 0;

protected:
    ICommProtocol() {}
};



//设备管理类接口
class ICommDeviceMgmt
{
public:
    virtual ~ICommDeviceMgmt() {}

    /**
     @brief 初始化
     @return  bool_t 成功返回TRUE, 失败返回FALSE
     @note
     @author hzf
    */
    virtual bool Init() = 0;
	
    /**
      @brief 设置通信协议
      @param[in] pCommProtocol：设备的通讯协议类
      @return  void
      @author hzf
      */
    virtual void SetProtocol(ICommProtocol* pCommProtocol) = 0;

    /**
     @brief 设置设备的参数，用于打开设备
     @param[in] argc：打开设备可能需要的参数，没有参数可以为NULL
     @return  void
     @author hzf
     */
    virtual void SetDeviceArgc(void* argc) = 0;

    /**
     @brief 判断设备是否已经连接
     @return  bool_t
     @author hzf
    */
    virtual bool IsConnectDevice() const=0;

    /**
     @brief 连接设备
     @param[in] pCommProtocol：设备的通讯协议类
     @param[in] dev_path 设备的路径
     @param[out] param1  参数1，如设备vid
     @param[out] param2  参数2，如设备pid
     @return  bool_t 连接成功返回TRUE, 失败返回FALSE
     @note
     @author hzf
     */
    virtual bool ConnectDevice(const char* dev_path, uint32_t param1, uint32_t param2) = 0;

        
    /**
     @brief 自动枚举和连接现有的设备
     @return  bool_t  连接成功返回TRUE, 失败返回FALSE
     @note
     @author hzf
     */
    virtual bool AutoConnectDevice() = 0;


    /**
     @brief 点击之后连接设备
     @return  bool_t  连接成功返回TRUE, 失败返回FALSE
     @note
     @author hzf
     */
    virtual bool ClickConnectDevice(int PID) = 0;

    /**
     @brief 获取所有已经插入的设备信息
     @return  bool_t  获取成功返回TRUE, 失败返回FALSE
     @note
     @author hzf
     */
    virtual QVector<int> GetDevicesInfo() = 0;

    /**
     @brief 断开设备连接
     @return  void
     @author hzf
    */
    virtual void DisconnectDevice()=0;
    
    /**
     @brief 发送一个数据包，并接收应答数据。不支持多线程调用。
     @param[in] send -- 待发送的数据包
     @param[in] recv -- 接收到的应答数据
     @param[in] nTimes -- 发送失败时的重复次数
     @param[in] dwWaitTime -- 每一次发送数据包，等待多久超时
     @return  bool_t  发送成功返回TRUE, 发送失败返回FALSE
     @note 发送数据包后，要等待接收到应答数据或者等待超时，函数才会返回
     @author hzf
     */
    virtual bool SendAndReceive(unsigned char *send, unsigned char *recv, int nTimes, uint32_t dwWaitTime)=0;

    virtual int pd400_SendAndReceive(unsigned char *send, mypc_random_write &recv, int nTimes, uint32_t dwWaitTime)=0;

    //
    virtual bool pd400_dfu_SendMess(unsigned char *send, mypc_random_write &recv, int n_number, uint32_t dwWaitTime)=0;

    virtual bool pd400_Receive(mypc_random_write &recv, int nTimes, uint32_t dwWaitTime)=0;

    virtual bool DM90_ReceiveMessage(pc_receive_continuitywrite_data &recv, int nTimes, uint32_t dwWaitTime)=0;

    virtual bool DM90_SendMessage(unsigned char *send, pc_receive_continuitywrite_data &recv, int nTimes, uint32_t dwWaitTime)=0;
    
    /**
     @brief 发送一个数据包。不支持多线程调用。
     @param[in] send: 待发送的数据包
     @return  bool_t 发送失败返回FALSE，但是返回TRUE不表示发送成功，而是表示发送过程未出现错误，但不保证设备能接收到数据
     @note 数据包发送后函数直接返回，不等待设备的应答数据
     @author hzf
     */
    virtual bool SendNotReceive(PACKDATA &send)=0;
    

    /**
     @brief 获取当前设备对象
     @return  ICommunicateDevice* 返回的设备对象，有可能未连接设备
     @author hzf
     */
    virtual const ICommunicateDevice* GetDevice()=0;


protected:

    ICommDeviceMgmt() {}
};




#endif // !defined(ICOMMUNICATEDEVICE_H__A171578B_017B_4E6A_B3CE_F01624B52945_)
