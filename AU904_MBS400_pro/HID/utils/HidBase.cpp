#include <QtGlobal>
#include "HidBase.h"
#include <QDebug>

CHidBase::CHidBase()
{
    memset(this->name, 0, sizeof(this->name));
    memset(this->device_path, 0, sizeof(this->device_path));
    this->handle = 0;
    this->input_report_length = 0;
    this->hid_enumeration_info = NULL;
    this->init_ready = false;
    this->device_change_callback = NULL;
    this->enum_device = false;
    this->device_vid = 0;
    this->device_pid = 0;
}

CHidBase::~CHidBase()
{
    if (hid_enumeration_info)
    {
        hid_free_enumeration(hid_enumeration_info);
        hid_enumeration_info = NULL;
    }

    if (init_ready)
    {
        hid_exit();
    }
}

bool CHidBase::Init()
{
    if (init_ready)
    {
        return true;
    }

    if (hid_init() == 0)
    {
        init_ready = true;
        return true;
    }

    return false;
}

bool CHidBase::IsDeviceOpen()
{

    if ( this->handle != 0  && this->device_vid==currentProPid)
    {
        return true;
    }
    else
    {
        return false;
    }
}


void CHidBase::CloseDevice()
{
    if (this->handle)
    {
        hid_close(this->handle);
        this->handle = 0;
    }
    this->name[0] = '\0';
    this->input_report_length = 0;
}

const char* CHidBase::GetDeviceName()const
{
    return this->name;
}

bool CHidBase::OpenDevice(const char* dev_path, uint32_t vid, uint32_t pid, void* param3)
{
    //return_value_if_fail(this->init_ready, false);
    //return_value_if_fail(dev_path, false);
    if(!this->init_ready){
        return false;
    }
    if(!dev_path){
        return false;
    }

    if(this->handle) return true;

    this->handle = hid_open_path(dev_path);
    if (!this->handle)
    {
        //log_error("hid_open error\n");
        return false;
    }

    wchar_t wstr[256] = { 0 };
    int res;
    res = hid_get_product_string(this->handle, wstr, 256);
    if (0 == res)
    {
        //wchar转char
        size_t i;
        char* pMBBuffer = (char *)malloc( 256 );
#if defined(Q_OS_WIN32)
        wcstombs_s(&i, pMBBuffer, (size_t)256,
                   wstr, (size_t)256 );
        strcpy_s(this->name,strlen(pMBBuffer)+1 ,pMBBuffer);
#else
        wcstombs(pMBBuffer, wstr, (size_t)256 );
        //        wcstombs_s(&i, pMBBuffer, (size_t)256,
        //                      wstr, (size_t)256 );
        strcpy(this->name,pMBBuffer);
#endif
        free(pMBBuffer);
        pMBBuffer = NULL;
        //printf(this->name,"%s",pMBBuffer);
        //        str_t str;
        //        str_init(&str, 0);
        //        str.str = this->name;
        //        str.size = 0;
        //        str.capacity = sizeof(this->name);

        //        str_from_wstr(&str, wstr);
    }
#if defined(Q_OS_WIN32)
    strcpy_s(this->device_path,strlen(dev_path)+1 ,dev_path);
#else
    strcpy(this->device_path,dev_path);
#endif


    this->input_report_length = hid_max_input_report_len(this->handle);

    this->device_vid = vid;
    this->device_pid = pid;

    return true;
}


int CHidBase::GetMaxReportLength()
{
    return this->input_report_length;
}



bool CHidBase::Read(unsigned char* pBuff, int waitTime)
{
    //return_value_if_fail(this->handle, false);
    if(!this->handle){
        return  false;
    }
    int res = 0;


    //res = hid_read_timeout(this->handle, this->recv_buff.buff, this->input_report_length, waitTime);
    res = hid_read_timeout(this->handle, this->pd400_randomrecv_buff.buff, 64, 1024);
    if ( -1 == res )
        return false;
    else if ( 0 == res )
        return false;


    if ( pBuff )
        // pBuff = (unsigned char*)&pd400_randomrecv_buff;
        //receive_radom_data = pd400_randomrecv_buff;


        return true;
}

