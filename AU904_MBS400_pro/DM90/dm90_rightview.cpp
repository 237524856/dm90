
#pragma once
#pragma execution_character_set("utf-8")
#include "dm90_rightview.h"
#include "custom_checkbox.h"
#include <QDebug>
#include "signalinstance.h"
#include "window_dm90.h"

#define GIGHDENGFENK    20000

dm90_rightview::dm90_rightview(QWidget *parent) : QWidget(parent)
{

    this->setAttribute(Qt::WA_StyledBackground,true);
    connect(&SignalInstance::GetInstance(),SIGNAL(changeResultTitle(int)),this,SLOT(receiveResult(int)));

}

void dm90_rightview::createUI(){

    //录音的视图
    m_recorderView = new dm90_recoderview(this);
    m_recorderView->move(0,0);
    m_recorderView->resize(this->width(),100*m_Magnification);
    m_recorderView->createUI();
    com_recordView = m_recorderView;

    QLabel *micGainLal = new QLabel(this);
    micGainLal->move(20*m_Magnification,100*m_Magnification);
    micGainLal->resize(100*m_Magnification,20*m_Magnification);
    micGainLal->setText(tr("输入增益"));
    QString micGainLal_stys = QString("background-color:transparent;color:rgb(255,255,255);font-size:%1px;font-family:'Source Han Sans CN Medium'").arg(14*m_Magnification);
    micGainLal->setStyleSheet(micGainLal_stys);

    MySlider *micGain_slider = new MySlider(this);
    micGain_slider->setOrientation(Qt::Horizontal);
    micGain_slider->setMinimum(0);
    micGain_slider->setMaximum(100);
    micGain_slider->setValue(xianfuqi_slider_values.at(0));
    micGain_slider->setGeometry(70*m_Magnification,micGainLal->y()+micGainLal->height()+20*m_Magnification,570*m_Magnification,12*m_Magnification);
    micGain_slider->setObjectName("300");
    connect(micGain_slider,SIGNAL(valueChanged(int)),this,SLOT(rightViewSlider_chanege_value(int)));
    connect(micGain_slider,SIGNAL(sliderReleasedAt(int)),this,SLOT(rightViewSlider_release_value(int)));
    m_horSliders.push_back(micGain_slider);

    //左边的静音按钮
    m_mute_btn= new custom_checkbox(this);
    m_mute_btn->resize(28*m_Magnification,28*m_Magnification);
    m_mute_btn->move(20*m_Magnification,micGainLal->y()+micGainLal->height()+10*m_Magnification);
    QString mute_bt_stys=QString("QCheckBox::indicator{width: %1px;height: %1px;color: rgb(255, 0, 0);}"
                                 "QCheckBox::indicator:unchecked{border-image:url(:/image/images/DM90/dm90_mic);}"
                                 "QCheckBox::indicator:unchecked:hover{border-image:url(:/image/images/DM90/dm90_mic);}"
                                 "QCheckBox::indicator:checked{border-image:url(:/image/images/DM90/DM90_mute);}"
                                 "QCheckBox:focus{outline: none;}").arg(28*m_Magnification);
    m_mute_btn->setStyleSheet(mute_bt_stys);
    connect(m_mute_btn,SIGNAL(stateChanged(int)),this,SLOT(mute_btn_click(int)));
    //左边的的数字
    QLabel *micGain_leftValue = new QLabel(this);
    micGain_leftValue->move(m_mute_btn->x()+m_mute_btn->height()+5*m_Magnification,micGain_slider->y()-2*m_Magnification);
    micGain_leftValue->resize(10*m_Magnification,14*m_Magnification);
    micGain_leftValue->setText("0");
    QString currentLabel_stys = QString("background-color:transparent;color:rgb(255,255,255);font-size:%1px;font-family:'Source Han Sans CN Medium'").arg(12*m_Magnification);
    micGain_leftValue->setStyleSheet(currentLabel_stys);

    //显示当前得值
    QLabel *micGain_value = new QLabel(this);
    micGain_value->move(micGain_slider->width()+micGain_slider->x()+10*m_Magnification,micGain_slider->y()-2*m_Magnification);
    micGain_value->resize(60*m_Magnification,14*m_Magnification);
    micGain_value->setText("100");
    micGain_value->setStyleSheet(currentLabel_stys);
    m_currentValues.push_back(micGain_value);

    //输出增益电平条
    QLabel *level_leftLabel = new QLabel(this);
    level_leftLabel->move(micGainLal->x(),micGain_slider->y()+micGain_slider->height()+20*m_Magnification);
    level_leftLabel->resize(50*m_Magnification,14*m_Magnification);
    level_leftLabel->setText("-100dB");
    level_leftLabel->setStyleSheet(currentLabel_stys);

    QLabel *level_rightLabel = new QLabel(this);
    level_rightLabel->move(micGain_value->x(),micGain_slider->y()+micGain_slider->height()+20*m_Magnification);
    level_rightLabel->resize(50*m_Magnification,14*m_Magnification);
    level_rightLabel->setText("0dB");
    level_rightLabel->setStyleSheet(currentLabel_stys);

    //输入电平的进度条---------------------------
    QLabel *input_dianping_lab_bg = new QLabel(this);
    input_dianping_lab_bg->resize(569*m_Magnification,11*m_Magnification);
    input_dianping_lab_bg->move(level_leftLabel->x()+50*m_Magnification,level_leftLabel->y());
    input_dianping_lab_bg->setPixmap(QPixmap(":/image/images/DM90/dm90_dianping_fon"));
    input_dianping_lab_bg->setScaledContents(true);
    input_dianping_lab_bg->setStyleSheet("background:transparent");

    m_inputDianping_moveLab = new QLabel(this);
    m_inputDianping_moveLab->resize(569*m_Magnification,11*m_Magnification);
    m_inputDianping_moveLab->move(level_leftLabel->x()+50*m_Magnification,level_leftLabel->y());
    m_inputDianping_moveLab->setStyleSheet("background-color:white");

    QLabel *intput_dianping_lab = new QLabel(this);
    intput_dianping_lab->resize(569*m_Magnification,11*m_Magnification);
    intput_dianping_lab->move(level_leftLabel->x()+50*m_Magnification,level_leftLabel->y());
    intput_dianping_lab->setPixmap(QPixmap(":/image/images/DM90/dm90_dianping_bg"));
    intput_dianping_lab->setScaledContents(true);
    intput_dianping_lab->setStyleSheet("background:transparent");

    //主输出音量
    QLabel *volumeLal = new QLabel(this);
    volumeLal->move(20*m_Magnification,200*m_Magnification);
    volumeLal->resize(200*m_Magnification,20*m_Magnification);
    volumeLal->setText(tr("主输出音量"));
    QString volumeLal_stys = QString("background-color:transparent;color:rgb(255,255,255);font-size:%1px;font-family:'Source Han Sans CN Medium'").arg(14*m_Magnification);
    volumeLal->setStyleSheet(micGainLal_stys);

    MySlider *mainvolume_slider = new MySlider(this);
    mainvolume_slider->setOrientation(Qt::Horizontal);
    mainvolume_slider->setMinimum(0);
    mainvolume_slider->setMaximum(100);
    mainvolume_slider->setValue(xianfuqi_slider_values.at(0));
    mainvolume_slider->setGeometry(70*m_Magnification,volumeLal->y()+volumeLal->height()+10*m_Magnification,570*m_Magnification,12*m_Magnification);
    mainvolume_slider->setObjectName("301");
    connect(mainvolume_slider,SIGNAL(valueChanged(int)),this,SLOT(rightViewSlider_chanege_value(int)));
    connect(mainvolume_slider,SIGNAL(sliderReleasedAt(int)),this,SLOT(rightViewSlider_release_value(int)));
    m_horSliders.push_back(mainvolume_slider);

    //左边的的数字
    QLabel *mainvolume_leftValue = new QLabel(this);
    mainvolume_leftValue->move(m_mute_btn->x()+m_mute_btn->height()+5*m_Magnification,mainvolume_slider->y()-2*m_Magnification);
    mainvolume_leftValue->resize(10*m_Magnification,14*m_Magnification);
    mainvolume_leftValue->setText("0");
    mainvolume_leftValue->setStyleSheet(currentLabel_stys);

    //显示当前得值
    QLabel *mainvolume_value = new QLabel(this);
    mainvolume_value->move(mainvolume_slider->width()+mainvolume_slider->x()+10*m_Magnification,mainvolume_slider->y()-2*m_Magnification);
    mainvolume_value->resize(60*m_Magnification,14*m_Magnification);
    mainvolume_value->setText("100");
    mainvolume_value->setStyleSheet(currentLabel_stys);
    m_currentValues.push_back(mainvolume_value);

    //输出音量电平条值
    QLabel *volumeLevel_leftLabel = new QLabel(this);
    volumeLevel_leftLabel->move(micGainLal->x(),mainvolume_slider->y()+mainvolume_slider->height()+20*m_Magnification);
    volumeLevel_leftLabel->resize(50*m_Magnification,14*m_Magnification);
    volumeLevel_leftLabel->setText("-100dB");
    volumeLevel_leftLabel->setStyleSheet(currentLabel_stys);

    QLabel *volumeLevel_rightLabel = new QLabel(this);
    volumeLevel_rightLabel->move(micGain_value->x(),mainvolume_slider->y()+mainvolume_slider->height()+20*m_Magnification);
    volumeLevel_rightLabel->resize(50*m_Magnification,14*m_Magnification);
    volumeLevel_rightLabel->setText("0dB");
    volumeLevel_rightLabel->setStyleSheet(currentLabel_stys);

    //输入电平的进度条---------------------------
    QLabel *output_dianping_lab_bg = new QLabel(this);
    output_dianping_lab_bg->resize(569*m_Magnification,11*m_Magnification);
    output_dianping_lab_bg->move(level_leftLabel->x()+50*m_Magnification,volumeLevel_leftLabel->y());
    output_dianping_lab_bg->setPixmap(QPixmap(":/image/images/DM90/dm90_dianping_fon"));
    output_dianping_lab_bg->setScaledContents(true);
    output_dianping_lab_bg->setStyleSheet("background:transparent");

    m_outputDianping_moveLab = new QLabel(this);
    m_outputDianping_moveLab->resize(569*m_Magnification,11*m_Magnification);
    m_outputDianping_moveLab->move(level_leftLabel->x()+50*m_Magnification,volumeLevel_leftLabel->y());
    m_outputDianping_moveLab->setStyleSheet("background-color:white");

    QLabel *output_dianping_lab = new QLabel(this);
    output_dianping_lab->resize(569*m_Magnification,11*m_Magnification);
    output_dianping_lab->move(level_leftLabel->x()+50*m_Magnification,volumeLevel_leftLabel->y());
    output_dianping_lab->setPixmap(QPixmap(":/image/images/DM90/dm90_dianping_bg"));
    output_dianping_lab->setScaledContents(true);
    output_dianping_lab->setStyleSheet("background:transparent");

    //本地声音 效果之间的切换
    m_localEffectView = new QStackedWidget(this);
    m_localEffectView->resize(this->width(),(270+32)*m_Magnification);
    m_localEffectView->move(0,280*m_Magnification);

    QWidget *localSoundView = new QWidget();
    localSoundView->resize(m_localEffectView->width(),m_localEffectView->height());
    m_localEffectView->addWidget(localSoundView);

    QString eq_singsn_stys = QString("QPushButton {background-color:transparent;color:rgb(255,255,255);text-align:left;font-size:%1px;font-family:'Source Han Sans CN Medium'}"
                                     "QPushButton:hover{background-color:transparent;color:rgb(54,54,54);text-align:left;font-size:%2px;font-family:'Source Han Sans CN Medium'}"
                                     "QPushButton:pressed{background-color:transparent;color:rgb(255,171,33);text-align:left;font-size:%2px;font-family:'Source Han Sans CN Medium'}"
                                     "QPushButton:focus{outline: none;}").arg(15*m_Magnification).arg(15*m_Magnification);

    //均衡器 信号处理器-------------------------
    QVector<QString> eqsingsTitle = {tr("均衡器"),tr("信号处理器")};
    QVector<QString> eqsings_tag = {"eq","signal"};

    for (int i=0;i<2;i++) {

        QPushButton *eq_sings_btn = new QPushButton(localSoundView);
        eq_sings_btn->setStyleSheet(eq_singsn_stys);
        if(i==0){
            eq_sings_btn->resize(80*m_Magnification,30*m_Magnification);
            eq_sings_btn->move(20*m_Magnification+80*i*m_Magnification,0*m_Magnification);
        }else{
            if(language_index==0){
                eq_sings_btn->resize(80*m_Magnification,30*m_Magnification);
                eq_sings_btn->move(20*m_Magnification+80*i*m_Magnification,0*m_Magnification);
            }else{
                eq_sings_btn->resize(120*m_Magnification,30*m_Magnification);
                eq_sings_btn->move(20*m_Magnification+80*i*m_Magnification,0*m_Magnification);
            }
        }
        eq_sings_btn->setText(eqsingsTitle.at(i));
        eq_sings_btn->setObjectName(eqsings_tag.at(i));
        connect(eq_sings_btn, SIGNAL(clicked()), this, SLOT(eq_singsClick()));
    }

    m_titleStatus = new QLabel(localSoundView);
    m_titleStatus->move(20*m_Magnification,30*m_Magnification);
    m_titleStatus->resize(50*m_Magnification,2*m_Magnification);
    m_titleStatus->setStyleSheet("background-color:rgb(255,171,33)");

    //切换的视图
    m_pStackedWidget = new QStackedWidget(localSoundView);
    m_pStackedWidget->move(0,m_titleStatus->y()+10*m_Magnification);
    m_pStackedWidget->resize(this->width(),250*m_Magnification);

    m_eqView = new QWidget();
    m_eqView->move(0,0);
    m_eqView->resize(m_pStackedWidget->width(),m_pStackedWidget->height());
    m_eqView->setStyleSheet("background-color:rgb(17,17,17)");
    m_pStackedWidget->addWidget(m_eqView);

    m_signalView = new QWidget();
    m_signalView->move(0,0);
    m_signalView->resize(m_pStackedWidget->width(),m_pStackedWidget->height());
    m_signalView->setStyleSheet("background-color:rgb(17,17,17)");
    m_pStackedWidget->addWidget(m_signalView);

    m_pStackedWidget->setCurrentIndex(0);

    //均衡器
    custom_checkbox *highpass_btn= new custom_checkbox(m_eqView);
    highpass_btn->setText(tr("  高通滤波器"));
    highpass_btn->resize(108*m_Magnification,25*m_Magnification);
    highpass_btn->move(20*m_Magnification , 10*m_Magnification);
    QString highpass_stys=QString("QCheckBox{font-family:'Source Han Sans CN Medium';color:rgb(255,255,255);font-size:%1px;}"
                                  "QCheckBox::hover{font-family:'Source Han Sans CN Medium';color:rgb(54,54,54);font-size:%1px;}"
                                  "QCheckBox::indicator{width:%2px;height:%2px;color: rgb(255, 0, 0);}"
                                  "QCheckBox::indicator:unchecked{border-image:url(:/image/images/pd400_xianfuunclick_btn.png.png);}"
                                  "QCheckBox::indicator:checked{border-image:url(:/image/images/pd400_xianfuclick_btn.png);}"
                                  "QCheckBox:focus{outline: none;}").arg(14*m_Magnification).arg(10*m_Magnification);
    highpass_btn->setStyleSheet(highpass_stys);
    highpass_btn->setCheckState(Qt::Checked);
    highpass_btn->setObjectName("highFilter");
    connect(highpass_btn,SIGNAL(stateChanged(int)),this,SLOT(isOpenCloseBtn_click(int)));
    open_close_btns.push_back(highpass_btn);

    MySlider *highpass_slider = new MySlider(m_eqView);
    highpass_slider->setOrientation(Qt::Horizontal);
    highpass_slider->setMinimum(0);
    highpass_slider->setMaximum(1000);
    highpass_slider->setValue(xianfuqi_slider_values.at(0));
    highpass_slider->setGeometry(20*m_Magnification,highpass_btn->y()+highpass_btn->height()+10*m_Magnification,280*m_Magnification,12*m_Magnification);

    highpass_slider->setObjectName("302");
    connect(highpass_slider,SIGNAL(valueChanged(int)),this,SLOT(rightViewSlider_chanege_value(int)));
    connect(highpass_slider,SIGNAL(sliderReleasedAt(int)),this,SLOT(rightViewSlider_release_value(int)));
    m_horSliders.push_back(highpass_slider);

    QLabel *EQcurrent_value = new QLabel(m_eqView);
    EQcurrent_value->move(highpass_slider->width()+highpass_slider->x()+10*m_Magnification,highpass_slider->y()-2*m_Magnification);
    EQcurrent_value->resize(60*m_Magnification,14*m_Magnification);
    EQcurrent_value->setText("1000Hz");
    EQcurrent_value->setStyleSheet(currentLabel_stys);
    m_currentValues.push_back(EQcurrent_value);

    QLabel *left_value = new QLabel(m_eqView);
    left_value->resize(90*m_Magnification,14*m_Magnification);
    left_value->move(highpass_slider->x(),highpass_slider->y()+highpass_slider->height()+5*m_Magnification);
    left_value->setText("20Hz");
    left_value->setStyleSheet(currentLabel_stys);

    QLabel *right_value = new QLabel(m_eqView);
    right_value->resize(50*m_Magnification,16*m_Magnification);
    right_value->move(highpass_slider->x()+highpass_slider->width()-right_value->width(),highpass_slider->y()+highpass_slider->height()+5*m_Magnification);
    right_value->setText("1000Hz");
    right_value->setStyleSheet(currentLabel_stys);
    right_value ->setAlignment(Qt::AlignRight);

    //VOICE EQ
    custom_checkbox *voiceq_btn= new custom_checkbox(m_eqView);
    voiceq_btn->setText(tr("  声音均衡器"));
    voiceq_btn->resize(108*m_Magnification,25*m_Magnification);
    voiceq_btn->move(380*m_Magnification , 10*m_Magnification);
    QString voiceq_btn_stys=QString("QCheckBox{font-family:'Source Han Sans CN Medium';color:rgb(255,255,255);font-size:%1px;}"
                                    "QCheckBox::hover{font-family:'Source Han Sans CN Medium';color:rgb(54,54,54);font-size:%1px;}"
                                    "QCheckBox::indicator{width:%2px;height:%2px;color: rgb(255, 0, 0);}"
                                    "QCheckBox::indicator:unchecked{border-image:url(:/image/images/pd400_xianfuunclick_btn.png.png);}"
                                    "QCheckBox::indicator:checked{border-image:url(:/image/images/pd400_xianfuclick_btn.png);}"
                                    "QCheckBox:focus{outline: none;}").arg(14*m_Magnification).arg(10*m_Magnification);
    voiceq_btn->setStyleSheet(voiceq_btn_stys);
    voiceq_btn->setCheckState(Qt::Checked);
    voiceq_btn->setObjectName("voiceq");
    connect(voiceq_btn,SIGNAL(stateChanged(int)),this,SLOT(isOpenCloseBtn_click(int)));
    open_close_btns.push_back(voiceq_btn);

    custom_button *select_btn = new custom_button(m_eqView);
    select_btn->resize(21*m_Magnification,40*m_Magnification);
    select_btn->move(this->width()-50*m_Magnification , 4*m_Magnification);

    select_btn->setStyleSheet("QPushButton{border-image:url(:/image/images/pd400_sandian_btn.png) 0 21 0 0}"
                              "QPushButton:hover{border-image:url(:/image/images/pd400_sandian_btn.png) 0 0 0 21}"
                              "QPushButton:disabled{border-image:url(:/image/images/pd400_sandian_btn.png) 0 0 0 21}"
                              "QPushButton:focus{outline: none;}"
                              );
    connect(select_btn,SIGNAL(clicked()),this,SLOT(voiceEQshow_btn_click()));

    QWidget *dBView = new QWidget(m_eqView);
    dBView->resize(50*m_Magnification,160*m_Magnification);
    dBView->move(voiceq_btn->x(),voiceq_btn->y()+voiceq_btn->height()+34*m_Magnification);
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
    QVector<QString>highmidlow_title = {tr("低"),tr("中"),tr("高")};

    for (int i=0;i<3;i++) {
        //显示的值
        QLabel *dB_label = new QLabel(m_eqView);
        dB_label->resize(40*m_Magnification,14*m_Magnification);
        dB_label->move(dBView->x()+dBView->width()+10*m_Magnification+84*m_Magnification*i,voiceq_btn->y()+voiceq_btn->height()+10*m_Magnification);
        QString updateLabel_stys = QString("background-color:transparent;color:rgb(255,255,255);font-size:%1px;font-family:'Source Han Sans CN Medium'").arg(12*m_Magnification);
        dB_label->setStyleSheet(updateLabel_stys);
        dB_label->setText("-11.6");
        dB_label ->setAlignment(Qt::AlignHCenter);
        m_voiceCurrentValue.push_back(dB_label);

        //创建SLIDER
        SliderWidget *verticalSlider = new SliderWidget(m_eqView);
        verticalSlider->SetValue(120);
        verticalSlider->SetRange(0,240);
        verticalSlider->SetVertical(true);
        verticalSlider->setObjectName(QString::number(i+100,10));
        verticalSlider->setGeometry(dB_label->x()+(dB_label->width()-10*m_Magnification)/2,dB_label->y()+dB_label->height()+8*m_Magnification,12*m_Magnification,dBView->height());
        connect(verticalSlider,SIGNAL(valueChanged(int)),this,SLOT(verticalSlider_setValue(int)));
        //connect(verticalSlider,SIGNAL(sliderReleased()),this,SLOT(verticalSlider_released()));
        m_verSliders.push_back(verticalSlider);

        QLabel *bottom_label = new QLabel(m_eqView);
        bottom_label->resize(40*m_Magnification,14*m_Magnification);
        bottom_label->move(dBView->x()+dBView->width()+10*m_Magnification+84*m_Magnification*i,verticalSlider->y()+verticalSlider->height()+4*m_Magnification);
        //QString updateLabel_stys = QString("background-color:transparent;color:rgb(255,255,255);font-size:%1px;font-family:'Source Han Sans CN Medium'").arg(12*m_Magnification);
        bottom_label->setStyleSheet(updateLabel_stys);
        bottom_label->setText(highmidlow_title.at(i));
        bottom_label ->setAlignment(Qt::AlignHCenter);
    }
    //显示右边的dB
    QWidget *r_dBView = new QWidget(m_eqView);
    r_dBView->resize(50*m_Magnification,160*m_Magnification);
    r_dBView->move(this->width()-r_dBView->width()-20*m_Magnification,voiceq_btn->y()+voiceq_btn->height()+34*m_Magnification);
    r_dBView->setAttribute(Qt::WA_StyledBackground,true);

    for (int i=0;i<3;i++) {
        QLabel *dB_label = new QLabel(r_dBView);
        dB_label->resize(dBView->width(),14*m_Magnification);
        dB_label->move(0,(dBView->height()/3+dB_label->height())*i);
        if(i==2)
            dB_label->move(0,dBView->height()-dB_label->height()-6*m_Magnification);
        QString updateLabel_stys = QString("background-color:transparent;color:rgb(255,255,255);font-size:%1px;font-family:'Source Han Sans CN Medium'").arg(12*m_Magnification);
        dB_label->setStyleSheet(updateLabel_stys);
        dB_label->setText(dB_labels.at(i));
        dB_label ->setAlignment(Qt::AlignHCenter);
    }
    //信号处理器
    createSignalView();
    //效果----------------------------------------
    QWidget *effectView = new QWidget();
    effectView->resize(m_localEffectView->width(),m_localEffectView->height());
    m_localEffectView->addWidget(effectView);

    QVector<QString> effectTitle = {tr("音调"),tr("环境")};
    QVector<QString> effect_tag = {"tone","environment"};

    for (int i=0;i<2;i++) {

        QPushButton *eq_sings_btn = new QPushButton(effectView);
        eq_sings_btn->setStyleSheet(eq_singsn_stys);
        eq_sings_btn->resize(80*m_Magnification,30*m_Magnification);
        eq_sings_btn->move(20*m_Magnification+80*i*m_Magnification,0*m_Magnification);
        eq_sings_btn->setText(effectTitle.at(i));
        eq_sings_btn->setObjectName(effect_tag.at(i));
        connect(eq_sings_btn, SIGNAL(clicked()), this, SLOT(effectClick()));
    }

    m_effectTitleStatus = new QLabel(effectView);
    m_effectTitleStatus->move(20*m_Magnification,30*m_Magnification);
    m_effectTitleStatus->resize(30*m_Magnification,2*m_Magnification);
    m_effectTitleStatus->setStyleSheet("background-color:rgb(255,171,33)");

    //效果器的切换视图
    m_effectStackedWidget = new QStackedWidget(effectView);
    m_effectStackedWidget->move(0,m_effectTitleStatus->y()+10*m_Magnification);
    m_effectStackedWidget->resize(this->width(),250*m_Magnification);
    //音调
    QWidget *toneView = new QWidget();
    toneView->resize(m_effectStackedWidget->width(),m_effectStackedWidget->height());
    toneView->setStyleSheet("background-color:rgb(17,17,17)");
    m_effectStackedWidget->addWidget(toneView);

    custom_checkbox *show_btn= new custom_checkbox(toneView);
    show_btn->setText(tr("  和声"));
    show_btn->resize(108*m_Magnification,25*m_Magnification);
    show_btn->move(20*m_Magnification,10*m_Magnification);
    QString showbtn_stys=QString("QCheckBox{font-family:'Source Han Sans CN Medium';color:rgb(255,255,255);font-size:%1px;}"
                                 "QCheckBox::hover{font-family:'Source Han Sans CN Medium';color:rgb(54,54,54);font-size:%1px;}"
                                 "QCheckBox::indicator{width:%2px;height:%2px;color: rgb(255, 0, 0);}"
                                 "QCheckBox::indicator:unchecked{border-image:url(:/image/images/pd400_xianfuunclick_btn.png.png);}"
                                 "QCheckBox::indicator:checked{border-image:url(:/image/images/pd400_xianfuclick_btn.png);}"
                                 "QCheckBox:focus{outline: none;}").arg(14*m_Magnification).arg(10*m_Magnification);
    show_btn->setStyleSheet(showbtn_stys);
    show_btn->setCheckState(Qt::Checked);
    show_btn->setObjectName("tone");
    connect(show_btn,SIGNAL(stateChanged(int)),this,SLOT(isOpenCloseBtn_click(int)));
    open_close_btns.push_back(show_btn);

    custom_button *toneselect_btn = new custom_button(toneView);
    toneselect_btn->resize(21*m_Magnification,40*m_Magnification);
    toneselect_btn->move(show_btn->x()+show_btn->width()+150*m_Magnification , 0);
    toneselect_btn->setStyleSheet("QPushButton{border-image:url(:/image/images/pd400_sandian_btn.png) 0 21 0 0}"
                                  "QPushButton:hover{border-image:url(:/image/images/pd400_sandian_btn.png) 0 0 0 21}"
                                  "QPushButton:disabled{border-image:url(:/image/images/pd400_sandian_btn.png) 0 0 0 21}"
                                  "QPushButton:focus{outline: none;}"
                                  );
    toneselect_btn->setObjectName("100");
    connect(toneselect_btn,SIGNAL(clicked()),this,SLOT(tone_environment_btn_click()));

    //环境
    QWidget *environmentView = new QWidget();
    environmentView->resize(m_effectStackedWidget->width(),m_effectStackedWidget->height());
    environmentView->setStyleSheet("background-color:rgb(17,17,17)");
    m_effectStackedWidget->addWidget(environmentView);

    QVector<QString>signal_titles = {tr("  混响"),tr("  延迟")};
    for (int i=0;i<2;i++) {

        QWidget *signalView_unit = new QWidget(environmentView);
        signalView_unit->setAttribute(Qt::WA_StyledBackground,true);
        signalView_unit->resize(environmentView->width()/2,80*m_Magnification);
        signalView_unit->move(environmentView->width()/2*i,0*m_Magnification);

        custom_checkbox *show_btn= new custom_checkbox(signalView_unit);
        show_btn->setText(signal_titles.at(i));
        show_btn->resize(108*m_Magnification,25*m_Magnification);
        show_btn->move(20*m_Magnification,10*m_Magnification);
        QString showbtn_stys=QString("QCheckBox{font-family:'Source Han Sans CN Medium';color:rgb(255,255,255);font-size:%1px;}"
                                     "QCheckBox::hover{font-family:'Source Han Sans CN Medium';color:rgb(54,54,54);font-size:%1px;}"
                                     "QCheckBox::indicator{width:%2px;height:%2px;color: rgb(255, 0, 0);}"
                                     "QCheckBox::indicator:unchecked{border-image:url(:/image/images/pd400_xianfuunclick_btn.png.png);}"
                                     "QCheckBox::indicator:checked{border-image:url(:/image/images/pd400_xianfuclick_btn.png);}"
                                     "QCheckBox:focus{outline: none;}").arg(14*m_Magnification).arg(10*m_Magnification);
        show_btn->setStyleSheet(showbtn_stys);
        show_btn->setCheckState(Qt::Checked);
        show_btn->setObjectName("environment"+QString::number(1+i,10));
        connect(show_btn,SIGNAL(stateChanged(int)),this,SLOT(isOpenCloseBtn_click(int)));
        open_close_btns.push_back(show_btn);

        custom_button *select_btn = new custom_button(signalView_unit);
        select_btn->resize(21*m_Magnification,40*m_Magnification);
        select_btn->move(signalView_unit->width()-50*m_Magnification , 0);
        select_btn->setObjectName(QString::number(i+101,10));
        select_btn->setStyleSheet("QPushButton{border-image:url(:/image/images/pd400_sandian_btn.png) 0 21 0 0}"
                                  "QPushButton:hover{border-image:url(:/image/images/pd400_sandian_btn.png) 0 0 0 21}"
                                  "QPushButton:disabled{border-image:url(:/image/images/pd400_sandian_btn.png) 0 0 0 21}"
                                  "QPushButton:focus{outline: none;}"
                                  );
        connect(select_btn,SIGNAL(clicked()),this,SLOT(tone_environment_btn_click()));

        //环境下的 混响 延迟------------------------------
        MySlider *signal_slider = new MySlider(signalView_unit);
        signal_slider->setOrientation(Qt::Horizontal);
        signal_slider->setMinimum(0);
        signal_slider->setMaximum(100);
        signal_slider->setValue(xianfuqi_slider_values.at(0));
        signal_slider->setGeometry(20*m_Magnification,show_btn->y()+show_btn->height()+10*m_Magnification,280*m_Magnification,12*m_Magnification);
        signal_slider->setObjectName(QString::number(308+i,10));
        connect(signal_slider,SIGNAL(valueChanged(int)),this,SLOT(rightViewSlider_chanege_value(int)));
        connect(signal_slider,SIGNAL(sliderReleasedAt(int)),this,SLOT(rightViewSlider_release_value(int)));
        m_horSliders.push_back(signal_slider);

        QLabel *current_value = new QLabel(signalView_unit);
        current_value->move(signalView_unit->width()-50*m_Magnification,signal_slider->y()-2*m_Magnification);
        current_value->resize(40*m_Magnification,14*m_Magnification);
        current_value->setText("-80dB");
        QString currentLabel_stys = QString("background-color:transparent;color:rgb(255,255,255);font-size:%1px;font-family:'Source Han Sans CN Medium'").arg(12*m_Magnification);
        current_value->setStyleSheet(currentLabel_stys);
        m_currentValues.push_back(current_value);

        QLabel *left_value = new QLabel(signalView_unit);
        left_value->resize(40*m_Magnification,14*m_Magnification);
        left_value->move(signal_slider->x(),signal_slider->y()+signal_slider->height()+5*m_Magnification);
        left_value->setText("0dB");
        left_value->setStyleSheet(currentLabel_stys);

        QLabel *right_value = new QLabel(signalView_unit);
        right_value->resize(40*m_Magnification,16*m_Magnification);
        right_value->move(signal_slider->x()+signal_slider->width()-40*m_Magnification,signal_slider->y()+signal_slider->height()+5*m_Magnification);
        right_value->setText("100dB");
        right_value->setStyleSheet(currentLabel_stys);
        right_value ->setAlignment(Qt::AlignRight);
    }
    //createShowSignalView();
}
void dm90_rightview::createSignalView(){
    QVector<QString>signal_titles = {tr("  门限"),tr("  去爆音器"),tr("  压缩器"),tr("  限制器"),tr("  齿音消除器"),tr("  镶边器/移相器")};
    QVector<int>slider_maxValue={47,79,395,255,79};
    QVector<QString>leftNumber = {"-75dB","-80dB","-40dB","-25.5dB","-80dB"};
    QVector<QString>rightNumber = {"-28dB","-1dB","-0.5dB","0dB","-1dB"};
    int xianbian_num = 0;
    for (int h = 0;h<3;++h) {

        for (int i=0;i<2;i++) {
            QWidget *signalView_unit = new QWidget(m_signalView);
            signalView_unit->setAttribute(Qt::WA_StyledBackground,true);
            signalView_unit->resize(m_signalView->width()/2,(m_signalView->height()-20*m_Magnification)/3);
            signalView_unit->move(m_signalView->width()/2*i,(m_signalView->height()-20*m_Magnification)/3*h);
            //signalView_unit->setStyleSheet("background-color:red");

            custom_checkbox *show_btn= new custom_checkbox(signalView_unit);
            show_btn->setText(signal_titles.at(i + 2*h));
            show_btn->resize(118*m_Magnification,25*m_Magnification);
            show_btn->move(20*m_Magnification,10*m_Magnification);
            QString showbtn_stys=QString("QCheckBox{font-family:'Source Han Sans CN Medium';color:rgb(255,255,255);font-size:%1px;}"
                                         "QCheckBox::hover{font-family:'Source Han Sans CN Medium';color:rgb(54,54,54);font-size:%1px;}"
                                         "QCheckBox::indicator{width:%2px;height:%2px;color: rgb(255, 0, 0);}"
                                         "QCheckBox::indicator:unchecked{border-image:url(:/image/images/pd400_xianfuunclick_btn.png.png);}"
                                         "QCheckBox::indicator:checked{border-image:url(:/image/images/pd400_xianfuclick_btn.png);}"
                                         "QCheckBox:focus{outline: none;}").arg(14*m_Magnification).arg(10*m_Magnification);
            show_btn->setStyleSheet(showbtn_stys);
            show_btn->setCheckState(Qt::Checked);
            show_btn->setObjectName("Signal"+QString::number(i + 2*h,10));
            connect(show_btn,SIGNAL(stateChanged(int)),this,SLOT(isOpenCloseBtn_click(int)));
            open_close_btns.push_back(show_btn);

            custom_button *select_btn = new custom_button(signalView_unit);
            select_btn->resize(21*m_Magnification,40*m_Magnification);
            select_btn->move(signalView_unit->width()-50*m_Magnification , 0);
            select_btn->setObjectName(QString::number(i + 2*h,10));
            select_btn->setStyleSheet("QPushButton{border-image:url(:/image/images/pd400_sandian_btn.png) 0 21 0 0}"
                                      "QPushButton:hover{border-image:url(:/image/images/pd400_sandian_btn.png) 0 0 0 21}"
                                      "QPushButton:disabled{border-image:url(:/image/images/pd400_sandian_btn.png) 0 0 0 21}"
                                      "QPushButton:focus{outline: none;}"
                                      );
            connect(select_btn,SIGNAL(clicked()),this,SLOT(signalselect_btn_click()));


            if(i + 2*h != 5){
                MySlider *signal_slider = new MySlider(signalView_unit);
                signal_slider->setOrientation(Qt::Horizontal);
                //设置最大值，最小值
                signal_slider->setMinimum(0);
                signal_slider->setMaximum(slider_maxValue.at(i + 2*h));
                signal_slider->setValue(xianfuqi_slider_values.at(0));
                signal_slider->setGeometry(20*m_Magnification,show_btn->y()+show_btn->height()+10*m_Magnification,280*m_Magnification,12*m_Magnification);
                signal_slider->setObjectName(QString::number(303+i + 2*h,10));
                connect(signal_slider,SIGNAL(valueChanged(int)),this,SLOT(rightViewSlider_chanege_value(int)));
                connect(signal_slider,SIGNAL(sliderReleasedAt(int)),this,SLOT(rightViewSlider_release_value(int)));
                m_horSliders.push_back(signal_slider);

                QLabel *current_value = new QLabel(signalView_unit);
                current_value->move(signalView_unit->width()-50*m_Magnification,signal_slider->y()-2*m_Magnification);
                current_value->resize(50*m_Magnification,14*m_Magnification);
                current_value->setText("-80dB");
                QString currentLabel_stys = QString("background-color:transparent;color:rgb(255,255,255);font-size:%1px;font-family:'Source Han Sans CN Medium'").arg(12*m_Magnification);
                current_value->setStyleSheet(currentLabel_stys);
                m_currentValues.push_back(current_value);

                QLabel *left_value = new QLabel(signalView_unit);
                left_value->resize(50*m_Magnification,14*m_Magnification);
                left_value->move(signal_slider->x(),signal_slider->y()+signal_slider->height()+5*m_Magnification);
                left_value->setText(leftNumber.at(i + 2*h));
                left_value->setStyleSheet(currentLabel_stys);

                QLabel *right_value = new QLabel(signalView_unit);
                right_value->resize(40*m_Magnification,16*m_Magnification);
                right_value->move(signal_slider->x()+signal_slider->width()-40*m_Magnification,signal_slider->y()+signal_slider->height()+5*m_Magnification);
                right_value->setText(rightNumber.at(i + 2*h));
                right_value->setStyleSheet(currentLabel_stys);
                right_value ->setAlignment(Qt::AlignRight);

                xianbian_num = signal_slider->y();
            }else{
                show_btn->move(20*m_Magnification,xianbian_num-4*m_Magnification);
                select_btn->move(signalView_unit->width()-50*m_Magnification , show_btn->y()-10*m_Magnification);
            }
        }
    }
}
//获取完界面的背景参数后刷新整个界面======================
void dm90_rightview::refreshUI(){

    //qDebug()<<"kkkk000000------"<<GET_DEVICE_DATA->dm90_Reverb_WetLevel;

    //麦克风静音按钮
    m_mute_btn->blockSignals(true);
    if(GET_DEVICE_DATA->dm90_micMute){//打开静音
        m_mute_btn->setChecked(true);
        setHideDianping();
    }else{
        m_mute_btn->setChecked(false);
    }
    m_mute_btn->blockSignals(false);

    //刷新各种水平进度条的值
    for (int i=0;i<m_horSliders.count();i++) {
        MySlider *current_slider = m_horSliders.at(i);
        current_slider->blockSignals(true);
        int select_number = i+300;
        switch (select_number) {

        case INPUTSLIDER:
            current_slider->setValue(GET_DEVICE_DATA->dm90_micInputGain);
            m_currentValues.at(0)->setText(QString::number(GET_DEVICE_DATA->dm90_micInputGain,10));
            break;

        case OUTPUTSLIDER://麦克风输出
            current_slider->setValue(GET_DEVICE_DATA->dm90_micOutputGain);
            m_currentValues.at(1)->setText(QString::number(GET_DEVICE_DATA->dm90_micOutputGain,10));
            break;

        case EQHPSLIDER://高通滤波器---需要转换一下

        {
            int freq_value = changeFreqfor_X(GET_DEVICE_DATA->dm90_HPF_Freq);
            current_slider->setValue(freq_value);
            //是否可点
            GET_DEVICE_DATA->dm90_HPF_Enable==0? current_slider->setSliderDisable(true):current_slider->setSliderDisable(false);
            m_currentValues.at(2)->setText(QString::number(GET_DEVICE_DATA->dm90_HPF_Freq,10)+"Hz");
        }
            break;

        case THRESHOLDSLIDER://门限
        {
            int show_number = dm90_deviceDataSource::getInstance()->loadingChangeNegativeNumber(GET_DEVICE_DATA->dm90_threshold_value);
            current_slider->setValue(show_number+75);
            m_currentValues.at(3)->setText(QString::number(show_number,10)+"dB");
            GET_DEVICE_DATA->dm90_threshold_Enable==0? current_slider->setSliderDisable(true):current_slider->setSliderDisable(false);
        }
            break;

        case DETONATORSLIDER://去爆音器
        {

            int show_number = dm90_deviceDataSource::getInstance()->loadingChangeNegativeNumber(GET_DEVICE_DATA->dm90_Detonator_Threshold);
            current_slider->setValue(show_number+80);
            m_currentValues.at(4)->setText(QString::number(show_number,10)+"dB");
            GET_DEVICE_DATA->dm90_Detonator_Enable==0?current_slider->setSliderDisable(true):current_slider->setSliderDisable(false);

        }
            break;

        case COMPRESSORSLIDER://压缩器
        {

            double show_number = dm90_deviceDataSource::getInstance()->loadingChangeNegativeNumber(GET_DEVICE_DATA->dm90_Comp_Threshold);
            current_slider->setValue(show_number*10+400);
            m_currentValues.at(5)->setText(QString::number(show_number,'f',1)+"dB");
            GET_DEVICE_DATA->dm90_Comp_Enable==0?current_slider->setSliderDisable(true):current_slider->setSliderDisable(false);

        }
            break;

        case LIMITERSLIDER://限制器
        {
            double show_number = dm90_deviceDataSource::getInstance()->loadingChangeNegativeNumber(GET_DEVICE_DATA->dm90_Limit_Threshold);
            current_slider->setValue(show_number*10+255);
            m_currentValues.at(6)->setText(QString::number(show_number,'f',1)+"dB");
            GET_DEVICE_DATA->dm90_Limit_Enable==0?current_slider->setSliderDisable(true):current_slider->setSliderDisable(false);

        }
            break;

        case DENTALSOUNDSLIDER://齿音消除器
        {
            double show_number = dm90_deviceDataSource::getInstance()->loadingChangeNegativeNumber(GET_DEVICE_DATA->dm90_Dental_Threshold);
            current_slider->setValue(show_number+80);
            m_currentValues.at(7)->setText(QString::number(show_number,'f',0)+"dB");
            GET_DEVICE_DATA->dm90_Dental_Enable==0?current_slider->setSliderDisable(true):current_slider->setSliderDisable(false);
        }
            break;

        case REVERBSLIDER://混响
        {

            current_slider->setValue(GET_DEVICE_DATA->dm90_Reverb_WetLevel);
            m_currentValues.at(8)->setText(QString::number(GET_DEVICE_DATA->dm90_Reverb_WetLevel,10));
            GET_DEVICE_DATA->dm90_Reverb_Enable==0?current_slider->setSliderDisable(true):current_slider->setSliderDisable(false);
        }
            break;

        case DELAYSLIDER://延迟
        {
            current_slider->setValue(GET_DEVICE_DATA->dm90_Echo_Level);
            m_currentValues.at(9)->setText(QString::number(GET_DEVICE_DATA->dm90_Echo_Level,10));
            GET_DEVICE_DATA->dm90_Echo_Enable==0?current_slider->setSliderDisable(true):current_slider->setSliderDisable(false);
        }
            break;

        default:
            break;

        }
        current_slider->blockSignals(false);
    }

    //刷新各种垂直进度条的值
    for (int i=0;i<m_verSliders.count();i++) {

        SliderWidget *current_slider = m_verSliders.at(i);
        current_slider->blockSignals(true);
        double show_number =0;
        show_number = dm90_deviceDataSource::getInstance()->loadingChangeNegativeNumber(GET_DEVICE_DATA->dm90micEQdatasource[i].Gain);
        current_slider->SetValue(show_number*10+120);

        m_voiceCurrentValue.at(i)->setText(QString::number(show_number,'f',1));
        current_slider->blockSignals(false);
        GET_DEVICE_DATA->dm90micEQdatasource[0].Enable==0?current_slider->SetDisable(true):current_slider->SetDisable(false);
    }

    //刷新开关按钮的状态
    //qDebug()<<"aaaaaaaa"<<GET_DEVICE_DATA->dm90_HPF_Enable;
    for (int i=0;i<open_close_btns.count();i++) {
        open_close_btns.at(i)->blockSignals(true);
    }
    GET_DEVICE_DATA->dm90_HPF_Enable==0?open_close_btns.at(0)->setCheckState(Qt::Unchecked):open_close_btns.at(0)->setCheckState(Qt::Checked);//高通滤波器
    GET_DEVICE_DATA->dm90micEQdatasource[0].Enable==0?open_close_btns.at(1)->setCheckState(Qt::Unchecked):open_close_btns.at(1)->setCheckState(Qt::Checked);//声音均衡器
    GET_DEVICE_DATA->dm90_threshold_Enable==0?open_close_btns.at(2)->setCheckState(Qt::Unchecked):open_close_btns.at(2)->setCheckState(Qt::Checked);//门限
    GET_DEVICE_DATA->dm90_Detonator_Enable==0?open_close_btns.at(3)->setCheckState(Qt::Unchecked):open_close_btns.at(3)->setCheckState(Qt::Checked);//去爆音
    GET_DEVICE_DATA->dm90_Comp_Enable==0?open_close_btns.at(4)->setCheckState(Qt::Unchecked):open_close_btns.at(4)->setCheckState(Qt::Checked);//压缩器
    GET_DEVICE_DATA->dm90_Limit_Enable==0?open_close_btns.at(5)->setCheckState(Qt::Unchecked):open_close_btns.at(5)->setCheckState(Qt::Checked);//限制器
    GET_DEVICE_DATA->dm90_Dental_Enable==0?open_close_btns.at(6)->setCheckState(Qt::Unchecked):open_close_btns.at(6)->setCheckState(Qt::Checked);//齿音
    GET_DEVICE_DATA->dm90_flanger_Enable==0?open_close_btns.at(7)->setCheckState(Qt::Unchecked):open_close_btns.at(7)->setCheckState(Qt::Checked);//镶边

    GET_DEVICE_DATA->dm90_harmony_Enable==0?open_close_btns.at(8)->setCheckState(Qt::Unchecked):open_close_btns.at(8)->setCheckState(Qt::Checked);//和声
    GET_DEVICE_DATA->dm90_Reverb_Enable==0?open_close_btns.at(9)->setCheckState(Qt::Unchecked):open_close_btns.at(9)->setCheckState(Qt::Checked);//混响
    GET_DEVICE_DATA->dm90_Echo_Enable==0?open_close_btns.at(10)->setCheckState(Qt::Unchecked):open_close_btns.at(10)->setCheckState(Qt::Checked);//延迟

    for (int i=0;i<open_close_btns.count();i++) {
        open_close_btns.at(i)->blockSignals(false);
    }
}

