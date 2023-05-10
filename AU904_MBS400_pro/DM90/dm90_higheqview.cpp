#pragma once
#pragma execution_character_set("utf-8")

#include "dm90_higheqview.h"
#include "custom_button.h"
#include <QDebug>

dm90_highEQView::dm90_highEQView(QWidget *parent) : QWidget(parent)
{
    this->setAttribute(Qt::WA_StyledBackground,true);
    this->setStyleSheet("background:rgb(17,17,17)");
}
void dm90_highEQView::createUI(){

    highEQ_btn= new custom_checkbox(this);
    highEQ_btn->setText(tr("  高级均衡器"));
    highEQ_btn->resize(110*m_Magnification,25*m_Magnification);
    highEQ_btn->move(20*m_Magnification , 10*m_Magnification);
    QString highpass_stys=QString("QCheckBox{font-family:'Source Han Sans CN Medium';color:rgb(255,255,255);font-size:%1px;}"
                                    "QCheckBox::hover{font-family:'Source Han Sans CN Medium';color:rgb(54,54,54);font-size:%1px;}"
                                    "QCheckBox::indicator{width:%2px;height:%2px;color: rgb(255, 0, 0);}"
                                    "QCheckBox::indicator:unchecked{border-image:url(:/image/images/pd400_xianfuunclick_btn.png.png);}"
                                    "QCheckBox::indicator:checked{border-image:url(:/image/images/pd400_xianfuclick_btn.png);}"
                                    "QCheckBox:focus{outline: none;}").arg(16*m_Magnification).arg(10*m_Magnification);
    highEQ_btn->setStyleSheet(highpass_stys);
    highEQ_btn->setCheckState(Qt::Checked);
    highEQ_btn->setLayoutDirection(Qt::RightToLeft);
    connect(highEQ_btn,SIGNAL(stateChanged(int)),this,SLOT(isopen_highEQ_click(int)));

    //左边的label
    QWidget *dBView = new QWidget(this);
    dBView->resize(40*m_Magnification,200*m_Magnification);
    dBView->move(20*m_Magnification,80*m_Magnification);
    dBView->setAttribute(Qt::WA_StyledBackground,true);

    QVector<QString>dB_labels ={"+12dB","0","-12dB"};
    for (int i=0;i<3;i++) {
        QLabel *dB_label = new QLabel(dBView);
        dB_label->resize(dBView->width(),14*m_Magnification);
        dB_label->move(0,(dBView->height()/3+dB_label->height())*i);
        if(i==2)
        dB_label->move(0,dBView->height()-dB_label->height()-6*m_Magnification);
        QString updateLabel_stys = QString("background-color:transparent;color:rgb(255,255,255);font-size:%1px;font-family:'Source Han Sans CN Medium'").arg(12*m_Magnification);
        dB_label->setStyleSheet(updateLabel_stys);
        dB_label->setText(dB_labels.at(i));
        dB_label ->setAlignment(Qt::AlignHCenter);
    }

    QVector<QString>bottom_titles={"32","64","125","250","500","1K","2K","4K","8K","16K"};
    for (int i=0;i<10;i++) {
        QLabel *dB_label = new QLabel(this);
        dB_label->resize(40*m_Magnification,14*m_Magnification);
        dB_label->move(dBView->x()+dBView->width()+20*m_Magnification+56*m_Magnification*i,60*m_Magnification);
        QString updateLabel_stys = QString("background-color:transparent;color:rgb(255,255,255);font-size:%1px;font-family:'Source Han Sans CN Medium'").arg(12*m_Magnification);
        dB_label->setStyleSheet(updateLabel_stys);
        dB_label->setText("-11.6");
        dB_label ->setAlignment(Qt::AlignHCenter);
        m_current_values.push_back(dB_label);

        //创建SLIDER
        SliderWidget *verticalSlider = new SliderWidget(this);
        verticalSlider->SetValue(12);
        verticalSlider->SetRange(0,240);
        verticalSlider->SetVertical(true);
        verticalSlider->setObjectName(QString::number(i,10));
        verticalSlider->setGeometry(dB_label->x()+(dB_label->width()-10*m_Magnification)/2,dB_label->y()+dB_label->height()+8*m_Magnification,12*m_Magnification,dBView->height());
        connect(verticalSlider,SIGNAL(valueChanged(int)),this,SLOT(verticalSlider_setValue(int)));
        connect(verticalSlider,SIGNAL(sliderReleasedAt(int)),this,SLOT(verticalSlider_released(int)));
        m_silders.push_back(verticalSlider);

        QLabel *bottom_label = new QLabel(this);
        bottom_label->resize(40*m_Magnification,14*m_Magnification);
        bottom_label->move(dBView->x()+dBView->width()+20*m_Magnification+56*m_Magnification*i,verticalSlider->y()+verticalSlider->height()+10*m_Magnification);
        //QString updateLabel_stys = QString("background-color:transparent;color:rgb(255,255,255);font-size:%1px;font-family:'Source Han Sans CN Medium'").arg(12*m_Magnification);
        bottom_label->setStyleSheet(updateLabel_stys);
        bottom_label->setText(bottom_titles.at(i));
        bottom_label ->setAlignment(Qt::AlignHCenter);
    }
    QWidget *dBViewBottom_right = new QWidget(this);
    dBViewBottom_right->resize(40*m_Magnification,200*m_Magnification);
    dBViewBottom_right->move(this->width()-dBViewBottom_right->width()-40*m_Magnification,80*m_Magnification);
    dBViewBottom_right->setAttribute(Qt::WA_StyledBackground,true);

    for (int i=0;i<3;i++) {
        QLabel *dB_label = new QLabel(dBViewBottom_right);
        dB_label->resize(dBView->width(),14*m_Magnification);
        dB_label->move(0,(dBView->height()/3+dB_label->height())*i);
        if(i==2)
        dB_label->move(0,dBView->height()-dB_label->height()-6*m_Magnification);
        QString updateLabel_stys = QString("background-color:transparent;color:rgb(255,255,255);font-size:%1px;font-family:'Source Han Sans CN Medium'").arg(12*m_Magnification);
        dB_label->setStyleSheet(updateLabel_stys);
        dB_label->setText(dB_labels.at(i));
        dB_label ->setAlignment(Qt::AlignHCenter);
    }
    //最后的重置按钮
    custom_button *btn = new custom_button(this);
    btn->resize(100*m_Magnification,30*m_Magnification);
    btn->move((this->width()-btn->width())/2,this->height()-btn->height()-20*m_Magnification);
    btn->setText(tr("重置"));
    QString btn_stys = QString("QPushButton{background-color:rgb(68,68,68) ;font-family:'Source Han Sans CN Medium';color:white;font-size:%1px;border:0px;border-radius: 15px;}"
                               "QPushButton:hover{background-color:rgb(90,90,90) ;font-family:'Source Han Sans CN Medium';color:white;font-size:%1px;border:0px;border-radius: 15px;}"
                               "QPushButton:pressed{background-color:rgb(255,171,33) ;font-family:'Source Han Sans CN Medium';color:white;font-size:%1px;border:0px}"
                               "QPushButton:focus{outline: none;}"
                               ).arg(16*m_Magnification);
    btn->setStyleSheet(btn_stys);
    btn->setCheckable(true);
    connect(btn,SIGNAL(clicked()),this,SLOT(reset_click()));

    m_maskView = new QWidget(this);
    m_maskView->move(0,0);
    m_maskView->resize(this->width(),this->height());
    m_maskView->setAttribute(Qt::WA_StyledBackground,true);
    m_maskView->setStyleSheet("background:rgba(3,3,3,200)");
    m_maskView->hide();

    highEQ_btn->raise();
}

