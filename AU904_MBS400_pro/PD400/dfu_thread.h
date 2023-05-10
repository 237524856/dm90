#ifndef DFU_THREAD_H
#define DFU_THREAD_H

#include <QWidget>
#include <QThread>
#include "../devoperator.h"

class DFU_thread : public QThread
{
    Q_OBJECT
public:
    explicit DFU_thread(QWidget *parent = nullptr);
    DevOperator *m_dfu_mess;
    QList<QByteArray> m_dfu_dataList;

    bool isReceiveData = false;
    bool m_ret;
    QMutex pause;
    void run();//线程入口函数（工作线程的主函数

signals:

};

#endif // DFU_THREAD_H