void dm90_rightview::refreshLevel(int Input,int Output){

    int max_value = 44;
    int inputvalue = ((Input-2000)/10 + max_value);

    int inputMoveValue = 570*m_Magnification/max_value *inputvalue;
    m_inputDianping_moveLab->move(70*m_Magnification + inputMoveValue,172*m_Magnification);
    m_inputDianping_moveLab->resize(569*m_Magnification-inputMoveValue,11*m_Magnification);

    if((m_inputDianping_moveLab->x()<=70*m_Magnification)){
        m_inputDianping_moveLab->move(70*m_Magnification,172*m_Magnification);
        m_inputDianping_moveLab->resize(570*m_Magnification ,11*m_Magnification);
    }
    //主输出电平条
    max_value = 44;
    int outputvalue = ((Output-2000)/10 + max_value);
    int outputMoveValue = 570*m_Magnification/max_value *outputvalue;

    m_outputDianping_moveLab->move(70*m_Magnification + outputMoveValue,262*m_Magnification);
    m_outputDianping_moveLab->resize(569*m_Magnification-outputMoveValue,11*m_Magnification);

    if((m_outputDianping_moveLab->x()<=70*m_Magnification)){
        m_outputDianping_moveLab->move(70*m_Magnification,262*m_Magnification);
        m_outputDianping_moveLab->resize(570*m_Magnification ,11*m_Magnification);
    }
}

