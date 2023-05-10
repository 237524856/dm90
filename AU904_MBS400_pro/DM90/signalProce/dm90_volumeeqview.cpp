
#pragma once
#pragma execution_character_set("utf-8")
#include "dm90_volumeeqview.h"
#include "custom_checkbox.h"
#include "custom_button.h"
#include <QDebug>
#include "../window_dm90.h"

dm90_volumeEQView::dm90_volumeEQView(QWidget *parent) : QWidget(parent)
{

    this->setAttribute(Qt::WA_StyledBackground,true);
}

void dm90_volumeEQView::createUI(){

    QWidget *mianView = new QWidget(this);
    mianView->move(40*m_Magnification,0*m_Magnification);
    mianView->setAttribute(Qt::WA_StyledBackground,true);
    mianView->resize(this->width()-10*m_Magnification,this->height()-0*m_Magnification);
    mianView->setStyleSheet("background:rgb(17,17,17);border-radius: 15px;");

    m_highEQ_btn= new custom_checkbox(mianView);
    m_highEQ_btn->setText(tr(" 声音均衡器"));
    m_highEQ_btn->resize(140*m_Magnification,40*m_Magnification);
    m_highEQ_btn->move(20*m_Magnification , 10*m_Magnification);
    QString highpass_stys=QString(  "QCheckBox{font-family:'Source Han Sans CN Medium';color:rgb(255,255,255);font-size:%1px;}"
                                    "QCheckBox::hover{font-family:'Source Han Sans CN Medium';color:rgb(54,54,54);font-size:%1px;}"
                                    "QCheckBox::indicator{width:%2px;height:%2px;color: rgb(255, 0, 0);}"
                                    "QCheckBox::indicator:unchecked{border-image:url(:/image/images/pd400_xianfuunclick_btn.png.png);}"
                                    "QCheckBox::indicator:checked{border-image:url(:/image/images/pd400_xianfuclick_btn.png);}"
                                    "QCheckBox:focus{outline: none;}").arg(20*m_Magnification).arg(14*m_Magnification);
    m_highEQ_btn->setStyleSheet(highpass_stys);
    m_highEQ_btn->setCheckState(Qt::Checked);
    m_highEQ_btn->setLayoutDirection(Qt::RightToLeft);
    connect(m_highEQ_btn,SIGNAL(stateChanged(int)),this,SLOT(isopen_enable_click(int)));

    QVector<QString>title_btns={tr("重置"),tr("确认")};
    for (int i=0;i<2;i++) {
        custom_button *reset_confirm_btn = new custom_button(this);
        reset_confirm_btn->resize(76*m_Magnification,26*m_Magnification);
        reset_confirm_btn->move(338*m_Magnification +i*156*m_Magnification ,mianView->height()-reset_confirm_btn->height()-20*m_Magnification);
        reset_confirm_btn->setText(title_btns.at(i));
        if(i==0){
            QString reset_confirm_btn_stys=QString("QPushButton{border-image:url(:/image/images/pd400_xianfu_reset.png);font-family:'Source Han Sans CN Medium';color:white;font-size:%1px;}"
                                                   "QPushButton:hover{border-image:url(:/image/images/pd400_xianfu_reset.png);font-family:'Source Han Sans CN Medium';color:rgb(54,54,54);font-size:%1px;}"
                                                   "QPushButton:focus{outline: none;}").arg(15*m_Magnification);
            reset_confirm_btn->setStyleSheet(reset_confirm_btn_stys);
            reset_confirm_btn->connect(reset_confirm_btn,SIGNAL(clicked()),this,SLOT(reset_click()));
        }else{
            QString reset_confirm_btn_stys=QString("QPushButton{border-image:url(:/image/images/pd400_xianfu_confirm.png);font-family:'Source Han Sans CN Medium';color:white;font-size:%1px;}"
                                                   "QPushButton:hover{border-image:url(:/image/images/pd400_xianfu_confirm.png);font-family:'Source Han Sans CN Medium';color:rgb(54,54,54);font-size:%1px;}"
                                                   "QPushButton:focus{outline: none;}").arg(15*m_Magnification);
            reset_confirm_btn->setStyleSheet(reset_confirm_btn_stys);
            reset_confirm_btn->connect(reset_confirm_btn,SIGNAL(clicked()),this,SLOT(confirm_click()));

        }
        //m_btns.push_back(reset_confirm_btn);
    }

    //190
    QWidget *dBView = new QWidget(mianView);
    dBView->resize(50*m_Magnification,190*m_Magnification);
    dBView->move(20*m_Magnification,m_highEQ_btn->y()+m_highEQ_btn->height()+24*m_Magnification);
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

        QLabel *dB_label = new QLabel(mianView);
        dB_label->resize(40*m_Magnification,14*m_Magnification);
        dB_label->move(100*m_Magnification+80*m_Magnification*i,m_highEQ_btn->y()+m_highEQ_btn->height());
        QString updateLabel_stys = QString("background-color:transparent;color:rgb(255,255,255);font-size:%1px;font-family:'Source Han Sans CN Medium'").arg(12*m_Magnification);
        dB_label->setStyleSheet(updateLabel_stys);
        dB_label->setText("-11.6");
        dB_label ->setAlignment(Qt::AlignHCenter);
        m_voiceCurrentValue.push_back(dB_label);

        //创建SLIDER
        SliderWidget *verticalSlider = new SliderWidget(mianView);
        verticalSlider->SetValue(12);
        verticalSlider->SetRange(0,240);
        verticalSlider->SetVertical(true);
        verticalSlider->setObjectName(QString::number(i,10));
        verticalSlider->setGeometry(dB_label->x()+(dB_label->width()-12*m_Magnification)/2,dB_label->y()+dB_label->height()+10*m_Magnification,12*m_Magnification,dBView->height());
        m_verSliders.push_back(verticalSlider);
        connect(verticalSlider,SIGNAL(valueChanged(int)),this,SLOT(verticalSlider_setValue(int)));

        QLabel *bottom_label = new QLabel(mianView);
        bottom_label->resize(40*m_Magnification,16*m_Magnification);
        bottom_label->move(100*m_Magnification+80*m_Magnification*i,verticalSlider->y()+verticalSlider->height()+10*m_Magnification);
        //QString updateLabel_stys = QString("background-color:transparent;color:rgb(255,255,255);font-size:%1px;font-family:'Source Han Sans CN Medium'").arg(12*m_Magnification);
        bottom_label->setStyleSheet(updateLabel_stys);
        bottom_label->setText(highmidlow_title.at(i));
        bottom_label ->setAlignment(Qt::AlignHCenter);
    }

    QWidget *dBView_right = new QWidget(mianView);
    dBView_right->resize(50*m_Magnification,190*m_Magnification);
    dBView_right->move(320*m_Magnification,m_highEQ_btn->y()+m_highEQ_btn->height()+24*m_Magnification);
    dBView_right->setAttribute(Qt::WA_StyledBackground,true);

    for (int i=0;i<3;i++) {
        QLabel *dB_label = new QLabel(dBView_right);
        dB_label->resize(dBView->width(),14*m_Magnification);
        dB_label->move(0,(dBView->height()/3+dB_label->height())*i);
        if(i==2)
        dB_label->move(0,dBView->height()-dB_label->height()-6*m_Magnification);
        QString updateLabel_stys = QString("background-color:transparent;color:rgb(255,255,255);font-size:%1px;font-family:'Source Han Sans CN Medium'").arg(12*m_Magnification);
        dB_label->setStyleSheet(updateLabel_stys);
        dB_label->setText(dB_labels.at(i));
        dB_label ->setAlignment(Qt::AlignHCenter);
    }

    //下面的拖动条
    QWidget *dBView_bottom = new QWidget(mianView);
    dBView_bottom->resize(50*m_Magnification,190*m_Magnification);
    dBView_bottom->move(20*m_Magnification,330*m_Magnification);
    dBView_bottom->setAttribute(Qt::WA_StyledBackground,true);

    for (int i=0;i<3;i++) {
        QLabel *dB_label = new QLabel(dBView_bottom);
        dB_label->resize(dBView->width(),14*m_Magnification);
        dB_label->move(0,(dBView->height()/3+dB_label->height())*i);
        if(i==2)
        dB_label->move(0,dBView->height()-dB_label->height()-6*m_Magnification);
        QString updateLabel_stys = QString("background-color:transparent;color:rgb(255,255,255);font-size:%1px;font-family:'Source Han Sans CN Medium'").arg(12*m_Magnification);
        dB_label->setStyleSheet(updateLabel_stys);
        dB_label->setText(dB_labels.at(i));
        dB_label ->setAlignment(Qt::AlignHCenter);
    }
    for (int i=0;i<5;i++) {

        QLabel *dB_label = new QLabel(mianView);
        dB_label->resize(40*m_Magnification,14*m_Magnification);
        dB_label->move(80*m_Magnification+50*m_Magnification*i,310*m_Magnification);
        QString updateLabel_stys = QString("background-color:transparent;color:rgb(255,255,255);font-size:%1px;font-family:'Source Han Sans CN Medium'").arg(12*m_Magnification);
        dB_label->setStyleSheet(updateLabel_stys);
        dB_label->setText("-11.6");
        dB_label ->setAlignment(Qt::AlignHCenter);
        m_voiceCurrentValue.push_back(dB_label);

        //创建SLIDER
        SliderWidget *verticalSlider = new SliderWidget(mianView);
        verticalSlider->SetValue(12);
        verticalSlider->SetRange(0,240);
        verticalSlider->SetVertical(true);
        verticalSlider->setObjectName(QString::number(i+3,10));
        verticalSlider->setGeometry(dB_label->x()+(dB_label->width()-12*m_Magnification)/2,dB_label->y()+dB_label->height()+10*m_Magnification,12*m_Magnification,dBView->height());
        m_verSliders.push_back(verticalSlider);
        connect(verticalSlider,SIGNAL(valueChanged(int)),this,SLOT(verticalSlider_setValue(int)));

        QLabel *bottom_label = new QLabel(mianView);
        bottom_label->resize(40*m_Magnification,14*m_Magnification);
        bottom_label->move(80*m_Magnification+50*m_Magnification*i,verticalSlider->y()+verticalSlider->height()+10*m_Magnification);
        //QString updateLabel_stys = QString("background-color:transparent;color:rgb(255,255,255);font-size:%1px;font-family:'Source Han Sans CN Medium'").arg(12*m_Magnification);
        bottom_label->setStyleSheet(updateLabel_stys);
        bottom_label->setText("1000");
        bottom_label ->setAlignment(Qt::AlignHCenter);
    }

    QWidget *dBViewBottom_right = new QWidget(mianView);
    dBViewBottom_right->resize(50*m_Magnification,190*m_Magnification);
    dBViewBottom_right->move(320*m_Magnification,330*m_Magnification);
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
    //右边的视图---------------------------------------------------------------------
    for (int i =0;i<6;i++) {
        QVector<QString>detonator_titles={tr("低(频率)"),tr("宽"),tr("中(频率)"),tr("宽"),tr("高(频率)"),tr("宽")};
        QVector<QString>highlowLeft={"20Hz",tr("宽"),"400Hz",tr("宽"),"1000Hz",tr("宽")};
        QVector<QString>highlowRight={"500Hz",tr("窄"),"5000Hz",tr("窄"),"12000Hz",tr("窄")};

        QVector<int>min_values={20,50,400,50,1000,50};
        QVector<int>max_values={500,1000,5000,1000,12000,1000};

        QWidget *threshould_unit = new QWidget(mianView);
        threshould_unit->setAttribute(Qt::WA_StyledBackground,true);
        threshould_unit->resize(mianView->width()/2,82*m_Magnification);
        threshould_unit->move(400*m_Magnification,m_highEQ_btn->y()+m_highEQ_btn->height()+threshould_unit->height()*i);

        QLabel *threshould_lab = new QLabel(threshould_unit);
        threshould_lab->move(40*m_Magnification,0*m_Magnification);
        threshould_lab->resize(100*m_Magnification,20*m_Magnification);

        QString strText = detonator_titles.at(i);
        QString highLow_lab_stys = QString("background-color:transparent;color:rgb(255,255,255);font-size:%1px;font-family:'Source Han Sans CN Medium'").arg(12*m_Magnification);
        threshould_lab->setStyleSheet(highLow_lab_stys);
        if(!strText.isEmpty())
            {
                QRegExp valueRegExp;
                if(i==0){
                    valueRegExp = QRegExp(QString("(%1)").arg("低"));
                    valueRegExp.setCaseSensitivity(Qt::CaseInsensitive);
                    QString font_str0 = QString("<font style='font-size:%1px; color:white;'>\\1</font>").arg(16*m_Magnification);
                    strText = strText.replace(valueRegExp,font_str0);
                }else if(i==2){
                    valueRegExp = QRegExp(QString("(%1)").arg("中"));
                    valueRegExp.setCaseSensitivity(Qt::CaseInsensitive);
                    QString font_str1 = QString("<font style='font-size:%1px; color:white;'>\\1</font>").arg(16*m_Magnification);
                    strText = strText.replace(valueRegExp,font_str1);
                }else if(i==4){
                    valueRegExp = QRegExp(QString("(%1)").arg("高"));
                    valueRegExp.setCaseSensitivity(Qt::CaseInsensitive);
                    QString font_str2 = QString("<font style='font-size:32px; color:white;'>\\1</font>").arg(16*m_Magnification);
                    strText = strText.replace(valueRegExp,font_str2);
                }

            }
        threshould_lab->setText(strText);

        MySlider *threshould_slider = new MySlider(threshould_unit);
        threshould_slider->setOrientation(Qt::Horizontal);
        threshould_slider->setMinimum(min_values.at(i));
        threshould_slider->setMaximum(max_values.at(i));
        threshould_slider->setValue(xianfuqi_slider_values.at(0));
        threshould_slider->setGeometry(40*m_Magnification,threshould_lab->y()+threshould_lab->height()+10*m_Magnification,380*m_Magnification,12*m_Magnification);
        threshould_slider->setObjectName(QString::number(i+8,10));
        connect(threshould_slider,SIGNAL(valueChanged(int)),this,SLOT(horVoiceEQ_chanege_value(int)));
        connect(threshould_slider,SIGNAL(sliderReleasedAt(int)),this,SLOT(horVoiceEQ_release_value(int)));
        m_horSliders.push_back(threshould_slider);

        QLabel *current_value = new QLabel(threshould_unit);
        current_value->move(threshould_slider->width()+threshould_slider->x()+10*m_Magnification,threshould_slider->y()-2*m_Magnification);
        current_value->resize(60*m_Magnification,14*m_Magnification);
        current_value->setText("-80dB");
        QString currentLabel_stys = QString("background-color:transparent;color:rgb(255,255,255);font-size:%1px;font-family:'Source Han Sans CN Medium'").arg(12*m_Magnification);
        current_value->setStyleSheet(currentLabel_stys);
        m_voiceCurrentValue.push_back(current_value);

        QLabel *left_value = new QLabel(threshould_unit);
        left_value->resize(60*m_Magnification,14*m_Magnification);
        left_value->move(threshould_slider->x(),threshould_slider->y()+threshould_slider->height()+5*m_Magnification);
        left_value->setText(highlowLeft.at(i));
        left_value->setStyleSheet(currentLabel_stys);

        QLabel *right_value = new QLabel(threshould_unit);
        right_value->resize(50*m_Magnification,16*m_Magnification);
        right_value->move(threshould_slider->x()+threshould_slider->width()-right_value->width(),threshould_slider->y()+threshould_slider->height()+5*m_Magnification);
        right_value->setText(highlowRight.at(i));
        right_value->setStyleSheet(currentLabel_stys);
        right_value ->setAlignment(Qt::AlignRight);
    }
}

