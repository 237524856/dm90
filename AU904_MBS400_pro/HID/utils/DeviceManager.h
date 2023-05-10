#ifndef _H_DEVICE_MANAGER_H_
#define _H_DEVICE_MANAGER_H_
#include <QMutex>
#include <QWaitCondition>
#include <QString>
#include "CommunicateInterface.h"
#include <QTimer>
#include <QDebug>
#include <QThread>
#include <QEventLoop>
#include <QTime>
#include <QByteArray>
#include <QList>
#include "./common.h"
#include <QElapsedTimer>
//#include "../header_class.h"

#ifndef INFINITE
#define  INFINITE   (-1)
#endif


template<class T>
class CDeviceManager : public ICommDeviceMgmt
{
public:
    CDeviceManager()
        : m_pDevice(NULL)
        , m_pProtocol(NULL)
        , m_bDeviceRunning(false)
        , m_bRecvThreadRunning(false)
        , m_pDeviceArgc(NULL)
    {
    }

    ~CDeviceManager(void)
    {
        if (IsConnectDevice())
        {
            DisconnectDevice();
        }

        m_pProtocol = NULL;
        

        if (m_pDevice)
        {
            delete m_pDevice;
            m_pDevice = NULL;
        }
    }

public:
    /**
     @brief 初始化
     @return  bool_t
     @sa
     @retval 成功返回TRUE, 失败返回FALSE
     @note
     @author toyoko
    */
    virtual bool Init()
    {
        m_pDevice = new T();

        //return_value_if_fail(m_pDevice, false);
        if(!m_pDevice){
            return false;
        }
        bool ret = m_pDevice->Init();
        // return_value_if_fail(ret, false);
        if(!ret){
            return false;
        }

        m_pDevice->RegisterDeviceChangeNotification(&CDeviceManager::OnDeviceChange, this);

        return true;
    }

    /**
     @brief 判断是否已连接设备
     @return  bool_t
     @sa
     @retval 已连接设备返回TRUE, 未连接返回FALSE
     @note
     @author toyoko
    */
    virtual bool IsConnectDevice() const
    {
        if (m_pDevice && m_pDevice->IsDeviceOpen())
        {
            return true;
        }

        return false;
    }


    /**
     @brief 设置通信协议
     @param[in] pCommProtocol：设备的通讯协议类
     @return  void
     @author toyoko
     */
    virtual void SetProtocol(ICommProtocol* pCommProtocol)
    {
        m_pProtocol = pCommProtocol;
    }

    /**
     @brief 设置设备的参数，用于打开设备
     @param[in] argc：打开设备可能需要的参数，没有参数可以为NULL
     @return  void
     @author toyoko
     */
    virtual void SetDeviceArgc(void* argc)
    {
        m_pDeviceArgc = argc;
    }

    /**
     @brief 连接设备
     @param[in] dev_path:设备的路径
     @param[out] param1  参数1，如设备vid
     @param[out] param2  参数2，如设备pid
     @return  bool_t 连接成功返回TRUE, 失败返回FALSE
     @note
     @author toyoko
     */
    virtual bool ConnectDevice(const char* dev_path, uint32_t param1, uint32_t param2)
    {

        if(!m_pProtocol){
            return false;
        }
        if(!m_pDevice){
            return false;
        }


        if (!m_pDevice->IsDeviceOpen())
        {
            if (!m_pDevice->OpenDevice(dev_path, param1, param2, m_pDeviceArgc))
            {
                return false;
            }
        }

        //数据包的有效字节数和设备传输的字节数要相等
        printf("--xxx---%d----%d",m_pProtocol->GetPackLength(),m_pDevice->GetMaxReportLength());

        qDebug()<<"xxxxx"<<m_pProtocol->GetPackLength()<<m_pDevice->GetMaxReportLength();
        if (m_pProtocol->GetPackLength() != m_pDevice->GetMaxReportLength())
        {
            //m_pCommProtocol->SetPackLength(m_pCommDevice->GetMaxReportLength());
            //DisconnectDevice();
            //return false;
        }


        m_pProtocol->OnConnectDevice(this);

        return true;
    }



