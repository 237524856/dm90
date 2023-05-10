
#pragma once
#pragma execution_character_set("utf-8")

#include "pd400_leftview.h"
#include <QVector>
#include <QDebug>
#include <QThread>
#include "../HID/utils/CRC.h"
#include <QElapsedTimer>
#include <QTimer>

#define LeftADDMICY 40

pd400_leftview::pd400_leftview(QWidget *parent) : QWidget(parent)
{
    m_DM30Timer = new QTimer(this);
    connect(m_DM30Timer, SIGNAL(timeout()), this, SLOT(handleTimeout()));
}
void pd400_leftview::createUI(){

    QLabel *back_left_image = new QLabel(this);
    back_left_image->move(0,0);
    back_left_image->resize(this->width(),this->height());
    back_left_image->setPixmap(QPixmap(":/image/images/pd400_backimage.png"));
    back_left_image->setScaledContents(true);

    QPalette pa;
    QColor niceBlue(255, 255, 255);
    pa.setColor(QPalette::WindowText,niceBlue);
    QFont font;
    font.setFamily("Source Han Sans CN Medium");
    font.setPixelSize(15*m_Magnification);

    m_micmute_label = new QLabel(this);
    m_micmute_label->move(20*m_Magnification,20*m_Magnification);
    m_micmute_label->resize(93*m_Magnification,24*m_Magnification);
    m_micmute_label->setText(tr("麦克风静音"));
    m_micmute_label->setPalette(pa);
    m_micmute_label->setFont(font);

    m_mute_btn= new custom_checkbox(this);
    m_mute_btn->resize(40*m_Magnification,40*m_Magnification);
    m_mute_btn->move(20*m_Magnification,53*m_Magnification);
    QString mute_bt_stys=QString("QCheckBox::indicator{width: %1px;height: %1px;color: rgb(255, 0, 0);}"
                                 "QCheckBox::indicator:unchecked{border-image:url(:/image/images/pd400_mute_btnDefalut);}"
                                 "QCheckBox::indicator:unchecked:hover{border-image:url(:/image/images/pd400_mute_btnHover);}"
                                 "QCheckBox::indicator:checked{border-image:url(:/image/images/pd400_mute_btnPress.png);}"
                                 "QCheckBox:focus{outline: none;}").arg((int)(40*m_Magnification)).arg((int)(80*m_Magnification));
    m_mute_btn->setStyleSheet(mute_bt_stys);
    connect(m_mute_btn,SIGNAL(stateChanged(int)),this,SLOT(mute_btn_click(int)));
    m_mute_arrs.push_back(m_mute_btn);

    QLabel *dianping_lab_bg = new QLabel(this);
    dianping_lab_bg->resize(348*m_Magnification,12*m_Magnification);
    dianping_lab_bg->move(75*m_Magnification,(73-6)*m_Magnification);
    dianping_lab_bg->setPixmap(QPixmap(":/image/images/pd400_dianping_fon.png"));
    dianping_lab_bg->setScaledContents(true);
    //dianping_lab_bg->setStyleSheet("background-color:black");

    m_dianping_lab_front = new QLabel(this);
    m_dianping_lab_front->resize(348*m_Magnification,12*m_Magnification);
    m_dianping_lab_front->move(75*m_Magnification,(73-6)*m_Magnification);
    m_dianping_lab_front->setStyleSheet("background-color:white");


    QLabel *dianping_lab = new QLabel(this);
    dianping_lab->resize(348*m_Magnification,12*m_Magnification);
    dianping_lab->move(75*m_Magnification,(73-6)*m_Magnification);
    dianping_lab->setPixmap(QPixmap(":/image/images/pd400_dianping_bg.png"));
    dianping_lab->setScaledContents(true);

    //耳机音量
    m_micvolume_label = new QLabel(this);
    m_micvolume_label->move(20*m_Magnification,106*m_Magnification);
    m_micvolume_label->resize(156*m_Magnification,24*m_Magnification);
    m_micvolume_label->setText(tr("耳机音量"));
    m_micvolume_label->setPalette(pa);
    m_micvolume_label->setFont(font);

    QLabel *micvolume_image = new QLabel(this);
    micvolume_image->move(20*m_Magnification,143*m_Magnification);
    micvolume_image->resize(20*m_Magnification,18*m_Magnification);
    micvolume_image->setPixmap(QPixmap(":/image/images/pd400_micvolume.png"));
    micvolume_image->setScaledContents(true);

    MySlider *mic_volue_slider = new MySlider(this);
    mic_volue_slider->setOrientation(Qt::Horizontal);
    mic_volue_slider->setGeometry((72-16)*m_Magnification,150*m_Magnification,300*m_Magnification,12*m_Magnification);
    mic_volue_slider->setMinimum(0);
    mic_volue_slider->setMaximum(100);

    connect(mic_volue_slider,SIGNAL(valueChanged(int)),this,SLOT(headsetVolume_eq_chanege_value(int)));
    connect(mic_volue_slider,SIGNAL(sliderReleasedAt(int)),this,SLOT(headsetVolume_release_value(int)));
    mic_volue_slider->setDisabled(true);
    m_synmicvolume_Slider_arrs.push_back(mic_volue_slider);

    m_mic_volumecurrent_label = new QLabel(this);
    m_mic_volumecurrent_label->move((395-16)*m_Magnification,140*m_Magnification);
    m_mic_volumecurrent_label->resize(36*m_Magnification,24*m_Magnification);
    m_mic_volumecurrent_label->setText("0");
    m_mic_volumecurrent_label->setPalette(pa);
    //font.setPixelSize(16);
    m_mic_volumecurrent_label->setFont(font);

    for (int i=0;i<2;i++) {
        QLabel*micvolume_bottomlabel = new QLabel(this);
        micvolume_bottomlabel->move(72*m_Magnification-16*m_Magnification+(310-36)*i*m_Magnification,170*m_Magnification);
        micvolume_bottomlabel->resize(36*m_Magnification,24*m_Magnification);
        if(i==0){
            micvolume_bottomlabel->setText("0");
        }else{
            micvolume_bottomlabel->setText("100");
        }

        micvolume_bottomlabel->setPalette(pa);
        //font.setPixelSize(12);
        micvolume_bottomlabel->setFont(font);
    }

    m_jianting_label = new QLabel(this);
    m_jianting_label->move(20*m_Magnification,(136+60)*m_Magnification);
    m_jianting_label->resize(116*m_Magnification,24*m_Magnification);
    m_jianting_label->setText(tr("监听混音比例"));
    m_jianting_label->setPalette(pa);
    m_jianting_label->setFont(font);

    QLabel *mic_image = new QLabel(this);
    mic_image->move(20*m_Magnification,(173+60)*m_Magnification);
    mic_image->resize(17*m_Magnification,26*m_Magnification);
    mic_image->setPixmap(QPixmap(":/image/images/pd400_mic.png"));
    mic_image->setScaledContents(true);

    QLabel *mic_mid_image = new QLabel(this);
    mic_mid_image->move((220-16)*m_Magnification,(176+60)*m_Magnification);
    mic_mid_image->resize(2*m_Magnification,20*m_Magnification);
    mic_mid_image->setPixmap(QPixmap(":/image/images/pd400_mic_slidermid.png"));
    mic_mid_image->setScaledContents(true);

    //混听比例
    m_mic_slider = new SliderWidget(this);
    m_mic_slider->SetValue(100);
    m_Magnification<1?(m_mic_slider->setGeometry((72-16)*m_Magnification,(180+60)*m_Magnification,300*m_Magnification,12)):
        (m_mic_slider->setGeometry((72-16)*m_Magnification,(180+60)*m_Magnification,300*m_Magnification,(int)(qRound(12*m_Magnification))));

    connect(m_mic_slider,SIGNAL(valueChanged(int)),this,SLOT(mic_chanege_value(int)));
    connect(m_mic_slider,SIGNAL(sliderReleasedAt(int)),this,SLOT(mix_release_value(int)));
    m_mic_slider->setDisabled(true);
    m_pd400SynSlider_arrs.push_back(m_mic_slider);

    QLabel *pc_image = new QLabel(this);
    pc_image->move((398-16)*m_Magnification,(176+60)*m_Magnification);
    pc_image->resize(32*m_Magnification,21*m_Magnification);
    pc_image->setPixmap(QPixmap(":/image/images/pd400_pc.png"));
    pc_image->setScaledContents(true);

    m_yinse_label = new QLabel(this);
    m_yinse_label->move(20*m_Magnification,(225+LeftADDMICY)*m_Magnification);
    m_yinse_label->resize(116*m_Magnification,24*m_Magnification);
    m_yinse_label->setText(tr("音色调节"));
    m_yinse_label->setPalette(pa);
    m_yinse_label->setFont(font);

    m_yinse_btn_img = new QLabel(this);
    m_yinse_btn_img->move(20*m_Magnification,(260+LeftADDMICY)*m_Magnification);
    m_yinse_btn_img->resize(270*m_Magnification,30*m_Magnification);
    m_yinse_btn_img->setPixmap(QPixmap(":/image/images/pd400_yinse.png"));
    m_yinse_btn_img->setScaledContents(true);

    m_dichen_btn = new custom_button(m_yinse_btn_img);
    m_dichen_btn->move(0,0);
    m_dichen_btn->resize(90*m_Magnification,30*m_Magnification);
    m_dichen_btn->setText(tr("低沉"));
    m_dichen_btn->setObjectName("dichen");
    QString dichen_btn_stys=QString("QPushButton{background-color:transparent;color:white;font-size:%1px;font-family:'Source Han Sans CN Medium'}"
                                    "QPushButton:hover{background-color:transparent;font-family:'Source Han Sans CN Medium';color:rgb(54,54,54);font-size:%1px;}"
                                    "QPushButton:focus{outline: none;}").arg(15*m_Magnification);
    m_dichen_btn->setStyleSheet(dichen_btn_stys);
    connect(m_dichen_btn,SIGNAL(clicked()),this,SLOT(change_yinse()));

    m_ziran_btn = new custom_button(m_yinse_btn_img);
    m_ziran_btn->move(90*m_Magnification,0);
    m_ziran_btn->resize(90*m_Magnification,30*m_Magnification);
    m_ziran_btn->setText(tr("自然"));
    m_ziran_btn->setObjectName("ziran");
    m_ziran_btn->setStyleSheet(dichen_btn_stys);
    connect(m_ziran_btn,SIGNAL(clicked()),this,SLOT(change_yinse()));

    m_mingliang_btn = new custom_button(m_yinse_btn_img);
    m_mingliang_btn->move(180*m_Magnification,0);
    m_mingliang_btn->resize(90*m_Magnification,30*m_Magnification);
    m_mingliang_btn->setText(tr("明亮"));
    m_mingliang_btn->setObjectName("mingliang");
    m_mingliang_btn->setStyleSheet(dichen_btn_stys);
    connect(m_mingliang_btn,SIGNAL(clicked()),this,SLOT(change_yinse()));

    m_jingdian_btn = new custom_button(this);
    m_jingdian_btn->move(330*m_Magnification,(260+LeftADDMICY)*m_Magnification);
    m_jingdian_btn->resize(100*m_Magnification,30*m_Magnification);
    m_jingdian_btn->setText(tr("经典"));
    m_jingdian_btn->setObjectName("jingdian");
    QString jingdian_btn_stys=QString("QPushButton{border-image:url(:/image/images/jingdian_btn.png);background-color:transparent;color:white;font-size:%1px;font-family:'Source Han Sans CN Medium'}"
                                      "QPushButton:hover{border-image:url(:/image/images/jingdian_btn.png);background-color:transparent;color:rgb(54,54,54);font-size:%1px;font-family:'Source Han Sans CN Medium'}"
                                      "QPushButton:focus{outline: none;}"
                                      ).arg(15*m_Magnification);
    m_jingdian_btn->setStyleSheet(jingdian_btn_stys);
    connect(m_jingdian_btn,SIGNAL(clicked()),this,SLOT(change_yinse()));

    m_mic_juli_label = new QLabel(this);
    m_mic_juli_label->move(20*m_Magnification,(306+LeftADDMICY)*m_Magnification);
    m_mic_juli_label->resize(168*m_Magnification,21*m_Magnification);
    m_mic_juli_label->setText(tr("麦克风使用距离"));
    m_mic_juli_label->setPalette(pa);
    m_mic_juli_label->setFont(font);

    m_mic_space_img = new QLabel(this);
    m_mic_space_img->move((this->width()-450*m_Magnification)/2,(338+30)*m_Magnification);
    m_mic_space_img->resize(450*m_Magnification,180*m_Magnification);
    m_mic_space_img->setPixmap(QPixmap(":/image/images/pd400_near.png"));
    m_mic_space_img->setScaledContents(true);

    m_space_number = new QLabel(this);
    m_space_number->move(140*m_Magnification,(306+LeftADDMICY)*m_Magnification);
    m_space_number->resize(105*m_Magnification,21*m_Magnification);
    m_space_number->setText("5CM-10CM");
    QString space_numbe_stys=QString("background-color:transparent;color:rgb(255,171,33);font-size:%1px;font-family:'Source Han Sans CN Medium'").arg(15*m_Magnification);
    m_space_number->setStyleSheet(space_numbe_stys);
    //m_space_number->hide();

    m_yuanzhongjin_bgimg = new QLabel(this);
    m_yuanzhongjin_bgimg->move(55*m_Magnification,556*m_Magnification);
    m_yuanzhongjin_bgimg->resize(360*m_Magnification,30*m_Magnification);
    m_yuanzhongjin_bgimg->setPixmap(QPixmap(":/image/images/pd400_yuanjinzhong_backimg.png"));
    m_yuanzhongjin_bgimg->setScaledContents(true);

    QVector<QString>yuanzhongjin_arrs = {tr("近"),tr("中"),tr("远")};
    for (int i=0;i<yuanzhongjin_arrs.count();i++) {

        custom_button *yuanzhongjin_btn = new custom_button(m_yuanzhongjin_bgimg);
        yuanzhongjin_btn->move(120*i*m_Magnification,0);
        yuanzhongjin_btn->resize(120*m_Magnification,30*m_Magnification);
        yuanzhongjin_btn->setText(yuanzhongjin_arrs[i]);
        yuanzhongjin_btn->setObjectName(QString::number(i+100,10));
        QString yuanzhongjin_btn_stys=QString("QPushButton{background-color:transparent;color:white;font-size:%1px;font-family:'Source Han Sans CN Medium'}"
                                         "QPushButton:hover{background-color:transparent;color:rgb(54,54,54);font-size:%1px;font-family:'Source Han Sans CN Medium'}"
                                         "QPushButton:focus{outline: none;}").arg(15*m_Magnification);
        yuanzhongjin_btn->setStyleSheet(yuanzhongjin_btn_stys);
        connect(yuanzhongjin_btn,SIGNAL(clicked()),this,SLOT(change_yuanzhongjin()));
        yuanzhongjin_btns.push_back(yuanzhongjin_btn);
    }
    m_mengban_image = new QLabel(this);
    m_mengban_image->move(0,0);
    m_mengban_image->resize(this->width(),this->height());
    m_mengban_image->setPixmap(QPixmap(":/image/images/pd400_mengban.png"));
    m_mengban_image->setScaledContents(true);
    m_mengban_image->show();

}

