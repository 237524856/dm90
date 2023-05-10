
#if !defined(AFX_HID_H__A5700CF5_9580_4B1F_B9A6_5FC546E0EF3B__INCLUDED_)
#define AFX_HID_H__A5700CF5_9580_4B1F_B9A6_5FC546E0EF3B__INCLUDED_

#include <iostream>
#include "CommunicateInterface.h"
#include "HID/hidapi.h"
#include "common.h"

class CHidBase : public ICommunicateDevice ,public QObject
{
public:
	CHidBase();
	virtual ~CHidBase();

    virtual bool Init();

    
    virtual void* EnumDevice(uint32_t param1, uint32_t param2, void* param3);

    virtual bool GetEnumInfo(void* dev_obj, char* dev_path, uint32_t& param1, uint32_t& param2);

    virtual uint32_t GetDeviceParam1() const;

    virtual uint32_t GetDeviceParam2() const;

    virtual bool IsDeviceOpen();

	virtual const char* GetDeviceName()const;

    virtual bool OpenDevice(const char* dev_path, uint32_t vid, uint32_t pid, void* param3);

	virtual void CloseDevice();

    virtual int GetMaxReportLength();

    virtual bool Write(unsigned char* data, int data_bytes);

    virtual bool Read(unsigned char* buff, int waitTime);

    virtual bool myRead(mypc_random_write* buff, int waitTime);

    virtual bool DM90Read(pc_receive_continuitywrite_data* buff, int waitTime);


    virtual const char* GetDevicePath() const;

    virtual void RegisterDeviceChangeNotification(FnDeviceChangeCallback fn, void* user_data);

protected:
    static void HidDeviceChangeCallback(unsigned short vid, unsigned short pid, const char* path, int removal, void* user_data);

signals:
    void mydevice_change(int);

private:
	/**
	 * hid设备路径
	 */	
	char device_path[256];

	/**
	 * 设备名称
	 */
	char name[128];
	
	/**
	 * hid设备句柄
	 */
	hid_device *handle;
	
	/**
	 * 接收数据的缓存
	 */
	PACKDATA recv_buff;


    /**
     * 接收pd400数据的缓存
     */
    mypc_random_write pd400_randomrecv_buff;

    /**
     * 接收DM90数据的缓存
     */
    pc_receive_continuitywrite_data DM90_randomrecv_buff;
	
	/**
	 * 接收数据报文的长度
	 */
	int input_report_length;

	/**
	 * 枚举设备返回的信息，需要手动释放
	 */
    hid_device_info* hid_enumeration_info;


    /**
     * 初始化是否完成
     */
    bool init_ready;

    /**
     * 设备拔插事件通知回调函数
     */
    FnDeviceChangeCallback device_change_callback;

    pd400_FnDeviceChangeCallback pd400_device_change_callback;

    /**
     * 设备拔插事件通知回调函数的用户传入的数据
     */
    void* user_data;


	/**
	 * 是否正在枚举设备
	 * Mac电脑上枚举设备时，会进入回调函数device_change_callback
	 */
    bool enum_device;

	/**
	 * hid设备的vid
	 */
	uint32_t device_vid;

	/**
	 * hid设备的pid
	 */
	uint32_t device_pid;
};

#endif // !defined(AFX_HID_H__A5700CF5_9580_4B1F_B9A6_5FC546E0EF3B__INCLUDED_)

