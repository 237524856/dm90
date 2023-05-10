#pragma once
#pragma execution_character_set("utf-8")
#include "dm90_detonatorview.h"
#include "custom_checkbox.h"
#include "custom_button.h"
#include "../window_dm90.h"

dm90_detonatorView::dm90_detonatorView(QWidget *parent) : QWidget(parent)
{
    this->setAttribute(Qt::WA_StyledBackground,true);

}

void dm90_detonatorView::createUI(){

    QWidget *mianView = new QWidget(this);
    mianView->move(40*m_Magnification,50*m_Magnification);
    mianView->setAttribute(Qt::WA_StyledBackground,true);
    mianView->resize(this->width()-80*m_Magnification,this->height()-100*m_Magnification);
    mianView->setStyleSheet("background:rgb(17,17,17);border-radius: 15px;");

    m_highEQ_btn= new custom_checkbox(mianView);
    m_highEQ_btn->setText(tr(" 去爆音器"));
    language_index==0?m_highEQ_btn->resize(110*m_Magnification,40*m_Magnification):m_highEQ_btn->resize(150*m_Magnification,40*m_Magnification);
    m_highEQ_btn->move(20*m_Magnification , 20*m_Magnification);
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

    //创建进度条
    QVector<QString>detonator_titles={tr("阈值"),tr("恢复时间"),tr("频率"),tr("最小增益"),tr("Q值"),tr("检测时间"),tr("启动时间")};
    QVector<QString>highlowLeft={"-80dB","1ms","50Hz","-50dB","0.2","1ms","1ms"};
    QVector<QString>highlowRight={"-1dB","4000ms","10000Hz","-1dB","20","255ms","255ms"};

    //最小值
    QVector<int>slider_minValue={0,1,50,1,20,1,1};
    //最大值
    QVector<int>slider_maxValue={79,4000,10000,49,2000,255,255};

    for (int j=0;j<4;j++) {

        for (int i=0;i<2;i++) {
            if(i+2*j != 7){

                QWidget *threshould_unit = new QWidget(mianView);
                threshould_unit->setAttribute(Qt::WA_StyledBackground,true);
                threshould_unit->resize(mianView->width()/2,100*m_Magnification);
                threshould_unit->move(mianView->width()/2*i,m_highEQ_btn->y()+m_highEQ_btn->height() +threshould_unit->height()*j);

                QLabel *threshould_lab = new QLabel(threshould_unit);
                threshould_lab->move(40*m_Magnification,14*m_Magnification);
                threshould_lab->resize(100*m_Magnification,16*m_Magnification);
                threshould_lab->setText(detonator_titles.at(i+2*j));
                QString highLow_lab_stys = QString("background-color:transparent;color:rgb(255,255,255);font-size:%1px;font-family:'Source Han Sans CN Medium'").arg(12*m_Magnification);
                threshould_lab->setStyleSheet(highLow_lab_stys);

                MySlider *detonator_slider = new MySlider(threshould_unit);
                detonator_slider->setOrientation(Qt::Horizontal);
                detonator_slider->setMinimum(slider_minValue.at(i+2*j));
                detonator_slider->setMaximum(slider_maxValue.at(i+2*j));
                detonator_slider->setValue(xianfuqi_slider_values.at(0));
                detonator_slider->setGeometry(40*m_Magnification,threshould_lab->y()+threshould_lab->height()+10*m_Magnification,310*m_Magnification,12*m_Magnification);
                detonator_slider->setObjectName(QString::number(i+2*j,10));
                connect(detonator_slider,SIGNAL(valueChanged(int)),this,SLOT(detonator_chanege_value(int)));
                connect(detonator_slider,SIGNAL(sliderReleasedAt(int)),this,SLOT(detonator_release_value(int)));
                m_detonatorSliders.push_back(detonator_slider);

                QLabel *current_value = new QLabel(threshould_unit);
                current_value->move(detonator_slider->width()+detonator_slider->x()+10*m_Magnification,detonator_slider->y()-2*m_Magnification);
                current_value->resize(60*m_Magnification,14*m_Magnification);
                current_value->setText("-80dB");
                QString currentLabel_stys = QString("background-color:transparent;color:rgb(255,255,255);font-size:%1px;font-family:'Source Han Sans CN Medium'").arg(12*m_Magnification);
                current_value->setStyleSheet(currentLabel_stys);
                m_showValueLabels.push_back(current_value);

                QLabel *left_value = new QLabel(threshould_unit);
                left_value->resize(40*m_Magnification,14*m_Magnification);
                left_value->move(detonator_slider->x(),detonator_slider->y()+detonator_slider->height()+5*m_Magnification);
                left_value->setText(highlowLeft.at(i+2*j));
                left_value->setStyleSheet(currentLabel_stys);

                QLabel *right_value = new QLabel(threshould_unit);
                right_value->resize(50*m_Magnification,16*m_Magnification);
                right_value->move(detonator_slider->x()+detonator_slider->width()-right_value->width(),detonator_slider->y()+detonator_slider->height()+5*m_Magnification);
                right_value->setText(highlowRight.at(i+2*j));
                right_value->setStyleSheet(currentLabel_stys);
                right_value ->setAlignment(Qt::AlignRight);
            }
        }

    }

    QVector<QString>title_btns={tr("重置"),tr("确认")};
    for (int i=0;i<2;i++) {
        custom_button *reset_confirm_btn = new custom_button(this);
        reset_confirm_btn->resize(76*m_Magnification,26*m_Magnification);
        reset_confirm_btn->move(338*m_Magnification +i*156*m_Magnification ,this->height()-reset_confirm_btn->height()-70*m_Magnification);
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

void dm90_detonatorView::refreshUI(){

    //按钮的使能状态
    if(GET_DEVICE_DATA->dm90_Detonator_Enable){//打开
        m_highEQ_btn->setCheckState(Qt::Checked);
    }else{//关闭
        m_highEQ_btn->setCheckState(Qt::Unchecked);
    }


    for (int i=0;i<m_detonatorSliders.count();i++) {

        MySlider *current_slider = m_detonatorSliders.at(i);
        current_slider->blockSignals(true);
        switch (i) {

        case DETONATOR_SLIDER:{

            int show_number = dm90_deviceDataSource::getInstance()->loadingChangeNegativeNumber(GET_DEVICE_DATA->dm90_Detonator_Threshold);
            current_slider->setValue(show_number+80);
            m_showValueLabels.at(i)->setText(QString::number(show_number,10)+"dB");
        }
            break;

        case RELEASE_SLIDER:{

            current_slider->setValue(GET_DEVICE_DATA->dm90_Detonator_Release);
            m_showValueLabels.at(i)->setText(QString::number(GET_DEVICE_DATA->dm90_Detonator_Release,10)+"ms");
        }
            break;

        case FREQ_SLIDER:{

            current_slider->setValue(GET_DEVICE_DATA->dm90_Detonator_Freq);
            m_showValueLabels.at(i)->setText(QString::number(GET_DEVICE_DATA->dm90_Detonator_Freq,10)+"Hz");
        }
            break;

        case MINGAIN_SLIDER:{

            int show_number = dm90_deviceDataSource::getInstance()->loadingChangeNegativeNumber(GET_DEVICE_DATA->dm90_Detonator_MinGain);
            current_slider->setValue(show_number+50);
            m_showValueLabels.at(i)->setText(QString::number(show_number,10)+"dB");
        }
            break;


        case Q_SLIDER:{

            current_slider->setValue(GET_DEVICE_DATA->dm90_Detonator_Q);
            m_showValueLabels.at(i)->setText(QString::number(GET_DEVICE_DATA->dm90_Detonator_Q/100,10));
        }
            break;

        case DETECTPERIOD_SLIDER:{//检测时间
            current_slider->setValue(GET_DEVICE_DATA->dm90_Detonator_DetectPeriod);
            m_showValueLabels.at(i)->setText(QString::number(GET_DEVICE_DATA->dm90_Detonator_DetectPeriod,10)+"ms");

        }
            break;

        case ATTACK_SLIDER:{

            current_slider->setValue(GET_DEVICE_DATA->dm90_Detonator_Attack);
            m_showValueLabels.at(i)->setText(QString::number(GET_DEVICE_DATA->dm90_Detonator_Attack,10)+"ms");
        }
            break;

        default:
            break;

        }

        current_slider->blockSignals(false);

    }

}

void dm90_detonatorView::detonator_chanege_value(int value){

    m_DM90delayTimer->stop();
    m_DM90delayTimer->start(20);
    MySlider *detonator_slider = static_cast<MySlider *>(sender());
    int slider_number = detonator_slider->objectName().toUInt();
    m_currentSliderNum = slider_number;
    switch (slider_number) {

    case DETONATOR_SLIDER:{

        m_showValueLabels.at(slider_number)->setText(QString::number(value-80,10)+"dB");
    }
        break;

    case RELEASE_SLIDER:{

        m_showValueLabels.at(slider_number)->setText(QString::number(value,10)+"ms");
    }
        break;

    case FREQ_SLIDER:{

        m_showValueLabels.at(slider_number)->setText(QString::number(value,10)+"Hz");
    }
        break;

    case MINGAIN_SLIDER:{

        m_showValueLabels.at(slider_number)->setText(QString::number(value-50,10)+"dB");
    }
        break;

    case Q_SLIDER:{

        double actual_value = (double)value/100;
        m_showValueLabels.at(slider_number)->setText(QString::number(actual_value,'f',2));
    }
        break;

    case DETECTPERIOD_SLIDER:{//检测时间

        m_showValueLabels.at(slider_number)->setText(QString::number(value,10)+"ms");

    }
        break;

    case ATTACK_SLIDER:{

        m_showValueLabels.at(slider_number)->setText(QString::number(value,10)+"ms");
    }
        break;

    default:
        break;

    }
}

//计时器发送数据
void dm90_detonatorView::detonatorStarDelayTimerFun(){

    if(Qwdget_timer_index==6){

        window_dm90* mainWindow = (window_dm90*)(this->parentWidget()->parentWidget());//获取列表视图
        //发送数据
        MySlider *current_slider = m_detonatorSliders.at(m_currentSliderNum);
        switch (m_currentSliderNum) {

        case DETONATOR_SLIDER:{

            int send_Value = dm90_deviceDataSource::getInstance()->loadingChangeDeviceNumber(current_slider->value()-80);
            m_commin_fun.sendMessageForDevice(DM90PARAM_MIC_DEPOP_START+1,1,send_Value);
            GET_DEVICE_DATA->dm90_Detonator_Threshold = send_Value;
            mainWindow->m_midView->m_localCustom_itemList->addItem(mainWindow->m_midView->m_defaultTtemRow,mainWindow->m_midView->m_itemtext,55+1,send_Value);
        }
            break;

        case RELEASE_SLIDER:{//恢复时间

            m_commin_fun.sendMessageForDevice(DM90PARAM_MIC_DEPOP_START+5,1,current_slider->value());
            GET_DEVICE_DATA->dm90_Detonator_Release = current_slider->value();
            mainWindow->m_midView->m_localCustom_itemList->addItem(mainWindow->m_midView->m_defaultTtemRow,mainWindow->m_midView->m_itemtext,55+5,current_slider->value());
        }
            break;

        case FREQ_SLIDER:{

            m_commin_fun.sendMessageForDevice(DM90PARAM_MIC_DEPOP_START+2,1,current_slider->value());
            GET_DEVICE_DATA->dm90_Detonator_Freq  = current_slider->value();;
            mainWindow->m_midView->m_localCustom_itemList->addItem(mainWindow->m_midView->m_defaultTtemRow,mainWindow->m_midView->m_itemtext,55+2,current_slider->value());

        }
            break;

        case MINGAIN_SLIDER:{

            int send_Value = dm90_deviceDataSource::getInstance()->loadingChangeDeviceNumber(current_slider->value()-50);
            m_commin_fun.sendMessageForDevice(DM90PARAM_MIC_DEPOP_START+6,1,send_Value);
            GET_DEVICE_DATA->dm90_Detonator_MinGain = send_Value;
            mainWindow->m_midView->m_localCustom_itemList->addItem(mainWindow->m_midView->m_defaultTtemRow,mainWindow->m_midView->m_itemtext,55+6,send_Value);
        }
            break;

        case Q_SLIDER:{

            m_commin_fun.sendMessageForDevice(DM90PARAM_MIC_DEPOP_START+3,1,current_slider->value());
            GET_DEVICE_DATA->dm90_Detonator_Q  = current_slider->value();
            mainWindow->m_midView->m_localCustom_itemList->addItem(mainWindow->m_midView->m_defaultTtemRow,mainWindow->m_midView->m_itemtext,55+3,current_slider->value());

        }
            break;

        case DETECTPERIOD_SLIDER:{//检测时间

            m_commin_fun.sendMessageForDevice(DM90PARAM_MIC_DEPOP_START+7,1,current_slider->value());
            GET_DEVICE_DATA->dm90_Detonator_DetectPeriod = current_slider->value();
            mainWindow->m_midView->m_localCustom_itemList->addItem(mainWindow->m_midView->m_defaultTtemRow,mainWindow->m_midView->m_itemtext,55+7,current_slider->value());

        }
            break;

        case ATTACK_SLIDER:{//启动时间

            m_commin_fun.sendMessageForDevice(DM90PARAM_MIC_DEPOP_START+4,1,current_slider->value());
            GET_DEVICE_DATA->dm90_Detonator_Attack = current_slider->value();
            mainWindow->m_midView->m_localCustom_itemList->addItem(mainWindow->m_midView->m_defaultTtemRow,mainWindow->m_midView->m_itemtext,55+4,current_slider->value());

        }
            break;

        default:
            break;
        }
        //发完数据之后停止计时器
        m_DM90delayTimer->stop();
    }

}

void dm90_detonatorView::detonator_release_value(int){


}
//重置按钮=====================
void dm90_detonatorView::reset_click(){

    GET_DEVICE_DATA->dm90_Detonator_Enable = 0;
    GET_DEVICE_DATA->dm90_Detonator_Threshold = 1700;
    GET_DEVICE_DATA->dm90_Detonator_Release = 130;
    GET_DEVICE_DATA->dm90_Detonator_Freq = 340;
    GET_DEVICE_DATA->dm90_Detonator_MinGain = 1800;
    GET_DEVICE_DATA->dm90_Detonator_Q = 100;
    GET_DEVICE_DATA->dm90_Detonator_DetectPeriod = 30;
    GET_DEVICE_DATA->dm90_Detonator_Attack = 28;
    refreshUI();//刷新界面
    //发送数据
    QVector<int>addrs;
    QVector<int>value = {0,1700,340,100,28,130,1800,30};
    for (int i=0;i<value.count();i++) {
        addrs.push_back(DM90PARAM_MIC_DEPOP_START+i);
    }

    int frame_len = 7 + 4 *value.count();
    m_commin_fun.sendRandomMessageForDevice(frame_len,addrs,value);
}
//确定按钮事件
void dm90_detonatorView::confirm_click(){
    QWidget* pWidget = this->parentWidget();
    pWidget->hide();
    this->hide();

    Qwdget_timer_index = 1;
    int send_Value = dm90_deviceDataSource::getInstance()->loadingChangeDeviceNumber(m_detonatorSliders.at(0)->value()-80);
    GET_DEVICE_DATA->dm90_Detonator_Threshold = send_Value;
    confirmClick(3);
}
void dm90_detonatorView::isopen_enable_click(int enable_status){

    if(enable_status){//打开
        if(GET_DEVICE_DATA->dm90_Detonator_Enable != enable_status-1)
        m_commin_fun.sendMessageForDevice(DM90PARAM_MIC_DEPOP_START,1,1);
        for (int i=0;i<m_detonatorSliders.count();i++) {
          m_detonatorSliders.at(i)->setSliderDisable(false);
        }
        GET_DEVICE_DATA->dm90_Detonator_Enable =1;

    }else{//关闭
        if(GET_DEVICE_DATA->dm90_Detonator_Enable != enable_status)
        m_commin_fun.sendMessageForDevice(DM90PARAM_MIC_DEPOP_START,1,0);
        for (int i=0;i<m_detonatorSliders.count();i++) {
          m_detonatorSliders.at(i)->setSliderDisable(true);
        }
        GET_DEVICE_DATA->dm90_Detonator_Enable =0;
    }
}
