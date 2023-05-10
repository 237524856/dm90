
#pragma once
#pragma execution_character_set("utf-8")
#include "pd400_rightview.h"
#include <QLabel>
#include <QVector>
#include <QDebug>
#include "pd400_xianzhiqi_view.h"
#include "pd400_yasuoqi_view.h"
#include <QStyle>
#include <QElapsedTimer>
#include <QTimer>
#include <QThread>

#define ADDMICY 60

pd400_rightview::pd400_rightview(QWidget *parent) : QWidget(parent)
{
    mic_slider_values={};
    m_eq_titles = {tr("平坦"),tr("低切"),tr("中频增强"),tr("低切&中频增强")};
    m_current_btn_select = "200";
    //计时器
    m_DM30Timer = new QTimer(this);
    connect(m_DM30Timer, SIGNAL(timeout()), this, SLOT(handleTimeout()));
}
void pd400_rightview::createUI(){

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
    m_micmute_label->move(36*m_Magnification,20*m_Magnification);
    m_micmute_label->resize(93*m_Magnification,24*m_Magnification);
    m_micmute_label->setText(tr("麦克风静音"));
    m_micmute_label->setPalette(pa);
    m_micmute_label->setFont(font);

    m_mute_btn= new custom_checkbox(this);
    m_mute_btn->resize(40*m_Magnification,40*m_Magnification);
    m_mute_btn->move(36*m_Magnification,53*m_Magnification);
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
    dianping_lab_bg->move((75+16)*m_Magnification,(73-6)*m_Magnification);
    dianping_lab_bg->setPixmap(QPixmap(":/image/images/pd400_dianping_fon.png"));
    dianping_lab_bg->setScaledContents(true);

    m_dianping_lab_front = new QLabel(this);
    m_dianping_lab_front->resize(348*m_Magnification,12*m_Magnification);
    m_dianping_lab_front->move((75+16)*m_Magnification,(73-6)*m_Magnification);
    m_dianping_lab_front->setStyleSheet("background-color:white");

    QLabel *dianping_lab = new QLabel(this);
    dianping_lab->resize(348*m_Magnification,12*m_Magnification);
    dianping_lab->move((75+16)*m_Magnification,(73-6)*m_Magnification);
    dianping_lab->setPixmap(QPixmap(":/image/images/pd400_dianping_bg.png"));
    dianping_lab->setScaledContents(true);

    //耳机音量
    m_micvolume_label = new QLabel(this);
    m_micvolume_label->move(36*m_Magnification,106*m_Magnification);
    m_micvolume_label->resize(156*m_Magnification,24*m_Magnification);
    m_micvolume_label->setText(tr("耳机音量"));
    m_micvolume_label->setPalette(pa);
    m_micvolume_label->setFont(font);

    QLabel *micvolume_image = new QLabel(this);
    micvolume_image->move(36*m_Magnification,143*m_Magnification);
    micvolume_image->resize(20*m_Magnification,18*m_Magnification);
    micvolume_image->setPixmap(QPixmap(":/image/images/pd400_micvolume.png"));
    micvolume_image->setScaledContents(true);

    MySlider *mic_volue_slider = new MySlider(this);
    mic_volue_slider->setOrientation(Qt::Horizontal);
    mic_volue_slider->setGeometry(72*m_Magnification,150*m_Magnification,300*m_Magnification,12*m_Magnification);
    mic_volue_slider->setMinimum(0);
    mic_volue_slider->setMaximum(100);
    connect(mic_volue_slider,SIGNAL(valueChanged(int)),this,SLOT(headsetVolume_eq_chanege_value(int)));
    connect(mic_volue_slider,SIGNAL(sliderReleasedAt(int)),this,SLOT(headsetVolume_release_value(int)));

    mic_volue_slider->setDisabled(true);
    m_synmicvolume_Slider_arrs.push_back(mic_volue_slider);


    m_mic_volumecurrent_label = new QLabel(this);
    m_mic_volumecurrent_label->move(395*m_Magnification,140*m_Magnification);
    m_mic_volumecurrent_label->resize(36*m_Magnification,24*m_Magnification);
    m_mic_volumecurrent_label->setText("0");
    m_mic_volumecurrent_label->setPalette(pa);
    //font.setPixelSize(16);
    m_mic_volumecurrent_label->setFont(font);

    for (int i=0;i<2;i++) {
        QLabel*micvolume_bottomlabel = new QLabel(this);
        micvolume_bottomlabel->move(72*m_Magnification+(310-36)*i*m_Magnification,170*m_Magnification);
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
    m_jianting_label->move(36*m_Magnification,(136+ADDMICY)*m_Magnification);
    m_jianting_label->resize(116*m_Magnification,24*m_Magnification);
    m_jianting_label->setText(tr("监听混音比例"));
    m_jianting_label->setPalette(pa);
    m_jianting_label->setFont(font);

    QLabel *mic_image = new QLabel(this);
    mic_image->move(36*m_Magnification,(173+ADDMICY)*m_Magnification);
    mic_image->resize(17*m_Magnification,26*m_Magnification);
    mic_image->setPixmap(QPixmap(":/image/images/pd400_mic.png"));
    mic_image->setScaledContents(true);

    QLabel *mic_mid_image = new QLabel(this);
    mic_mid_image->move(221*m_Magnification,(176+ADDMICY)*m_Magnification);
    mic_mid_image->resize(2*m_Magnification,20*m_Magnification);
    mic_mid_image->setPixmap(QPixmap(":/image/images/pd400_mic_slidermid.png"));
    mic_mid_image->setScaledContents(true);

    //监听比例
    m_mic_slider = new SliderWidget(this);
    m_Magnification<1?m_mic_slider->setGeometry(72*m_Magnification,(180+ADDMICY)*m_Magnification,300*m_Magnification,12):
    m_mic_slider->setGeometry(72*m_Magnification,(180+ADDMICY)*m_Magnification,300*m_Magnification,12*m_Magnification);

    connect(m_mic_slider,SIGNAL(valueChanged(int)),this,SLOT(hunyin_chanege_value(int)));
    connect(m_mic_slider,SIGNAL(sliderReleasedAt(int)),this,SLOT(hunyin_release_value(int)));
    m_mic_slider->setDisabled(true);
    m_pd400SynSlider_arrs.push_back(m_mic_slider);


    QLabel *pc_image = new QLabel(this);
    pc_image->move(398*m_Magnification,(176+60)*m_Magnification);
    pc_image->resize(32*m_Magnification,21*m_Magnification);
    pc_image->setPixmap(QPixmap(":/image/images/pd400_pc.png"));
    pc_image->setScaledContents(true);


    m_eqtitle_lal =new QLabel(this);
    m_eqtitle_lal->move(0,(205+ADDMICY)*m_Magnification);
    m_eqtitle_lal->resize(this->width()-80*m_Magnification,24*m_Magnification);
    QString eqtitle_lal_stys=QString("font-family:'Source Han Sans CN Medium';color:rgb(255,255,255);font-size:%1px;").arg(15*m_Magnification);
    m_eqtitle_lal->setStyleSheet(eqtitle_lal_stys);
    m_eqtitle_lal->setText(m_eq_titles.at(0));
    m_eqtitle_lal->setAlignment(Qt::AlignRight);

    m_junheng_label = new QLabel(this);
    m_junheng_label->move(36*m_Magnification,(205+ ADDMICY)*m_Magnification);
    m_junheng_label->resize(116*m_Magnification,24*m_Magnification);
    m_junheng_label->setText(tr("均衡器"));
    m_junheng_label->setPalette(pa);
    m_junheng_label->setFont(font);

    m_EQ_bgimage = new QLabel(this);
    m_EQ_bgimage->move(36*m_Magnification,(257-15+ ADDMICY)*m_Magnification);
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
                                 "QPushButton:focus{outline: none;}"};
    //最右边跟slider动的LABEL
    for (int i=0;i<eqbtnimgs.count();i++) {
        custom_button *eq_btn = new custom_button(m_EQ_bgimage);
        eq_btn->move(100*i*m_Magnification,0);
        eq_btn->resize(100*m_Magnification,30*m_Magnification);
        eq_btn->setStyleSheet(eqbtnimgs.at(i));
        eq_btn->setObjectName(QString::number(i+200,10));
        connect(eq_btn,SIGNAL(clicked()),this,SLOT(change_eq()));
    }

    m_miceq_label = new QLabel(this);
    m_miceq_label->move(36*m_Magnification,(304-15 + ADDMICY)*m_Magnification);
    m_miceq_label->resize(116*m_Magnification,24*m_Magnification);
    m_miceq_label->setText(tr("麦克风增益"));
    m_miceq_label->setPalette(pa);
    m_miceq_label->setFont(font);

    QVector<QString>titles={"0","100","-40dB","0dB","-80dB","0dB"};
    QVector<QString>maxtitles={"28dB","-20dB","-20dB"};
    QVector<QString>xinfutitles={"",tr("限制器"),tr("压缩器")};
    for (int i=0;i<3;i++) {

        QLabel *max_label = new QLabel(this);
        max_label->move(395*m_Magnification,330*m_Magnification-15*m_Magnification + i*(70+10)*m_Magnification +ADDMICY*m_Magnification);
        max_label->resize(36*m_Magnification,24*m_Magnification);
        //          max_label->setText(maxtitles.at(i));
        max_label->setPalette(pa);
        font.setPixelSize(12*m_Magnification);
        max_label->setFont(font);
        m_current_labels.push_back(max_label);

        MySlider *mic_xianfu_yaxian_slider = new MySlider(this);
        mic_xianfu_yaxian_slider->setOrientation(Qt::Horizontal);

        if(i==1){
            mic_xianfu_yaxian_slider->setMinimum(0);
            mic_xianfu_yaxian_slider->setMaximum(40);
            mic_xianfu_yaxian_slider->setValue(xianfuqi_slider_values.at(0));
            max_label->setText(QString::number(xianfuqi_slider_values.at(0)-40,10)+"dB");
        }else if(i==2){
            mic_xianfu_yaxian_slider->setMinimum(0);
            mic_xianfu_yaxian_slider->setMaximum(80);
            mic_xianfu_yaxian_slider->setValue(yaxianqi_slider_values.at(0));
            max_label->setText(QString::number(xianfuqi_slider_values.at(0)-80,10)+"dB");
        }else{
            mic_xianfu_yaxian_slider->setMinimum(0);
            mic_xianfu_yaxian_slider->setMaximum(100);
            mic_xianfu_yaxian_slider->setValue(pd400_mic_value);
            max_label->setText(QString::number(pd400_mic_value,10));
        }
        mic_xianfu_yaxian_slider->setGeometry(35*m_Magnification,(338-15)*m_Magnification + i*80*m_Magnification+ADDMICY*m_Magnification,350*m_Magnification,12*m_Magnification);
        mic_xianfu_yaxian_slider->setObjectName(QString::number(1000+i,10));
        connect(mic_xianfu_yaxian_slider,SIGNAL(valueChanged(int)),this,SLOT(mic_eq_chanege_value(int)));
        connect(mic_xianfu_yaxian_slider,SIGNAL(sliderReleasedAt(int)),this,SLOT(slider_release_value(int)));
        mic_xianfu_yaxian_slider->setDisabled(true);
        sliders.push_back(mic_xianfu_yaxian_slider);
        pd400ComLimGainSliders.push_back(mic_xianfu_yaxian_slider);

        for (int j=0;j<2;j++) {
            QLabel *mic_label = new QLabel(this);
            mic_label->move(35*m_Magnification+325*j*m_Magnification,345*m_Magnification + i*75*m_Magnification+ADDMICY*m_Magnification);
            mic_label->resize(36*m_Magnification,24*m_Magnification);
            mic_label->setText(titles.at(j + 2*i));
            mic_label->setPalette(pa);
            font.setPixelSize(12*m_Magnification);
            mic_label->setFont(font);
        }

        if(i !=0){//复选框
            custom_checkbox *xianfu_btn= new custom_checkbox(this);
            xianfu_btn->setText(xinfutitles.at(i));
            xianfu_btn->resize(100*m_Magnification,40*m_Magnification);
            xianfu_btn->move(36*m_Magnification , 360*m_Magnification+(i-1)*80*m_Magnification+ADDMICY*m_Magnification);
            QString xianfu_btn_stys=QString("QCheckBox{font-family:'Source Han Sans CN Medium';color:rgb(255,255,255);font-size:%1px;}"
                                            "QCheckBox::hover{font-family:'Source Han Sans CN Medium';color:rgb(54,54,54);font-size:%1px;}"
                                            "QCheckBox::indicator{width:%2px;height:%2px;color: rgb(255, 0, 0);}"
                                            "QCheckBox::indicator:unchecked{border-image:url(:/image/images/pd400_xianfuunclick_btn.png.png);}"
                                            "QCheckBox::indicator:checked{border-image:url(:/image/images/pd400_xianfuclick_btn.png);}"
                                            "QCheckBox:focus{outline: none;}").arg((int)(14*m_Magnification)).arg(10*m_Magnification);

            xianfu_btn->setStyleSheet(xianfu_btn_stys);
            xianfu_btn->setCheckState(Qt::Checked);
            xianfu_btn->setObjectName(QString::number(100+i,10));
            connect(xianfu_btn,SIGNAL(stateChanged(int)),this,SLOT(isopen_btn_click(int)));
            enable_btns.push_back(xianfu_btn);

            QLabel *xianfu_label = new QLabel(this);
            xianfu_label->move(55*m_Magnification,360*m_Magnification +  (i-1)*80*m_Magnification+ADDMICY*m_Magnification);
            xianfu_label->resize(116*m_Magnification,24*m_Magnification);
            xianfu_label->setText(xinfutitles.at(i));
            xianfu_label->setPalette(pa);
            xianfu_label->setFont(font);
            xianfuyaxian_lbs.push_back(xianfu_label);
            xianfu_label->hide();

            custom_button *select_btn = new custom_button(this);
            select_btn->resize(21*m_Magnification,40*m_Magnification);
            select_btn->move(395*m_Magnification , 360*m_Magnification+(i-1)*80*m_Magnification+ADDMICY*m_Magnification);

            select_btn->setStyleSheet("QPushButton{border-image:url(:/image/images/pd400_sandian_btn.png) 0 21 0 0}"
                                      "QPushButton:hover{border-image:url(:/image/images/pd400_sandian_btn.png) 0 0 0 21}"
                                      "QPushButton:disabled{border-image:url(:/image/images/pd400_sandian_btn.png) 0 0 0 21}"
                                      "QPushButton:focus{outline: none;}"
                                      );
            select_btn->setObjectName(QString::number(100+i,10));
            connect(select_btn,SIGNAL(clicked()),this,SLOT(select_btn_click()));
            select_btns.push_back(select_btn);
        }

    }
    m_mengban_image = new QLabel(this);
    m_mengban_image->move(0,0);
    m_mengban_image->resize(this->width(),this->height());
    m_mengban_image->setPixmap(QPixmap(":/image/images/pd400_mengban.png"));
    m_mengban_image->setScaledContents(true);
    //m_mengban_image->hide();
}
void pd400_rightview:: mute_btn_click(int status){
    QCheckBox *right_mute_btn = m_mute_arrs.at(0);
    if(status==0){
        right_mute_btn->setChecked(false);
    }else{
        right_mute_btn->setChecked(true);
    }
    qDebug()<<"xxsssss"<<status;
}