bool CHidBase::myRead(mypc_random_write* pBuff, int waitTime)
{
    //return_value_if_fail(this->handle, false);
    if(!this->handle){
        return  false;
    }
    int res = 0;


    //res = hid_read_timeout(this->handle, this->recv_buff.buff, this->input_report_length, waitTime);
    res = hid_read_timeout(this->handle, this->pd400_randomrecv_buff.buff, 64, 64);
    //    qDebug()<<"receive_data------"<<"------xx"<<pd400_randomrecv_buff.param_value[0]<<pd400_randomrecv_buff.param_value[1]<<pd400_randomrecv_buff.report<<pd400_randomrecv_buff.frame_header;
    if ( -1 == res )
        return false;
    else if ( 0 == res )
        return false;


    if ( pBuff )
        *pBuff = pd400_randomrecv_buff;

    return true;
}

bool CHidBase::DM90Read(pc_receive_continuitywrite_data* pBuff, int waitTime)
{
    //return_value_if_fail(this->handle, false);
    if(!this->handle){
        return  false;
    }
    int res = 0;

    //res = hid_read_timeout(this->handle, this->recv_buff.buff, this->input_report_length, waitTime);
    res = hid_read_timeout(this->handle, this->DM90_randomrecv_buff.buff, this->input_report_length, waitTime);
    if ( -1 == res )
        return false;
    else if ( 0 == res )
        return false;

    if ( pBuff )
        *pBuff = DM90_randomrecv_buff;

    return true;
}


int testqq = 0;
bool CHidBase::Write(unsigned char* data, int data_bytes)
{
    //return_value_if_fail(this->handle, false);
    if(!this->handle){
        return  false;
    }

    //	if ( NULL == data || data_bytes != this->input_report_length )
    //	{
    //		return false;
    //	}

    int res = hid_write(this->handle, data, data_bytes);

    QByteArray array_data;
    array_data.append((char*)data, sizeof(data));
    QString str;
    for (int k=0;k<sizeof(data);k++) {
        uint8_t a = array_data[k];
        str += " "+QString::number(a,16);
    }
    testqq ++;
    //qDebug()<<"send-----"<<testqq<<str<<res;
    //if ( res != data_bytes )
    //     if ( res != 64 )
    //        return false;

    return res;
}


void* CHidBase::EnumDevice(uint32_t param1, uint32_t param2, void* param3)
{
    if (hid_enumeration_info)
    {
        hid_free_enumeration(hid_enumeration_info);
        hid_enumeration_info = NULL;
    }

    this->enum_device = true;
    hid_enumeration_info = hid_enumerate((uint16_t)param1, (uint16_t)param2);
    this->enum_device = false;

    return hid_enumeration_info;
}

bool CHidBase::GetEnumInfo(void* dev_obj, char* dev_path, uint32_t& param1, uint32_t& param2)
{
    hid_device_info* obj = (hid_device_info*)dev_obj;
    //return_value_if_fail(obj, false);
    if(!obj){
        return false;
    }

    //tk_strcpy(dev_path, obj->path);
#if defined(Q_OS_WIN32)

    strcpy_s(dev_path, strlen(obj->path)+1,obj->path);
#else
    strcpy(dev_path,obj->path);
#endif
    param1 = obj->vendor_id;
    param2 = obj->product_id;
    return true;
}

uint32_t CHidBase::GetDeviceParam1() const
{
    return this->device_vid;
}

uint32_t CHidBase::GetDeviceParam2() const
{
    return this->device_pid;
}


const char* CHidBase::GetDevicePath() const
{
    return this->device_path;
}

/*static */void CHidBase::HidDeviceChangeCallback(unsigned short vid, unsigned short pid, const char* path, int removal, void* user_data)
{

    if(m_MainTimer)
        m_MainTimer->start(230);
    if (user_data)
    {
        CHidBase* p = (CHidBase*)user_data;
        if (!p) return;
        //Mac电脑上枚举设备时，会进入回调函数device_change_callback, 要排除这种情况
        if (p->enum_device)
            return;
        //暂时删除

        qDebug()<<"testddddddd"<<path<<removal<<vid<<pid<< p->user_data;
        if(p->user_data)
            p->device_change_callback(path, removal, vid, pid, p->user_data);
    }
}

void CHidBase::RegisterDeviceChangeNotification(FnDeviceChangeCallback fn, void* user_data)
{
    if (fn)
    {
        hid_register_device_notification(&CHidBase::HidDeviceChangeCallback, this);
    }
    else
    {
        hid_register_device_notification(NULL, NULL);
    }

    this->device_change_callback = fn;
    this->user_data = user_data;
}