void dm90_volumeEQView::refreshUI(){

    //按钮的使能状态
    if(GET_DEVICE_DATA->dm90micEQdatasource[0].Enable){//打开
        m_highEQ_btn->setCheckState(Qt::Checked);
    }else{//关闭
        m_highEQ_btn->setCheckState(Qt::Unchecked);
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
    }

    //各种水平的进度条

    for (int i=0;i<m_horSliders.count();i++) {

        MySlider *current_slider = m_horSliders.at(i);
        current_slider->blockSignals(true);
        if(i%2==0){
           current_slider->setValue(GET_DEVICE_DATA->dm90micEQdatasource[i/2].Freq);
           m_voiceCurrentValue.at(i+8)->setText(QString::number(GET_DEVICE_DATA->dm90micEQdatasource[i/2].Freq,10)+"Hz");

        }else{
            //整数时去掉后面的0
            QRegExp reg;
            reg.setPattern("(\\.){0,1}0+$");

            double show_number = (double)GET_DEVICE_DATA->dm90micEQdatasource[i/2].Q/100;

            current_slider->setValue(GET_DEVICE_DATA->dm90micEQdatasource[i/2].Q);

            QString value_num = QString("%1").arg(show_number, 0, 'f', 2).replace(reg, "");
            m_voiceCurrentValue.at(i+8)->setText(value_num);
        }
        current_slider->blockSignals(false);
    }

}


