
#pragma once
#pragma execution_character_set("utf-8")
#include "pm481_leftview.h"
#include <QButtonGroup>
#include "pm481_common.h"
#include <QDebug>
#include <QElapsedTimer>
#include <QTimer>
#include <QThread>

#include "../header_class.h"

pm481_leftview::pm481_leftview(QWidget *parent) : QWidget(parent)
{
    //    m_DM30Timer = new QTimer(this);
    //    connect(m_DM30Timer, SIGNAL(timeout()), this, SLOT(handleTimeout()));
    // m_DM30Timer->start(1000);

}
void pm481_leftview::createUI(int device_index){

    QVector<QString>switch_btns ={tr("关"),tr("开")};

    QLabel *back_left_image = new QLabel(this);
    back_left_image->move(0,0);
    back_left_image->resize(this->width(),380*m_Magnification);
    back_left_image->setPixmap(QPixmap(":/image/images/pm481/leftview_bgimg_top.png"));
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
    dianping_lab_bg->move(75*m_Magnification,73*m_Magnification-6*m_Magnification);
    dianping_lab_bg->setPixmap(QPixmap(":/image/images/pd400_dianping_fon.png"));
    dianping_lab_bg->setScaledContents(true);
    //dianping_lab_bg->setStyleSheet("background-color:black");

    m_dianping_lab_front = new QLabel(this);
    m_dianping_lab_front->resize(348*m_Magnification,12*m_Magnification);
    m_dianping_lab_front->move(75*m_Magnification,73*m_Magnification-6*m_Magnification);
    m_dianping_lab_front->setStyleSheet("background-color:white");

    QLabel *dianping_lab = new QLabel(this);
    dianping_lab->resize(348*m_Magnification,12*m_Magnification);
    dianping_lab->move(75*m_Magnification,73*m_Magnification-6*m_Magnification);
    dianping_lab->setPixmap(QPixmap(":/image/images/pd400_dianping_bg.png"));
    dianping_lab->setScaledContents(true);

    //麦克风增益
    m_jianting_label = new QLabel(this);
    m_jianting_label->move(20*m_Magnification,106*m_Magnification);
    m_jianting_label->resize(116*m_Magnification,24*m_Magnification);
    m_jianting_label->setText(tr("麦克风增益"));
    m_jianting_label->setPalette(pa);
    m_jianting_label->setFont(font);

    QLabel *mic_image = new QLabel(this);
    mic_image->move(20*m_Magnification,143*m_Magnification);
    mic_image->resize(17*m_Magnification,26*m_Magnification);
    mic_image->setPixmap(QPixmap(":/image/images/pd400_mic.png"));
    mic_image->setScaledContents(true);

    MySlider *mic_gain_slider = new MySlider(this);
    mic_gain_slider->setOrientation(Qt::Horizontal);
    mic_gain_slider->setGeometry(72*m_Magnification-16*m_Magnification,150*m_Magnification,300*m_Magnification,12*m_Magnification);
    mic_gain_slider->setMinimum(0);
    mic_gain_slider->setMaximum(100);
    //    QString mic_gain_slider_stys=QString("QSlider::groove:horizontal{background: rgba(0, 0, 0,100);height:%2px;border: 0px solid #FFFFFF;border-image:url(:/image/images/pd400_right_slider_bgimg.png);}"
    //                                         "QSlider::handle:horizontal{width:%1px;height:%1px;margin-top: -%2px;margin-left: 0px;margin-bottom: -%2px;margin-right: 0px;border-image:url(:/image/images/pd400_round.png);}"
    //                                         "QSlider::sub-page:horizontal{background-color:rgba(0,0,0,100);border-radius: %2px;border-image:url(:/image/images/pd400_right_addslider_bgimg.png);}").arg(12*m_Magnification).arg(4*m_Magnification);
    //    mic_gain_slider->setStyleSheet(mic_gain_slider_stys);
    connect(mic_gain_slider,SIGNAL(valueChanged(int)),this,SLOT(micgain_eq_change_value(int)));
    connect(mic_gain_slider,SIGNAL(sliderReleasedAt(int)),this,SLOT(micgain_release_value(int)));
    mic_gain_slider->setDisabled(true);
    m_synSlider_arrs.push_back(mic_gain_slider);

    m_mic_gaincurrent_label = new QLabel(this);
    m_mic_gaincurrent_label->move(395*m_Magnification-16*m_Magnification,145*m_Magnification);
    m_mic_gaincurrent_label->resize(36*m_Magnification,24*m_Magnification);
    m_mic_gaincurrent_label->setText("0");
    m_mic_gaincurrent_label->setPalette(pa);
    m_mic_gaincurrent_label->setFont(font);
    for (int i=0;i<2;i++) {
        QLabel*micvolume_bottomlabel = new QLabel(this);
        micvolume_bottomlabel->move(72*m_Magnification-16*m_Magnification+(310*m_Magnification-36*m_Magnification)*i,170*m_Magnification);
        micvolume_bottomlabel->resize(36*m_Magnification,24*m_Magnification);
        if(i==0){
            micvolume_bottomlabel->setText("0");
        }else{
            micvolume_bottomlabel->setText("100");
        }

        micvolume_bottomlabel->setPalette(pa);
        micvolume_bottomlabel->setFont(font);
    }
    //耳机音量
    m_micvolume_label = new QLabel(this);
    m_micvolume_label->move(20*m_Magnification,(136+70)*m_Magnification);
    m_micvolume_label->resize(156*m_Magnification,24*m_Magnification);
    m_micvolume_label->setText(tr("耳机音量"));
    m_micvolume_label->setPalette(pa);
    m_micvolume_label->setFont(font);

    //监听比例
    QLabel *mix_label = new QLabel(this);
    mix_label->move(160*m_Magnification,136*m_Magnification+70*m_Magnification);
    mix_label->resize(116*m_Magnification,24*m_Magnification);
    mix_label->setText(tr("麦克风监听"));
    mix_label->setPalette(pa);
    mix_label->setFont(font);
    mix_label->setAlignment(Qt::AlignRight);
    //mix_label->setStyleSheet("background:red");

    m_mix_btn_bgimage = new QLabel(this);
    m_mix_btn_bgimage->move(mix_label->x()+136*m_Magnification,136*m_Magnification+70*m_Magnification);
    m_mix_btn_bgimage->resize(120*m_Magnification,20*m_Magnification);
    m_mix_btn_bgimage->setPixmap(QPixmap(":/image/images/pm481/pm481_open.png"));
    m_mix_btn_bgimage->setScaledContents(true);

    m_micMon_arrs.push_back(m_mix_btn_bgimage);
    for (int i=0;i<2;i++) {
        QPushButton *btn = new QPushButton(m_mix_btn_bgimage);
        btn->move(i*60*m_Magnification ,0);
        btn->resize(60*m_Magnification,20*m_Magnification);
        btn->setText(switch_btns.at(i));
        QString btn_stys=QString("QPushButton{background-color:transparent;font-family:'Source Han Sans CN Medium';color:white;font-size:%1px;}"
                                 "QPushButton:hover{font-family:'Source Han Sans CN Medium';color:rgb(54,54,54);font-size:%1px;}"
                                 "QPushButton:focus{outline: none;}").arg(15*m_Magnification);
        btn->setStyleSheet(btn_stys);
        btn->setObjectName(QString::number(400+i,10));
        connect(btn,SIGNAL(clicked()),this,SLOT(mix_switch_click()));
        //btn->setDisabled(true);
        //m_switch_btns.push_back(btn);
    }

    QLabel *micvolume_image = new QLabel(this);
    micvolume_image->move(20*m_Magnification,173*m_Magnification+70*m_Magnification);
    micvolume_image->resize(20*m_Magnification,18*m_Magnification);
    micvolume_image->setPixmap(QPixmap(":/image/images/pd400_micvolume.png"));
    micvolume_image->setScaledContents(true);

    MySlider *mic_volue_slider = new MySlider(this);
    mic_volue_slider->setOrientation(Qt::Horizontal);
    mic_volue_slider->setGeometry(72*m_Magnification-16*m_Magnification,250*m_Magnification,300*m_Magnification,12*m_Magnification);
    mic_volue_slider->setMinimum(0);
    mic_volue_slider->setMaximum(100);
    //    mic_volue_slider->setStyleSheet(mic_gain_slider_stys);
    connect(mic_volue_slider,SIGNAL(valueChanged(int)),this,SLOT(headsetVolume_eq_chanege_value(int)));
    connect(mic_volue_slider,SIGNAL(sliderReleasedAt(int)),this,SLOT(headsetVolume_release_value(int)));

    mic_volue_slider->setDisabled(true);
    m_synmicvolume_Slider_arrs.push_back(mic_volue_slider);

    m_mic_volumecurrent_label = new QLabel(this);
    m_mic_volumecurrent_label->move(395*m_Magnification-16*m_Magnification,150*m_Magnification+95*m_Magnification);
    m_mic_volumecurrent_label->resize(36*m_Magnification,24*m_Magnification);
    m_mic_volumecurrent_label->setText("0");
    m_mic_volumecurrent_label->setPalette(pa);
    m_mic_volumecurrent_label->setFont(font);

    for (int i=0;i<2;i++) {
        QLabel*micvolume_bottomlabel = new QLabel(this);
        micvolume_bottomlabel->move(72*m_Magnification-16*m_Magnification+(310-36)*i*m_Magnification,170*m_Magnification+100*m_Magnification);
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

    m_yinse_label = new QLabel(this);
    m_yinse_label->move(20*m_Magnification,300*m_Magnification);
    m_yinse_label->resize(116*m_Magnification,24*m_Magnification);
    m_yinse_label->setText(tr("音色调节"));
    m_yinse_label->setPalette(pa);
    m_yinse_label->setFont(font);

    m_yinse_btn_img = new QLabel(this);
    m_yinse_btn_img->move(20*m_Magnification,335*m_Magnification);
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
    m_jingdian_btn->move(330*m_Magnification,335*m_Magnification);
    m_jingdian_btn->resize(100*m_Magnification,30*m_Magnification);
    m_jingdian_btn->setText(tr("经典"));
    m_jingdian_btn->setObjectName("jingdian");
    m_jingdian_btn->setStyleSheet(dichen_btn_stys);
    connect(m_jingdian_btn,SIGNAL(clicked()),this,SLOT(change_yinse()));
    //上面部分的蒙版
    m_basic_mask = new QLabel(this);
    m_basic_mask->move(0,0);
    m_basic_mask->resize(this->width(),380*m_Magnification);
    m_basic_mask->setPixmap(QPixmap(":/image/images/pm481/leftViewTop.png"));
    m_basic_mask->show();
    m_basic_mask->setScaledContents(true);


    //下面部分
    QLabel *back_light_image = new QLabel(this);
    back_light_image->move(0,389*m_Magnification);
    back_light_image->resize(this->width(),215*m_Magnification);
    back_light_image->setPixmap(QPixmap(":/image/images/pm481/leftview_bgimg_light.png"));
    back_light_image->setScaledContents(true);

    QLabel *m_rgb_label = new QLabel(back_light_image);
    m_rgb_label->move(20*m_Magnification,10*m_Magnification);
    m_rgb_label->resize(116*m_Magnification,24*m_Magnification);
    m_rgb_label->setText(tr("RGB灯光"));
    m_rgb_label->setPalette(pa);
    m_rgb_label->setFont(font);

    m_RGB_btn_bgimage = new QLabel(back_light_image);
    m_RGB_btn_bgimage->move(100*m_Magnification,15*m_Magnification);
    m_RGB_btn_bgimage->resize(120*m_Magnification,20*m_Magnification);
    m_RGB_btn_bgimage->setPixmap(QPixmap(":/image/images/pm481/pm481_open.png"));
    m_RGB_btn_bgimage->setScaledContents(true);

    //灯光的开关按钮
    for (int i=0;i<2;i++) {
        QPushButton *btn = new QPushButton(m_RGB_btn_bgimage);
        btn->move(i*60*m_Magnification,0);
        btn->resize(60*m_Magnification,20*m_Magnification);
        btn->setText(switch_btns.at(i));
        QString btn_stys=QString("QPushButton{background-color:transparent;font-family:'Source Han Sans CN Medium';color:white;font-size:%1px;}"
                                 "QPushButton:hover{font-family:'Source Han Sans CN Medium';color:rgb(54,54,54);font-size:%1px;}"
                                 "QPushButton:focus{outline: none;}").arg(15*m_Magnification);
        btn->setStyleSheet(btn_stys);
        btn->setObjectName(QString::number(200+i,10));
        connect(btn,SIGNAL(clicked()),this,SLOT(light_switch_click()));
        btn->setDisabled(true);
        m_switch_btns.push_back(btn);
    }

    QLabel *m_rgb_adjust_label = new QLabel(back_light_image);
    m_rgb_adjust_label->move(20*m_Magnification,34*m_Magnification+15*m_Magnification);
    m_rgb_adjust_label->resize(170*m_Magnification,24*m_Magnification);
    m_rgb_adjust_label->setText(tr("亮度调节"));
    m_rgb_adjust_label->setPalette(pa);
    m_rgb_adjust_label->setFont(font);

    QLabel *ligh_image = new QLabel(back_light_image);
    ligh_image->move(20*m_Magnification,83*m_Magnification);
    ligh_image->resize(22*m_Magnification,22*m_Magnification);
    ligh_image->setPixmap(QPixmap(":/image/images/pm481/pm481_light_bgimg.png"));
    ligh_image->setScaledContents(true);


    m_light_slider = new MySlider(back_light_image);
    m_light_slider->setOrientation(Qt::Horizontal);
    m_light_slider->setGeometry(72*m_Magnification-16*m_Magnification,88*m_Magnification,300*m_Magnification,12*m_Magnification);
    m_light_slider->setMinimum(0);
    m_light_slider->setMaximum(100);
    //    m_light_slider->setStyleSheet(mic_gain_slider_stys);
    connect(m_light_slider,SIGNAL(valueChanged(int)),this,SLOT(light_change_value(int)));
    connect(m_light_slider,SIGNAL(sliderReleasedAt(int)),this,SLOT(light_release_value(int)));

    m_mic_lightcurrent_label = new QLabel(back_light_image);
    m_mic_lightcurrent_label->move(395*m_Magnification-16*m_Magnification,81*m_Magnification);
    m_mic_lightcurrent_label->resize(36*m_Magnification,24*m_Magnification);
    m_mic_lightcurrent_label->setText("0");
    m_mic_lightcurrent_label->setPalette(pa);
    m_mic_lightcurrent_label->setFont(font);

    QLabel *m_rgb_adjust_label2 = new QLabel(back_light_image);
    m_rgb_adjust_label2->move(20*m_Magnification,120*m_Magnification);
    m_rgb_adjust_label2->resize(116*m_Magnification,24*m_Magnification);
    m_rgb_adjust_label2->setText(tr("颜色选择"));
    m_rgb_adjust_label2->setPalette(pa);
    m_rgb_adjust_label2->setFont(font);

    //彩色圆
    QButtonGroup *m_buttonGroup = new QButtonGroup();
    m_buttonGroup->setExclusive(true);

    QVector<QString> rgb_light_btn_names;
    if(device_index==MaonoDM30RGB){

        if(m_Magnification<1){
            rgb_light_btn_names = {
                                                     m_co_fun.add_checkBox_forstatus(QString("url(:/image/images/pm481/pm481_rgb_%1.png)").arg(1),24*1),
                                                     m_co_fun.add_checkBox_forstatus(QString("url(:/image/images/pm481/pm481_red_%1.png)").arg(1),24*1),
                                                     m_co_fun.add_checkBox_forstatus(QString("url(:/image/images/pm481/pm481_orange_%1.png)").arg(1),24*1),
                                                     m_co_fun.add_checkBox_forstatus(QString("url(:/image/images/pm481/pm481_yellow_%1.png)").arg(1),24*1),
                                                     m_co_fun.add_checkBox_forstatus(QString("url(:/image/images/pm481/pm481_green_%1.png)").arg(1),24*1),
                                                     m_co_fun.add_checkBox_forstatus(QString("url(:/image/images/pm481/pm481_iceblue_%1.png)").arg(1),24*1),
                                                     m_co_fun.add_checkBox_forstatus(QString("url(:/image/images/pm481/pm481_darkblue_%1.png)").arg(1),24*1),
                                                     m_co_fun.add_checkBox_forstatus(QString("url(:/image/images/pm481/pm481_pruple_%1.png)").arg(1),24*1),
                                                     };
        }else{
            rgb_light_btn_names = {
                                                     m_co_fun.add_checkBox_forstatus(QString("url(:/image/images/pm481/pm481_rgb_%1.png)").arg(m_Magnification),24*m_Magnification),
                                                     m_co_fun.add_checkBox_forstatus(QString("url(:/image/images/pm481/pm481_red_%1.png)").arg(m_Magnification),24*m_Magnification),
                                                     m_co_fun.add_checkBox_forstatus(QString("url(:/image/images/pm481/pm481_orange_%1.png)").arg(m_Magnification),24*m_Magnification),
                                                     m_co_fun.add_checkBox_forstatus(QString("url(:/image/images/pm481/pm481_yellow_%1.png)").arg(m_Magnification),24*m_Magnification),
                                                     m_co_fun.add_checkBox_forstatus(QString("url(:/image/images/pm481/pm481_green_%1.png)").arg(m_Magnification),24*m_Magnification),
                                                     m_co_fun.add_checkBox_forstatus(QString("url(:/image/images/pm481/pm481_iceblue_%1.png)").arg(m_Magnification),24*m_Magnification),
                                                     m_co_fun.add_checkBox_forstatus(QString("url(:/image/images/pm481/pm481_darkblue_%1.png)").arg(m_Magnification),24*m_Magnification),
                                                     m_co_fun.add_checkBox_forstatus(QString("url(:/image/images/pm481/pm481_pruple_%1.png)").arg(m_Magnification),24*m_Magnification),
                                                     };

        }

    }else if(device_index==MaonoDM30){
        if(m_Magnification<1){
            rgb_light_btn_names = {                  m_co_fun.add_checkBox_forstatus(QString("url(:/image/images/pm481/pm481_red_%1.png)").arg(1),24*1),
                                                     m_co_fun.add_checkBox_forstatus(QString("url(:/image/images/pm481/pm481_orange_%1.png)").arg(1),24*1),
                                                     m_co_fun.add_checkBox_forstatus(QString("url(:/image/images/pm481/pm481_yellow_%1.png)").arg(1),24*1),
                                                     m_co_fun.add_checkBox_forstatus(QString("url(:/image/images/pm481/pm481_green_%1.png)").arg(1),24*1),
                                                     m_co_fun.add_checkBox_forstatus(QString("url(:/image/images/pm481/pm481_iceblue_%1.png)").arg(1),24*1),
                                                     m_co_fun.add_checkBox_forstatus(QString("url(:/image/images/pm481/pm481_darkblue_%1.png)").arg(1),24*1),
                                                     m_co_fun.add_checkBox_forstatus(QString("url(:/image/images/pm481/pm481_pruple_%1.png)").arg(1),24*1),
                                                     m_co_fun.add_checkBox_forstatus(QString("url(:/image/images/pm481/pm481_rgb_%1.png)").arg(1),24*1)};
        }else{

            rgb_light_btn_names = {                  m_co_fun.add_checkBox_forstatus(QString("url(:/image/images/pm481/pm481_red_%1.png)").arg(m_Magnification),24*m_Magnification),
                                                     m_co_fun.add_checkBox_forstatus(QString("url(:/image/images/pm481/pm481_orange_%1.png)").arg(m_Magnification),24*m_Magnification),
                                                     m_co_fun.add_checkBox_forstatus(QString("url(:/image/images/pm481/pm481_yellow_%1.png)").arg(m_Magnification),24*m_Magnification),
                                                     m_co_fun.add_checkBox_forstatus(QString("url(:/image/images/pm481/pm481_green_%1.png)").arg(m_Magnification),24*m_Magnification),
                                                     m_co_fun.add_checkBox_forstatus(QString("url(:/image/images/pm481/pm481_iceblue_%1.png)").arg(m_Magnification),24*m_Magnification),
                                                     m_co_fun.add_checkBox_forstatus(QString("url(:/image/images/pm481/pm481_darkblue_%1.png)").arg(m_Magnification),24*m_Magnification),
                                                     m_co_fun.add_checkBox_forstatus(QString("url(:/image/images/pm481/pm481_pruple_%1.png)").arg(m_Magnification),24*m_Magnification),
                                                     m_co_fun.add_checkBox_forstatus(QString("url(:/image/images/pm481/pm481_rgb_%1.png)").arg(m_Magnification),24*m_Magnification)};
        }

    }else if(device_index==MaonoPD200X){

        if(m_Magnification<1){
            rgb_light_btn_names = {
                                                     m_co_fun.add_checkBox_forstatus(QString("url(:/image/images/pm481/pm481_white_%1.png)").arg(1),24*1),
                                                     m_co_fun.add_checkBox_forstatus(QString("url(:/image/images/pm481/pm481_red_%1.png)").arg(1),24*1),
                                                     m_co_fun.add_checkBox_forstatus(QString("url(:/image/images/pm481/pm481_orange_%1.png)").arg(1),24*1),
                                                     m_co_fun.add_checkBox_forstatus(QString("url(:/image/images/pm481/pm481_yellow_%1.png)").arg(1),24*1),
                                                     m_co_fun.add_checkBox_forstatus(QString("url(:/image/images/pm481/pm481_green_%1.png)").arg(1),24*1),
                                                     m_co_fun.add_checkBox_forstatus(QString("url(:/image/images/pm481/pm481_iceblue_%1.png)").arg(1),24*1),
                                                     m_co_fun.add_checkBox_forstatus(QString("url(:/image/images/pm481/pm481_darkblue_%1.png)").arg(1),24*1),
                                                     m_co_fun.add_checkBox_forstatus(QString("url(:/image/images/pm481/pm481_pruple_%1.png)").arg(1),24*1),
                                                     m_co_fun.add_checkBox_forstatus(QString("url(:/image/images/pm481/pm481_rgb_%1.png)").arg(1),24*1)};
        }else{

            rgb_light_btn_names = {

                                                     m_co_fun.add_checkBox_forstatus(QString("url(:/image/images/pm481/pm481_white_%1.png)").arg(m_Magnification),24*m_Magnification),
                                                     m_co_fun.add_checkBox_forstatus(QString("url(:/image/images/pm481/pm481_red_%1.png)").arg(m_Magnification),24*m_Magnification),
                                                     m_co_fun.add_checkBox_forstatus(QString("url(:/image/images/pm481/pm481_orange_%1.png)").arg(m_Magnification),24*m_Magnification),
                                                     m_co_fun.add_checkBox_forstatus(QString("url(:/image/images/pm481/pm481_yellow_%1.png)").arg(m_Magnification),24*m_Magnification),
                                                     m_co_fun.add_checkBox_forstatus(QString("url(:/image/images/pm481/pm481_green_%1.png)").arg(m_Magnification),24*m_Magnification),
                                                     m_co_fun.add_checkBox_forstatus(QString("url(:/image/images/pm481/pm481_iceblue_%1.png)").arg(m_Magnification),24*m_Magnification),
                                                     m_co_fun.add_checkBox_forstatus(QString("url(:/image/images/pm481/pm481_darkblue_%1.png)").arg(m_Magnification),24*m_Magnification),
                                                     m_co_fun.add_checkBox_forstatus(QString("url(:/image/images/pm481/pm481_pruple_%1.png)").arg(m_Magnification),24*m_Magnification),
                                                     m_co_fun.add_checkBox_forstatus(QString("url(:/image/images/pm481/pm481_rgb_%1.png)").arg(m_Magnification),24*m_Magnification)};
        }

    }

    for (int i=0;i<rgb_light_btn_names.count();i++) {
        custom_checkbox *rgb_btn = new custom_checkbox(back_light_image);
        rgb_btn->move(i*50*m_Magnification+20*m_Magnification,163*m_Magnification);
        if(m_Magnification<1){
           rgb_btn->resize(24*1,24*1);
        }else{
          rgb_btn->resize(24*m_Magnification,24*m_Magnification);
        }

        const QString str = rgb_light_btn_names.at(i);
        rgb_btn->setStyleSheet(str);
        rgb_btn->setObjectName(QString::number(300+i,10));
        if(i==0)rgb_btn->setCheckState(Qt::Checked);
        connect(rgb_btn,SIGNAL(clicked(bool)),this,SLOT(rgb_btn_click(bool)));
        rgb_btns.push_back(rgb_btn);
        m_buttonGroup->addButton(rgb_btn);
    }
    //最底下的三角形
    m_bottom_triangle = new QCheckBox(back_light_image);
    m_bottom_triangle->move(26*m_Magnification,192*m_Magnification);
    if(m_Magnification<1){
      m_bottom_triangle->resize(12*1,10*1);
    }else{
      m_bottom_triangle->resize(12*m_Magnification,10*m_Magnification);
    }
    QString bottom_triangle_stys;
    if(m_Magnification<1){
         bottom_triangle_stys=QString("QCheckBox::indicator{width:%1px;height:%2px;color:rgb(255, 0, 0);}"
                                             "QCheckBox::indicator:unchecked{border-image:url(:/image/images/pm481/pm481_arrow_%3.png) 0 0 0 %4;}"
                                             "QCheckBox::indicator:unchecked:hover{border-image:url(:/image/images/pm481/pm481_arrow_%3.png) 0 0 0 %4;}"
                                             "QCheckBox::indicator:checked{border-image:url(:/image/images/pm481/pm481_arrow_%3.png) 0 0 0 %4;}"
                                             "QCheckBox:focus{outline: none;}").arg(12*1).arg(10*1).arg(1).arg(24*1);
    }else{
         bottom_triangle_stys=QString("QCheckBox::indicator{width:%1px;height:%2px;color:rgb(255, 0, 0);}"
                                             "QCheckBox::indicator:unchecked{border-image:url(:/image/images/pm481/pm481_arrow_%3.png) 0 0 0 %4;}"
                                             "QCheckBox::indicator:unchecked:hover{border-image:url(:/image/images/pm481/pm481_arrow_%3.png) 0 0 0 %4;}"
                                             "QCheckBox::indicator:checked{border-image:url(:/image/images/pm481/pm481_arrow_%3.png) 0 0 0 %4;}"
                                             "QCheckBox:focus{outline: none;}").arg(12*m_Magnification).arg(10*m_Magnification).arg(m_Magnification).arg(24*m_Magnification);
    }

    m_bottom_triangle->setStyleSheet(bottom_triangle_stys);
    //蒙版
    m_rgb_mask = new QLabel(back_light_image);
    m_rgb_mask->move(0,45*m_Magnification);
    m_rgb_mask->resize(this->width(),170*m_Magnification);
    m_rgb_mask->setPixmap(QPixmap(":/image/images/pm481/leftViewBottom.png"));
    m_rgb_mask->setScaledContents(true);

    m_fullrgb_mask = new QLabel(back_light_image);
    m_fullrgb_mask->move(0,0);
    m_fullrgb_mask->resize(this->width(),back_light_image->height());
    m_fullrgb_mask->setPixmap(QPixmap(":/image/images/pm481/DM30_rgb_mask.png"));
    m_fullrgb_mask->setScaledContents(true);

}
//获取到背景的参数，同步界面
void pm481_leftview::refresh_ui(uint8_t mute,uint16_t HeadphoneVolume,uint8_t micGain,uint8_t yinse,
                                uint8_t rgbSwitch,uint8_t rgbAdj,uint8_t rgbNumber,uint8_t mic_mon){
    if(mute==0){//非静音
        m_mute_btn->blockSignals(true);
        m_mute_btn->setChecked(false);
        m_mute_btn->blockSignals(false);
    }else{
        m_mute_btn->blockSignals(true);
        m_mute_btn->setChecked(true);
        m_mute_btn->blockSignals(false);
    }

    //设置耳机音量的SLIDER
    MySlider *micVolume_slider = m_synmicvolume_Slider_arrs.at(0);
    micVolume_slider->blockSignals(true);
    micVolume_slider->setValue(HeadphoneVolume);
    micVolume_slider->blockSignals(false);
    m_mic_volumecurrent_label->setText(QString::number(HeadphoneVolume,10));
    qDebug()<<"ddddddddddd"<<mic_mon;
    //设置麦克风监听
    if(mic_mon == 100){//监听打开
        for (int i =0;i<m_micMon_arrs.count();i++) {
            m_micMon_arrs.at(i)->setPixmap(QPixmap(":/image/images/pm481/pm481_open.png"));
        }
    }else{//监听关闭
        for (int i =0;i<m_micMon_arrs.count();i++) {
            m_micMon_arrs.at(i)->setPixmap(QPixmap(":/image/images/pm481/pm481_close.png"));
        }
    }
    //设置耳机增益的SLIDER
    if(m_synSlider_arrs.count()>0){
        MySlider *micGain_slider = m_synSlider_arrs.at(0);
        micGain_slider->blockSignals(true);
        micGain_slider->setValue(micGain);
        micGain_slider->blockSignals(false);
        m_mic_gaincurrent_label->setText(QString::number(micGain,10));
    }
    qDebug()<<"kkkkkkkkkkk"<<mic_mon;
    //音色的UI
    QString jingdian_btn_stys=QString("QPushButton{border-image:url(:/image/images/jingdian_btn.png);background-color:transparent;color:white;font-size:%1px;font-family:'Source Han Sans CN Medium'}"
                                      "QPushButton:hover{border-image:url(:/image/images/jingdian_btn.png);background-color:transparent;color:rgb(54,54,54);font-size:%1px;font-family:'Source Han Sans CN Medium'}").arg(15*m_Magnification);
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
        jingdian_btn_stys=QString("QPushButton{border-image:url(:/image/images/jingdian_btn_high.png);background-color:transparent;color:white;font-size:%1px;font-family:'Source Han Sans CN Medium'}"
                                  "QPushButton:hover{border-image:url(:/image/images/jingdian_btn_high.png);background-color:transparent;color:rgb(54,54,54);font-size:%1px;font-family:'Source Han Sans CN Medium'}"
                                  ).arg(15*m_Magnification);
        m_jingdian_btn->setStyleSheet(jingdian_btn_stys);
    }
    QVector<QString> yinse_btn_titles={"dichen","ziran","mingliang","jingdian"};
    m_current_yinse_number = yinse_btn_titles.at(yinse);

     qDebug()<<"qqqqqq"<<mic_mon;
    //根据不同的设备做不同的逻辑=======================
    if(currentProPid==0x0104){

        QString strPD200x = tr("    明亮").simplified();
        m_ziran_btn->setText(strPD200x);
        m_ziran_btn->setToolTip(strPD200x);

        m_mingliang_btn->setToolTip(tr("自然"));
        m_mingliang_btn->setText(tr("自然"));

        m_jingdian_btn->setToolTip(tr("经典"));

        m_dichen_btn->setToolTip(tr("低沉"));
    }


    //RGB灯光的开关
    if(rgbSwitch==0){
        m_RGB_btn_bgimage->setPixmap(QPixmap(":/image/images/pm481/pm481_open.png"));
        m_rgb_mask->show();
    }else{
        m_RGB_btn_bgimage->setPixmap(QPixmap(":/image/images/pm481/pm481_close.png"));
        m_rgb_mask->hide();
    }
    //亮度调节
    m_light_slider->blockSignals(true);
    m_light_slider->setValue(rgbAdj);
    m_light_slider->blockSignals(false);
    m_mic_lightcurrent_label->setText(QString::number(rgbAdj,10));

    qDebug()<<"测试------------------"<<rgbNumber;
    //if(rgbNumber>7)return;
    //颜色选择
    QCheckBox *rgb_btn = rgb_btns.at(rgbNumber);
    rgb_btn->setCheckState(Qt::Checked);
    m_bottom_triangle->move(rgb_btn->x()+6*m_Magnification,192*m_Magnification);

}
void pm481_leftview::sys_receive_data(uint8_t address,int value,uint8_t address2,int value2,int level_data){

    //m_PM481Timer->stop();
    if(address ==0x22){
        if(value==0){//非静音
            m_mute_btn->blockSignals(true);
            m_mute_btn->setChecked(false);
            m_mute_btn->blockSignals(false);
        }else{
            m_mute_btn->blockSignals(true);
            m_mute_btn->setChecked(true);
            m_mute_btn->blockSignals(false);
        }
    }else if(address == (PARAM_HP_GAIN & 0xff)){//耳机音量
        MySlider *micVolume_slider = m_synmicvolume_Slider_arrs.at(0);
        micVolume_slider->blockSignals(true);
        if(!m_DM30phsilder_runing){
            micVolume_slider->setValue(value);
            m_mic_volumecurrent_label->setText(QString::number(value,10));
        }
        micVolume_slider->blockSignals(false);
        DM30_send_value.insert(QString::number(PARAM_HP_GAIN & 0xff,16),value);
        DM30_different_value.insert(QString::number(PARAM_HP_GAIN & 0xff,16),value);


    }else if(address == (PARAM_MIC_GAIN & 0xff)){//麦克风增益
        MySlider *micGain_slider = m_synSlider_arrs.at(0);
        micGain_slider->blockSignals(true);
        if(!m_DM30micsilder_runing){
            micGain_slider->setValue(value);
            m_mic_gaincurrent_label->setText(QString::number(value,10));
        }
        micGain_slider->blockSignals(false);
        DM30_send_value.insert(QString::number(PARAM_MIC_GAIN & 0xff,16),value);
        DM30_different_value.insert(QString::number(PARAM_MIC_GAIN & 0xff,16),value);

    }else if(address == (PARAM_RGB_SHOW_ONOFF & 0xff)){//彩色灯的开关
        if(value==0){//灯关闭
            m_RGB_btn_bgimage->setPixmap(QPixmap(":/image/images/pm481/pm481_open.png"));
            m_RGB_btn_bgimage->setScaledContents(true);
            m_rgb_mask->show();
        }else{
            m_RGB_btn_bgimage->setPixmap(QPixmap(":/image/images/pm481/pm481_close.png"));
            m_RGB_btn_bgimage->setScaledContents(true);
            m_rgb_mask->hide();
        }
        if(value2 <= 8){
            QCheckBox *rgb_btn = rgb_btns.at(value2);
            rgb_btn->setCheckState(Qt::Checked);
            m_bottom_triangle->move(rgb_btn->x()+6*m_Magnification,192*m_Magnification);
        }
        qDebug()<<"打印数据------"<<value2;
    }else if(address ==(PARAM_MIC_SIG_LEVEL & 0xff)){//电平

        //如果是静音，没效果
        QCheckBox *right_mute_btn = m_mute_arrs.at(1);
        if(right_mute_btn->checkState() !=0){
            setHideDianping();
            return;
        }
        m_dianping_lab_front->show();
        int max_value = 35;

        int myvalue = ((level_data-2000)/10 + max_value);
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
    }else if(address ==(PARAM_LR_MONITOR & 0xff)){//监听比例

        int mix_mon = (level_data-2000)/10;
        if(mix_mon == 100){//监听打开
            for (int i =0;i<m_micMon_arrs.count();i++) {
                m_micMon_arrs.at(i)->setPixmap(QPixmap(":/image/images/pm481/pm481_open.png"));
            }

        }else{//监听关闭
            for (int i =0;i<m_micMon_arrs.count();i++) {
                m_micMon_arrs.at(i)->setPixmap(QPixmap(":/image/images/pm481/pm481_close.png"));
            }
        }
    }
}