//获取完整个背景参数之后刷新UI
void dm90_highEQView::refresh(){

    for (int i =0;i<m_silders.count();i++) {

        SliderWidget *current_slider = m_silders.at(i);
        current_slider->blockSignals(true);
        double show_number = dm90_deviceDataSource::getInstance()->loadingChangeNegativeNumber(GET_DEVICE_DATA->dm90UsbPalyEQdatasource[i].Gain);
        m_current_values.at(i)->setText(QString::number(show_number,'f',1));
        int slider_value = show_number*10 +120;
        current_slider->SetValue(slider_value);
        current_slider->blockSignals(false);
    }

    //高级均衡器按钮
    int disableEq = GET_DEVICE_DATA->dm90UsbPalyEQdatasource[0].Enable;
    highEQ_btn->blockSignals(true);
    if(disableEq){//打开
        highEQ_btn->setCheckState(Qt::Checked);
        m_maskView->hide();

    }else{//关闭
        highEQ_btn->setCheckState(Qt::Unchecked);
        m_maskView->show();
    }
    highEQ_btn->blockSignals(false);
}

void dm90_highEQView::verticalSlider_setValue(int currentValue){

    SliderWidget *verticalSlider = static_cast<SliderWidget *>(sender());
    int slider_number = verticalSlider->objectName().toInt();
    QLabel *showvalue_label = m_current_values.at(slider_number);
    double showCurrent = ((double)currentValue-120)/10;
    showvalue_label->setText(QString::number(showCurrent,'f',1));

    emit block_highEQSlider(slider_number);
}
void dm90_highEQView::verticalSlider_released(int value){

    SliderWidget *verticalSlider = static_cast<SliderWidget *>(sender());
    int slider_number = verticalSlider->objectName().toInt();
    emit block_sliderRelease(slider_number);

}
//重置按钮的事件/***************************/
void dm90_highEQView::reset_click(){

    for (int i = 0;i<m_silders.count();i++) {

        SliderWidget *verticalSlider = m_silders.at(i);
        verticalSlider->SetValue(120);
        m_current_values.at(i)->setText("0.0");
        highView_customTempPreData.replace(i+3,"2000");
    }
    //发生数据-------------------
    QVector<int>addrs_disable;
    QVector<int>value_disable;
    for (int i=0;i<10;i++) {
        addrs_disable.push_back(0x205a+5*i);
        value_disable.push_back(2000);
    }
   int frame_len = 7 + 4 *value_disable.count();
   m_commin_fun.sendRandomMessageForDevice(frame_len,addrs_disable,value_disable);

   addrs_disable.clear();
   value_disable.clear();
   for (int i=0;i<10;i++) {
       addrs_disable.push_back(0x2057+5*i);
       value_disable.push_back(1);
   }
  frame_len = 7 + 4 *value_disable.count();
  m_commin_fun.sendRandomMessageForDevice(frame_len,addrs_disable,value_disable);

}
void dm90_highEQView::isopen_highEQ_click(int status){

    qDebug()<<"xxxxx"<<status;
    if(status==0){//没选中
        m_maskView->show();
        QVector<int>addrs;
        QVector<int>value;
        for (int i=0;i<10;i++) {
            addrs.push_back(0x2057+5*i);
            value.push_back(0);
        }
       m_commin_fun.sendRandomMessageForDevice(0x2f,addrs,value);

    }else{//选中
        m_maskView->hide();
        QVector<int>addrs;
        QVector<int>value;
        for (int i=0;i<10;i++) {
            addrs.push_back(0x2057+5*i);
            value.push_back(1);
        }
       m_commin_fun.sendRandomMessageForDevice(0x2f,addrs,value);
    }

    emit block_disable_usbplay(status);

}