    /**
     @brief 自动枚举和连接现有的设备
     @return  bool_t 已连接设备成功返回TRUE, 失败返回FALSE
     @note
     @author toyoko
     */
    virtual bool AutoConnectDevice()
    {

        if(!m_pProtocol){
            return false;
        }
        if(!m_pDevice){

            return false;
        }
        const DeviceInfo* head = m_pProtocol->GetSupportDevices();
        //qDebug()<<"kkkhhhhxxxxx"<<head->param1<<head->param2;

        if(!head){
            return false;
        }
        // 自动连接第一个枚举到的设备
        while (head)
        {
            void* obj = m_pDevice->EnumDevice(head->param1, head->param2, head->param3);
            if (obj)
            {
                char path[512] = { 0 };
                uint32_t param1 = 0;
                uint32_t param2 = 0;

                bool ret = m_pDevice->GetEnumInfo(obj, path, param1, param2);
                if(!ret){
                    return false;
                }
                return ConnectDevice(path, param1, param2);
            }
            else
            {
                head = head->next;
            }
        }
        return false;
    }

    /**
     @brief 点击之后连接设备
     @return  bool_t  连接成功返回TRUE, 失败返回FALSE
     @note
     @author toyoko
     */
    virtual bool ClickConnectDevice(int PID){

        if(!m_pProtocol){
            return false;
        }
        if(!m_pDevice){

            return false;
        }
        const DeviceInfo* head = m_pProtocol->GetSupportDevices();
        if(!head){
            return false;
        }
        // 判断连接了那一个设备再连接
        while (head)
        {
            void* obj = m_pDevice->EnumDevice(head->param1, head->param2, head->param3);
            if (obj)
            {
                char path[512] = { 0 };
                uint32_t param1 = 0;
                uint32_t param2 = 0;

                bool ret = m_pDevice->GetEnumInfo(obj, path, param1, param2);
                if(!ret){
                    return false;
                }
                if(PID == param2)
                    return ConnectDevice(path, param1, param2);
            }

            head = head->next;

        }

        return false;
    }

    /**
     @brief 获取所有已经插入的设备信息
     @return  bool_t  获取成功返回TRUE, 失败返回FALSE
     @note
     @author toyoko
     */
    virtual QVector<int> GetDevicesInfo(){

        QVector<int> devices={};
        if(!m_pProtocol){
            return devices;
        }
        if(!m_pDevice){

            return devices;
        }
        const DeviceInfo* head = m_pProtocol->GetSupportDevices();
        //qDebug()<<"kkkhhhhxxxxx"<<head->param1<<head->param2;
        if(!head){

            return devices;
        }
        while (head)
        {

            void* obj = m_pDevice->EnumDevice(head->param1, head->param2, head->param3);
            if (obj){
                devices.push_back(head->param2);
                //qDebug()<<"ppppppppppppppp"<<head->param1<<head->param2;
            }
            head = head->next;
        }

        return devices;
    }
    /**
     @brief 断开设备连接
     @return  void
     @note
     @author toyoko
    */
    virtual void DisconnectDevice()
    {
        m_bDeviceRunning = false;

        if (m_pDevice)
        {
            m_pDevice->CloseDevice();
        }

        QMutex mutex;
        QWaitCondition sleep;
        mutex.lock();
        sleep.wait(&mutex, 500);
        mutex.unlock();

        //sleep_ms(10); //等待接收线程退出当前设备的接收循环

        if (m_pProtocol)
        {
            m_pProtocol->OnDisconnectDeviceFinish();
        }
    }
    /**
     @brief 发送一个数据包，并接收应答数据。不支持多线程调用。
     @param[in] send -- 待发送的数据包
     @param[in] recv -- 接收到的应答数据
     @param[in] nTimes -- 发送失败时的重复次数
     @param[in] dwWaitTime -- 每一次发送数据包，等待多久超时
     @return  bool_t 发送成功返回TRUE, 发送失败返回FALSE
     @note 发送数据包后，要等待接收到应答数据或者等待超时，函数才会返回
     @author toyoko
     */
    virtual bool SendAndReceive(unsigned char *send, unsigned char *recv, int nTimes, uint32_t dwWaitTime)
    {
        if (!m_pProtocol || !m_pDevice)
            return false;

        //加密数据域
        //m_pProtocol->Encrypt(send);

        //CRC计算
        //m_pProtocol->CalcCRC(send);



        memset(recv, 0, sizeof(64));

        //使信号量被耗尽
        //m_current.command = send.command;
        // m_current.recv = &recv;

        nTimes = 1;
        while (nTimes)
        {
            //QThread::msleep(300);
            if (m_pDevice == NULL)
                return false;

            //unsigned char *mydata =  send;
            if (false == m_pDevice->Write(send,dwWaitTime))
            {

                qDebug()<<"Send failed. cmd";
                //SLOGFMTE("Send failed. cmd=0x%02x", send.Command);
                nTimes--;
                continue;
            }else{
                bool result = m_pDevice->Read(recv,10);
                //if(!result)qDebug()<<"receive failed. cmd";
            }

            //if (tk_semaphore_wait(m_pWaitRecvData, dwWaitTime) == RET_OK)
            //SLOGFMTE("WaitTimeout. cmd=0x%02x", send.Command);
            nTimes--;
        }


        //m_current.command = 0;
        // m_current.recv = NULL;

        return false;
    }