//各种垂直进度条的方法
void dm90_volumeEQView::verticalSlider_setValue(int currentValue){

    m_DM90delayTimer->stop();
    m_DM90delayTimer->start(20);
    SliderWidget *verticalSlider = static_cast<SliderWidget *>(sender());

    int slider_number = verticalSlider->objectName().toUInt();
    QLabel *currentValue_label = m_voiceCurrentValue.at(slider_number);
    double showCurrent = ((double)currentValue-120)/10;
    currentValue_label->setText(QString::number(showCurrent,'f',1));
    m_currentSliderNum = slider_number+1;
}

//水平滑块的方法
void dm90_volumeEQView::horVoiceEQ_chanege_value(int currentValue){

    MySlider *verticalSlider = static_cast<MySlider *>(sender());
    int slider_number = verticalSlider->objectName().toUInt();
    QLabel *currentValue_label = m_voiceCurrentValue.at(slider_number);

    QRegExp reg;
    reg.setPattern("(\\.){0,1}0+$");
    QString value_num="";
    if(slider_number%2==0){
        value_num = QString("%1").arg((double)currentValue, 0, 'f', 2).replace(reg, "")+"Hz";
    }else{
        value_num = QString("%1").arg((double)currentValue/100, 0, 'f', 2).replace(reg, "");
    }
    currentValue_label->setText(value_num);

    m_DM90delayTimer->stop();
    m_DM90delayTimer->start(20);
    m_currentSliderNum = slider_number+100;

}
//延时发送数据的方法==================================
void dm90_volumeEQView::VoluemeStarDelayTimerFun(){

    if(Qwdget_timer_index==4){//垂直滑块
        window_dm90* mainWindow = (window_dm90*)(this->parentWidget()->parentWidget());//获取列表视图

        if(m_currentSliderNum<100){
            SliderWidget *verticalSlider =  m_verSliders.at(m_currentSliderNum-1);
            int send_Value = dm90_deviceDataSource::getInstance()->loadingChangeDeviceNumber((double)(verticalSlider->GetValue()-120)/10);
            m_commin_fun.sendMessageForDevice(DM90PARAM_MIC_PEQ_START+3+5*(m_currentSliderNum-1),1,send_Value);
            //赋值
            GET_DEVICE_DATA->dm90micEQdatasource[m_currentSliderNum-1].Gain = send_Value;
            mainWindow->m_midView->m_localCustom_itemList->addItem(mainWindow->m_midView->m_defaultTtemRow,mainWindow->m_midView->m_itemtext,7+3+5*(m_currentSliderNum-1),send_Value);

        }else{//水平滑块
            if(m_currentSliderNum%2==0){
                MySlider *hor_slider = m_horSliders.at(m_currentSliderNum-100-8);
                //发送值
                m_commin_fun.sendMessageForDevice(DM90PARAM_MIC_PEQ_START+2+5*((m_currentSliderNum-100-8)/2),1,hor_slider->value());
                GET_DEVICE_DATA->dm90micEQdatasource[(m_currentSliderNum-100-8)/2].Freq = hor_slider->value();
                mainWindow->m_midView->m_localCustom_itemList->addItem(mainWindow->m_midView->m_defaultTtemRow,mainWindow->m_midView->m_itemtext,7+2+5*((m_currentSliderNum-100-8)/2),hor_slider->value());

            }else{
                MySlider *hor_slider = m_horSliders.at(m_currentSliderNum-100-8);
                m_commin_fun.sendMessageForDevice(DM90PARAM_MIC_PEQ_START+4+5*((m_currentSliderNum-100-8)/2),1,hor_slider->value());
                GET_DEVICE_DATA->dm90micEQdatasource[(m_currentSliderNum-100-8)/2].Q = hor_slider->value();
                mainWindow->m_midView->m_localCustom_itemList->addItem(mainWindow->m_midView->m_defaultTtemRow,mainWindow->m_midView->m_itemtext,7+4+5*((m_currentSliderNum-100-8)/2),hor_slider->value());
            }
        }
        //发完数据之后停止计时器
        m_DM90delayTimer->stop();
    }
}

