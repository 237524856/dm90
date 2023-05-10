
#pragma once
#pragma execution_character_set("utf-8")
#include "pd400_progress_view.h"
#include <QLabel>
#include "common.h"

pd400_progress_view::pd400_progress_view(QWidget *parent) : QWidget(parent)
{

}
void pd400_progress_view::createUI(){


    QLabel *back_image_msk = new QLabel(this);
    back_image_msk->resize(this->width(),this->height());
    back_image_msk->move(0,0);
    back_image_msk->setPixmap(QPixmap(":/image/images/pd400_setting_bgimg.png"));
    back_image_msk->setScaledContents(true);

    QLabel *back_image = new QLabel(this);
    back_image->resize(320*m_Magnification,100*m_Magnification);
    back_image->move((this->width()-320*m_Magnification)/2,(this->height()-100*m_Magnification)/2);
    back_image->setPixmap(QPixmap(":/image/images/pd400_progress_bgimg.png"));
    back_image->setScaledContents(true);

    QLabel *load_label = new QLabel(back_image);
    load_label->setGeometry(10*m_Magnification,(5+10)*m_Magnification,this->width(),30*m_Magnification);
    load_label->setText(tr("升级固件中，请勿拔掉USB"));
    QString load_label_stys=QString("color:rgb(255,0,0);background-color:transparent;font-size:%1px;font-family:'Source Han Sans CN Medium'").arg(13*m_Magnification);
    load_label->setStyleSheet(load_label_stys);

    m_progress_view = new QProgressBar(back_image);
    m_progress_view->move(12*m_Magnification,(35+10)*m_Magnification);
    m_progress_view->resize((270-4)*m_Magnification,16*m_Magnification);
    m_progress_view->setMinimum(0);  // 最小值
    m_progress_view->setMaximum(100);  // 最大值
    m_progress_view->setValue(99);  // 当前进度
    QString progress_view_stys=QString("QProgressBar {border-radius: %1px;background-color:rgb(27,27,27);}"
                                       "QProgressBar::chunk{background-color:rgb(255,171,33);}").arg(8*m_Magnification);
    m_progress_view->setStyleSheet(progress_view_stys);
    m_progress_view->setTextVisible(false);

    QLabel *progress_image = new QLabel(back_image);
    progress_image->resize(270*m_Magnification,16*m_Magnification);
    progress_image->move(10*m_Magnification,45*m_Magnification);
    progress_image->setPixmap(QPixmap(":/image/images/pd400_progress_image.png"));
    progress_image->setScaledContents(true);

    m_current_value = new QLabel(back_image);
    m_current_value->setGeometry(285*m_Magnification,45*m_Magnification,30*m_Magnification,16*m_Magnification);
    m_current_value->setText(QString::number(m_progress_view->value(),10)+"%");
    QString current_value_stys=QString("color:white;background-color:transparent;font-size:%1px;font-family:'Source Han Sans CN Medium'").arg(13*m_Magnification);
    m_current_value->setStyleSheet(current_value_stys);


}