void pd400_leftview::handleTimeout(){

    if(m_slider_current_number==1){
        m_commin_fun.pd400_headset_volume(m_slider_current_value);
        PD400_send_value.insert(QString::number(PARAM_HP_GAIN & 0xff,16),m_slider_current_value);
    }else if(m_slider_current_number==2){
        m_commin_fun.pd400_monitor_mix(m_slider_current_value);
        PD400_send_value.insert(QString::number(PARAM_LR_MONITOR & 0xff,16),m_slider_current_value);
    }
    m_DM30Timer->stop();
}
//静音事件
void pd400_leftview::mute_btn_click(int status){

    QCheckBox *right_mute_btn = m_mute_arrs.at(1);
    if(status==0){
        right_mute_btn->setChecked(false);
    }else{
        setHideDianping();
        right_mute_btn->setChecked(true);
    }
    m_commin_fun.pd400_mute(status);
    qDebug()<<"xxsssss"<<status;

}
void pd400_leftview::headsetVolume_eq_chanege_value(int slidervalue){

    m_mic_volumecurrent_label->setText(QString::number(slidervalue,10));
    //同步右边耳机音量
    MySlider *slider = m_synmicvolume_Slider_arrs.at(1);
    //m_mic_slider->blockSignals(true);
    slider->setValue(slidervalue);
    //m_mic_slider->blockSignals(false);

    m_slider_current_value = slidervalue;
    m_slider_current_number = 1;
    //发送信息
    m_commin_fun.pd400_headset_volume(slidervalue);
    //pd400_send_value = slidervalue;
    PD400_send_value.insert(QString::number(PARAM_HP_GAIN & 0xff,16),slidervalue);
    m_DM30Timer->stop();
    if(m_pd400current_model==1)
        m_DM30Timer->start(40);
    m_PD400phsilder_runing = true;
}
void pd400_leftview::headsetVolume_release_value(int slidervalue){
    MySlider *micVolume_slider = m_synmicvolume_Slider_arrs.at(0);
    micVolume_slider->blockSignals(true);
    QMap<QString,int>::iterator send_value_iter = PD400_send_value.find(QString::number((PARAM_HP_GAIN & 0xff),16));
    micVolume_slider->setValue(send_value_iter.value());
    m_synmicvolume_Slider_arrs.at(1)->setValue(send_value_iter.value());
    m_mic_volumecurrent_label->setText(QString::number(send_value_iter.value(),10));
    micVolume_slider->blockSignals(false);
    m_PD400phsilder_runing = false;
    //    QElapsedTimer time;
    //    time.start();
    //    while(time.elapsed() < 40)  {//等待200MS接收数据
    //        m_commin_fun.pd400_headset_volume(slidervalue);
    //        pd400_send_value = slidervalue;
    //    }
}