void dm90_volumeEQView::horVoiceEQ_release_value(int currentValue){

}

//重置按钮=====================
void dm90_volumeEQView::reset_click(){

    //刷新当前UI界面
    for (int i=0;i<m_verSliders.count();i++) {

        GET_DEVICE_DATA->dm90micEQdatasource[i].Gain = dm90_deviceDataSource::getInstance()->loadingChangeDeviceNumber(0);
        GET_DEVICE_DATA->dm90micEQdatasource[i].Enable = 0;
    }

    QVector<int>freqValues={250,1000,4000};
    for (int i=0;i<m_horSliders.count();i++) {

        if(i%2==0){
            GET_DEVICE_DATA->dm90micEQdatasource[i/2].Freq = freqValues.at(i/2);
        }else{
            GET_DEVICE_DATA->dm90micEQdatasource[i/2].Q = 100;
        }
    }
    refreshUI();
    //发送数据
    QVector<int>addrs;
    QVector<int>value;
    for (int i=0;i<m_verSliders.count();i++) {
        addrs.push_back(0x2032+5*i);
        value.push_back(2000);
    }
    for (int i=0;i<m_horSliders.count();i++) {
        if(i%2==0){
            addrs.push_back(0x2031+5*(i/2));
            qDebug()<<"jjjjjj"<<i/2;
            value.push_back(freqValues.at(i/2));
        }else{//Q值
            addrs.push_back(0x2033+5*(i/2));
            value.push_back(100);
        }
    }
   m_commin_fun.sendRandomMessageForDevice(0x3f,addrs,value);



   //使能开关的数据发送
   QVector<int>addrs_disable;
   QVector<int>value_disable;
   for (int i=0;i<8;i++) {
       addrs_disable.push_back(0x202f+5*i);
       value_disable.push_back(0);
   }
  m_commin_fun.sendRandomMessageForDevice(0x27,addrs_disable,value_disable);

}