void pm481_leftview::setHideDianping(){

    m_dianping_lab_front->resize(348*m_Magnification,12*m_Magnification);
    m_dianping_lab_front->move(75*m_Magnification,73*m_Magnification-6*m_Magnification);
    m_dianping_lab_front->show();
}
//计时器的事件,发送最后一包数据
void pm481_leftview::handleTimeout(){

    //    if(m_slider_current_number==1){
    //        m_commin_fun.pd400_headset_volume(m_slider_current_value);
    //        DM30_send_value.insert(QString::number(PARAM_HP_GAIN & 0xff,16),m_slider_current_value);
    //    }else if(m_slider_current_number==2){
    //        m_commin_fun.pd400_mic_gain(m_slider_current_value);
    //        DM30_send_value.insert(QString::number(PARAM_MIC_GAIN & 0xff,16),m_slider_current_value);
    //    }else if(m_slider_current_number==3){
    //        m_commin_fun.rgb_adjust(m_slider_current_value);
    //        DM30_send_value.insert(QString::number(PARAM_RGB_BRIGHT & 0xff,16),m_slider_current_value);
    //    }
}
//静音
void pm481_leftview::mute_btn_click(int status){

    QCheckBox *right_mute_btn = m_mute_arrs.at(1);
    if(status==0){
        right_mute_btn->setChecked(false);
    }else{
        right_mute_btn->setChecked(true);
    }
    m_commin_fun.pd400_mute(status);
}
//调节耳机的音量
void pm481_leftview::headsetVolume_eq_chanege_value(int slidervalue){

    m_mic_volumecurrent_label->setText(QString::number(slidervalue,10));
    //同步右边耳机音量
    MySlider *slider = m_synmicvolume_Slider_arrs.at(1);
    //slider->blockSignals(true);
    slider->setValue(slidervalue);
    //slider->blockSignals(false);
    m_PM481slider_current_value = slidervalue;
    m_PM481slider_current_number = 1;
    m_commin_fun.pd400_headset_volume(slidervalue);
    //pd400_send_value = slidervalue;
    DM30_send_value.insert(QString::number(PARAM_HP_GAIN & 0xff,16),slidervalue);
    m_DM30phsilder_runing = true;
    m_PM481Timer->stop();
    m_PM481Timer->start(20);

    //    m_DM30Timer->stop();
    //    if(m_current_model==1)
    //    m_DM30Timer->start(20);
}
void pm481_leftview::headsetVolume_release_value(int slidervalue){

    //同步设备数据
    MySlider *micVolume_slider = m_synmicvolume_Slider_arrs.at(0);
    QMap<QString,int>::iterator send_value_iter = DM30_send_value.find(QString::number(PARAM_HP_GAIN & 0xff,16));
    micVolume_slider->blockSignals(true);
    micVolume_slider->setValue(send_value_iter.value());
    m_synmicvolume_Slider_arrs.at(1)->setValue(send_value_iter.value());
    m_mic_volumecurrent_label->setText(QString::number(send_value_iter.value(),10));
    micVolume_slider->blockSignals(false);
    m_DM30phsilder_runing = false;
    //    QElapsedTimer time;
    //    time.start();
    //    while(time.elapsed() < 40)  {
    //        m_commin_fun.pd400_headset_volume(slidervalue);
    //        pd400_send_value = slidervalue;
    //    }
}
//麦克风增益
void pm481_leftview::micgain_eq_change_value(int slidervalue){

    m_mic_gaincurrent_label->setText(QString::number(slidervalue,10));
    //同步右边耳机的增益
    MySlider *slider = m_synSlider_arrs.at(1);
    //slider->blockSignals(true);
    slider->setValue(slidervalue);
    //slider->blockSignals(false);
    //发送数据
    m_PM481slider_current_value = slidervalue;
    m_PM481slider_current_number = 2;
    m_commin_fun.pd400_mic_gain(slidervalue);
    //pd400_send_value = slidervalue;
    DM30_send_value.insert(QString::number(PARAM_MIC_GAIN & 0xff,16),slidervalue);
    m_DM30micsilder_runing = true;
    m_PM481Timer->stop();
    m_PM481Timer->start(20);

    //    m_DM30Timer->stop();
    //    if(m_current_model==1)
    //    m_DM30Timer->start(20);
}
void pm481_leftview::micgain_release_value(int slidervalue){


    MySlider *micGain_slider = m_synSlider_arrs.at(0);
    QMap<QString,int>::iterator send_value_iter = DM30_send_value.find(QString::number(PARAM_MIC_GAIN & 0xff,16));
    micGain_slider->blockSignals(true);
    micGain_slider->setValue(send_value_iter.value());
    m_synSlider_arrs.at(1)->setValue(send_value_iter.value());
    m_mic_gaincurrent_label->setText(QString::number(send_value_iter.value(),10));
    micGain_slider->blockSignals(false);
    m_DM30micsilder_runing = false;

    //    QElapsedTimer time;
    //    time.start();
    //    while(time.elapsed() < 40)  {
    //        m_commin_fun.pd400_mic_gain(slidervalue);
    //        pd400_send_value = slidervalue;
    //    }
}