void dm90_rightview::setHideDianping(){
    m_inputDianping_moveLab->move(70*m_Magnification,172*m_Magnification);
    m_inputDianping_moveLab->resize(569*m_Magnification ,11*m_Magnification);

    m_outputDianping_moveLab->move(70*m_Magnification,262*m_Magnification);
    m_outputDianping_moveLab->resize(569*m_Magnification ,11*m_Magnification);

}
//点击预设刷新界面的方法----------------------------------------------
void dm90_rightview::clickItemRefreshUI(bool listClick){

    m_isList_click = listClick;
    refreshUI();
}

void dm90_rightview::createShowSignalView(){

    QWidget* pWidget = this->parentWidget();

    m_thresholdView = new dm90_thresholdview(m_maskView);
    m_thresholdView->resize(m_maskView->width(),m_maskView->height()-64*m_Magnification);
    m_thresholdView->move(0,64*m_Magnification);
    m_thresholdView->setStyleSheet("background:rgba(0,0,0,70)");
    m_maskView->setStyleSheet("background:rgba(0,0,0,200)");
    m_thresholdView->createUI();
    //增加信号槽
    connect(m_thresholdView,SIGNAL(confirmClick(int)),this,SLOT(view_confirmClick(int)));
    m_thresholdView->hide();

    m_detonatorView = new dm90_detonatorView(m_maskView);
    m_detonatorView->resize(m_maskView->width(),m_maskView->height()-64*m_Magnification);
    m_detonatorView->move(0,64*m_Magnification);
    m_detonatorView->setStyleSheet("background:rgba(0,0,0,70)");
    m_detonatorView->createUI();
    //增加信号槽
    connect(m_detonatorView,SIGNAL(confirmClick(int)),this,SLOT(view_confirmClick(int)));
    m_detonatorView->hide();

    m_compressorView = new dm90_compressorView(m_maskView);
    m_compressorView->resize(m_maskView->width(),m_maskView->height()-64*m_Magnification);
    m_compressorView->move(0,64*m_Magnification);
    m_compressorView->setStyleSheet("background:rgba(0,0,0,70)");
    m_compressorView->createUI();
    //增加信号槽
    connect(m_compressorView,SIGNAL(confirmClick(int)),this,SLOT(view_confirmClick(int)));
    m_compressorView->hide();

    m_limiterView = new dm90_limiterView(m_maskView);
    m_limiterView->resize(m_maskView->width(),m_maskView->height()-64*m_Magnification);
    m_limiterView->move(0,64*m_Magnification);
    m_limiterView->setStyleSheet("background:rgba(0,0,0,70)");
    m_limiterView->createUI();
    //增加信号槽
    connect(m_limiterView,SIGNAL(confirmClick(int)),this,SLOT(view_confirmClick(int)));
    m_limiterView->hide();

    m_dentalsoundView = new dm90_dentalSoundView(m_maskView);
    m_dentalsoundView->resize(m_maskView->width(),m_maskView->height()-64*m_Magnification);
    m_dentalsoundView->move(0,64*m_Magnification);
    m_dentalsoundView->setStyleSheet("background:rgba(0,0,0,70)");
    m_dentalsoundView->createUI();
    //增加信号槽
    connect(m_dentalsoundView,SIGNAL(confirmClick(int)),this,SLOT(view_confirmClick(int)));
    m_dentalsoundView->hide();

    m_flangerView = new dm90_flangerView(m_maskView);
    m_flangerView->resize(m_maskView->width(),m_maskView->height()-64*m_Magnification);
    m_flangerView->move(0,64*m_Magnification);
    m_flangerView->setStyleSheet("background:rgba(0,0,0,70)");
    m_flangerView->createUI();
    //增加信号槽
    connect(m_flangerView,SIGNAL(confirmClick(int)),this,SLOT(view_confirmClick(int)));
    m_flangerView->hide();

    m_volumeEqView = new dm90_volumeEQView(m_maskView);
    m_volumeEqView->resize(m_maskView->width(),m_maskView->height());
    m_volumeEqView->setStyleSheet("background:rgba(0,0,0,70)");
    m_volumeEqView->createUI();
    //增加信号槽
    connect(m_volumeEqView,SIGNAL(confirmClick(int)),this,SLOT(view_confirmClick(int)));
    m_volumeEqView->hide();

    m_toneView = new dm90_toneView(m_maskView);
    m_toneView->resize(m_maskView->width(),m_maskView->height());
    m_toneView->setStyleSheet("background:rgba(0,0,0,100)");
    m_toneView->createUI();
    //增加信号槽
    connect(m_toneView,SIGNAL(confirmClick(int)),this,SLOT(view_confirmClick(int)));
    m_toneView->hide();

    //混响
    //window_dm90* mainWindow ;
    //        foreach (QWidget *w, qApp->topLevelWidgets()){
    //            if (window_dm90* mainWindow = qobject_cast<window_dm90*>(w)){
    //mainWindow = mainWidget;

    m_reverbView = new dm90_reverbView(m_maskView);
    m_reverbView->resize(m_maskView->width(),m_maskView->height());
    m_reverbView->move(0,0);
    m_reverbView->setStyleSheet("background:rgba(0,0,0,100)");
    m_reverbView->createUI();
    //增加信号槽
    connect(m_reverbView,SIGNAL(confirmClick(int)),this,SLOT(view_confirmClick(int)));
    m_reverbView->hide();

    //延迟
    m_delayView = new dm90_delayView(m_maskView);
    m_delayView->resize(m_maskView->width(),m_maskView->height()-64*m_Magnification);
    m_delayView->move(0,64*m_Magnification);
    m_delayView->setStyleSheet("background:rgba(0,0,0,100)");
    m_delayView->createUI();
    m_delayView->hide();
    //增加信号槽
    connect(m_delayView,SIGNAL(confirmClick(int)),this,SLOT(view_confirmClick(int)));

}
void dm90_rightview::mute_btn_click(int isMute){

    if(isMute){//静音
        m_commin_fun.sendMessageForDevice(DM90PARAM_MUTE_START,1,1);
        setHideDianping();
    }else{
        m_commin_fun.sendMessageForDevice(DM90PARAM_MUTE_START,1,0);
    }
    qDebug()<<"mute--------"<<isMute;

}
//本地声音得切换
void dm90_rightview::eq_singsClick(){

    QPushButton *btn = static_cast<QPushButton *>(sender());
    if(btn->objectName()=="eq"){
        m_pStackedWidget->setCurrentIndex(0);
    }else{
        m_pStackedWidget->setCurrentIndex(1);
    }
    int value = btn->fontMetrics().boundingRect(btn->text()).width();
    m_titleStatus->move(btn->x(),30*m_Magnification);
    m_titleStatus->resize(value,2*m_Magnification);

}
//效果器的切换
void dm90_rightview::effectClick(){

    QPushButton *btn = static_cast<QPushButton *>(sender());
    if(btn->objectName()=="tone"){
        m_effectStackedWidget->setCurrentIndex(0);
    }else{
        m_effectStackedWidget->setCurrentIndex(1);
    }
    int value = btn->fontMetrics().boundingRect(btn->text()).width();
    m_effectTitleStatus->move(btn->x(),30*m_Magnification);
    m_effectTitleStatus->resize(value,2*m_Magnification);
}