void dm90_volumeEQView::confirm_click(){

    this->hide();
    QWidget* pWidget = this->parentWidget();
    pWidget->hide();
    Qwdget_timer_index = 1;
    //同步当前的值
    for (int i=0;i<3;i++) {

        SliderWidget *verticalSlider =  m_verSliders.at(i);
        int send_Value = dm90_deviceDataSource::getInstance()->loadingChangeDeviceNumber((double)(verticalSlider->GetValue()-120)/10);
        GET_DEVICE_DATA->dm90micEQdatasource[i].Gain = send_Value;
    }
    emit confirmClick(1);

}
//开关的方法
void dm90_volumeEQView::isopen_enable_click(int enable_status){

    if(enable_status){//打开

        for (int i=0;i<m_verSliders.count();i++) {

            m_verSliders.at(i)->SetDisable(false);
        }

        for (int i=0;i<m_horSliders.count();i++) {

            m_horSliders.at(i)->setSliderDisable(false);
        }

         GET_DEVICE_DATA->dm90micEQdatasource[0].Enable = 1;

         //发送数据
         QVector<int>addrs;
         QVector<int>value;
         for (int i=0;i<8;i++) {
             addrs.push_back(0x202f+5*i);
             value.push_back(1);
         }
         //if(GET_DEVICE_DATA->dm90micEQdatasource[0].Enable  != enable_status-1)
        m_commin_fun.sendRandomMessageForDevice(0x27,addrs,value);
        GET_DEVICE_DATA->dm90micEQdatasource[0].Enable = 1;


    }else{//关闭

        for (int i=0;i<m_verSliders.count();i++) {

            m_verSliders.at(i)->SetDisable(true);

        }
        for (int i=0;i<m_horSliders.count();i++) {

            m_horSliders.at(i)->setSliderDisable(true);
        }

        GET_DEVICE_DATA->dm90micEQdatasource[0].Enable = 0;

        QVector<int>addrs;
        QVector<int>value;
        for (int i=0;i<8;i++) {
            addrs.push_back(0x202f+5*i);
            value.push_back(0);
        }
       //if(GET_DEVICE_DATA->dm90micEQdatasource[0].Enable  != enable_status)
       m_commin_fun.sendRandomMessageForDevice(0x27,addrs,value);
       GET_DEVICE_DATA->dm90micEQdatasource[0].Enable = 0;

    }

}
