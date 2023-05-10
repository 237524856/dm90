
#pragma once
#pragma execution_character_set("utf-8")

#include "pm481_rightview.h"
#include "../common.h"
#include "pm481_common.h"
#include <QElapsedTimer>
#include <QTimer>
#include <QThread>

pm481_rightview::pm481_rightview(QWidget *parent) : QWidget(parent)
{
    m_eq_titles = {tr("平坦"),tr("低切"),tr("中频增强"),tr("低切&中频增强")};
//    m_DM30Timer = new QTimer(this);
//    connect(m_DM30Timer, SIGNAL(timeout()), this, SLOT(handleTimeout()));
}
void pm481_rightview::createUI(){

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


    m_dianping_lab_front = new QLabel(this);
    m_dianping_lab_front->resize(348*m_Magnification,12*m_Magnification);
    m_dianping_lab_front->move(75*m_Magnification,73*m_Magnification-6*m_Magnification);
    m_dianping_lab_front->setStyleSheet("background-color:white");

    QLabel *dianping_lab = new QLabel(this);
    dianping_lab->resize(348*m_Magnification,12*m_Magnification);
    dianping_lab->move(75*m_Magnification,(73-6)*m_Magnification);
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
    connect(mic_gain_slider,SIGNAL(valueChanged(int)),this,SLOT(micgain_eq_chanege_value(int)));
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
        micvolume_bottomlabel->move(72*m_Magnification-16*m_Magnification+(310-36)*i*m_Magnification,170*m_Magnification);
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
    m_micvolume_label->move(20*m_Magnification,136*m_Magnification+70*m_Magnification);
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
    QVector<QString>switch_btns ={tr("关"),tr("开")};
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
    m_mic_volumecurrent_label->move((395-16)*m_Magnification,(150+95)*m_Magnification);
    m_mic_volumecurrent_label->resize(36*m_Magnification,24*m_Magnification);
    m_mic_volumecurrent_label->setText("0");
    m_mic_volumecurrent_label->setPalette(pa);
    m_mic_volumecurrent_label->setFont(font);

    for (int i=0;i<2;i++) {
        QLabel*micvolume_bottomlabel = new QLabel(this);
        micvolume_bottomlabel->move(72*m_Magnification-16*m_Magnification+(310-36)*i*m_Magnification,270*m_Magnification);
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

    m_eqtitle_lal =new QLabel(this);
    m_eqtitle_lal->move(0,300*m_Magnification);
    m_eqtitle_lal->resize(this->width()-80*m_Magnification,24*m_Magnification);
    QString eqtitle_lalstys=QString("font-family:'Source Han Sans CN Medium';color:rgb(255,255,255);font-size:%1px;").arg((int)(13*m_Magnification));
    m_eqtitle_lal->setStyleSheet(eqtitle_lalstys);
    m_eqtitle_lal->setText(m_eq_titles.at(0));
    m_eqtitle_lal->setAlignment(Qt::AlignRight);

    m_junheng_label = new QLabel(this);
    m_junheng_label->move(20*m_Magnification,300*m_Magnification);
    m_junheng_label->resize(116*m_Magnification,24*m_Magnification);
    m_junheng_label->setText(tr("均衡器"));
    m_junheng_label->setPalette(pa);
    m_junheng_label->setFont(font);

    m_EQ_bgimage = new QLabel(this);
    m_EQ_bgimage->move(20*m_Magnification,335*m_Magnification);
    m_EQ_bgimage->resize(400*m_Magnification,30*m_Magnification);
    m_EQ_bgimage->setPixmap(QPixmap(":/image/images/pd400_eq_bgimg.png"));
    m_EQ_bgimage->setScaledContents(true);

    QVector<QString>eqbtnimgs = {"QPushButton{border-image:url(:/image/images/pd400_eq1_btn.png) 0 100 0 0}"
                                 "QPushButton:hover{border-image:url(:/image/images/pd400_eq1_btn.png) 0 0 0 100}"
                                 "QPushButton:focus{outline: none;}",
                                 "QPushButton{border-image:url(:/image/images/pd400_eq2_btn.png) 0 100 0 0}"
                                 "QPushButton:hover{border-image:url(:/image/images/pd400_eq2_btn.png) 0 0 0 100}"
                                 "QPushButton:focus{outline: none;}",
                                 "QPushButton{border-image:url(:/image/images/pd400_eq3_btn.png) 0 100 0 0}"
                                 "QPushButton:hover{border-image:url(:/image/images/pd400_eq3_btn.png) 0 0 0 100}"
                                 "QPushButton:focus{outline: none;}",
                                 "QPushButton{border-image:url(:/image/images/pd400_eq4_btn.png) 0 100 0 0}"
                                 "QPushButton:hover{border-image:url(:/image/images/pd400_eq4_btn.png) 0 0 0 100}"
                                 "QPushButton:focus{outline: none;}",};
    //最右边跟slider动的LABEL
    for (int i=0;i<eqbtnimgs.count();i++) {
        custom_button *eq_btn = new custom_button(m_EQ_bgimage);
        eq_btn->move(100*i*m_Magnification,0);
        eq_btn->resize(100*m_Magnification,30*m_Magnification);
        eq_btn->setStyleSheet(eqbtnimgs.at(i));
        eq_btn->setObjectName(QString::number(i+200,10));
        connect(eq_btn,SIGNAL(clicked()),this,SLOT(change_eq()));
    }

    //限幅器，压缩器
    QVector<QString>xinfutitles={tr("限制器"),tr("压缩器")};
    QVector<QString>titles={"-40dB","0dB","-80dB","0dB"};
    for (int i=0;i<2;i++) {
        custom_checkbox *xianfu_btn= new custom_checkbox(this);
        xianfu_btn->setText(xinfutitles.at(i));
        xianfu_btn->resize(100*m_Magnification,40*m_Magnification);
        xianfu_btn->move(20*m_Magnification, 392*m_Magnification+i*95*m_Magnification);

        QString xianfu_btn_stys=QString("QCheckBox{font-family:'Source Han Sans CN Medium';color:rgb(255,255,255);font-size:%1px;}"
                                 "QCheckBox::hover{font-family:'Source Han Sans CN Medium';color:rgb(54,54,54);font-size:%1px;}"
                                 "QCheckBox::indicator{width:%2px;height:%2px;color: rgb(255, 0, 0);}"
                                 "QCheckBox::indicator:unchecked{border-image:url(:/image/images/pd400_xianfuunclick_btn.png.png);}"
                                 "QCheckBox::indicator:checked{border-image:url(:/image/images/pd400_xianfuclick_btn.png);}"
                                 "QCheckBox:focus{outline: none;}").arg((int)(14*m_Magnification)).arg((int)(10*m_Magnification));

        xianfu_btn->setStyleSheet(xianfu_btn_stys);
        xianfu_btn->setCheckState(Qt::Checked);
        xianfu_btn->setObjectName(QString::number(100+i,10));
        connect(xianfu_btn,SIGNAL(stateChanged(int)),this,SLOT(isopen_btn_click(int)));
        enable_btns.push_back(xianfu_btn);

        QLabel *xianfu_label = new QLabel(this);
        xianfu_label->move(55*m_Magnification-16*m_Magnification,397*m_Magnification+i*95*m_Magnification);
        xianfu_label->resize(116*m_Magnification,24*m_Magnification);
        xianfu_label->setText(xinfutitles.at(i));
        xianfu_label->setPalette(pa);
        xianfu_label->setFont(font);
        xianfuyaxian_lbs.push_back(xianfu_label);
        xianfu_label->hide();

        MySlider *mic_xianfu_yaxian_slider = new MySlider(this);
        mic_xianfu_yaxian_slider->setOrientation(Qt::Horizontal);
        mic_xianfu_yaxian_slider->setMinimum(0);
        if(i==0){
           mic_xianfu_yaxian_slider->setMaximum(40);
        }else{
           mic_xianfu_yaxian_slider->setMaximum(80);
        }

        mic_xianfu_yaxian_slider->setGeometry(20*m_Magnification,431*m_Magnification+i*95*m_Magnification,350*m_Magnification,12*m_Magnification);
//        mic_xianfu_yaxian_slider->setStyleSheet(mic_gain_slider_stys);
        mic_xianfu_yaxian_slider->setObjectName(QString::number(100+i,10));
        connect(mic_xianfu_yaxian_slider,SIGNAL(valueChanged(int)),this,SLOT(change_limiter_comp(int)));
        connect(mic_xianfu_yaxian_slider,SIGNAL(sliderReleasedAt(int)),this,SLOT(limiter_comp_release_value(int)));
        sliders.push_back(mic_xianfu_yaxian_slider);

        QLabel *currentvalue_label = new QLabel(this);
        currentvalue_label->move(385*m_Magnification,425*m_Magnification+ i*95*m_Magnification);
        currentvalue_label->resize(36*m_Magnification,24*m_Magnification);
        if(i==0){
           currentvalue_label->setText("-40dB");
        }else{
           currentvalue_label->setText("-80dB");
        }

        currentvalue_label->setPalette(pa);
        font.setPixelSize(12*m_Magnification);
        currentvalue_label->setFont(font);
        m_comp_limiter_lables.push_back(currentvalue_label);

        for (int j=0;j<2;j++) {
            QLabel *mic_label = new QLabel(this);
            mic_label->move(20*m_Magnification+325*j*m_Magnification,453*m_Magnification + i*95*m_Magnification);
            mic_label->resize(36*m_Magnification,24*m_Magnification);
            mic_label->setText(titles.at(j + 2*i));
            mic_label->setPalette(pa);
            font.setPixelSize(12*m_Magnification);
            mic_label->setFont(font);
        }
    }
    //上面部分的蒙版
    m_high_mask = new QLabel(this);
    m_high_mask->move(0,0);
    m_high_mask->resize(this->width(),this->height());
    m_high_mask->setPixmap(QPixmap(":/image/images/pm481/rightView.png"));
    m_high_mask->setScaledContents(true);
    //m_high_mask->hide();
}
//获取到背景参数后同步界面
void pm481_rightview::refresh_ui(uint8_t mute,uint16_t HeadphoneVolume,uint8_t micGain,uint8_t eqValue,uint8_t limiter_status,
                                 int limiter_value,uint8_t comper_status,int comper_value,uint8_t mic_mon){

    if(mute==0){//非静音
        m_mute_btn->setChecked(false);
    }else{
        m_mute_btn->setChecked(true);
    }

    //设置耳机音量的SLIDER
    MySlider *micVolume_slider = m_synmicvolume_Slider_arrs.at(1);
    micVolume_slider->blockSignals(true);
    micVolume_slider->setValue(HeadphoneVolume);
    micVolume_slider->blockSignals(false);
    m_mic_volumecurrent_label->setText(QString::number(HeadphoneVolume,10));

    //设置耳机增益的SLIDER
    if(m_synSlider_arrs.count()>1){

        MySlider *micGain_slider = m_synSlider_arrs.at(1);
        micGain_slider->blockSignals(true);
        micGain_slider->setValue(micGain);
        micGain_slider->blockSignals(false);
        m_mic_gaincurrent_label->setText(QString::number(micGain,10));
    }

    //设置均衡器
    if(eqValue==0){
        m_EQ_bgimage->setPixmap(QPixmap(":/image/images/pd400_eq1_bgimg.png"));
        m_eqtitle_lal->setText(tr("平坦"));
    }else if(eqValue==1){
        m_EQ_bgimage->setPixmap(QPixmap(":/image/images/pd400_eq2_bgimg.png"));
        m_eqtitle_lal->setText(tr("低切"));
    }else if(eqValue==2){
        m_EQ_bgimage->setPixmap(QPixmap(":/image/images/pd400_eq3_bgimg.png"));
        m_eqtitle_lal->setText(tr("中频增强"));
    }else{
        m_EQ_bgimage->setPixmap(QPixmap(":/image/images/pd400_eq4_bgimg.png"));
        m_eqtitle_lal->setText(tr("低切&中频增强"));
    }
    m_current_btn_select = QString::number(eqValue+200,10);
    //限制器
    if(limiter_status ==0){
        enable_btns.at(0)->blockSignals(true);
        enable_btns.at(0)->setCheckState(Qt::Unchecked);
        enable_btns.at(0)->blockSignals(false);
        //不同的使能禁点
        sliders.at(0)->setSliderDisable(true);
//        QString slider_stys=QString("QSlider::groove:horizontal{background: rgba(0, 0, 0,100);height:%2px;border: 0px solid #FFFFFF;border-image:url(:/image/images/pd400_right_slider_bgimg.png);}"
//                                             "QSlider::handle:horizontal{width:%1px;height:%1px;margin-top: -%2px;margin-left: 0px;margin-bottom: -%2px;margin-right: 0px;border-image:url(:/image/images/pd400_round.png);}"
//                                             "QSlider::sub-page:horizontal{background-color:rgba(0,0,0,100);border-radius: %2px;border-image:url(:/image/images/pd400_right_addslider_disabled.png);}").arg(12*m_Magnification).arg(4*m_Magnification);
//        sliders.at(0)->setStyleSheet(slider_stys);
    }else{
        enable_btns.at(0)->blockSignals(true);
        enable_btns.at(0)->setCheckState(Qt::Checked);
        enable_btns.at(0)->blockSignals(false);
//        QString slider_stys=QString("QSlider::groove:horizontal{background: rgba(0, 0, 0,100);height:%2px;border: 0px solid #FFFFFF;border-image:url(:/image/images/pd400_right_slider_bgimg.png);}"
//                                             "QSlider::handle:horizontal{width:%1px;height:%1px;margin-top: -%2px;margin-left: 0px;margin-bottom: -%2px;margin-right: 0px;border-image:url(:/image/images/pd400_round.png);}"
//                                             "QSlider::sub-page:horizontal{background-color:rgba(0,0,0,100);border-radius: %2px;border-image:url(:/image/images/pd400_right_addslider_bgimg.png);}").arg(12*m_Magnification).arg(4*m_Magnification);
//        sliders.at(0)->setStyleSheet(slider_stys);
    }
    //压缩器
    if(comper_status==0){
        enable_btns.at(1)->blockSignals(true);
        enable_btns.at(1)->setCheckState(Qt::Unchecked);
        enable_btns.at(1)->blockSignals(false);
        sliders.at(1)->setSliderDisable(true);
//        QString slider_stys=QString("QSlider::groove:horizontal{background: rgba(0, 0, 0,100);height:%2px;border: 0px solid #FFFFFF;border-image:url(:/image/images/pd400_right_slider_bgimg.png);}"
//                                             "QSlider::handle:horizontal{width:%1px;height:%1px;margin-top: -%2px;margin-left: 0px;margin-bottom: -%2px;margin-right: 0px;border-image:url(:/image/images/pd400_round.png);}"
//                                             "QSlider::sub-page:horizontal{background-color:rgba(0,0,0,100);border-radius: %2px;border-image:url(:/image/images/pd400_right_addslider_disabled.png);}").arg(12*m_Magnification).arg(4*m_Magnification);
//        sliders.at(1)->setStyleSheet(slider_stys);

    }else{
        enable_btns.at(1)->blockSignals(true);
        enable_btns.at(1)->setCheckState(Qt::Checked);
        enable_btns.at(1)->blockSignals(false);
//        QString slider_stys=QString("QSlider::groove:horizontal{background: rgba(0, 0, 0,100);height:%2px;border: 0px solid #FFFFFF;border-image:url(:/image/images/pd400_right_slider_bgimg.png);}"
//                                             "QSlider::handle:horizontal{width:%1px;height:%1px;margin-top: -%2px;margin-left: 0px;margin-bottom: -%2px;margin-right: 0px;border-image:url(:/image/images/pd400_round.png);}"
//                                             "QSlider::sub-page:horizontal{background-color:rgba(0,0,0,100);border-radius: %2px;border-image:url(:/image/images/pd400_right_addslider_bgimg.png);}").arg(12*m_Magnification).arg(4*m_Magnification);
//        sliders.at(1)->setStyleSheet(slider_stys);
    }

    //设置显示器压缩器
    int limit_comp_value = 0;
    for (int i=0;i<2;i++) {
        sliders.at(i)->blockSignals(true);
        if(i==0){
            limit_comp_value = (limiter_value-2000) /10 +40;
        }else{
            limit_comp_value = (comper_value-2000) /10 +80;
        }

        sliders.at(i)->setValue(limit_comp_value);

        sliders.at(i)->blockSignals(false);
    }
    m_comp_limiter_lables.at(0)->setText(QString::number((limiter_value-2000) /10,10)+"dB");
    m_comp_limiter_lables.at(1)->setText(QString::number((comper_value-2000) /10,10)+"dB");

}
void pm481_rightview::sys_receive_data(uint8_t address,int value,int level_data){

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
        MySlider *micVolume_slider = m_synmicvolume_Slider_arrs.at(1);
        micVolume_slider->blockSignals(true);
        if(!m_DM30phsilder_runing){
            micVolume_slider->setValue(value);
            m_mic_volumecurrent_label->setText(QString::number(value,10));
        }

        micVolume_slider->blockSignals(false);

    }else if(address == (PARAM_MIC_GAIN & 0xff)){//麦克风增益
        MySlider *micGain_slider = m_synSlider_arrs.at(1);
        micGain_slider->blockSignals(true);
        if(!m_DM30micsilder_runing){
          micGain_slider->setValue(value);
          m_mic_gaincurrent_label->setText(QString::number(value,10));
        }

        micGain_slider->blockSignals(false);

    }else if(address == (PARAM_MIC_SIG_LEVEL & 0xff)){//电平

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
        //        uint16_t level_value = level_data;
        //        //如果是静音，没效果
        //        QCheckBox *right_mute_btn = m_mute_arrs.at(1);
        //        if(right_mute_btn->checkState() !=0){
        //            setHideDianping();
        //            return;
        //        }
        //        int myvalue = ((level_value-2000)/10.0 + 100);
        //        if(myvalue>=100){
        //            myvalue=100;
        //        }
        //        //qDebug()<<"打印数据22------"<<level_value<<(value-2000)/10+100;
        //        m_dianping_lab_front->show();
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
    }
}
void pm481_rightview::setHideDianping(){
    m_dianping_lab_front->resize(348*m_Magnification,12*m_Magnification);
    m_dianping_lab_front->move(75*m_Magnification,73*m_Magnification-6*m_Magnification);
    m_dianping_lab_front->show();
}
void pm481_rightview::handleTimeout(){
//    if(m_slider_current_number==1){
//        m_commin_fun.pd400_headset_volume(m_slider_current_value);
//       // pd400_send_value = m_slider_current_value;
//        DM30_send_value.insert(QString::number(PARAM_HP_GAIN & 0xff,16),m_slider_current_value);
//    }else if(m_slider_current_number==2){
//        m_commin_fun.pd400_mic_gain(m_slider_current_value);
//        //pd400_send_value = m_slider_current_value;
//        DM30_send_value.insert(QString::number(PARAM_MIC_GAIN & 0xff,16),m_slider_current_value);
//    }else if(m_slider_current_number==3){
//        m_commin_fun.pd400_limiter(m_slider_current_value);
//        //pd400_send_value = m_slider_current_value;
//        DM30_send_value.insert(QString::number((PARAM_MIC_LIMIT_START & 0xff)+ 1,16),m_slider_current_value);
//    }else if(m_slider_current_number==4){
//        m_commin_fun.pd400_comp(m_slider_current_value);
//        //pd400_send_value = m_slider_current_value;
//        DM30_send_value.insert(QString::number((PARAM_MIC_COMP_START & 0xff) +1,16),m_slider_current_value);
//    }
}
void pm481_rightview::mute_btn_click(int status){

    QCheckBox *right_mute_btn = m_mute_arrs.at(0);
    if(status==0){
        right_mute_btn->setChecked(false);
    }else{
        right_mute_btn->setChecked(true);
    }
}
void pm481_rightview::mix_switch_click(){
    QPushButton *btn = dynamic_cast<QPushButton *>(sender());
    if(btn->objectName()=="400"){//关闭
       m_mix_btn_bgimage->setPixmap(QPixmap(":/image/images/pm481/pm481_open.png"));
       m_commin_fun.monitor_mix_switch(200);
       //同步右边的麦克风监听
       m_micMon_arrs.at(0)->setPixmap(QPixmap(":/image/images/pm481/pm481_open.png"));
    }else{
       m_mix_btn_bgimage->setPixmap(QPixmap(":/image/images/pm481/pm481_close.png"));
       m_commin_fun.monitor_mix_switch(100);
       m_micMon_arrs.at(0)->setPixmap(QPixmap(":/image/images/pm481/pm481_close.png"));
    }
}
//调节耳机的音量
void pm481_rightview::headsetVolume_eq_chanege_value(int slidervalue){

    m_mic_volumecurrent_label->setText(QString::number(slidervalue,10));
    //同步左边的数据
    MySlider *slider = m_synmicvolume_Slider_arrs.at(0);
    //slider->blockSignals(true);
    slider->setValue(slidervalue);
    //slider->blockSignals(false);
    //发送数据
    m_PM481slider_current_value = slidervalue;
    m_PM481slider_current_number = 1;
    m_commin_fun.pd400_headset_volume(slidervalue);
    //pd400_send_value = slidervalue;
    DM30_send_value.insert(QString::number(PARAM_HP_GAIN & 0xff,16),slidervalue);

    m_PM481Timer->stop();
    m_PM481Timer->start(40);
    m_DM30phsilder_runing = true;

//    m_DM30Timer->stop();
//    if(m_current_model==2)
//    m_DM30Timer->start(40);
}
void pm481_rightview::headsetVolume_release_value(int slidervalue){

    //同步设备数据
    MySlider *micVolume_slider = m_synmicvolume_Slider_arrs.at(1);
    QMap<QString,int>::iterator send_value_iter = DM30_send_value.find(QString::number(PARAM_HP_GAIN & 0xff,16));
    micVolume_slider->blockSignals(true);
    micVolume_slider->setValue(send_value_iter.value());
    m_synmicvolume_Slider_arrs.at(0)->setValue(send_value_iter.value());
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
void pm481_rightview::micgain_eq_chanege_value(int slidervalue){

    m_mic_gaincurrent_label->setText(QString::number(slidervalue,10));
    //同步左边耳机的增益
    MySlider *slider = m_synSlider_arrs.at(0);
    //slider->blockSignals(true);
    slider->setValue(slidervalue);
    //slider->blockSignals(false);
    //发送数据
    m_PM481slider_current_value = slidervalue;
    m_PM481slider_current_number = 2;
    m_commin_fun.pd400_mic_gain(slidervalue);
    //pd400_send_value = slidervalue;
    DM30_send_value.insert(QString::number(PARAM_MIC_GAIN & 0xff,16),slidervalue);

    m_PM481Timer->stop();
    m_PM481Timer->start(40);
    m_DM30micsilder_runing = true;
//    m_DM30Timer->stop();
//    if(m_current_model==2)
//    m_DM30Timer->start(40);
}
void pm481_rightview::micgain_release_value(int slidervalue){

    MySlider *micGain_slider = m_synSlider_arrs.at(1);
    QMap<QString,int>::iterator send_value_iter = DM30_send_value.find(QString::number(PARAM_MIC_GAIN & 0xff,16));
    micGain_slider->blockSignals(true);
    micGain_slider->setValue(send_value_iter.value());
    m_synSlider_arrs.at(0)->setValue(send_value_iter.value());
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
void pm481_rightview::change_eq(){

    QPushButton* btn = dynamic_cast<QPushButton*>(sender());
    m_current_btn_select = btn->objectName();
    if(btn->objectName()=="200"){
        m_EQ_bgimage->setPixmap(QPixmap(":/image/images/pd400_eq1_bgimg.png"));
        m_commin_fun.pd400_mic(0);
        m_eqtitle_lal->setText(tr("平坦"));
    }else if(btn->objectName()=="201"){
        m_EQ_bgimage->setPixmap(QPixmap(":/image/images/pd400_eq2_bgimg.png"));
        m_commin_fun.pd400_mic(1);
        m_eqtitle_lal->setText(tr("低切"));
    }else if(btn->objectName()=="202"){
        m_EQ_bgimage->setPixmap(QPixmap(":/image/images/pd400_eq3_bgimg.png"));
        m_commin_fun.pd400_mic(2);
        m_eqtitle_lal->setText(tr("中频增强"));
    }else{
        m_EQ_bgimage->setPixmap(QPixmap(":/image/images/pd400_eq4_bgimg.png"));
        m_commin_fun.pd400_mic(3);
        m_eqtitle_lal->setText(tr("低切&中频增强"));
    }
}
void pm481_rightview::isopen_btn_click(int status){

    QCheckBox *btn = dynamic_cast<QCheckBox *>(sender());
//    QString slider_dis_stys=QString("QSlider::groove:horizontal{background: rgba(0, 0, 0,100);height:%2px;border: 0px solid #FFFFFF;border-image:url(:/image/images/pd400_right_slider_bgimg.png);}"
//                                "QSlider::handle:horizontal{width:%1px;height:%1px;margin-top: -%2px;margin-left: 0px;margin-bottom: -%2px;margin-right: 0px;border-image:url(:/image/images/pd400_round.png);}"
//                                "QSlider::sub-page:horizontal{background-color:rgba(0,0,0,100);border-radius: %2px;border-image:url(:/image/images/pd400_right_addslider_disabled.png);}").arg(12*m_Magnification).arg(4*m_Magnification);
//    QString slider_stys=QString("QSlider::groove:horizontal{background: rgba(0, 0, 0,100);height:%2px;border: 0px solid #FFFFFF;border-image:url(:/image/images/pd400_right_slider_bgimg.png);}"
//                                "QSlider::handle:horizontal{width:%1px;height:%1px;margin-top: -%2px;margin-left: 0px;margin-bottom: -%2px;margin-right: 0px;border-image:url(:/image/images/pd400_round.png);}"
//                                "QSlider::sub-page:horizontal{background-color:rgba(0,0,0,100);border-radius: %2px;border-image:url(:/image/images/pd400_right_addslider_bgimg.png);}").arg(12*m_Magnification).arg(4*m_Magnification);
    if(btn->objectName()=="100"){//限制器
        if(status==0){
            sliders.at(0)->setSliderDisable(true);
            m_commin_fun.pd400_enble_limiter(0);
//            sliders.at(0)->setStyleSheet(slider_dis_stys);
        }else{//打开
            sliders.at(0)->setSliderDisable(false);
            m_commin_fun.pd400_enble_limiter(1);            
//            sliders.at(0)->setStyleSheet(slider_stys);
        }
    }else{//压缩器
        if(status==0){
            sliders.at(1)->setSliderDisable(true);
            m_commin_fun.pd400_enable_comp(0);
//            sliders.at(1)->setStyleSheet(slider_dis_stys);
        }else{
            sliders.at(1)->setSliderDisable(false);
            m_commin_fun.pd400_enable_comp(1);
//            sliders.at(1)->setStyleSheet(slider_stys);
        }
    }
}
void pm481_rightview::change_limiter_comp(int slider_value){

    MySlider *mic_xianfu_yaxian_slider = dynamic_cast<MySlider *>(sender());

    if(mic_xianfu_yaxian_slider->objectName() =="100"){
        m_comp_limiter_lables.at(0)->setText(QString::number(slider_value-40,10)+"dB");
        m_PM481slider_current_value = slider_value;
        m_PM481slider_current_number = 4;
        //发送数据
        m_commin_fun.pd400_limiter(slider_value);
        //pd400_send_value = slider_value;
        DM30_send_value.insert(QString::number((PARAM_MIC_LIMIT_START & 0xff)+ 1,16),slider_value);
        m_PM481Timer->stop();
        m_PM481Timer->start(40);

    }else{
        m_comp_limiter_lables.at(1)->setText(QString::number(slider_value-80,10)+"dB");
        m_PM481slider_current_value = slider_value;
        m_PM481slider_current_number = 5;
        //发送数据
        m_commin_fun.pd400_comp(slider_value);
        //pd400_send_value = slider_value;
        DM30_send_value.insert(QString::number((PARAM_MIC_COMP_START & 0xff) +1,16),slider_value);
        m_PM481Timer->stop();
        m_PM481Timer->start(40);
    }
}
void pm481_rightview::limiter_comp_release_value(int slider_value){
//    MySlider *mic_xianfu_yaxian_slider = dynamic_cast<MySlider *>(sender());

//    QElapsedTimer time;
//    time.start();
//    while(time.elapsed() < 40)  {
//        if(mic_xianfu_yaxian_slider->objectName() =="100"){
//            m_commin_fun.pd400_limiter(slider_value);
//            pd400_send_value = slider_value;
//        }else{
//            m_commin_fun.pd400_comp(slider_value);
//            pd400_send_value = slider_value;
//        }
//    }
}
