#include "yy_custombutton.h"
#include "common.h"
#define CONTENTH 12*m_Magnification

YY_customButton::YY_customButton(QWidget *parent) : QPushButton(parent)
{



}
void YY_customButton::setUI(QString imgStr,QString contentStr){


    m_topImage = new QLabel(this);
    m_topImage->move(0,0);
    m_topImage->resize(this->width()-7*m_Magnification,this->height()-CONTENTH-5*m_Magnification);
    m_topImage->setPixmap(QPixmap(imgStr));
    m_topImage->setScaledContents(true);


    m_content = new QLabel(this);
    m_content->move(0,this->height()-CONTENTH);
    m_content->resize(this->width(),CONTENTH);
    QString updateLabel_stys = QString("background-color:transparent;color:rgb(255,255,255);font-size:%1px;font-family:'Source Han Sans CN Medium'").arg(12*m_Magnification);
    m_content->setStyleSheet(updateLabel_stys);
    m_content->setText(contentStr);

}
