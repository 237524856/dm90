#ifndef DM90_RECEIVEMESSAGE_THREAD_H
#define DM90_RECEIVEMESSAGE_THREAD_H

#include <QObject>
#include <QThread>
#include "../devoperator.h"

class dm90_receiveMessage_thread : public QThread
{
    Q_OBJECT
public:
    explicit dm90_receiveMessage_thread(QObject *parent = nullptr);


    DevOperator *m_device_class;
    QList<QByteArray> m_dfu_dataList;

    bool m_receive_data = false;
    bool m_ret;
    QMutex pause;
    void run();//线程入口函数（工作线程的主函数

signals:

};

#endif // DM90_RECEIVEMESSAGE_THREAD_H
