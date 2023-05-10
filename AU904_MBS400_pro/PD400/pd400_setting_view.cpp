

#pragma once
#pragma execution_character_set("utf-8")
#include "pd400_setting_view.h"
#include <QLabel>
#include <QVector>
#include <QComboBox>
#include <QDebug>
#include <QApplication>
#include "../common.h"

pd400_setting_view::pd400_setting_view(QWidget *parent) : QWidget(parent)
{
    //    if (translator.load(":/pd400_lang/pd400_zh.qm")) {
    //        qApp->installTranslator(&translator);
    //    }
}

void pd400_setting_view::createUI(){

    QLabel *back_bg_image = new QLabel(this);
    back_bg_image->move(0,0);
    back_bg_image->resize(this->width(),this->height());
    back_bg_image->setPixmap(QPixmap(":/image/images/pd400_setting_bgimg.png"));
    back_bg_image->setScaledContents(true);

    QLabel *logo_bg_image = new QLabel(this);
    logo_bg_image->move(this->width()-245*m_Magnification,72*m_Magnification);
    logo_bg_image->resize(220*m_Magnification,400*m_Magnification);
    logo_bg_image->setPixmap(QPixmap(":/image/images/pd400_setting_logo.png"));
    logo_bg_image->setScaledContents(true);

    QVector<QString>titles={tr("产品型号: "),tr("固件版本: "),tr("DSP版本: "),tr("设备SN: "),tr(""),tr("恢复出厂设置")};
    for (int i=0;i<titles.count();i++) {
        QLabel *title_lab = new QLabel(logo_bg_image);
        title_lab->resize(this->width()-40*m_Magnification,22*m_Magnification);
        title_lab->move(20*m_Magnification,60*m_Magnification+22*i*m_Magnification);

        if(i==0){
            //            title_lab->setText(titles.at(i));
            m_device_name_label = new QLabel(title_lab);
            m_device_name_label->resize(title_lab->width(),title_lab->height());
            m_device_name_label->move(0,0);
            m_device_name_label->setText(titles.at(i)+"");
            //            m_device_name_label->setStyleSheet("color:white;font-size:15px;font-family:'Source Han Sans CN Medium'");

        }else if(i==1){//固件版本号
            //            title_lab->setText(titles.at(i));
            m_version_label = new QLabel(title_lab);
            m_version_label->resize(title_lab->width(),title_lab->height());
            m_version_label->move(0,0);
            m_version_label->setText(titles.at(i)+"");
            //            m_version_label->setStyleSheet("color:white;font-size:15px;font-family:'Source Han Sans CN Medium'");
        }else if(i==2){//DSP版本号
            m_DSP_label = new QLabel(title_lab);
            m_DSP_label->resize(title_lab->width(),title_lab->height());
            m_DSP_label->move(0,0);
            m_DSP_label->setText(titles.at(i)+"");
            //            m_version_label->setStyleSheet("color:white;font-size:15px;font-family:'Source Han Sans CN Medium'");
        }else if(i==3){//设备SN号
            m_SN_label = new QLabel(title_lab);
            m_SN_label->resize(title_lab->width(),title_lab->height());
            m_SN_label->move(0,0);
            m_SN_label->setText(titles.at(i)+"");
            //            m_version_label->setStyleSheet("color:white;font-size:15px;font-family:'Source Han Sans CN Medium'");
        }else if(i==4){
            title_lab->setText(titles.at(i));
            m_check_btn = new custom_button(logo_bg_image);
            m_check_btn->resize(180*m_Magnification,22*m_Magnification);
            m_check_btn->move(20*m_Magnification,60*m_Magnification+22*i*m_Magnification);
            m_check_btn->setText(tr("检查固件更新"));
            QString check_btn_stys=QString("QPushButton {background-color:rgb(255,171,33);border-radius:%3px; color:white;text-align:center;font-size:%1px;font-family:'Source Han Sans CN Medium'}"
                                           "QPushButton:hover{background-color:rgb(255,171,33);color:rgb(54,54,54);text-align:center;font-size:%1px;font-family:'Source Han Sans CN Medium'}"
                                           "QPushButton:focus{outline: none;}").arg(15*m_Magnification).arg(15/2*m_Magnification).arg(11*m_Magnification);
            m_check_btn->setStyleSheet(check_btn_stys);
            connect(m_check_btn,SIGNAL(clicked()),this,SLOT(check_click()));
#if defined(Q_OS_WIN32)

#else
            m_check_btn->hide();
#endif
        }else if(i==5){
            custom_button *reset_btn = new custom_button(logo_bg_image);
            reset_btn->resize(180*m_Magnification,22*m_Magnification);
#if defined(Q_OS_WIN32)
            reset_btn->move(20*m_Magnification,60*m_Magnification+22*i*m_Magnification+10);
#else
            reset_btn->move(20*m_Magnification,60*m_Magnification+22*(i-1)*m_Magnification+10);
#endif

            reset_btn->setText(tr("恢复出厂设置"));
            QString check_btn_stys=QString("QPushButton {background-color:rgb(255,171,33);border-radius:%3px; color:white;text-align:center;font-size:%1px;font-family:'Source Han Sans CN Medium'}"
                                         "QPushButton:hover{background-color:rgb(255,171,33);color:rgb(54,54,54);text-align:center;font-size:%1px;font-family:'Source Han Sans CN Medium'}"
                                         "QPushButton:focus{outline: none;}").arg(15*m_Magnification).arg(15/2*m_Magnification).arg(11*m_Magnification);
            reset_btn->setStyleSheet(check_btn_stys);
            connect(reset_btn,SIGNAL(clicked()),this,SLOT(reset_click()));
        }
        QString title_lab_stys=QString("color:white;font-size:%1px;font-family:'Source Han Sans CN Medium'").arg(15*m_Magnification);
        title_lab->setStyleSheet(title_lab_stys);
        seting_labels.push_back(title_lab);
    }
//切换语言
    if(language_index>0){
        for (int i=0;i<seting_labels.count();i++) {
            seting_labels.at(i)->setStyleSheet(QString("color:white;font-size:%1px;font-family:'Source Han Sans CN Medium'").arg(12*m_Magnification));
        }
        m_check_btn->move(20*m_Magnification,60*m_Magnification+22*4*m_Magnification);
        QString check_btn_stys=QString("QPushButton {background-color:rgb(255,171,33);border-radius:%3px; color:white;text-align:center;font-size:%1px;font-family:'Source Han Sans CN Medium'}"
                                       "QPushButton:hover{background-color:rgb(255,171,33);color:rgb(54,54,54);text-align:center;font-size:%1px;font-family:'Source Han Sans CN Medium'}"
                                       "QPushButton:focus{outline: none;}").arg(13*m_Magnification).arg(15/2*m_Magnification).arg(11*m_Magnification);
        m_check_btn->setStyleSheet(check_btn_stys);

    }else{
        for (int i=0;i<seting_labels.count();i++) {
            seting_labels.at(i)->setStyleSheet(QString("color:white;font-size:%1px;font-family:'Source Han Sans CN Medium'").arg(15*m_Magnification));
        }
        m_check_btn->move(20*m_Magnification,60*m_Magnification+22*4*m_Magnification);

        QString check_btn_stys=QString("QPushButton {background-color:rgb(255,171,33);border-radius:%3px; color:white;text-align:center;font-size:%1px;font-family:'Source Han Sans CN Medium'}"
                                       "QPushButton:hover{background-color:rgb(255,171,33);color:rgb(54,54,54);text-align:center;font-size:%1px;font-family:'Source Han Sans CN Medium'}"
                                       "QPushButton:focus{outline: none;}").arg(15*m_Magnification).arg(15/2*m_Magnification).arg(11*m_Magnification);
        m_check_btn->setStyleSheet(check_btn_stys);
    }
}

void pd400_setting_view::check_click(){

    emit check_version();
}
void pd400_setting_view::reset_click(){

    emit reset_clickBlock();
}
//选择语言
void pd400_setting_view::mousePressEvent(QMouseEvent *qevent)
{
    // this->hide();
    emit setting_mouse_press();
}
void pd400_setting_view::mouseReleaseEvent(QMouseEvent *qevent)
{

}
void pd400_setting_view::mouseMoveEvent(QMouseEvent *qevent){

}