int dm90_rightview::changexfor_freq(int x){

    //    double fZeroFreq = log10 (1000.0);
    //    double Freq_value = pow(10.0, (double)fZeroFreq/1000 * x) *20.0;
    //    return  qRound(Freq_value);

    double fZeroFreq = log10 (50.0);
    double Freq_value = pow(10.0, (double)fZeroFreq/1000 * x) *20.0;
    return  qRound(Freq_value);
}
int dm90_rightview::changeFreqfor_X(int freq){

    //    double fZeroFreq = log10 (1000.0);
    //    int X = log10((double)freq/20.0)*1000/(double)fZeroFreq;
    //    return X;
    double fZeroFreq = log10 (50.0);
    int X = log10((double)freq/20.0)*1000/(double)fZeroFreq;
    return X;
}
//各种使能开关的方法=============================
void dm90_rightview::isOpenCloseBtn_click(int isOpen){

    //获取父视图
    window_dm90* m_mainWindow ;
    foreach (QWidget *w, qApp->topLevelWidgets()){
        if (window_dm90* mainWindow = qobject_cast<window_dm90*>(w)){
            m_mainWindow = mainWindow;
        }
    }
    custom_checkbox *show_btn = static_cast<custom_checkbox *>(sender());
    if(show_btn->objectName()=="highFilter"){//高通滤波器

        MySlider *current_slider = m_horSliders.at(EQHPSLIDER-300);
        if(isOpen){//打开
            current_slider->setSliderDisable(false);
            m_commin_fun.sendMessageForDevice(DM90PARAM_MIC_HPF_START,1,1);
            GET_DEVICE_DATA->dm90_HPF_Enable = 1;

        }else{//关闭
            current_slider->setSliderDisable(true);
            m_commin_fun.sendMessageForDevice(DM90PARAM_MIC_HPF_START,1,0);
            GET_DEVICE_DATA->dm90_HPF_Enable = 0;
        }
        //本地声音添加自定义预设音效
        emit rightView_addLocaltItem(5,isOpen>1?isOpen=1:isOpen=0);
        //GET_DEVICE_DATA->dm90_HPF_Enable = isOpen>1?isOpen=1:isOpen=0;

    }else if(show_btn->objectName()=="voiceq"){//声音均衡器

        //m_volumeEqView->m_highEQ_btn->blockSignals(true);
        if(isOpen){//打开

            //m_volumeEqView->m_highEQ_btn->setCheckState(Qt::Checked);
            QVector<int>addrs;
            QVector<int>value;
            for (int i=0;i<8;i++) {
                addrs.push_back(0x202f+5*i);
                value.push_back(1);
            }
            if(GET_DEVICE_DATA->dm90micEQdatasource[0].Enable  != isOpen-1)
                m_commin_fun.sendRandomMessageForDevice(0x27,addrs,value);
            GET_DEVICE_DATA->dm90micEQdatasource[0].Enable  =1;

            for (int i=0;i<m_verSliders.count();i++) {
                m_verSliders.at(i)->SetDisable(false);
            }

        }else{//关闭
            // m_volumeEqView->m_highEQ_btn->setCheckState(Qt::Unchecked);

            QVector<int>addrs;
            QVector<int>value;
            for (int i=0;i<8;i++) {
                addrs.push_back(0x202f+5*i);
                value.push_back(0);
            }
            if(GET_DEVICE_DATA->dm90micEQdatasource[0].Enable  != isOpen)
                m_commin_fun.sendRandomMessageForDevice(0x27,addrs,value);
            GET_DEVICE_DATA->dm90micEQdatasource[0].Enable  =0;

            for (int i=0;i<m_verSliders.count();i++) {
                m_verSliders.at(i)->SetDisable(true);
            }
        }
        // m_volumeEqView->m_highEQ_btn->blockSignals(false);
        //本地声音添加自定义预设音效
        emit rightView_addLocaltItem(7,isOpen>1?isOpen=1:isOpen=0);


    }else if(show_btn->objectName()=="tone"){//音调

        if(isOpen){//打开
            if(GET_DEVICE_DATA->dm90_harmony_Enable != isOpen-1)
                m_commin_fun.sendMessageForDevice(DM90PARAM_MIC_CHOR_START,1,1);
            GET_DEVICE_DATA->dm90_harmony_Enable = 1;

        }else{//关闭
            if(GET_DEVICE_DATA->dm90_harmony_Enable != isOpen)
                m_commin_fun.sendMessageForDevice(DM90PARAM_MIC_CHOR_START,1,0);
            GET_DEVICE_DATA->dm90_harmony_Enable = 0;
        }
        //增加ITEME

        isOpen>1?isOpen=1:isOpen=0;
        emit rightView_addEffectItem(0,isOpen);


    }else if(show_btn->objectName()=="environment1"){//混响

        MySlider *current_slider = m_horSliders.at(REVERBSLIDER-300);
        if(isOpen){//打开
            current_slider->setSliderDisable(false);
            if(GET_DEVICE_DATA->dm90_Reverb_Enable != isOpen-1)
                m_commin_fun.sendMessageForDevice(DM90PARAM_MIC_REVERB_START+1,1,1);
            GET_DEVICE_DATA->dm90_Reverb_Enable = 1;
        }else{//关闭
            current_slider->setSliderDisable(true);
            if(GET_DEVICE_DATA->dm90_Reverb_Enable != isOpen)
                m_commin_fun.sendMessageForDevice(DM90PARAM_MIC_REVERB_START+1,1,0);
            GET_DEVICE_DATA->dm90_Reverb_Enable = 0;
        }
        //增加ITEME
        isOpen>1?isOpen=1:isOpen=0;
        emit rightView_addEffectItem(8,isOpen);

    }else if(show_btn->objectName()=="environment2"){//延迟

        MySlider *current_slider = m_horSliders.at(DELAYSLIDER-300);
        if(isOpen){//打开
            current_slider->setSliderDisable(false);
            if(GET_DEVICE_DATA->dm90_Echo_Enable != isOpen-1)
                m_commin_fun.sendMessageForDevice(DM90PARAM_MIC_ECHO_START+1,1,1);
            GET_DEVICE_DATA->dm90_Echo_Enable = 1;
        }else{//关闭
            current_slider->setSliderDisable(true);
            if(GET_DEVICE_DATA->dm90_Echo_Enable != isOpen)
                m_commin_fun.sendMessageForDevice(DM90PARAM_MIC_ECHO_START+1,1,0);
            GET_DEVICE_DATA->dm90_Echo_Enable = 0;
        }
        //增加ITEME
        isOpen>1?isOpen=1:isOpen=0;
        emit rightView_addEffectItem(23,isOpen);

    }else if(show_btn->objectName()=="Signal0"){//门限

        MySlider *current_slider = m_horSliders.at(THRESHOLDSLIDER-300);
        if(isOpen){//打开
            current_slider->setSliderDisable(false);
            if(GET_DEVICE_DATA->dm90_threshold_Enable != isOpen-1)
                m_commin_fun.sendMessageForDevice(DM90PARAM_MIC_GATE_START,1,1);
            GET_DEVICE_DATA->dm90_threshold_Enable =1;

        }else{//关闭
            current_slider->setSliderDisable(true);
            if(GET_DEVICE_DATA->dm90_threshold_Enable != isOpen)
                m_commin_fun.sendMessageForDevice(DM90PARAM_MIC_GATE_START,1,0);
            GET_DEVICE_DATA->dm90_threshold_Enable =0;
        }
        //本地声音添加自定义预设音效
        emit rightView_addLocaltItem(0,isOpen>1?isOpen=1:isOpen=0);


    }else if(show_btn->objectName()=="Signal1"){//去爆音器

        MySlider *current_slider = m_horSliders.at(DETONATORSLIDER-300);
        if(isOpen){//打开
            current_slider->setSliderDisable(false);
            if(GET_DEVICE_DATA->dm90_Detonator_Enable != isOpen-1)
                m_commin_fun.sendMessageForDevice(DM90PARAM_MIC_DEPOP_START,1,1);
            GET_DEVICE_DATA->dm90_Detonator_Enable =1;
        }else{//关闭
            current_slider->setSliderDisable(true);
            if(GET_DEVICE_DATA->dm90_Detonator_Enable != isOpen)
                m_commin_fun.sendMessageForDevice(DM90PARAM_MIC_DEPOP_START,1,0);
            GET_DEVICE_DATA->dm90_Detonator_Enable =0;
        }
        //本地声音添加自定义预设音效
        emit rightView_addLocaltItem(55,isOpen>1?isOpen=1:isOpen=0);

    }else if(show_btn->objectName()=="Signal2"){//压缩器

        MySlider *current_slider = m_horSliders.at(COMPRESSORSLIDER-300);
        if(isOpen){//打开
            current_slider->setSliderDisable(false);
            if(GET_DEVICE_DATA->dm90_Comp_Enable != isOpen-1)
                m_commin_fun.sendMessageForDevice(DM90PARAM_MIC_COMP_START,1,1);
            GET_DEVICE_DATA->dm90_Comp_Enable =1;
        }else{//关闭
            current_slider->setSliderDisable(true);
            if(GET_DEVICE_DATA->dm90_Comp_Enable != isOpen)
                m_commin_fun.sendMessageForDevice(DM90PARAM_MIC_COMP_START,1,0);
            GET_DEVICE_DATA->dm90_Comp_Enable =0;
        }

        //本地声音添加自定义预设音效
        emit rightView_addLocaltItem(71,isOpen>1?isOpen=1:isOpen=0);

    }else if(show_btn->objectName()=="Signal3"){//限制器

        MySlider *current_slider = m_horSliders.at(LIMITERSLIDER-300);
        if(isOpen){//打开
            current_slider->setSliderDisable(false);
            if(GET_DEVICE_DATA->dm90_Limit_Enable != isOpen-1)
                m_commin_fun.sendMessageForDevice(DM90PARAM_MIC_LIMIT_START,1,1);
            GET_DEVICE_DATA->dm90_Limit_Enable =1;
        }else{//关闭
            current_slider->setSliderDisable(true);
            if(GET_DEVICE_DATA->dm90_Limit_Enable != isOpen)
                m_commin_fun.sendMessageForDevice(DM90PARAM_MIC_LIMIT_START,1,0);
            GET_DEVICE_DATA->dm90_Limit_Enable =0;
        }
        //本地声音添加自定义预设音效
        emit rightView_addLocaltItem(76,isOpen>1?isOpen=1:isOpen=0);

    }else if(show_btn->objectName()=="Signal4"){//齿音消除器

        MySlider *current_slider = m_horSliders.at(DENTALSOUNDSLIDER-300);
        if(isOpen){//打开
            current_slider->setSliderDisable(false);
            if(GET_DEVICE_DATA->dm90_Dental_Enable != isOpen-1)
                m_commin_fun.sendMessageForDevice(DM90PARAM_MIC_DEESS_START,1,1);
            GET_DEVICE_DATA->dm90_Dental_Enable =1;
        }else{//关闭
            current_slider->setSliderDisable(true);
            if(GET_DEVICE_DATA->dm90_Dental_Enable != isOpen)
                m_commin_fun.sendMessageForDevice(DM90PARAM_MIC_DEESS_START,1,0);
            GET_DEVICE_DATA->dm90_Dental_Enable =0;
        }

        //本地声音添加自定义预设音效
        emit rightView_addLocaltItem(47,isOpen>1?isOpen=1:isOpen=0);

    }else if(show_btn->objectName()=="Signal5"){//镶边器/移相器

        if(isOpen){//打开
            if(GET_DEVICE_DATA->dm90_flanger_Enable != isOpen-1)
                m_commin_fun.sendMessageForDevice(DM90PARAM_MIC_FLAN_START,1,1);
            GET_DEVICE_DATA->dm90_flanger_Enable = 1;
        }else{//关闭
            if(GET_DEVICE_DATA->dm90_flanger_Enable != isOpen)
                m_commin_fun.sendMessageForDevice(DM90PARAM_MIC_FLAN_START,1,0);
            GET_DEVICE_DATA->dm90_flanger_Enable = 0;
        }

        //本地声音添加自定义预设音效
        emit rightView_addLocaltItem(65,isOpen>1?isOpen=1:isOpen=0);
    }
}