void pm481_leftview::change_yinse(){

    QString jingdian_btn_stys=QString("QPushButton{border-image:url(:/image/images/jingdian_btn.png);background-color:transparent;color:white;font-size:%1px;font-family:'Source Han Sans CN Medium'}"
                                      "QPushButton:hover{border-image:url(:/image/images/jingdian_btn.png);background-color:transparent;color:rgb(54,54,54);font-size:%1px;font-family:'Source Han Sans CN Medium'}"
                                      "QPushButton:focus{outline: none;}").arg(15*m_Magnification);
    m_jingdian_btn->setStyleSheet(jingdian_btn_stys);
    QPushButton* btn = dynamic_cast<QPushButton*>(sender());
    m_current_yinse_number = btn->objectName();
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

//灯光
void pm481_leftview::light_change_value(int light_value){

    m_mic_lightcurrent_label->setText(QString::number(light_value,10));
    //发送数据
    m_PM481slider_current_value = light_value;
    m_PM481slider_current_number = 3;
    m_commin_fun.rgb_adjust(light_value);
    //pd400_send_value = light_value;
    DM30_send_value.insert(QString::number(PARAM_RGB_BRIGHT & 0xff,16),light_value);

    m_PM481Timer->stop();
    m_PM481Timer->start(20);

    //    m_DM30Timer->stop();
    //    m_DM30Timer->start(40);

}
void pm481_leftview::light_release_value(int light_value){

    //    QElapsedTimer time;
    //    time.start();
    //    while(time.elapsed() < 40)  {
    //        m_commin_fun.rgb_adjust(light_value);
    //        pd400_send_value = light_value;
    //    }
}

void pm481_leftview::light_switch_click(){

    QPushButton *btn = dynamic_cast<QPushButton *>(sender());
    if(btn->objectName()=="200"){
        m_RGB_btn_bgimage->setPixmap(QPixmap(":/image/images/pm481/pm481_open.png"));
        m_rgb_mask->show();
        m_commin_fun.rgb_switch(0);
    }else{
        m_RGB_btn_bgimage->setPixmap(QPixmap(":/image/images/pm481/pm481_close.png"));
        m_rgb_mask->hide();
        m_commin_fun.rgb_switch(1);
    }
}
//监听比例
void pm481_leftview::mix_switch_click(){
    QPushButton *btn = dynamic_cast<QPushButton *>(sender());
    if(btn->objectName()=="400"){
        m_mix_btn_bgimage->setPixmap(QPixmap(":/image/images/pm481/pm481_open.png"));
        m_commin_fun.monitor_mix_switch(200);
        //同步右边的麦克风监听
        m_micMon_arrs.at(1)->setPixmap(QPixmap(":/image/images/pm481/pm481_open.png"));
    }else{
        m_mix_btn_bgimage->setPixmap(QPixmap(":/image/images/pm481/pm481_close.png"));
        m_commin_fun.monitor_mix_switch(100);
        m_micMon_arrs.at(1)->setPixmap(QPixmap(":/image/images/pm481/pm481_close.png"));
    }
}
void pm481_leftview::rgb_btn_click(bool){
    QCheckBox *mybtn = dynamic_cast<QCheckBox *>(sender());

    m_bottom_triangle->move(mybtn->x()+6*m_Magnification,192*m_Magnification);
    //发送数据
    int color_index = mybtn->objectName().toInt()-300;

    //    m_slider_current_value = color_index;
    //    m_slider_current_number = 5;
    m_commin_fun.rgb_color_select(color_index);
    //pd400_send_value = color_index;
    //    m_DM30Timer->stop();
    //    m_DM30Timer->start(40);

}
void pm481_leftview::change_language(){

}
