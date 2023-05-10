#ifndef HEADER_CLASS_H
#define HEADER_CLASS_H

#include <QObject>
#include "devoperator.h"
#include <QVector>

extern DevOperator *m_devoperator;
extern DevOperator *pm481_devoperator;

//获取所有设备的VID
extern QVector<int>device_pids;



#endif // HEADER_CLASS_H