//各种水平拖动条的事件=====================
void dm90_rightview::rightViewSlider_chanege_value(int currentValue){

    MySlider *slider = static_cast<MySlider *>(sender());
    int select_number = m_currentSliderNum = slider->objectName().toInt();
    QLabel *current_value = m_currentValues.at(select_number-300);

    m_DM90delayTimer->stop();
    m_DM90delayTimer->start(20);
    switch (select_number) {

    case INPUTSLIDER:{

        current_value->setText(QString::number(currentValue,10));

    }
        break;
    case OUTPUTSLIDER:{
        current_value->setText(QString::number(currentValue,10));
    }
        break;
    case EQHPSLIDER:{
        int change_freq = changexfor_freq(currentValue);
        current_value->setText(QString::number(change_freq,10)+"Hz");
    }
        break;
    case THRESHOLDSLIDER:{
        current_value->setText(QString::number(currentValue-75,10)+"dB");
    }
        break;
    case DETONATORSLIDER:{
        current_value->setText(QString::number(currentValue-80,10)+"dB");
    }
        break;
    case COMPRESSORSLIDER:{

        QString valueStr = QString::number(((double)currentValue-400)/10,'f', 1);
        current_value->setText(valueStr+"dB");
    }
        break;
    case LIMITERSLIDER:{

        QString valueStr = QString::number(((double)currentValue-255)/10,'f', 1);
        current_value->setText(valueStr+"dB");
    }
        break;
    case DENTALSOUNDSLIDER:{
        current_value->setText(QString::number(currentValue-80,10)+"dB");
    }
        break;
    case REVERBSLIDER:{//混响的拖动条事件
        current_value->setText(QString::number(currentValue,10));
    }
        break;
    case DELAYSLIDER:{//延迟
        current_value->setText(QString::number(currentValue,10));
    }
        break;

    }

}
//延时计时器的方法发送数据--------------------------------
void dm90_rightview::starDelayTimerFun(){

    if(Qwdget_timer_index==1){

        //发送延时的数据
        switch (m_currentSliderNum) {

        case INPUTSLIDER:{
            MySlider *current_slider = m_horSliders.at(INPUTSLIDER-300);
            m_commin_fun.sendMessageForDevice(DM90PARAM_GAIN_START,1,current_slider->value());
            GET_DEVICE_DATA->dm90_micInputGain = current_slider->value();
        }
            break;
        case OUTPUTSLIDER:{

            MySlider *current_slider = m_horSliders.at(OUTPUTSLIDER-300);
            m_commin_fun.sendMessageForDevice(DM90PARAM_GAIN_START+1,1,current_slider->value());
            GET_DEVICE_DATA->dm90_micOutputGain = current_slider->value();
        }
            break;
        case EQHPSLIDER:{//转换之后发送数据,高通滤波器
            MySlider *current_slider = m_horSliders.at(EQHPSLIDER-300);
            m_commin_fun.sendMessageForDevice(DM90PARAM_MIC_HPF_END,1,changexfor_freq(current_slider->value()));
            emit rightView_addLocaltItem(6,changexfor_freq(current_slider->value()));
            GET_DEVICE_DATA->dm90_HPF_Freq = changexfor_freq(current_slider->value());
        }
            break;
        case THRESHOLDSLIDER:{//门限

            MySlider *current_slider = m_horSliders.at(THRESHOLDSLIDER-300);
            int send_number = dm90_deviceDataSource::getInstance()->loadingChangeDeviceNumber(current_slider->value()-75);
            m_commin_fun.sendMessageForDevice(DM90PARAM_MIC_GATE_START+1,1,send_number);
            GET_DEVICE_DATA->dm90_threshold_value = send_number;
            emit rightView_addLocaltItem(1,send_number);
        }
            break;
        case DETONATORSLIDER:{//去爆音

            MySlider *current_slider = m_horSliders.at(DETONATORSLIDER-300);
            int send_number = dm90_deviceDataSource::getInstance()->loadingChangeDeviceNumber(current_slider->value()-80);
            m_commin_fun.sendMessageForDevice(DM90PARAM_MIC_DEPOP_START+1,1,send_number);
            GET_DEVICE_DATA->dm90_Detonator_Threshold = send_number;
            emit rightView_addLocaltItem(55+1,send_number);
        }
            break;
        case COMPRESSORSLIDER:{//压缩器

            MySlider *current_slider = m_horSliders.at(COMPRESSORSLIDER-300);
            int send_number = dm90_deviceDataSource::getInstance()->loadingChangeDeviceNumber((double)current_slider->value()/10-40);
            m_commin_fun.sendMessageForDevice(DM90PARAM_MIC_COMP_START+1,1,send_number);
            GET_DEVICE_DATA->dm90_Comp_Threshold = send_number;
            emit rightView_addLocaltItem(71+1,send_number);
        }
            break;
        case LIMITERSLIDER:{//限制器

            MySlider *current_slider = m_horSliders.at(LIMITERSLIDER-300);
            int send_number = dm90_deviceDataSource::getInstance()->loadingChangeDeviceNumber((double)current_slider->value()/10-25.5);
            m_commin_fun.sendMessageForDevice(DM90PARAM_MIC_LIMIT_START+1,1,send_number);
            GET_DEVICE_DATA->dm90_Limit_Threshold = send_number;
            emit rightView_addLocaltItem(76+1,send_number);
        }
            break;
        case DENTALSOUNDSLIDER:{//齿音消除

            MySlider *current_slider = m_horSliders.at(DENTALSOUNDSLIDER-300);
            int send_number = dm90_deviceDataSource::getInstance()->loadingChangeDeviceNumber(current_slider->value()-80);
            m_commin_fun.sendMessageForDevice(DM90PARAM_MIC_DEESS_START+1,1,send_number);
            GET_DEVICE_DATA->dm90_Dental_Threshold = send_number;
            emit rightView_addLocaltItem(47+1,send_number);
        }
            break;
        case REVERBSLIDER:{//混响

            MySlider *current_slider = m_horSliders.at(REVERBSLIDER-300);
            m_commin_fun.sendMessageForDevice(DM90PARAM_MIC_REVERB_START+4,1,current_slider->value());
            GET_DEVICE_DATA->dm90_Reverb_WetLevel = current_slider->value();

        }
            break;
        case DELAYSLIDER:{//延迟

            MySlider *current_slider = m_horSliders.at(DELAYSLIDER-300);
            m_commin_fun.sendMessageForDevice(DM90PARAM_MIC_ECHO_START+2,1,current_slider->value());
            GET_DEVICE_DATA->dm90_Echo_Level = current_slider->value();
        }
            break;

        case 100:
        case 101:
        case 102:
        {//声音均衡器

            SliderWidget *verticalSlider =  m_verSliders.at(m_currentSliderNum-100);
            qDebug()<<"jjjjjj"<<verticalSlider->GetValue();
            int send_Value = dm90_deviceDataSource::getInstance()->loadingChangeDeviceNumber((double)(verticalSlider->GetValue()-120)/10);
            m_commin_fun.sendMessageForDevice(DM90PARAM_MIC_PEQ_START+3+5*(m_currentSliderNum-100),1,send_Value);
            emit rightView_addLocaltItem(7+3+5*(m_currentSliderNum-100),send_Value);

        }
            break;
        }
        //发完数据之后停止计时器
        m_DM90delayTimer->stop();
    }
}
//SLIDER释放
void dm90_rightview::rightViewSlider_release_value(int currentValue){

    switch (m_currentSliderNum) {
    case EQHPSLIDER:{

        qDebug()<<"mmmmm";
    }
        break;

    default:
        break;
    }

}

