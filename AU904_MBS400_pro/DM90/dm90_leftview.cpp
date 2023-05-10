
#pragma once
#pragma execution_character_set("utf-8")
#include "dm90_leftview.h"
#include "yy_custombutton.h"
#include <QDebug>


dm90_leftview::dm90_leftview(QWidget *parent) : QWidget(parent)
{
    this->setAttribute(Qt::WA_StyledBackground,true);


    // createUI();
}

void dm90_leftview::createUI(){

    //创建各种视图
    QLabel *zhixiangLbl = new QLabel(this);
    zhixiangLbl->move(0,10);
    zhixiangLbl->resize(this->width(),30*m_Magnification);
    zhixiangLbl->setText(tr("指向"));
    int fontSize;
    language_index==0?fontSize=16:fontSize=10;
    QString updateLabel_stys = QString("background-color:transparent;color:rgb(255,255,255);font-size:%1px;font-family:'Source Han Sans CN Medium'").arg(fontSize*m_Magnification);
    zhixiangLbl->setStyleSheet(updateLabel_stys);
    zhixiangLbl ->setAlignment(Qt::AlignHCenter);

    QLabel *hLabel = new QLabel(this);
    hLabel->move(5*m_Magnification,zhixiangLbl->x()+zhixiangLbl->height());
    hLabel->resize(this->width()-10*m_Magnification,2*m_Magnification);
    hLabel->setStyleSheet("background-color:rgb(255,255,255)");

    QVector<QString> btnImage = {":/image/images/DM90/dm90_heart",":/image/images/DM90/dm90_omnid",":/image/images/DM90/dm90_point"};
    QVector<QString> btnTitle = {tr("心形"),tr("全向"),tr("8字形")};
    QVector<QString> btnObjectName = {"dm90_heart","dm90_omnid","dm90_point"};

    for (int i=0;i<3;++i) {
        custom_button *btn = new custom_button(this);
        if(i==0){
            btn->resize(26*m_Magnification,22*m_Magnification);
        }else if(i==1){
            btn->resize(26*m_Magnification,26*m_Magnification);
        }else{
            btn->resize(18*m_Magnification,28*m_Magnification);
        }

        btn->move((this->width()-btn->width())/2,50*m_Magnification + (btn->height()+40*m_Magnification)*i);
        QString mainBtn_stys=QString("QPushButton{border-image:url(%1);}"
                                     "QPushButton:hover{border-image:url(%1);}"
                                     "QPushButton:pressed{border-image:url(%2);}"
                                     "QPushButton:checked{border-image:url(%2);}"
                                     "QPushButton:focus{outline: none;}").arg(btnImage.at(i)).arg(btnImage.at(i)+"_select");
        btn->setStyleSheet(mainBtn_stys);
        btn->setCheckable(true);
        btn->setObjectName(btnObjectName.at(i));
        connect(btn,SIGNAL(clicked()),this,SLOT(changezhixiangClick()));
        m_pointBtns.push_back(btn);

        //下面得LBEL
        QLabel *btnbottom_label = new QLabel(this);
        btnbottom_label->resize(this->width(),20*m_Magnification);
        btnbottom_label->move(0,btn->y()+btn->height()+6*m_Magnification);
        QString btnbottom_labelstys = QString("background-color:transparent;color:rgb(255,255,255);font-size:%1px;font-family:'Source Han Sans CN Medium'").arg(10*m_Magnification);
        btnbottom_label->setStyleSheet(btnbottom_labelstys);
        btnbottom_label ->setAlignment(Qt::AlignHCenter);
        btnbottom_label->setText(btnTitle.at(i));
        m_btnTitles.push_back(btnbottom_label);
    }
}

void dm90_leftview::refreshUI(){

    for (auto btn : m_pointBtns) {
       btn->setChecked(false);
    }

    for (auto label : m_btnTitles) {

        QString btnbottom_labelstys = QString("background-color:transparent;color:rgb(255,255,255);font-size:%1px;font-family:'Source Han Sans CN Medium'").arg(10*m_Magnification);
        label->setStyleSheet(btnbottom_labelstys);
    }

    if(GET_DEVICE_DATA->dm90_PolarPattern<3){

        custom_button *btn = m_pointBtns.at(GET_DEVICE_DATA->dm90_PolarPattern);
        btn->setChecked(true);
        QString current_labelstys = QString("background-color:transparent;color:rgb(255,171,33);font-size:%1px;font-family:'Source Han Sans CN Medium'").arg(10*m_Magnification);
        m_btnTitles.at(GET_DEVICE_DATA->dm90_PolarPattern)->setStyleSheet(current_labelstys);

        switch (GET_DEVICE_DATA->dm90_PolarPattern) {

        case HEART:{//

        }
            break;

        case OMNI:{//全指向

        }
            break;

        case GLYPH:{//8字型

        }
            break;

        default:
            break;

        }
    }


}

void dm90_leftview::changezhixiangClick(){


    for (int i =0;i<m_pointBtns.count();i++) {
        m_pointBtns.at(i)->setChecked(false);
        QString btnbottom_labelstys = QString("background-color:transparent;color:rgb(255,255,255);font-size:%1px;font-family:'Source Han Sans CN Medium'").arg(10*m_Magnification);
        m_btnTitles.at(i)->setStyleSheet(btnbottom_labelstys);
    }
    custom_button *btn = static_cast<custom_button *>(sender());
    btn->setChecked(true);
    int select_number = 0;
    if(btn->objectName()=="dm90_heart"){//心型
        select_number =0;

    }else if(btn->objectName()=="dm90_omnid"){//全向
        select_number =1;
    }else{//8字型号
        select_number =2;
    }

    //发送数据
    m_commin_fun.sendMessageForDevice(0xda,1,select_number);

    QString current_labelstys = QString("background-color:transparent;color:rgb(255,171,33);font-size:%1px;font-family:'Source Han Sans CN Medium'").arg(10*m_Magnification);
    m_btnTitles.at(select_number)->setStyleSheet(current_labelstys);

}
