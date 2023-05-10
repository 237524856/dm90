#include "dm90_receivemessage_thread.h"

dm90_receiveMessage_thread::dm90_receiveMessage_thread(QObject *parent) : QThread(parent)
{

}

void dm90_receiveMessage_thread::run(){

    while (m_receive_data) {
        //pause.lock();
        QThread::msleep(1);
        if(is_modelconnect)
        m_device_class->DM90Thread_receive_message();
       // pause.unlock();
        //if(!m_ret)return;

        //qDebug()<<"kkkxxxxxxx";
    }
}
