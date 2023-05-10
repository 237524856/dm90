#ifndef CUSTOM_LOADINGVIEW_H
#define CUSTOM_LOADINGVIEW_H

#include <QWidget>
#include "loadingwidget.h"

class custom_loadingview : public QWidget
{
    Q_OBJECT
public:
    explicit custom_loadingview(const int&w,const int&h,QWidget *parent = nullptr);

signals:

};

#endif // CUSTOM_LOADINGVIEW_H
