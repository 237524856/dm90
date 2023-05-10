#ifndef SIGNALINSTANCE_H
#define SIGNALINSTANCE_H

#include <QObject>
#include <QWidget>

class SignalInstance : public QObject
{
    Q_OBJECT
public:

    static SignalInstance &GetInstance();


private:
    SignalInstance();

    SignalInstance(const SignalInstance &) = delete;
    SignalInstance(const SignalInstance &&) = delete;
    SignalInstance &operator=(const SignalInstance &) = delete;


signals:

    void changeResultTitle(int num);
    void sigFreedDragRet(bool ret);
    void sigEndFreedDragRet(bool ret);


public slots:

};

#endif // SIGNALINSTANCE_H