    virtual int pd400_SendAndReceive(unsigned char *send, mypc_random_write &recv, int nTimes, uint32_t send_length)
    {
        if (!m_pProtocol || !m_pDevice)
            return -1;

        //加密数据域
        //m_pProtocol->Encrypt(send);

        //CRC计算
        //m_pProtocol->CalcCRC(send);
        param_addrL = send[6];
        param_addrH = send[7];
        m_command = send[5];

        m_bDeviceRunning=true;

        memset(&recv, 0, sizeof(mypc_random_write));

        //使信号量被耗尽
        //m_current.command = send.command;
        // m_current.recv = &recv;
        //m_bDeviceRunning = true;
        nTimes = 3;

        while (nTimes)
        {
            if (m_pDevice == NULL)
                return -1;
            //unsigned char *mydata =  send;

            if (false == m_pDevice->Write(send,send_length))
            {
                QThread::msleep(500);
                qDebug()<<"ggggSend failed. cmd"<<nTimes;
                //SLOGFMTE("Send failed. cmd=0x%02x", send.Command);
                nTimes--;

                continue;
            }else{//等待数据得返回
                QElapsedTimer time;
                time.start();
                while(time.elapsed() < 1000)  {//等待200MS接收数据
                    if(!m_bDeviceRunning){
                        return  1;
                    }
                }
                qDebug()<<"kkkkkSend failed. cmd"<<nTimes;

            }

            nTimes--;
        }
        //m_current.command = 0;
        // m_current.recv = NULL;
        qDebug()<<"Send failed. cmd"<<nTimes;
        return fail_code;
    }
    virtual bool pd400_dfu_SendMess(unsigned char *send, mypc_random_write &recv, int n_number, uint32_t dwWaitTime){

        if (!m_pProtocol || !m_pDevice)
            return false;
        memset(&recv, 0, sizeof(mypc_random_write));


        if (!m_pProtocol || !m_pDevice)
            return false;

        m_bDeviceRunning=true;

        int nTimes = 1;

        if(n_number==0)m_sendDfuMessage.clear();//第一打包发送时清空数组
        while (nTimes)
        {
            if (m_pDevice == NULL)
                return false;
            //unsigned char *mydata =  send;

            if (false == m_pDevice->Write(send,64))
            {
                QThread::msleep(200);
                qDebug()<<"Send failed. cmd";
                //SLOGFMTE("Send failed. cmd=0x%02x", send.Command);
                nTimes--;

                continue;
            }else{//等待数据得返回
                m_sendDfuMessage.push_back(send);
                //if(m_sendDfuMessage.count())
                //                                QTime time;
                //                                time.start();
                //                                while(time.elapsed() < 200)  {//等待200MS接收数据
                //                                    if(!m_bDeviceRunning){
                //                                        return  true;
                //                                    }
                //                                }
                //                                if(n_number==0)QThread::msleep(2000);
                //qDebug()<<"dfuNumber-----0xxxxxxxxxxxxxxx. cmd"<<m_sendDfuMessage.count()<<n_number;
            }

            nTimes--;
        }
        return false;
    }

