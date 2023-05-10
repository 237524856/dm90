#ifndef CHILDTHREAD_H
#define CHILDTHREAD_H

#include <QObject>
#include <QThread>
#include <QMutex>
#include "../devoperator.h"

class childThread : public QThread
{
    Q_OBJECT
public:
    explicit childThread(QObject *parent = nullptr);

    QMutex pauseLock;
    void pauseThread();
    void resumeThread();
    void run();//线程入口函数（工作线程的主函数

    bool mytest = false;

signals:

};

#endif // CHILDTHREAD_H