void pd400_leftview::mic_chanege_value(int slidervalue){


    m_slider_current_value = slidervalue;
    m_slider_current_number = 2;
    //发送数据
    m_commin_fun.pd400_monitor_mix(slidervalue);
    //pd400_send_value = slidervalue;
    PD400_send_value.insert(QString::number(PARAM_LR_MONITOR & 0xff,16),slidervalue);
    //同步右边monitor mix
    SliderWidget *slider = m_pd400SynSlider_arrs.at(1);
    //m_mic_slider->blockSignals(true);
    slider->SetValue(slidervalue);
    //m_mic_slider->blockSignals(false);
    m_DM30Timer->stop();
    if(m_current_model==1)
        m_DM30Timer->start(40);
    m_PD400mixsilder_runing = true;

}
void pd400_leftview::mix_release_value(int slidervalue){

    for (int i =0;i<m_pd400SynSlider_arrs.count();i++) {
        SliderWidget *micVolume_slider = m_pd400SynSlider_arrs.at(i);
        micVolume_slider->blockSignals(true);
        QMap<QString,int>::iterator send_value_iter = PD400_send_value.find(QString::number((PARAM_LR_MONITOR & 0xff),16));
        micVolume_slider->SetValue(send_value_iter.value());
        micVolume_slider->blockSignals(false);
    }

    m_PD400mixsilder_runing = false;
    //    QElapsedTimer time;
    //    time.start();
    //    while(time.elapsed() < 40)  {
    //        m_commin_fun.pd400_monitor_mix(slidervalue);
    //        pd400_send_value = slidervalue;
    //    }
}
//远中近的点击事件
void pd400_leftview::change_yuanzhongjin(){

    QPushButton* btn = dynamic_cast<QPushButton*>(sender());
    m_dis_current_number = btn->objectName();
    if(btn->objectName()=="100"){
        m_yuanzhongjin_bgimg->setPixmap(QPixmap(":/image/images/pd400_jin_img.png"));
        m_commin_fun.pd400_yuanzhongjin(0);
        m_mic_space_img->setPixmap(QPixmap(":/image/images/pd400_near.png"));
        m_space_number->setText("5CM-10CM");
    }else if (btn->objectName()=="101") {
        m_yuanzhongjin_bgimg->setPixmap(QPixmap(":/image/images/pd400_zhong_img.png"));
        m_commin_fun.pd400_yuanzhongjin(1);
        m_mic_space_img->setPixmap(QPixmap(":/image/images/pd400_mid.png"));
        m_space_number->setText("11CM-30CM");
    }else{
        m_yuanzhongjin_bgimg->setPixmap(QPixmap(":/image/images/pd400_yuan_img.png"));
        m_commin_fun.pd400_yuanzhongjin(2);
        m_mic_space_img->setPixmap(QPixmap(":/image/images/pd400_far.png"));
        m_space_number->setText("31CM-60CM");
    }
}
void pd400_leftview::change_yinse(){


    QString jingdian_btn_stys=QString("QPushButton{border-image:url(:/image/images/jingdian_btn.png);background-color:transparent;color:white;font-size:%1px;font-family:'Source Han Sans CN Medium'}"
                                      "QPushButton:hover{border-image:url(:/image/images/jingdian_btn.png);background-color:transparent;color:rgb(54,54,54);font-size:%1px;font-family:'Source Han Sans CN Medium'}"
                                      "QPushButton:focus{outline: none;}"
                                      ).arg(15*m_Magnification);
    m_jingdian_btn->setStyleSheet(jingdian_btn_stys);
    QPushButton* btn = dynamic_cast<QPushButton*>(sender());
    m_tone_current_number = btn->objectName();
    if(btn->objectName()=="dichen"){
        m_yinse_btn_img->setPixmap(QPixmap(":/image/images/dichen_btn.png"));
        m_commin_fun.pd400_yinse(0);
    }else if(btn->objectName()=="ziran"){
        m_yinse_btn_img->setPixmap(QPixmap(":/image/images/ziran_btn.png"));
        m_commin_fun.pd400_yinse(1);
    }else if(btn->objectName()=="mingliang"){
        m_yinse_btn_img->setPixmap(QPixmap(":/image/images/mingliang_btn.png"));
        m_commin_fun.pd400_yinse(2);
    }else{
        m_yinse_btn_img->setPixmap(QPixmap(":/image/images/pd400_yinse.png"));
        QString jingdian_btn_stys=QString("QPushButton{border-image:url(:/image/images/jingdian_btn_high.png);background-color:transparent;color:white;font-size:%1px;font-family:'Source Han Sans CN Medium'}"
                                          "QPushButton:hover{border-image:url(:/image/images/jingdian_btn_high.png);background-color:transparent;color:rgb(54,54,54);font-size:%1px;font-family:'Source Han Sans CN Medium'}"
                                          "QPushButton:focus{outline: none;}").arg(15*m_Magnification);
        btn->setStyleSheet(jingdian_btn_stys);
        m_commin_fun.pd400_yinse(3);
    }

}
void pd400_leftview::setHideDianping(){

    m_dianping_lab_front->resize(348*m_Magnification,12*m_Magnification);
    m_dianping_lab_front->move(75*m_Magnification,(73-6)*m_Magnification);
    m_dianping_lab_front->show();
}
//刷新各种UI
void pd400_leftview::refresh_ui(uint8_t mute,uint16_t mix,uint8_t yinse,uint8_t micjuli,uint8_t micVolume){

    if(mute==0){//非静音
        m_mute_btn->blockSignals(true);
        m_mute_btn->setChecked(false);
        m_mute_btn->blockSignals(false);
    }else{
        m_mute_btn->blockSignals(true);
        m_mute_btn->setChecked(true);
        m_mute_btn->blockSignals(false);
    }
    int mix_slider_value = (mix-2000) /10 +100;
    m_mic_slider->blockSignals(true);
    m_mic_slider->SetValue(mix_slider_value);

    m_mic_slider->blockSignals(false);
    //设置耳机音量的SLIDER
    MySlider *micVolume_slider = m_synmicvolume_Slider_arrs.at(0);
    micVolume_slider->blockSignals(true);
    micVolume_slider->setValue(micVolume);
    micVolume_slider->blockSignals(false);
    m_mic_volumecurrent_label->setText(QString::number(micVolume,10));

    //音色的UI
    QString jingdian_btn_stys=QString("QPushButton{border-image:url(:/image/images/jingdian_btn.png);background-color:transparent;color:white;font-size:%1px;font-family:'Source Han Sans CN Medium'}"
                                      "QPushButton:hover{border-image:url(:/image/images/jingdian_btn.png);background-color:transparent;color:rgb(54,54,54);font-size:%1px;font-family:'Source Han Sans CN Medium'}"
                                      "QPushButton:focus{outline: none;}").arg(15*m_Magnification);
    if(yinse==0){
        m_jingdian_btn->setStyleSheet(jingdian_btn_stys);
        m_yinse_btn_img->setPixmap(QPixmap(":/image/images/dichen_btn.png"));

    }else if(yinse==1){
        m_jingdian_btn->setStyleSheet(jingdian_btn_stys);
        m_yinse_btn_img->setPixmap(QPixmap(":/image/images/ziran_btn.png"));
    }else if(yinse==2){
        m_jingdian_btn->setStyleSheet(jingdian_btn_stys);
        m_yinse_btn_img->setPixmap(QPixmap(":/image/images/mingliang_btn.png"));
    }else{
        m_yinse_btn_img->setPixmap(QPixmap(":/image/images/pd400_yinse.png"));
        QString jingdian_btn_stys=QString("QPushButton{border-image:url(:/image/images/jingdian_btn_high.png);background-color:transparent;color:white;font-size:%1px;font-family:'Source Han Sans CN Medium'}"
                                          "QPushButton:hover{border-image:url(:/image/images/jingdian_btn_high.png);background-color:transparent;color:rgb(54,54,54);font-size:%1px;font-family:'Source Han Sans CN Medium'}"
                                          "QPushButton:focus{outline: none;}"
                                          ).arg(15*m_Magnification);
        m_jingdian_btn->setStyleSheet(jingdian_btn_stys);
    }
    QVector<QString> yinse_btn_titles={"dichen","ziran","mingliang","jingdian"};
    m_tone_current_number = yinse_btn_titles.at(yinse);

    if(micjuli==0){
        m_yuanzhongjin_bgimg->setPixmap(QPixmap(":/image/images/pd400_jin_img.png"));
        m_mic_space_img->setPixmap(QPixmap(":/image/images/pd400_near.png"));
        m_space_number->setText("5CM-10CM");
    }else if(micjuli==1){
        m_yuanzhongjin_bgimg->setPixmap(QPixmap(":/image/images/pd400_zhong_img.png"));
        m_mic_space_img->setPixmap(QPixmap(":/image/images/pd400_mid.png"));
        m_space_number->setText("11CM-30CM");
    }else{
        m_yuanzhongjin_bgimg->setPixmap(QPixmap(":/image/images/pd400_yuan_img.png"));
        m_mic_space_img->setPixmap(QPixmap(":/image/images/pd400_far.png"));
        m_space_number->setText("31CM-60CM");
    }
    m_dis_current_number = QString::number(micjuli+100,10);

    qDebug()<<"刷新数据------"<<mute<<mix<<yinse<<micjuli;
}