    virtual bool DM90_SendMessage(unsigned char *send, pc_receive_continuitywrite_data &recv, int n_number, uint32_t dwWaitTime){

        if (!m_pProtocol || !m_pDevice)
            return false;
        memset(&recv, 0, sizeof(mypc_random_write));


        if (!m_pProtocol || !m_pDevice)
            return false;

        m_bDeviceRunning=true;

        int nTimes = 1;

        //if(n_number==0)m_sendDfuMessage.clear();//第一打包发送时清空数组

        while (nTimes)
        {
            if (m_pDevice == NULL)
                return false;
            //unsigned char *mydata =  send;

            if (false == m_pDevice->Write(send,64))
            {

                qDebug()<<"Send failed. cmd";
                //SLOGFMTE("Send failed. cmd=0x%02x", send.Command);
                nTimes--;
                continue;
            }else{//等待数据得返回
                // m_sendDfuMessage.push_back(send);
                //                bool result = m_pDevice->DM90Read(&recv,64);
                //                return result;
                return true;
            }

            nTimes--;
        }


        return false;
    }

    virtual bool DM90_ReceiveMessage(pc_receive_continuitywrite_data &recv, int nTimes, uint32_t dwWaitTime){

        if (!m_pProtocol || !m_pDevice)
            return false;
        memset(&recv, 0, sizeof(pc_receive_continuitywrite_data));
        if (m_pDevice == NULL)
            return false;

        bool result = m_pDevice->DM90Read(&recv,dwWaitTime);
        if(recv.command==0x06)
            //qDebug()<<"receive---xxxxxgggg------------------------"<<result;
            return result;
    }

    virtual bool pd400_Receive(mypc_random_write &recv, int nTimes, uint32_t dwWaitTime){

        if (!m_pProtocol || !m_pDevice)
            return false;
        memset(&recv, 0, sizeof(mypc_random_write));

        if (m_pDevice == NULL)
            return false;

        bool result = m_pDevice->myRead(&recv,10);
        if(result){
            //不是主动返回的电平
            //判断是不是有效正确的数据
            bool crc_result = m_pProtocol->maonoCalcCRC(recv);

            QByteArray array_data;
            array_data.append((char*)&recv, sizeof(recv));
            QString str;
            for (int k=0;k<64;k++) {
                uint8_t a = array_data[k];
                str += " "+QString::number(a,16);
            }
            //qDebug()<<"receive failedxxxxxxxxxxxxxxx. cmd"<<str<<"cc"<<crc_result;
            if(m_command == 0x04){
                //qDebug()<<"receive failedxxxxxxxxxxxxxxx. cmd"<<str<<"xxx"<<param_addrL<<param_addrH<<"xxx"<<recv.param_addr[0]<<recv.param_addr[1];
                if(param_addrL == recv.param_addr[0] && param_addrH == recv.param_addr[1] && crc_result){

                    m_bDeviceRunning=false;
                }

                if(param_addrL != recv.param_addr[0] || param_addrH != recv.param_addr[1]){//数据错误
                    fail_code = -201;
                }
                if(!crc_result){//校验码错误
                    fail_code = -301;
                }

            }else{
                m_bDeviceRunning=false;
            }

        }else{

        }


        return result;
    }
    /**
     @brief 发送一个数据包。不支持多线程调用。
     @param[in] send: 待发送的数据包
     @return  bool_t 发送失败返回FALSE，但是返回TRUE不表示发送成功，而是表示发送过程未出现错误，但不保证设备能接收到数据
     @note 数据包发送后函数直接返回，不等待设备的应答数据
     @author toyoko
     */
    virtual bool SendNotReceive(PACKDATA& send)
    {
        if (!m_pProtocol)
            return false;
        //加密数据域
        m_pProtocol->Encrypt(send);
        //CRC计算
        m_pProtocol->CalcCRC(send);
        //send.buff[m_dwMaxWriteSize-1] = CCRC::GetCrc8((uint8_t*)&send, m_dwMaxWriteSize-1);

        int nTimes = 5;
        while (nTimes > 0)
        {
            if (m_pDevice == NULL)
                return false;

            unsigned char *mydata =  (unsigned char*)&send;
            if (false == m_pDevice->Write(mydata, m_pProtocol->GetPackLength()))
            {
                //SLOGFMTE("send failed. cmd=0x%02x", send.Command);
                nTimes--;
                continue;
            }
            else
            {
                return true;
            }
        }
        return false;
    }
    /**
     @brief 获取当前设备对象
     @return  ICommunicateDevice* 返回的设备对象，有可能未连接设备
     @author toyoko
     */
    virtual const ICommunicateDevice* GetDevice()
    {
        return m_pDevice;
    }

protected:
    /**
     @brief 接收数据的线程
     @return  void*
     @author toyoko
     */
    static void* RecvThread(void* args)
    {
        CDeviceManager* pManager = (CDeviceManager*)args;
        if (pManager) pManager->RecvThreaProc();
        //log_info("CDeviceManager::RecvThread end.");
        return 0;
    }

