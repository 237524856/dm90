#ifndef PM481SENDRECEIVECLASS_H
#define PM481SENDRECEIVECLASS_H

#include <QObject>

class pm481SendReceiveClass : public QObject
{
    Q_OBJECT
public:
    explicit pm481SendReceiveClass(QObject *parent = nullptr);

    //获取所有的背景参数
    void bg_datasource();

signals:

};

#endif // PM481SENDRECEIVECLASS_H
