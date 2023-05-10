#include "dfu_thread.h"
#include <QDebug>


DFU_thread::DFU_thread(QWidget *parent) : QThread(parent)
{

}
void DFU_thread::run(){

//    for (int i=1;i<m_dfu_dataList.count()-1;i++) {

//        QByteArray send_data = m_dfu_dataList.at(22);
//        //分成多包发送
//        //是否需要补0
//        int add_data = send_data.count()%63;
//        if(add_data !=0){//后面补0

//            for (int j =0;j<(63-add_data);j++) {
//                send_data.append((char)0);
//            }
//        }
//        QThread::msleep(10);
//        for (int n=0;n<send_data.count()/63;n++) {
//            QByteArray send_mess = send_data.mid(n*63,63);
//            send_mess.insert(0,0X4B);
//            QThread::msleep(10);
//            //if(!m_devoperator->m_pc_isconnect)return;
//            QString str;
//            for (int k=0;k<64;k++) {
//                uint8_t a = send_mess[k];
//                str += " "+QString::number(a,16);
//            }

//           qDebug()<<"send_message"<<str;
//           if(m_dfu_mess)
//          m_dfu_mess->send_updata_DFU_message((unsigned char*)send_mess.data());

//        }

//        //qDebug()<< m_check_view->m_dataSourceList.count()<<"kkkk"<<i;
//    }


    while (isReceiveData) {
        pause.lock();
        QThread::msleep(2);
        if(is_modelconnect)
        m_dfu_mess->thread_receive_message();
        pause.unlock();
        if(!m_ret)return;

        //qDebug()<<"kkkxxxxxxx";
    }
}
