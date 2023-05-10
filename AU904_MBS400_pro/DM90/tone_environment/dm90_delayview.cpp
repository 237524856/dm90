
#pragma once
#pragma execution_character_set("utf-8")
#include "dm90_delayview.h"
#include "custom_checkbox.h"
#include "custom_button.h"
#include "../window_dm90.h"

dm90_delayView::dm90_delayView(QWidget *parent) : QWidget(parent)
{
    this->setAttribute(Qt::WA_StyledBackground,true);
}
void dm90_delayView::createUI(){

    QWidget *mianView = new QWidget(this);
    mianView->move(40*m_Magnification,50*m_Magnification);
    mianView->setAttribute(Qt::WA_StyledBackground,true);
    mianView->resize(this->width()-80*m_Magnification,this->height()-100*m_Magnification);
    mianView->setStyleSheet("background:rgb(17,17,17);border-radius: 15px;");


    m_highEQ_btn= new custom_checkbox(mianView);
    m_highEQ_btn->setText(tr("  延迟"));
    language_index==0?m_highEQ_btn->resize(100*m_Magnification,40*m_Magnification):m_highEQ_btn->resize(110*m_Magnification,40*m_Magnification);
    m_highEQ_btn->move(20*m_Magnification , 20*m_Magnification);
    QString highpass_stys=QString(  "QCheckBox{font-family:'Source Han Sans CN Medium';color:rgb(255,255,255);font-size:%1px;}"
                                    "QCheckBox::hover{font-family:'Source Han Sans CN Medium';color:rgb(54,54,54);font-size:%1px;}"
                                    "QCheckBox::indicator{width:%2px;height:%2px;color: rgb(255, 0, 0);}"
                                    "QCheckBox::indicator:unchecked{border-image:url(:/image/images/pd400_xianfuunclick_btn.png.png);}"
                                    "QCheckBox::indicator:checked{border-image:url(:/image/images/pd400_xianfuclick_btn.png);}"
                                    "QCheckBox:focus{outline: none;}").arg(30*m_Magnification).arg(14*m_Magnification);
    m_highEQ_btn->setStyleSheet(highpass_stys);
    m_highEQ_btn->setCheckState(Qt::Checked);
    m_highEQ_btn->setLayoutDirection(Qt::RightToLeft);
    connect(m_highEQ_btn,SIGNAL(stateChanged(int)),this,SLOT(isopen_enable_click(int)));

    //创建进度条
    QVector<QString>threshouldTitle={tr("延迟音量"),tr("延迟时间"),tr("反馈衰减"),tr("最大延迟")};
    QVector<QString>highlowLeft={"0","8ms","0","8ms"};
    QVector<QString>highlowRight={"100","500ms","0.90","500ms"};

    //最小值
    QVector<int>slider_minValue={0,8,0,8};
    //最大值
    QVector<int>slider_maxValue={100,500,90,500};

    for (int j=0;j<2;j++) {

        for (int i=0;i<2;i++) {
            //         if(i+2*j != 3){
            QWidget *threshould_unit = new QWidget(mianView);
            threshould_unit->setAttribute(Qt::WA_StyledBackground,true);
            threshould_unit->resize(mianView->width()/2,100*m_Magnification);
            threshould_unit->move(mianView->width()/2*i,m_highEQ_btn->height()+m_highEQ_btn->height() +threshould_unit->height()*j);

            QLabel *threshould_lab = new QLabel(threshould_unit);
            threshould_lab->move(40*m_Magnification,14*m_Magnification);
            threshould_lab->resize(150*m_Magnification,16*m_Magnification);
            threshould_lab->setText(threshouldTitle.at(i+2*j));
            QString highLow_lab_stys = QString("background-color:transparent;color:rgb(255,255,255);font-size:%1px;font-family:'Source Han Sans CN Medium'").arg(12*m_Magnification);
            threshould_lab->setStyleSheet(highLow_lab_stys);

            MySlider *threshould_slider = new MySlider(threshould_unit);
            threshould_slider->setOrientation(Qt::Horizontal);
            threshould_slider->setMinimum(slider_minValue.at(i+2*j));
            threshould_slider->setMaximum(slider_maxValue.at(i+2*j));
            threshould_slider->setValue(xianfuqi_slider_values.at(0));
            threshould_slider->setObjectName(QString::number(i+2*j,10));
            threshould_slider->setGeometry(40*m_Magnification,threshould_lab->y()+threshould_lab->height()+10*m_Magnification,310*m_Magnification,12*m_Magnification);
            connect(threshould_slider,SIGNAL(valueChanged(int)),this,SLOT(delay_chanege_value(int)));
            connect(threshould_slider,SIGNAL(sliderReleasedAt(int)),this,SLOT(delay_release_value(int)));
            m_delaySliders.push_back(threshould_slider);

            QLabel *current_value = new QLabel(threshould_unit);
            current_value->move(threshould_slider->width()+threshould_slider->x()+10*m_Magnification,threshould_slider->y()-2*m_Magnification);
            current_value->resize(60*m_Magnification,14*m_Magnification);
            current_value->setText("-80dB");
            QString currentLabel_stys = QString("background-color:transparent;color:rgb(255,255,255);font-size:%1px;font-family:'Source Han Sans CN Medium'").arg(12*m_Magnification);
            current_value->setStyleSheet(currentLabel_stys);
            m_showValueLabels.push_back(current_value);

            QLabel *left_value = new QLabel(threshould_unit);
            left_value->resize(90*m_Magnification,14*m_Magnification);
            left_value->move(threshould_slider->x(),threshould_slider->y()+threshould_slider->height()+5*m_Magnification);
            left_value->setText(highlowLeft.at(i+2*j));
            left_value->setStyleSheet(currentLabel_stys);

            QLabel *right_value = new QLabel(threshould_unit);
            right_value->resize(50*m_Magnification,16*m_Magnification);
            right_value->move(threshould_slider->x()+threshould_slider->width()-right_value->width(),threshould_slider->y()+threshould_slider->height()+5*m_Magnification);
            right_value->setText(highlowRight.at(i+2*j));
            right_value->setStyleSheet(currentLabel_stys);
            right_value ->setAlignment(Qt::AlignRight);
            if(i+2*j == 1){
                m_changeMaxLabel = right_value;
            }
            //         }
        }

    }
    QVector<QString>title_btns={tr("重置"),tr("确认")};
    for (int i=0;i<2;i++) {
        custom_button *reset_confirm_btn = new custom_button(this);
        reset_confirm_btn->resize(76*m_Magnification,26*m_Magnification);
        reset_confirm_btn->move(338*m_Magnification +i*156*m_Magnification ,this->height()-reset_confirm_btn->height()-100*m_Magnification);
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
}

void dm90_delayView::refreshUI(){

    //按钮的使能状态
    if(GET_DEVICE_DATA->dm90_Echo_Enable){//打开
        m_highEQ_btn->setCheckState(Qt::Checked);
    }else{//关闭
        m_highEQ_btn->setCheckState(Qt::Unchecked);
    }

    for (int i =0;i<m_delaySliders.count();i++) {

        MySlider *current_slider = m_delaySliders.at(i);
        current_slider->blockSignals(true);
        switch (i) {

        case LEVEL_SLIDER:{
            current_slider->setValue(GET_DEVICE_DATA->dm90_Echo_Level);
            m_showValueLabels.at(i)->setText(QString::number(GET_DEVICE_DATA->dm90_Echo_Level,10));
        }
            break;

        case DELAY_SLIDER:{//两种情况，第一种Delay没有大于最大延迟，第二种是大于Delay大于最大延迟

            current_slider->setValue(GET_DEVICE_DATA->dm90_Echo_Delay);
            m_showValueLabels.at(i)->setText(QString::number(GET_DEVICE_DATA->dm90_Echo_Delay,10)+"ms");

        }
            break;

        case FEEDBACK_SLIDER:{

            current_slider->setValue(GET_DEVICE_DATA->dm90_Echo_Feedback);
            double show_number = (double)GET_DEVICE_DATA->dm90_Echo_Feedback/100;
            m_showValueLabels.at(i)->setText(QString::number(show_number,'f',2));

        }
            break;
        case MAXDELAY:{
            current_slider->setValue(GET_DEVICE_DATA->dm90_Echo_MaxDelay);
            m_showValueLabels.at(i)->setText(QString::number(GET_DEVICE_DATA->dm90_Echo_MaxDelay,10)+"ms");
            //刷新可变的值
            m_changeMaxLabel->setText(QString::number(GET_DEVICE_DATA->dm90_Echo_MaxDelay,10)+"ms");
        }
            break;

        default:
            break;
        }
        current_slider->blockSignals(false);
    }

}

void dm90_delayView::delay_chanege_value(int value){

    m_DM90delayTimer->stop();
    m_DM90delayTimer->start(20);
    MySlider *detonator_slider = static_cast<MySlider *>(sender());
    int slider_number = detonator_slider->objectName().toUInt();
    m_currentSliderNum = slider_number;

    switch (slider_number) {
    case LEVEL_SLIDER:{

        m_showValueLabels.at(slider_number)->setText(QString::number(value,10));
    }
        break;

    case DELAY_SLIDER:
    {

        m_showValueLabels.at(slider_number)->setText(QString::number(value,10)+"ms");

    }
        break;

    case FEEDBACK_SLIDER:{

        double show_number = (double)value/100;
        m_showValueLabels.at(slider_number)->setText(QString::number(show_number,'f',2));

    }
        break;

    case MAXDELAY:{

        m_delaySliders.at(DELAY_SLIDER)->blockSignals(true);
        m_delaySliders.at(DELAY_SLIDER)->setMaximum(value);
        //分两种情况，第一种Delay没有大于最大延迟，第二种是大于Delay大于最大延迟
        if(m_delaySliders.at(MAXDELAY)->value()>m_delaySliders.at(DELAY_SLIDER)->value()){

            m_showValueLabels.at(DELAY_SLIDER)->setText(QString::number(m_delaySliders.at(DELAY_SLIDER)->value(),10)+"ms");
        }else{
            m_showValueLabels.at(DELAY_SLIDER)->setText(QString::number(value,10)+"ms");
        }
        m_delaySliders.at(DELAY_SLIDER)->blockSignals(false);
        m_changeMaxLabel->setText(QString::number(value,10)+"ms");
        m_showValueLabels.at(slider_number)->setText(QString::number(value,10)+"ms");

    }
        break;
    default:
        break;

    }

}
//计时器延时发送数据
void dm90_delayView::delayStarDelayTimerFun(){

    if(Qwdget_timer_index==13){
        //发送数据
        MySlider *current_slider = m_delaySliders.at(m_currentSliderNum);
        window_dm90* mainWindow = (window_dm90*)(this->parentWidget()->parentWidget());

        switch (m_currentSliderNum) {
        case LEVEL_SLIDER:{

            m_commin_fun.sendMessageForDevice(DM90PARAM_MIC_ECHO_START+2,1,current_slider->value());
            GET_DEVICE_DATA->dm90_Echo_Level = current_slider->value();
            mainWindow->m_midView->m_effectCustom_itemList->addItem(mainWindow->m_midView->m_defaultTtemRow,mainWindow->m_midView->m_itemtext,22+2,GET_DEVICE_DATA->dm90_Echo_Level);
        }
            break;

        case DELAY_SLIDER:{//延迟时间

            m_commin_fun.sendMessageForDevice(DM90PARAM_MIC_ECHO_START+4,1,current_slider->value());
            GET_DEVICE_DATA->dm90_Echo_Delay = current_slider->value();
            mainWindow->m_midView->m_effectCustom_itemList->addItem(mainWindow->m_midView->m_defaultTtemRow,mainWindow->m_midView->m_itemtext,22+4,GET_DEVICE_DATA->dm90_Echo_Delay);

        }
            break;

        case FEEDBACK_SLIDER:{

            m_commin_fun.sendMessageForDevice(DM90PARAM_MIC_ECHO_START+3,1,current_slider->value());
            GET_DEVICE_DATA->dm90_Echo_Feedback = current_slider->value();
            mainWindow->m_midView->m_effectCustom_itemList->addItem(mainWindow->m_midView->m_defaultTtemRow,mainWindow->m_midView->m_itemtext,22+3,GET_DEVICE_DATA->dm90_Echo_Feedback);
        }
            break;
        case MAXDELAY:{//最大延迟时间

            if(current_slider->value()>m_delaySliders.at(DELAY_SLIDER)->value()){

                m_commin_fun.sendMessageForDevice(DM90PARAM_MIC_ECHO_START+5,1,current_slider->value());
                GET_DEVICE_DATA->dm90_Echo_Delay = current_slider->value();
                mainWindow->m_midView->m_effectCustom_itemList->addItem(mainWindow->m_midView->m_defaultTtemRow,mainWindow->m_midView->m_itemtext,22+5,GET_DEVICE_DATA->dm90_Echo_Delay);

            }else{//第二种情况只能是等于,等于的时候同时发延时和最大延时的数据

                //发送数据
                QVector<int>addrs;
                QVector<int>value = {current_slider->value(),current_slider->value()};
                for (int i=0;i<value.count();i++) {
                    addrs.push_back(DM90PARAM_MIC_ECHO_START+4+i);
                }
                int frame_len = 7 + 4 *value.count();
                m_commin_fun.sendRandomMessageForDevice(frame_len,addrs,value);
            }

        }
            break;

        default:
            break;

        }

    }
}
void dm90_delayView::delay_release_value(int){

}
//重置按钮=====================
void dm90_delayView::reset_click(){

    //刷新当前UI界面
    GET_DEVICE_DATA->dm90_Echo_Enable = 0;
    GET_DEVICE_DATA->dm90_Echo_Level = 100;
    GET_DEVICE_DATA->dm90_Echo_Feedback = 40;
    GET_DEVICE_DATA->dm90_Echo_Delay = 40;
    refreshUI();//刷新界面
    //发送数据
    QVector<int>addrs;
    QVector<int>value = {0,100,40,40};
    for (int i=0;i<value.count();i++) {
        addrs.push_back(DM90PARAM_MIC_ECHO_START+1+i);
    }
    int frame_len = 7 + 4 *value.count();
    m_commin_fun.sendRandomMessageForDevice(frame_len,addrs,value);
}

void dm90_delayView::confirm_click(){
    this->hide();
    QWidget* pWidget = this->parentWidget();
    pWidget->hide();
    com_recordView->m_waveView->m_chartView->show();
    Qwdget_timer_index = 1;
    int send_Value = m_delaySliders.at(0)->value();
    GET_DEVICE_DATA->dm90_Echo_Level = send_Value;
    confirmClick(8);
}

void dm90_delayView::isopen_enable_click(int enable_status){

    if(enable_status){//打开
        if(GET_DEVICE_DATA->dm90_Echo_Enable != enable_status-1)
            m_commin_fun.sendMessageForDevice(DM90PARAM_MIC_ECHO_START+1,1,1);
        for (int i=0;i<m_delaySliders.count();i++) {
            m_delaySliders.at(i)->setSliderDisable(false);
        }
        GET_DEVICE_DATA->dm90_Echo_Enable =1;

    }else{//关闭
        if(GET_DEVICE_DATA->dm90_Echo_Enable != enable_status)
            m_commin_fun.sendMessageForDevice(DM90PARAM_MIC_ECHO_START+1,1,0);
        for (int i=0;i<m_delaySliders.count();i++) {
            m_delaySliders.at(i)->setSliderDisable(true);
        }
        GET_DEVICE_DATA->dm90_Echo_Enable =0;
    }
}
