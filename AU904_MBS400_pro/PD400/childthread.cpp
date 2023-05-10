#include "childthread.h"
#include "../header_class.h"

childThread::childThread(QObject *parent) : QThread(parent)
{

}
void childThread::pauseThread(){

    this->pauseLock.lock();
}
void childThread::resumeThread(){

    this->pauseLock.unlock();
}
void childThread::run(){

    pauseLock.lock();
    while (1) {
        if(pm481_devoperator){

           pm481_devoperator->pd400_startSendMsg();
        }
    }
    pauseLock.unlock();

}
