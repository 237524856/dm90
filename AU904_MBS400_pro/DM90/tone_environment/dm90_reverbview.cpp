#pragma once
#pragma execution_character_set("utf-8")

#include "dm90_reverbview.h"
#include "custom_checkbox.h"
#include "custom_button.h"
#include "../window_dm90.h"

dm90_reverbView::dm90_reverbView(QWidget *parent) : QWidget(parent)
{
    this->setAttribute(Qt::WA_StyledBackground,true);
}
void dm90_reverbView::createUI(){


    QWidget *mianView = new QWidget(this);
    mianView->move(40*m_Magnification,40*m_Magnification);
    mianView->setAttribute(Qt::WA_StyledBackground,true);
    mianView->resize(this->width()-80*m_Magnification,this->height()-80*m_Magnification);
    mianView->setStyleSheet("background:rgb(17,17,17);border-radius: 15px;");

    m_highEQ_btn = new custom_checkbox(mianView);
    m_highEQ_btn->setText(tr(" 混响"));
    language_index==0?m_highEQ_btn->resize(75*m_Magnification,40*m_Magnification):m_highEQ_btn->resize(100*m_Magnification,40*m_Magnification);
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
    QVector<QString>detonator_titles={tr("预延时"),tr("混响时间"),tr("直达声音量"),tr("阻尼"),tr("混响音量"),tr("前期反射声比例"),tr("扩散度"),tr("后期反射声比例"),tr("房间尺寸")};
    QVector<QString>highlowLeft={"1ms","0.10s","0","0.25","0","0","0.25","0","6"};
    QVector<QString>highlowRight={"64ms","6.40s","100","8","100","51","16","76.50","64"};

    //最小值
    QVector<int>slider_minValue={1,10,0,25,0,0,25,0,600};
    //最大值
    QVector<int>slider_maxValue={64,640,100,800,100,510,1600,765,6400};

    for (int j=0;j<5;j++) {

        for (int i=0;i<2;i++) {
            if(i+2*j != 9){

                QWidget *threshould_unit = new QWidget(mianView);
                threshould_unit->setAttribute(Qt::WA_StyledBackground,true);
                threshould_unit->resize(mianView->width()/2,100*m_Magnification);
                threshould_unit->move(mianView->width()/2*i,m_highEQ_btn->y()+m_highEQ_btn->height() +threshould_unit->height()*j);

                QLabel *threshould_lab = new QLabel(threshould_unit);
                threshould_lab->move(40*m_Magnification,14*m_Magnification);
                threshould_lab->resize(150*m_Magnification,16*m_Magnification);
                threshould_lab->setText(detonator_titles.at(i+2*j));
                QString highLow_lab_stys = QString("background-color:transparent;color:rgb(255,255,255);font-size:%1px;font-family:'Source Han Sans CN Medium'").arg(12*m_Magnification);
                threshould_lab->setStyleSheet(highLow_lab_stys);

                MySlider *threshould_slider = new MySlider(threshould_unit);
                threshould_slider->setOrientation(Qt::Horizontal);
                threshould_slider->setMinimum(slider_minValue.at(i+2*j));
                threshould_slider->setMaximum(slider_maxValue.at(i+2*j));
                threshould_slider->setValue(xianfuqi_slider_values.at(0));
                threshould_slider->setObjectName(QString::number(i+2*j,10));
                threshould_slider->setGeometry(40*m_Magnification,threshould_lab->y()+threshould_lab->height()+10*m_Magnification,310*m_Magnification,12*m_Magnification);
                connect(threshould_slider,SIGNAL(valueChanged(int)),this,SLOT(reverb_chanege_value(int)));
                connect(threshould_slider,SIGNAL(sliderReleasedAt(int)),this,SLOT(reverb_release_value(int)));
                m_reverbSliders.push_back(threshould_slider);

                QLabel *current_value = new QLabel(threshould_unit);
                current_value->move(threshould_slider->width()+threshould_slider->x()+10*m_Magnification,threshould_slider->y()-2*m_Magnification);
                current_value->resize(60*m_Magnification,14*m_Magnification);
                current_value->setText("-80dB");
                QString currentLabel_stys = QString("background-color:transparent;color:rgb(255,255,255);font-size:%1px;font-family:'Source Han Sans CN Medium'").arg(12*m_Magnification);
                current_value->setStyleSheet(currentLabel_stys);
                m_showValueLabels.push_back(current_value);

                QLabel *left_value = new QLabel(threshould_unit);
                left_value->resize(40*m_Magnification,14*m_Magnification);
                left_value->move(threshould_slider->x(),threshould_slider->y()+threshould_slider->height()+5*m_Magnification);
                left_value->setText(highlowLeft.at(i+2*j));
                left_value->setStyleSheet(currentLabel_stys);

                QLabel *right_value = new QLabel(threshould_unit);
                right_value->resize(50*m_Magnification,16*m_Magnification);
                right_value->move(threshould_slider->x()+threshould_slider->width()-right_value->width(),threshould_slider->y()+threshould_slider->height()+5*m_Magnification);
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
        reset_confirm_btn->move(338*m_Magnification +i*156*m_Magnification ,this->height()-reset_confirm_btn->height()-60*m_Magnification);
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

//获取设备的数据刷新整个页面
void dm90_reverbView::refreshUI(){

    //按钮的使能状态
    if(GET_DEVICE_DATA->dm90_Reverb_Enable){//打开
        m_highEQ_btn->setCheckState(Qt::Checked);
    }else{//关闭
        m_highEQ_btn->setCheckState(Qt::Unchecked);
    }

    for (int i =0;i<m_reverbSliders.count();i++) {
        MySlider *current_slider = m_reverbSliders.at(i);
        current_slider->blockSignals(true);
        switch (i) {

        case PREDELAY_SLIDER:{

            current_slider->setValue(GET_DEVICE_DATA->dm90_Reverb_PreDelay);
            m_showValueLabels.at(i)->setText(QString::number(GET_DEVICE_DATA->dm90_Reverb_PreDelay,10)+"ms");

        }
            break;

        case TIME_SLIDER:{

            current_slider->setValue(GET_DEVICE_DATA->dm90_Reverb_Time);
            double show_number = (double)GET_DEVICE_DATA->dm90_Reverb_Time/100;
            m_showValueLabels.at(i)->setText(QString::number(show_number,'f',2));

        }
            break;

        case DRYLEVEL_SLIDER:{

            current_slider->setValue(GET_DEVICE_DATA->dm90_Reverb_DryLevel);
            m_showValueLabels.at(i)->setText(QString::number(GET_DEVICE_DATA->dm90_Reverb_DryLevel,10));
        }
            break;

        case DAMPING_SLIDER:{//阻尼


            current_slider->setValue(GET_DEVICE_DATA->dm90_Reverb_Damping);
            double show_number = (double)GET_DEVICE_DATA->dm90_Reverb_Damping/100;
            m_showValueLabels.at(i)->setText(QString::number(show_number,'f',2));

        }
            break;

        case WETLEVEL_SLIDER:{//混响音量

            current_slider->setValue(GET_DEVICE_DATA->dm90_Reverb_WetLevel);
            m_showValueLabels.at(i)->setText(QString::number(GET_DEVICE_DATA->dm90_Reverb_WetLevel,10));
        }
            break;

        case EARLYLEVEL_SLIDER:{//前期反射声比例

            current_slider->setValue(GET_DEVICE_DATA->dm90_Reverb_EarlyLevel/10);
            double show_number = (double)GET_DEVICE_DATA->dm90_Reverb_EarlyLevel/100;
            m_showValueLabels.at(i)->setText(QString::number(show_number,'f',1));
        }
            break;

        case SPREAD_SLIDER:{

            current_slider->setValue(GET_DEVICE_DATA->dm90_Reverb_Spread);
            double show_number = (double)GET_DEVICE_DATA->dm90_Reverb_Spread/100;
            m_showValueLabels.at(i)->setText(QString::number(show_number,'f',2));
        }
            break;

        case TAILLEVEL_SLIDER:{//后期反射声比例

            current_slider->setValue(GET_DEVICE_DATA->dm90_Reverb_TailLevel/10);
            double show_number = (double)GET_DEVICE_DATA->dm90_Reverb_TailLevel/100;
            m_showValueLabels.at(i)->setText(QString::number(show_number,'f',1));
        }
            break;

        case ROOMSIZE_SLIDER:{

            current_slider->setValue(GET_DEVICE_DATA->dm90_Reverb_RoomSize);
            double show_number = (double)GET_DEVICE_DATA->dm90_Reverb_RoomSize/100;
            m_showValueLabels.at(i)->setText(QString::number(show_number,'f',2));

        }
            break;

        default:
            break;

        }
        current_slider->blockSignals(false);
    }
}

//滑动条的方法----------------------------
void dm90_reverbView::reverb_chanege_value(int value){

    m_DM90delayTimer->stop();
    m_DM90delayTimer->start(20);
    MySlider *detonator_slider = static_cast<MySlider *>(sender());
    int slider_number = detonator_slider->objectName().toUInt();
    m_currentSliderNum = slider_number;

    switch (slider_number) {

    case PREDELAY_SLIDER:{

        m_showValueLabels.at(slider_number)->setText(QString::number(value,10)+"ms");

    }
        break;

    case TIME_SLIDER:{

        double show_number = (double)value/100;
        m_showValueLabels.at(slider_number)->setText(QString::number(show_number,'f',2));

    }
        break;

    case DRYLEVEL_SLIDER:{

        m_showValueLabels.at(slider_number)->setText(QString::number(value,10));
    }
        break;

    case DAMPING_SLIDER:{//阻尼

        double show_number = (double)value/100;
        m_showValueLabels.at(slider_number)->setText(QString::number(show_number,'f',2));

    }
        break;

    case WETLEVEL_SLIDER:{//混响音量

        m_showValueLabels.at(slider_number)->setText(QString::number(value,10));
    }
        break;

    case EARLYLEVEL_SLIDER:{//前期反射声比例


        double show_number = (double)value/10;
        m_showValueLabels.at(slider_number)->setText(QString::number(show_number,'f',1));
    }
        break;

    case SPREAD_SLIDER:{


        double show_number = (double)value/100;
        m_showValueLabels.at(slider_number)->setText(QString::number(show_number,'f',2));
    }
        break;

    case TAILLEVEL_SLIDER:{//后期反射声比例

        double show_number = (double)value/10;
        m_showValueLabels.at(slider_number)->setText(QString::number(show_number,'f',1));
    }
        break;

    case ROOMSIZE_SLIDER:{

        double show_number = (double)value/100;
        m_showValueLabels.at(slider_number)->setText(QString::number(show_number,'f',2));

    }
        break;

    default:
        break;

    }

}
//计时器延时发送数据
void dm90_reverbView::reverbStarDelayTimerFun(){

    if(Qwdget_timer_index==12){

        //发送数据
        MySlider *current_slider = m_reverbSliders.at(m_currentSliderNum);
        window_dm90* mainWindow = (window_dm90*)(this->parentWidget()->parentWidget());

        switch (m_currentSliderNum) {

        case PREDELAY_SLIDER:{

            m_commin_fun.sendMessageForDevice(DM90PARAM_MIC_REVERB_START+2,1,current_slider->value());
            GET_DEVICE_DATA->dm90_Reverb_PreDelay = current_slider->value();
            mainWindow->m_midView->m_effectCustom_itemList->addItem(mainWindow->m_midView->m_defaultTtemRow,mainWindow->m_midView->m_itemtext,7+2,GET_DEVICE_DATA->dm90_Reverb_PreDelay);
        }
            break;

        case TIME_SLIDER:{

            m_commin_fun.sendMessageForDevice(DM90PARAM_MIC_REVERB_START+7,1,current_slider->value());
            GET_DEVICE_DATA->dm90_Reverb_Time = current_slider->value();
            mainWindow->m_midView->m_effectCustom_itemList->addItem(mainWindow->m_midView->m_defaultTtemRow,mainWindow->m_midView->m_itemtext,7+7,GET_DEVICE_DATA->dm90_Reverb_PreDelay);
        }
            break;

        case DRYLEVEL_SLIDER:{

            m_commin_fun.sendMessageForDevice(DM90PARAM_MIC_REVERB_START+3,1,current_slider->value());
            GET_DEVICE_DATA->dm90_Reverb_DryLevel = current_slider->value();
            mainWindow->m_midView->m_effectCustom_itemList->addItem(mainWindow->m_midView->m_defaultTtemRow,mainWindow->m_midView->m_itemtext,7+3,GET_DEVICE_DATA->dm90_Reverb_DryLevel);

        }
            break;

        case DAMPING_SLIDER:{//阻尼

            m_commin_fun.sendMessageForDevice(DM90PARAM_MIC_REVERB_START+8,1,current_slider->value());
            GET_DEVICE_DATA->dm90_Reverb_Damping = current_slider->value();
            mainWindow->m_midView->m_effectCustom_itemList->addItem(mainWindow->m_midView->m_defaultTtemRow,mainWindow->m_midView->m_itemtext,7+8,GET_DEVICE_DATA->dm90_Reverb_Damping);
        }
            break;

        case WETLEVEL_SLIDER:{//混响音量

            m_commin_fun.sendMessageForDevice(DM90PARAM_MIC_REVERB_START+4,1,current_slider->value());
            GET_DEVICE_DATA->dm90_Reverb_WetLevel = current_slider->value();
            mainWindow->m_midView->m_effectCustom_itemList->addItem(mainWindow->m_midView->m_defaultTtemRow,mainWindow->m_midView->m_itemtext,7+4,GET_DEVICE_DATA->dm90_Reverb_WetLevel);
        }
            break;

        case EARLYLEVEL_SLIDER:{//前期反射声比例

            m_commin_fun.sendMessageForDevice(DM90PARAM_MIC_REVERB_START+9,1,current_slider->value()*10);
            GET_DEVICE_DATA->dm90_Reverb_EarlyLevel = current_slider->value()*10;
            mainWindow->m_midView->m_effectCustom_itemList->addItem(mainWindow->m_midView->m_defaultTtemRow,mainWindow->m_midView->m_itemtext,7+9,GET_DEVICE_DATA->dm90_Reverb_EarlyLevel);
        }
            break;

        case SPREAD_SLIDER:{//扩展度

            m_commin_fun.sendMessageForDevice(DM90PARAM_MIC_REVERB_START+5,1,current_slider->value());
            GET_DEVICE_DATA->dm90_Reverb_Spread = current_slider->value();
            mainWindow->m_midView->m_effectCustom_itemList->addItem(mainWindow->m_midView->m_defaultTtemRow,mainWindow->m_midView->m_itemtext,7+5,GET_DEVICE_DATA->dm90_Reverb_Spread);
        }
            break;

        case TAILLEVEL_SLIDER:{//后期反射声比例

            m_commin_fun.sendMessageForDevice(DM90PARAM_MIC_REVERB_START+10,1,current_slider->value()*10);
            GET_DEVICE_DATA->dm90_Reverb_TailLevel = current_slider->value()*10;
            mainWindow->m_midView->m_effectCustom_itemList->addItem(mainWindow->m_midView->m_defaultTtemRow,mainWindow->m_midView->m_itemtext,7+10,GET_DEVICE_DATA->dm90_Reverb_TailLevel);
        }
            break;

        case ROOMSIZE_SLIDER:{//房间尺寸

            m_commin_fun.sendMessageForDevice(DM90PARAM_MIC_REVERB_START+6,1,current_slider->value());
            GET_DEVICE_DATA->dm90_Reverb_RoomSize = current_slider->value();
            mainWindow->m_midView->m_effectCustom_itemList->addItem(mainWindow->m_midView->m_defaultTtemRow,mainWindow->m_midView->m_itemtext,7+6,GET_DEVICE_DATA->dm90_Reverb_RoomSize);

        }
            break;

        default:
            break;

        }
        //发完数据之后停止计时器
        m_DM90delayTimer->stop();
    }
}

void dm90_reverbView::reverb_release_value(int){

}
//重置按钮=====================
void dm90_reverbView::reset_click(){

    //刷新当前UI界面
    GET_DEVICE_DATA->dm90_Reverb_Enable = 0;
    GET_DEVICE_DATA->dm90_Reverb_PreDelay = 1;
    GET_DEVICE_DATA->dm90_Reverb_DryLevel = 100;
    GET_DEVICE_DATA->dm90_Reverb_WetLevel = 100;
    GET_DEVICE_DATA->dm90_Reverb_Spread = 1600;
    GET_DEVICE_DATA->dm90_Reverb_RoomSize = 6400;
    GET_DEVICE_DATA->dm90_Reverb_Time = 280;
    GET_DEVICE_DATA->dm90_Reverb_Damping = 300;
    GET_DEVICE_DATA->dm90_Reverb_EarlyLevel = 1440;
    GET_DEVICE_DATA->dm90_Reverb_TailLevel = 3840;
    refreshUI();//刷新界面
    //发送数据
    QVector<int>addrs;
    QVector<int>value = {0,1,100,100,1600,6400,280,300,1440,3840};
    for (int i=0;i<value.count();i++) {
        addrs.push_back(DM90PARAM_MIC_REVERB_START+1+i);
    }
    int frame_len = 7 + 4 *value.count();
    m_commin_fun.sendRandomMessageForDevice(frame_len,addrs,value);
}

void dm90_reverbView::confirm_click(){
    this->hide();
    QWidget* pWidget = this->parentWidget();
    pWidget->hide();
    com_recordView->m_waveView->m_chartView->show();
    Qwdget_timer_index = 1;
    int send_Value = m_reverbSliders.at(4)->value();
    GET_DEVICE_DATA->dm90_Reverb_WetLevel = send_Value;
    confirmClick(7);
}
void dm90_reverbView::isopen_enable_click(int enable_status){

    if(enable_status){//打开
        if(GET_DEVICE_DATA->dm90_Reverb_Enable != enable_status-1)
        m_commin_fun.sendMessageForDevice(DM90PARAM_MIC_REVERB_START+1,1,1);
        for (int i=0;i<m_reverbSliders.count();i++) {
          m_reverbSliders.at(i)->setSliderDisable(false);
        }
        GET_DEVICE_DATA->dm90_Reverb_Enable =1;

    }else{//关闭
        if(GET_DEVICE_DATA->dm90_Reverb_Enable != enable_status)
        m_commin_fun.sendMessageForDevice(DM90PARAM_MIC_REVERB_START+1,1,0);
        for (int i=0;i<m_reverbSliders.count();i++) {
          m_reverbSliders.at(i)->setSliderDisable(true);
        }
        GET_DEVICE_DATA->dm90_Reverb_Enable =0;
    }
}
