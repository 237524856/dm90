#include "custom_loadingview.h"
#include "common.h"

custom_loadingview::custom_loadingview(const int&w,const int&h,QWidget *parent) : QWidget(parent)
{

    resize(w,h);

    QLabel *back_bg_image = new QLabel(this);
    back_bg_image->resize(this->width(),this->height());
    back_bg_image->setPixmap(QPixmap(":/image/images/pd400_setting_bgimg.png"));
    back_bg_image->setScaledContents(true);

    //创建视图
    LoadingWidget *loadView = new LoadingWidget(this);
    loadView->resize(100*m_Magnification,100*m_Magnification);
    loadView->move((this->width()-loadView->width())/2,(this->height()-loadView->height())/2);
    loadView->start();

}