void pd400_leftview::sys_receive_data(uint8_t address,int value){

    //    if(address !=(PARAM_MIC_SIG_LEVEL & 0xff)){
    //       m_DM30Timer->stop();
    //    }
    if(address == (PARAM_MIC_MUTE & 0xff)){
        if(value==0){//非静音
            m_mute_btn->blockSignals(true);
            m_mute_btn->setChecked(false);
            m_mute_btn->blockSignals(false);
        }else{
            m_mute_btn->blockSignals(true);
            m_mute_btn->setChecked(true);
            m_mute_btn->blockSignals(false);
        }
    }else if(address ==(PARAM_MIC_SIG_LEVEL & 0xff)){//电平

        //如果是静音，没效果
        QCheckBox *right_mute_btn = m_mute_arrs.at(1);
        if(right_mute_btn->checkState() !=0){
            setHideDianping();
            return;
        }
        m_dianping_lab_front->show();
        int max_value = 40;

        int myvalue = ((value-2000)/10 + max_value);
        if(myvalue>=max_value){
            myvalue=max_value;
            m_dianping_lab_front->hide();
        }
        //qDebug()<<"打印数据22------"<<level_data<<(level_data-2000)/10;
        m_dianping_lab_front->move(75*m_Magnification + (349*m_Magnification/max_value *myvalue),73*m_Magnification-6*m_Magnification);
        m_dianping_lab_front->resize(349*m_Magnification-349*m_Magnification/max_value *myvalue,12*m_Magnification);

        if((m_dianping_lab_front->x()<=75*m_Magnification)){
            m_dianping_lab_front->move(75*m_Magnification,73*m_Magnification-6*m_Magnification);
            m_dianping_lab_front->resize(349*m_Magnification ,12*m_Magnification);
        }

        //        if(348/100.0 *myvalue < 348 *18/24.0){//只显示橙色
        //            m_dianping_lab_front->resize(348/100.0 *myvalue,12);
        //            //m_dianping_lab_front->resize(348/100.0 *myvalue,12);
        //            m_dianping_lab_gree_front->hide();
        //            m_dianping_lab_red_front->hide();

        //        }else if(348/100.0 *myvalue < (348-348*2/24.0)){//显示橙色跟绿色

        //            m_dianping_lab_front->resize(348-348/24.0 *5,12);
        //            m_dianping_lab_gree_front->show();
        //            int test = 348/100.0 *(100-myvalue);
        //            m_dianping_lab_gree_front->resize(test ,12);

        //            //qDebug()<<"打印数据22------"<<myvalue<<test;

        //            m_dianping_lab_red_front->hide();
        //        }else if(348/100.0 *myvalue >= (348-348*2/24.0)){//整条显示

        //            m_dianping_lab_front->resize(348-348/24.0 *5,12);
        //            m_dianping_lab_gree_front->show();
        //            m_dianping_lab_gree_front->resize(348 *4/24,12);

        //            m_dianping_lab_red_front->show();
        //        }
    }else if(address == (PARAM_LR_MONITOR & 0xff)){//监听比例
        int mix_slider_value = (value-2000) /10 +100;
        //qDebug()<<"打印数据3333------"<<mix_slider_value;
        m_mic_slider->blockSignals(true);
        if(!m_PD400mixsilder_runing){
            m_mic_slider->SetValue(mix_slider_value);
        }
        m_mic_slider->blockSignals(false);
        PD400_send_value.insert(QString::number(PARAM_LR_MONITOR & 0xff,16),mix_slider_value);
        PD400_different_value.insert(QString::number(PARAM_LR_MONITOR & 0xff,16),mix_slider_value);
    }else if(address == (PARAM_HP_GAIN & 0xff)){//耳机音量
        MySlider *micVolume_slider = m_synmicvolume_Slider_arrs.at(0);
        micVolume_slider->blockSignals(true);
        if(!m_PD400phsilder_runing){
            micVolume_slider->setValue(value);
            m_mic_volumecurrent_label->setText(QString::number(value,10));
        }

        micVolume_slider->blockSignals(false);
        PD400_send_value.insert(QString::number(PARAM_HP_GAIN & 0xff,16),value);
        PD400_different_value.insert(QString::number(PARAM_HP_GAIN & 0xff,16),value);
    }
}

void pd400_leftview::change_language(){

    m_micmute_label->setText(tr("麦克风静音"));
    m_jianting_label->setText(tr("监听混音比例"));
    m_yinse_label->setText(tr("音色调节"));
    m_dichen_btn->setText(tr("低沉"));
    m_ziran_btn->setText(tr("自然"));
    m_mingliang_btn->setText(tr("明亮"));
    m_jingdian_btn->setText(tr("经典"));
    m_mic_juli_label->setText(tr("麦克风使用距离"));
    m_micvolume_label->setText(tr("耳机音量"));

    QVector<QString>yuanzhongjin_arrs = {tr("近"),tr("中"),tr("远")};
    for (int i=0;i<3;i++) {
        QPushButton *btn = yuanzhongjin_btns.at(i);
        btn->setText(yuanzhongjin_arrs[i]);
    }

}