void pd400_rightview::select_btn_click(){

    QPushButton *btn = dynamic_cast<QPushButton *>(sender());
    if(btn->objectName() == "101"){//限幅器

        emit show_limitOrcomp_view(1);
        //        pd400_xianzhiqi_view *show_xianfuView = new pd400_xianzhiqi_view();
        //        connect(show_xianfuView,SIGNAL(close_xianfuqi_view()),this,SLOT(load_xianfuqi_data()));
        //        show_xianfuView->exec();

    }else{//压缩器
        emit show_limitOrcomp_view(2);
        //        pd400_yasuoqi_view *yasuoqi_view = new pd400_yasuoqi_view();
        //        connect(yasuoqi_view,SIGNAL(close_yasuoqi_view()),this,SLOT(load_yasuoqi_data()));
        //        yasuoqi_view->exec();
    }
}
void pd400_rightview::handleTimeout(){
    //emit DM30_timer_out();
    if(m_slider_current_number==1){
        m_commin_fun.pd400_headset_volume(m_slider_current_value);
        PD400_send_value.insert(QString::number(PARAM_HP_GAIN & 0xff,16),m_slider_current_value);
    }else if(m_slider_current_number==2){
        m_commin_fun.pd400_monitor_mix(m_slider_current_value);

        PD400_send_value.insert(QString::number(PARAM_LR_MONITOR & 0xff,16),m_slider_current_value);
    }else if(m_slider_current_number==3){
        m_commin_fun.pd400_mic_gain(m_slider_current_value);
        qDebug()<<"best-------------------"<<m_slider_current_value;
        PD400_send_value.insert(QString::number(PARAM_MIC_GAIN & 0xff,16),m_slider_current_value);
    }else if(m_slider_current_number==4){
        m_commin_fun.pd400_limiter(m_slider_current_value);
        PD400_send_value.insert(QString::number((PARAM_MIC_LIMIT_START & 0xff)+ 1,16),m_slider_current_value);
    }else if(m_slider_current_number==5){
        m_commin_fun.pd400_comp(m_slider_current_value);
        PD400_send_value.insert(QString::number((PARAM_MIC_COMP_START & 0xff) +1,16),m_slider_current_value);
    }
    m_DM30Timer->stop();
}
void pd400_rightview::headsetVolume_eq_chanege_value(int slidervalue){

    m_mic_volumecurrent_label->setText(QString::number(slidervalue,10));
    MySlider *slider = m_synmicvolume_Slider_arrs.at(0);
    //m_mic_slider->blockSignals(true);
    slider->setValue(slidervalue);
    //m_mic_slider->blockSignals(false);
    m_slider_current_value = slidervalue;
    m_slider_current_number = 1;
    m_commin_fun.pd400_headset_volume(slidervalue);
    //pd400_send_value = slidervalue;
    PD400_send_value.insert(QString::number(PARAM_HP_GAIN & 0xff,16),slidervalue);
    m_DM30Timer->stop();
    if(m_pd400current_model==2)
        m_DM30Timer->start(40);
    m_PD400phsilder_runing =true;
}
void pd400_rightview::headsetVolume_release_value(int slidervalue){
    MySlider *micVolume_slider = m_synmicvolume_Slider_arrs.at(1);
    micVolume_slider->blockSignals(true);
    QMap<QString,int>::iterator send_value_iter = PD400_send_value.find(QString::number((PARAM_HP_GAIN & 0xff),16));
    micVolume_slider->setValue(send_value_iter.value());
    m_synmicvolume_Slider_arrs.at(0)->setValue(send_value_iter.value());
    m_mic_volumecurrent_label->setText(QString::number(send_value_iter.value(),10));
    micVolume_slider->blockSignals(false);
    m_PD400phsilder_runing =false;
    //    QElapsedTimer time;
    //    time.start();
    //    while(time.elapsed() < 40)  {//等待200MS接收数据
    //        m_commin_fun.pd400_headset_volume(slidervalue);
    //        pd400_send_value = slidervalue;
    //    }
}
//监听混音比例
void pd400_rightview::hunyin_chanege_value(int slidervalue){

    qDebug()<<"value======="<<slidervalue;

    m_slider_current_value = slidervalue;
    m_slider_current_number = 2;
    m_commin_fun.pd400_monitor_mix(slidervalue);
    //pd400_send_value = slidervalue;
    PD400_send_value.insert(QString::number(PARAM_LR_MONITOR & 0xff,16),slidervalue);
    //同步右边monitor mix
    SliderWidget *slider = m_pd400SynSlider_arrs.at(0);
    //m_mic_slider->blockSignals(true);
    slider->SetValue(slidervalue);
    //m_mic_slider->blockSignals(false);
    m_DM30Timer->stop();
    if(m_current_model==2)
        m_DM30Timer->start(40);
    m_PD400mixsilder_runing = true;
}
void pd400_rightview::hunyin_release_value(int slidervalue){
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
void pd400_rightview::mic_eq_chanege_value(int myvalue){

    MySlider *slider = dynamic_cast<MySlider *>(sender());
    if(slider->objectName()=="1000"){//麦克风增益
        pd400_mic_value = myvalue;
        m_current_labels.at(0)->setText(QString::number(pd400_mic_value,10));
        m_slider_current_value = myvalue;
        m_slider_current_number = 3;
        m_commin_fun.pd400_mic_gain(myvalue);
        //pd400_send_value = myvalue;
        PD400_send_value.insert(QString::number(PARAM_MIC_GAIN & 0xff,16),myvalue);
        m_DM30Timer->stop();
        m_DM30Timer->start(40);
        m_PD400gainsilder_runing = true;

    }else if(slider->objectName()=="1001"){//限制器
        xianfuqi_slider_values.replace(0,myvalue);
        m_current_labels.at(1)->setText(QString::number(myvalue-40,10)+"dB");
        m_slider_current_value = myvalue;
        m_slider_current_number = 4;
        m_commin_fun.pd400_limiter(myvalue);
        //pd400_send_value = myvalue;
        PD400_send_value.insert(QString::number((PARAM_MIC_LIMIT_START & 0xff)+ 1,16),myvalue);
        m_DM30Timer->stop();
        m_DM30Timer->start(40);
    }else{
        yaxianqi_slider_values.replace(0,myvalue);
        m_current_labels.at(2)->setText(QString::number(myvalue-80,10)+"dB");
        m_slider_current_value = myvalue;
        m_slider_current_number = 5;
        m_commin_fun.pd400_comp(myvalue);
        //pd400_send_value = myvalue;
        PD400_send_value.insert(QString::number((PARAM_MIC_COMP_START & 0xff) +1,16),myvalue);
        m_DM30Timer->stop();
        m_DM30Timer->start(40);
    }

    //qDebug()<<"kk"<<myvalue;
}

void pd400_rightview::slider_release_value(int myvalue){

    MySlider *slider = dynamic_cast<MySlider *>(sender());
    if(slider->objectName()=="1000"){
        slider->blockSignals(true);
        QMap<QString,int>::iterator send_value_iter = PD400_send_value.find(QString::number((PARAM_MIC_GAIN & 0xff),16));
        slider->setValue(send_value_iter.value());
        m_current_labels.at(0)->setText(QString::number(send_value_iter.value(),10));
        slider->blockSignals(false);
        m_PD400gainsilder_runing =false;
    }

    //    QElapsedTimer time;
    //    time.start();
    //    while(time.elapsed() < 40)  {//等待200MS接收数据
    //        if(slider->objectName()=="1000"){//麦克风增益
    //            pd400_mic_value = myvalue;
    //            m_commin_fun.pd400_mic_gain(myvalue);
    //            pd400_send_value = myvalue;

    //        }else if(slider->objectName()=="1001"){//限制器
    //            xianfuqi_slider_values.replace(0,myvalue);
    //            m_commin_fun.pd400_limiter(myvalue);
    //            pd400_send_value = myvalue;
    //        }else{
    //            yaxianqi_slider_values.replace(0,myvalue);
    //            m_commin_fun.pd400_comp(myvalue);
    //            pd400_send_value = myvalue;
    //        }
    //    }
}
void pd400_rightview::change_eq(){

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
//是否打开各种限制器压缩器
void pd400_rightview::isopen_btn_click(int status){
    QCheckBox *btn = dynamic_cast<QCheckBox *>(sender());

    if(btn->objectName()=="101"){//限制器
        if(status==0){
            select_btns.at(0)->setDisabled(true);
            sliders.at(1)->setSliderDisable(true);
            m_commin_fun.pd400_enble_limiter(0);
        }else{//打开
            select_btns.at(0)->setDisabled(false);
            sliders.at(1)->setSliderDisable(false);
            m_commin_fun.pd400_enble_limiter(1);
        }
    }else{//压缩器
        if(status==0){
            select_btns.at(1)->setDisabled(true);
            sliders.at(2)->setSliderDisable(true);
            m_commin_fun.pd400_enable_comp(0);
        }else{
            select_btns.at(1)->setDisabled(false);
            sliders.at(2)->setSliderDisable(false);
            m_commin_fun.pd400_enable_comp(1);
        }
    }
    qDebug()<<status;
}
//同步限制器的SLIDER
void pd400_rightview::load_xianfuqi_data(){

    MySlider *slider = sliders.at(1);
    slider->blockSignals(true);
    slider->setValue(xianfuqi_slider_values.at(0));
    slider->blockSignals(false);
    m_current_labels.at(1)->setText(QString::number(slider->value()-40,10)+"dB");
}
//同步压缩器的SLIDER
void pd400_rightview::load_yasuoqi_data(){
    MySlider *slider = sliders.at(2);
    slider->blockSignals(true);
    slider->setValue(yaxianqi_slider_values.at(0));
    slider->blockSignals(false);
    m_current_labels.at(2)->setText(QString::number(slider->value()-80,10)+"dB");

}

//获取背景参数刷新UI
void pd400_rightview::refresh_ui(int mute,int mix,int eq,int gain,int limit,int comp,int limit_enable,int com_enable,uint8_t micVolume){
    if(mute==0){//非静音
        m_mute_btn->setChecked(false);
    }else{
        m_mute_btn->setChecked(true);
    }
    int mix_slider_value = (mix-2000) /10 +100;
    m_mic_slider->blockSignals(true);
    m_mic_slider->SetValue(mix_slider_value);

    m_mic_slider->blockSignals(false);
    //设置耳机音量的SLIDER
    MySlider *micVolume_slider = m_synmicvolume_Slider_arrs.at(1);
    micVolume_slider->blockSignals(true);
    micVolume_slider->setValue(micVolume);
    micVolume_slider->blockSignals(false);
    m_mic_volumecurrent_label->setText(QString::number(micVolume,10));
    //设置均衡器
    m_current_btn_select = QString::number(eq +200,10);
    if(eq==0){
        m_EQ_bgimage->setPixmap(QPixmap(":/image/images/pd400_eq1_bgimg.png"));
        m_eqtitle_lal->setText(tr("平坦"));
    }else if(eq==1){
        m_EQ_bgimage->setPixmap(QPixmap(":/image/images/pd400_eq2_bgimg.png"));
        m_eqtitle_lal->setText(tr("低切"));
    }else if(eq==2){
        m_EQ_bgimage->setPixmap(QPixmap(":/image/images/pd400_eq3_bgimg.png"));
        m_eqtitle_lal->setText(tr("中频增强"));
    }else{
        m_EQ_bgimage->setPixmap(QPixmap(":/image/images/pd400_eq4_bgimg.png"));
        m_eqtitle_lal->setText(tr("低切&中频增强"));
    }

    //设置增益
    sliders.at(0)->blockSignals(true);
    sliders.at(0)->setValue(gain);
    sliders.at(0)->blockSignals(false);

    //设置各种slider的值
    int limit_comp_value = 0;
    for (int i=1;i<3;i++) {
        sliders.at(i)->blockSignals(true);
        if(i==1){
            limit_comp_value = (limit-2000) /10 +40;
        }else{
            limit_comp_value = (comp-2000) /10 +80;
        }

        sliders.at(i)->setValue(limit_comp_value);

        sliders.at(i)->blockSignals(false);
    }
    m_current_labels.at(0)->setText(QString::number(gain,10));
    m_current_labels.at(1)->setText(QString::number((limit-2000) /10,10)+"dB");
    m_current_labels.at(2)->setText(QString::number((comp-2000) /10,10)+"dB");
    //设置2个使能
    if(limit_enable ==0){
        enable_btns.at(0)->blockSignals(true);
        enable_btns.at(0)->setCheckState(Qt::Unchecked);
        enable_btns.at(0)->blockSignals(false);
        //不同的使能禁点
        select_btns.at(0)->setDisabled(true);
        sliders.at(1)->setSliderDisable(true);
    }else{
        enable_btns.at(0)->blockSignals(true);
        enable_btns.at(0)->setCheckState(Qt::Checked);
        enable_btns.at(0)->blockSignals(false);
    }

    if(com_enable==0){
        enable_btns.at(1)->blockSignals(true);
        enable_btns.at(1)->setCheckState(Qt::Unchecked);
        enable_btns.at(1)->blockSignals(false);
        select_btns.at(1)->setDisabled(true);
        sliders.at(2)->setSliderDisable(true);
    }else{
        enable_btns.at(1)->blockSignals(true);
        enable_btns.at(1)->setCheckState(Qt::Checked);
        enable_btns.at(1)->blockSignals(false);

    }

}

void pd400_rightview::sys_receive_data(uint8_t address,int value){

    if(address ==(PARAM_MIC_MUTE & 0xff)){
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
        //qDebug()<<"打印数据22------"<<(value-2000)/10 +max_value;

        int myvalue = ((value-2000)/10 + max_value);
        if(myvalue>=max_value){
            myvalue=max_value;
            m_dianping_lab_front->hide();
        }
        //qDebug()<<"打印数据22------"<<myvalue<<(value-2000)/10;

        m_dianping_lab_front->move((75+16)*m_Magnification + (349*m_Magnification/max_value *myvalue),(73-6)*m_Magnification);
        m_dianping_lab_front->resize(349*m_Magnification-349*m_Magnification/max_value *myvalue,12*m_Magnification);
        if((m_dianping_lab_front->x()<=(75+16)*m_Magnification)){

            m_dianping_lab_front->move((75+16)*m_Magnification,(73-6)*m_Magnification);
            m_dianping_lab_front->resize(349*m_Magnification ,12*m_Magnification);
        }

    }
    else if(address == (PARAM_LR_MONITOR & 0xff)){//混响

        int mix_slider_value = (value-2000) /10 +100;
        //qDebug()<<"打印数据3333------"<<mix_slider_value;
        m_mic_slider->blockSignals(true);
        if(!m_PD400mixsilder_runing){
            m_mic_slider->SetValue(mix_slider_value);
        }
        m_mic_slider->blockSignals(false);
    }else if(address == (PARAM_MIC_GAIN & 0xff)){//麦克风增益
        sliders.at(0)->blockSignals(true);
        if(!m_PD400gainsilder_runing){
            sliders.at(0)->setValue(value);
            m_current_labels.at(0)->setText(QString::number(value,10));
        }

        sliders.at(0)->blockSignals(false);
        PD400_send_value.insert(QString::number(PARAM_MIC_GAIN & 0xff,16),value);
        PD400_different_value.insert(QString::number(PARAM_MIC_GAIN & 0xff,16),value);
        pd400_mic_value = value;

    }else if(address == (PARAM_HP_GAIN & 0xff)){//耳机音量

        MySlider *micVolume_slider = m_synmicvolume_Slider_arrs.at(1);
        micVolume_slider->blockSignals(true);
        if(!m_PD400phsilder_runing){
            micVolume_slider->setValue(value);
            m_mic_volumecurrent_label->setText(QString::number(value,10));
        }
        micVolume_slider->blockSignals(false);

    }else if(address == (PARAM_MIC_PEQ & 0xff)){//均衡器
        if(value==0){
            m_EQ_bgimage->setPixmap(QPixmap(":/image/images/pd400_eq1_bgimg.png"));
            m_eqtitle_lal->setText(tr("平坦"));
        }else if(value==1){
            m_EQ_bgimage->setPixmap(QPixmap(":/image/images/pd400_eq2_bgimg.png"));
            m_eqtitle_lal->setText(tr("低切"));
        }else if(value==2){
            m_EQ_bgimage->setPixmap(QPixmap(":/image/images/pd400_eq3_bgimg.png"));
            m_eqtitle_lal->setText(tr("中频增强"));
        }else{
            m_EQ_bgimage->setPixmap(QPixmap(":/image/images/pd400_eq4_bgimg.png"));
            m_eqtitle_lal->setText(tr("低切&中频增强"));
        }
        m_current_btn_select = QString::number(value + 200,10);
    }
}
//bool pd400_rightview::eventFilter(QObject *obj, QEvent *event){

//        qDebug()<<"打印数据22-xxx-----";

//        if(obj==m_synmicvolume_Slider_arrs.at(1))
//            {
//                if (event->type()==QEvent::MouseButtonPress)           //判断类型
//                {
//                    QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
//                    if (mouseEvent->button() == Qt::LeftButton)	//判断左键
//                    {
//                       int value = QStyle::sliderValueFromPosition(m_synmicvolume_Slider_arrs.at(1)->minimum(), m_synmicvolume_Slider_arrs.at(1)->maximum(), mouseEvent->pos().x(), m_synmicvolume_Slider_arrs.at(1)->width());
//                       m_synmicvolume_Slider_arrs.at(1)->setValue(value);
//                    }
//                }
//            }

//    return QObject::eventFilter(obj,event);
//}
void pd400_rightview::headsetVolume_eq_press_value(){

    qDebug()<<"XXXXXXXXXXKKKKKKmouseMoveEvent";
}
void pd400_rightview::setHideDianping(){

    m_dianping_lab_front->resize(348*m_Magnification,12*m_Magnification);
    m_dianping_lab_front->move((75+16)*m_Magnification,(73-6)*m_Magnification);
    m_dianping_lab_front->show();
}
void pd400_rightview::change_language(){

    m_micmute_label->setText(tr("麦克风静音"));
    m_jianting_label->setText(tr("监听混音比例"));
    m_junheng_label->setText(tr("均衡器"));
    m_miceq_label->setText(tr("麦克风增益"));
    enable_btns.at(0)->setText(tr("限制器"));
    enable_btns.at(1)->setText(tr("压缩器"));
    m_micvolume_label->setText(tr("耳机音量"));
    if(m_current_btn_select=="200"){
        m_eqtitle_lal->setText(tr("平坦"));
    }else if(m_current_btn_select=="201"){
        m_eqtitle_lal->setText(tr("低切"));
    }else if(m_current_btn_select=="202"){
        m_eqtitle_lal->setText(tr("中频增强"));
    }else{
        m_eqtitle_lal->setText(tr("低切&中频增强"));
    }
    // m_eqtitle_lal->setText(m_eq_titles.at(0));
}