//垂直拖动条的方法
void dm90_rightview::verticalSlider_setValue(int currentValue){

    m_DM90delayTimer->stop();
    m_DM90delayTimer->start(20);

    SliderWidget *verticalSlider = static_cast<SliderWidget *>(sender());

    int slider_number = verticalSlider->objectName().toUInt()-100;
    QLabel *currentValue_label = m_voiceCurrentValue.at(slider_number);
    double showCurrent = ((double)currentValue-120)/10;
    currentValue_label->setText(QString::number(showCurrent,'f',1));

    m_currentSliderNum = verticalSlider->objectName().toUInt();

    qDebug()<<"kkkk"<<slider_number<<currentValue;
    //同步详情页的进度条
    int send_Value = dm90_deviceDataSource::getInstance()->loadingChangeDeviceNumber((double)(currentValue-120)/10);
    GET_DEVICE_DATA->dm90micEQdatasource[slider_number].Gain = send_Value;

}
//void dm90_rightview::verticalSlider_released(){

//}

void dm90_rightview::select_btn_click(){


}
void dm90_rightview::isopen_btn_click(int){

}

//点击时间弹出各种信号处理的页面--------------------------------------------------
void dm90_rightview::signalselect_btn_click(){

    custom_button *select_btn = static_cast<custom_button *>(sender());
    int btn_number = select_btn->objectName().toInt()+1;
    m_maskView->show();

    switch (btn_number) {
    case THRESHOLD:{

        m_thresholdView->show();
        m_thresholdView->refreshUI();
        Qwdget_timer_index = 5;
    }
        break;
    case DETONATOR:{//去爆音

        m_detonatorView->show();
        m_detonatorView->refreshUI();
        Qwdget_timer_index = 6;
    }
        break;
    case COMPRESSOR:{
        m_compressorView->show();
        m_compressorView->refreshUI();
        Qwdget_timer_index = 7;
    }
        break;
    case LIMITER:{
        m_limiterView->show();
        m_limiterView->refreshUI();
        Qwdget_timer_index = 8;
    }
        break;
    case DENTALSOUND:{
        m_dentalsoundView->show();
        m_dentalsoundView->refreshUI();
        Qwdget_timer_index = 9;
    }
        break;
    case FLANGER:{
        m_flangerView->show();
        m_flangerView->refreshUI();
        Qwdget_timer_index = 10;
    }
        break;
    default:
        break;
    }
    m_recorderView->hide();
}
void dm90_rightview::tone_environment_btn_click(){

    custom_button *select_btn = static_cast<custom_button *>(sender());
    int btn_number = select_btn->objectName().toInt();
    switch (btn_number) {
    case TONE:{
        m_maskView->show();
        m_toneView->show();
        m_toneView->refreshUI();
        Qwdget_timer_index = 11;
    }
        break;
    case REVERB:{
        m_maskView->show();
        m_reverbView->show();
        m_reverbView->refreshUI();
        Qwdget_timer_index = 12;
    }
        break;
    case DELAY:{
        m_maskView->show();
        m_delayView->show();
        m_delayView->refreshUI();
        Qwdget_timer_index = 13;
    }
        break;
    default:
        break;
    }
    com_recordView->m_waveView->m_chartView->hide();
}
//显示声音均衡器的详情--------
void dm90_rightview::voiceEQshow_btn_click(){
    m_maskView->show();
    m_volumeEqView->show();
    m_volumeEqView->refreshUI();
    Qwdget_timer_index = 4;
    m_recorderView->hide();
}
//本地声音、效果之间的切换
void dm90_rightview::receiveResult(int number){

    m_localEffectView->setCurrentIndex(number);
    qDebug()<<"kkkkk"<<number;
}