    void  RecvThreaProc()
    {
        PACKDATA data = {  };
        bool bRet;
        bool bFound;

    }


    /**
     @brief 设备拔插事件通知回调函数
     @param[in] path 设备路径
     @param[in] removal 1--拔出设备  0--插入设备
     @param[in] param1  参数1，如设备vid
     @param[in] param2  参数2， 如设备pid
     @param[in] param3  参数3
     @return  void
     @author toyoko
     */
    static void OnDeviceChange(const char* path, int removal, uint32_t param1, uint32_t param2, void* param3)
    {
        printf("vid:pid=%04X:%04X removal=%d, path:%s\n", param1, param2, removal, path);
        qDebug()<<"xxxxxxxxxxxvidkkxxxxxxxxxxxxxxxx"<<removal<<path;
        //打开计时器


        if (!param3)
            return;

        //bool_t flag = 0;

        CDeviceManager* p = (CDeviceManager*)param3;
        if (!p->m_pProtocol)
            return;

        if (removal)
        {
            QString str1(path);
            QString str2(p->GetDevice()->GetDevicePath());

            //if (p->IsConnectDevice()&& str1.compare(str2, Qt::CaseInsensitive) == false)

            if (str1.compare(str2, Qt::CaseInsensitive) == false)
            {
                p->DisconnectDevice();
                //flag = 1;
            }
            m_MainTimer->start(230);
        }
        else
        {
            m_MainTimer->start(3000);
            if (p->m_pProtocol->IsSupportDevic(path, param1, param2))
            {

                QString str1(path);
                QString str2(p->GetDevice()->GetDevicePath());
                qDebug()<<"path-------"<<str1<<str2;
                //flag = 1;//判断PID跟地址是不是一样的
#if defined(Q_OS_WIN32)
//                if(param2==currentProPid && str1.compare(str2, Qt::CaseInsensitive) == false)
//                    p->ConnectDevice(path, param1, param2);
#else

#endif

            }
        }
        //if (flag)
        //{
        //    p->m_pProtocol->OnDeviceChange(path, removal, param1, param2);
        //}
    }

protected:
    ICommunicateDevice* m_pDevice;   //设备
    ICommProtocol* m_pProtocol; //协议
    void* m_pDeviceArgc; //打开设备时需要用到的参数

    // 当前等待接收的数据包相关信息
    typedef struct RecvObject
    {
        uint8_t   command;// 将要接收的命令编码

        PACKDATA* recv;	  // 于存放接收数据的buff

    }RECVOBJECT;

    RECVOBJECT m_current;				// 监听当前需要应答的数据包

    //tk_mutex_t* m_pRecvObjectMutex;    // 用于互斥访问m_current
    //tk_thread_t* m_pDataRecvThread;     // 接收线程

    //tk_semaphore_t* m_pWaitConnectDevice; // 等待连接设备
    //tk_semaphore_t* m_pWaitRecvData;	// 等待硬机架下发音效的dsp代码完成事件句柄

    bool  m_bDeviceRunning;			// 是否正在接收数据
    bool  m_bRecvThreadRunning;       // 接收线程是否正在运行

    QList<unsigned char *> m_dfu_sendpack;//需要发送的包数据
    QVector<unsigned char *> m_sendDfuMessage;

    //判断收发的地址是否是一样
    uint8_t param_addrL =0;
    uint8_t param_addrH =0;
    uint8_t m_command =0;
    //错误码
    int fail_code = -100;

};
#endif //_H_DEVICE_MANAGER_H_