//实际的负参数上传到设备中
double dm90_rightview::uploadChangeNegativeNumber(int upload_number){

    upload_number = upload_number * 10 +2000;
    return upload_number;
}

//各种开关按钮的方法---------------------------

//高通滤波器的打开
void dm90_rightview::highpassBtn_click(int isOpen){

    MySlider *current_slider = m_horSliders.at(2);
    if(isOpen){//打开
        current_slider->setSliderDisable(false);
    }else{//关闭
        current_slider->setSliderDisable(true);
    }
}
//各种视图确定按钮的槽
void dm90_rightview::view_confirmClick(int view_number){

    if(view_number==1){//声音均衡器
        for (int i=0;i<3;i++) {
            double show_number = dm90_deviceDataSource::getInstance()->loadingChangeNegativeNumber(GET_DEVICE_DATA->dm90micEQdatasource[i].Gain);
            m_verSliders.at(i)->blockSignals(true);
            m_verSliders.at(i)->SetValue(show_number*10+120);
            m_verSliders.at(i)->blockSignals(false);
            GET_DEVICE_DATA->dm90micEQdatasource[0].Enable==0?open_close_btns.at(1)->setCheckState(Qt::Unchecked):open_close_btns.at(1)->setCheckState(Qt::Checked);
            //设置对应的LABEL
            m_voiceCurrentValue.at(i)->setText(QString::number(show_number,'f',1));

        }
    }else if(view_number==2){//门限
        int show_number = dm90_deviceDataSource::getInstance()->loadingChangeNegativeNumber(GET_DEVICE_DATA->dm90_threshold_value);
        MySlider *current_slider = m_horSliders.at(THRESHOLDSLIDER-300);
        current_slider->blockSignals(true);
        current_slider->setValue(show_number+75);
        current_slider->blockSignals(false);
        m_currentValues.at(3)->setText(QString::number(show_number,10)+"dB");
        GET_DEVICE_DATA->dm90_threshold_Enable==0?open_close_btns.at(2)->setCheckState(Qt::Unchecked):open_close_btns.at(2)->setCheckState(Qt::Checked);


    }else if(view_number==3){//去爆音

        int show_number = dm90_deviceDataSource::getInstance()->loadingChangeNegativeNumber(GET_DEVICE_DATA->dm90_Detonator_Threshold);
        MySlider *current_slider = m_horSliders.at(DETONATORSLIDER-300);
        current_slider->blockSignals(true);
        current_slider->setValue(show_number+80);
        current_slider->blockSignals(false);
        m_currentValues.at(4)->setText(QString::number(show_number,10)+"dB");
        GET_DEVICE_DATA->dm90_Detonator_Enable==0?open_close_btns.at(3)->setCheckState(Qt::Unchecked):open_close_btns.at(3)->setCheckState(Qt::Checked);
    }else if(view_number==4){//压缩器

        double show_number = dm90_deviceDataSource::getInstance()->loadingChangeNegativeNumber(GET_DEVICE_DATA->dm90_Comp_Threshold);
        MySlider *current_slider = m_horSliders.at(COMPRESSORSLIDER-300);
        current_slider->blockSignals(true);
        current_slider->setValue(show_number*10+400);
        current_slider->blockSignals(false);
        m_currentValues.at(5)->setText(QString::number(show_number,'f',1)+"dB");
        GET_DEVICE_DATA->dm90_Comp_Enable==0?open_close_btns.at(4)->setCheckState(Qt::Unchecked):open_close_btns.at(4)->setCheckState(Qt::Checked);
    }else if(view_number==5){//限制器
        double show_number = dm90_deviceDataSource::getInstance()->loadingChangeNegativeNumber(GET_DEVICE_DATA->dm90_Limit_Threshold);
        MySlider *current_slider = m_horSliders.at(LIMITERSLIDER-300);
        current_slider->blockSignals(true);
        current_slider->setValue(show_number*10+255);
        current_slider->blockSignals(false);
        m_currentValues.at(6)->setText(QString::number(show_number,'f',1)+"dB");
        GET_DEVICE_DATA->dm90_Limit_Enable==0?open_close_btns.at(5)->setCheckState(Qt::Unchecked):open_close_btns.at(5)->setCheckState(Qt::Checked);
    }else if(view_number==6){//齿音消除器

        int show_number = dm90_deviceDataSource::getInstance()->loadingChangeNegativeNumber(GET_DEVICE_DATA->dm90_Dental_Threshold);
        MySlider *current_slider = m_horSliders.at(DENTALSOUNDSLIDER-300);
        current_slider->blockSignals(true);
        current_slider->setValue(show_number+80);
        current_slider->blockSignals(false);
        m_currentValues.at(7)->setText(QString::number(show_number,10)+"dB");
        GET_DEVICE_DATA->dm90_Dental_Enable==0?open_close_btns.at(6)->setCheckState(Qt::Unchecked):open_close_btns.at(6)->setCheckState(Qt::Checked);

    }else if(view_number==9){//镶边器

        GET_DEVICE_DATA->dm90_flanger_Enable==0?open_close_btns.at(7)->setCheckState(Qt::Unchecked):open_close_btns.at(7)->setCheckState(Qt::Checked);
    }else if(view_number==7){//混响
        int show_number = GET_DEVICE_DATA->dm90_Reverb_WetLevel;
        MySlider *current_slider = m_horSliders.at(REVERBSLIDER-300);
        current_slider->blockSignals(true);
        current_slider->setValue(show_number);
        current_slider->blockSignals(false);
        m_currentValues.at(8)->setText(QString::number(show_number,10));
        GET_DEVICE_DATA->dm90_Reverb_Enable==0?open_close_btns.at(9)->setCheckState(Qt::Unchecked):open_close_btns.at(9)->setCheckState(Qt::Checked);
    }else if(view_number==8){//延迟
        int show_number = GET_DEVICE_DATA->dm90_Echo_Level;
        MySlider *current_slider = m_horSliders.at(DELAYSLIDER-300);
        current_slider->blockSignals(true);
        current_slider->setValue(show_number);
        current_slider->blockSignals(false);
        m_currentValues.at(9)->setText(QString::number(show_number,10));
        GET_DEVICE_DATA->dm90_Echo_Enable==0?open_close_btns.at(10)->setCheckState(Qt::Unchecked):open_close_btns.at(10)->setCheckState(Qt::Checked);

    }else if(view_number==10){//音调下的和声

        GET_DEVICE_DATA->dm90_harmony_Enable==0?open_close_btns.at(8)->setCheckState(Qt::Unchecked):open_close_btns.at(8)->setCheckState(Qt::Checked);//和声

    }

    m_recorderView->show();
}

