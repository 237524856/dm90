
#pragma once
#pragma execution_character_set("utf-8")
#include "dm90_flangerview.h"
#include "custom_checkbox.h"
#include "custom_button.h"
#include "../window_dm90.h"

dm90_flangerView::dm90_flangerView(QWidget *parent) : QWidget(parent)
{
    this->setAttribute(Qt::WA_StyledBackground,true);
}
void dm90_flangerView::createUI(){


    QWidget *mianView = new QWidget(this);
    mianView->move(40*m_Magnification,50*m_Magnification);
    mianView->setAttribute(Qt::WA_StyledBackground,true);
    mianView->resize(this->width()-80*m_Magnification,this->height()-100*m_Magnification);
    mianView->setStyleSheet("background:rgb(17,17,17);border-radius: 15px;");

    m_highEQ_btn= new custom_checkbox(mianView);
    m_highEQ_btn->setText(tr(" 镶边器/移相器"));
    language_index==0?m_highEQ_btn->resize(160*m_Magnification,40*m_Magnification):m_highEQ_btn->resize(180*m_Magnification,40*m_Magnification);
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
    QVector<QString>detonator_titles={tr("效果音量"),tr("调制深度"),tr("延迟"),tr("反馈比例"),tr("频率调制")};
    QVector<QString>highlowLeft={"0","0ms","0ms","0","0Hz"};
    QVector<QString>highlowRight={"100","1ms","100ms","95","10Hz"};

    //最小值
    QVector<int>slider_minValue={0,0,0,0,0};
    //最大值
    QVector<int>slider_maxValue={100,100,100,95,100};


    for (int j=0;j<3;j++) {

        for (int i=0;i<2;i++) {
            if(i+2*j != 5){

                QWidget *threshould_unit = new QWidget(mianView);
                threshould_unit->setAttribute(Qt::WA_StyledBackground,true);
                threshould_unit->resize(mianView->width()/2,100*m_Magnification);
                threshould_unit->move(mianView->width()/2*i,m_highEQ_btn->y()+m_highEQ_btn->height() +threshould_unit->height()*j);

                QLabel *threshould_lab = new QLabel(threshould_unit);
                threshould_lab->move(40*m_Magnification,14*m_Magnification);
                threshould_lab->resize(180*m_Magnification,14*m_Magnification);
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
                connect(threshould_slider,SIGNAL(valueChanged(int)),this,SLOT(flanger_chanege_value(int)));
                connect(threshould_slider,SIGNAL(sliderReleasedAt(int)),this,SLOT(flanger_release_value(int)));
                m_flangerSliders.push_back(threshould_slider);

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


void dm90_flangerView::refreshUI(){

    //按钮的使能状态
    if(GET_DEVICE_DATA->dm90_flanger_Enable){//打开
        m_highEQ_btn->setCheckState(Qt::Checked);
    }else{//关闭
        m_highEQ_btn->setCheckState(Qt::Unchecked);
    }

    for (int i =0;i<m_flangerSliders.count();i++) {

        MySlider *current_slider = m_flangerSliders.at(i);
        current_slider->blockSignals(true);
        switch (i) {

        case LEVEL_SLIDER:{

            current_slider->setValue(GET_DEVICE_DATA->dm90_flanger_Level);
            m_showValueLabels.at(i)->setText(QString::number(GET_DEVICE_DATA->dm90_flanger_Level,10));

        }
            break;

        case DEPTMODU_SLIDER:{

            current_slider->setValue(GET_DEVICE_DATA->dm90_flanger_DeptModu);
            double show_number = (double)GET_DEVICE_DATA->dm90_flanger_DeptModu/100;
            m_showValueLabels.at(i)->setText(QString::number(show_number,'f',2)+"ms");

        }
            break;

        case DELAY_SLIDER:{

            current_slider->setValue(GET_DEVICE_DATA->dm90_flanger_Delay);
            m_showValueLabels.at(i)->setText(QString::number(GET_DEVICE_DATA->dm90_flanger_Delay,10)+"ms");
        }
            break;

        case FEEDRATE_SLIDER:{

            current_slider->setValue(GET_DEVICE_DATA->dm90_flanger_FeedRate);
            m_showValueLabels.at(i)->setText(QString::number(GET_DEVICE_DATA->dm90_flanger_FeedRate,10));
        }
            break;

        case FREQMODU_SLIDER:{

            current_slider->setValue(GET_DEVICE_DATA->dm90_flanger_FreqModu/10);
            double show_number = (double)GET_DEVICE_DATA->dm90_flanger_FreqModu/100;
            m_showValueLabels.at(i)->setText(QString::number(show_number,'f',2)+"Hz");
        }
            break;

        default:
            break;

        }
        current_slider->blockSignals(false);
    }
}

void dm90_flangerView::flanger_chanege_value(int value){

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

    case DEPTMODU_SLIDER:{

        double show_number = (double)value/100;
        m_showValueLabels.at(slider_number)->setText(QString::number(show_number,'f',2)+"ms");

    }
        break;

    case DELAY_SLIDER:{

        m_showValueLabels.at(slider_number)->setText(QString::number(value,10)+"ms");

    }
        break;

    case FEEDRATE_SLIDER:{

        m_showValueLabels.at(slider_number)->setText(QString::number(value,10));

    }
        break;

    case FREQMODU_SLIDER:{

        double show_number = (double)value/10;
        m_showValueLabels.at(slider_number)->setText(QString::number(show_number,'f',1)+"Hz");

    }
        break;

    default:
        break;

    }

}
//发送数据到设备
void dm90_flangerView::flangerStarDelayTimerFun(){

    if(Qwdget_timer_index==10){

        window_dm90* mainWindow = (window_dm90*)(this->parentWidget()->parentWidget());//获取列表视图
        //发送数据
        MySlider *current_slider = m_flangerSliders.at(m_currentSliderNum);
        switch (m_currentSliderNum) {

        case LEVEL_SLIDER:{

            m_commin_fun.sendMessageForDevice(DM90PARAM_MIC_FLAN_START+1,1,current_slider->value());
            GET_DEVICE_DATA->dm90_flanger_Level = current_slider->value();
            mainWindow->m_midView->m_localCustom_itemList->addItem(mainWindow->m_midView->m_defaultTtemRow,mainWindow->m_midView->m_itemtext,65+1,current_slider->value());
        }
            break;

        case DEPTMODU_SLIDER:{//调制深度

            m_commin_fun.sendMessageForDevice(DM90PARAM_MIC_FLAN_START+4,1,current_slider->value());
            GET_DEVICE_DATA->dm90_flanger_DeptModu = current_slider->value();
            mainWindow->m_midView->m_localCustom_itemList->addItem(mainWindow->m_midView->m_defaultTtemRow,mainWindow->m_midView->m_itemtext,65+4,current_slider->value());

        }
            break;

        case DELAY_SLIDER:{//延迟

            m_commin_fun.sendMessageForDevice(DM90PARAM_MIC_FLAN_START+2,1,current_slider->value());
            GET_DEVICE_DATA->dm90_flanger_Delay = current_slider->value();
            mainWindow->m_midView->m_localCustom_itemList->addItem(mainWindow->m_midView->m_defaultTtemRow,mainWindow->m_midView->m_itemtext,65+2,current_slider->value());

        }
            break;

        case FEEDRATE_SLIDER:{//反馈比例

            m_commin_fun.sendMessageForDevice(DM90PARAM_MIC_FLAN_START+5,1,current_slider->value());
            GET_DEVICE_DATA->dm90_flanger_FeedRate = current_slider->value();
            mainWindow->m_midView->m_localCustom_itemList->addItem(mainWindow->m_midView->m_defaultTtemRow,mainWindow->m_midView->m_itemtext,65+5,current_slider->value());
        }
            break;

        case FREQMODU_SLIDER:{

            m_commin_fun.sendMessageForDevice(DM90PARAM_MIC_FLAN_START+3,1,current_slider->value()*10);
            GET_DEVICE_DATA->dm90_flanger_FreqModu = current_slider->value()*10;
            mainWindow->m_midView->m_localCustom_itemList->addItem(mainWindow->m_midView->m_defaultTtemRow,mainWindow->m_midView->m_itemtext,65+3,current_slider->value()*10);

        }
            break;

        default:
            break;
        }

    }
    //发完数据之后停止计时器
    m_DM90delayTimer->stop();
}

void dm90_flangerView::flanger_release_value(int){

}
//重置按钮=====================
void dm90_flangerView::reset_click(){

    GET_DEVICE_DATA->dm90_flanger_Enable = 0;
    GET_DEVICE_DATA->dm90_flanger_Level = 50;
    GET_DEVICE_DATA->dm90_flanger_Delay = 50;
    GET_DEVICE_DATA->dm90_flanger_FreqModu = 50;
    GET_DEVICE_DATA->dm90_flanger_DeptModu = 100;
    GET_DEVICE_DATA->dm90_flanger_FeedRate = 50;
    refreshUI();//刷新界面
    //发送数据
    QVector<int>addrs;
    QVector<int>value = {0,50,50,50,100,50};
    for (int i=0;i<value.count();i++) {
        addrs.push_back(DM90PARAM_MIC_FLAN_START+i);
    }
    int frame_len = 7 + 4 *value.count();
    m_commin_fun.sendRandomMessageForDevice(frame_len,addrs,value);

}

void dm90_flangerView::confirm_click(){
    this->hide();
    QWidget* pWidget = this->parentWidget();
    pWidget->hide();

    Qwdget_timer_index = 1;
    confirmClick(9);
}

void dm90_flangerView::isopen_enable_click(int enable_status){

    if(enable_status){//打开
        if(GET_DEVICE_DATA->dm90_flanger_Enable != enable_status-1)
        m_commin_fun.sendMessageForDevice(DM90PARAM_MIC_FLAN_START,1,1);
        for (int i=0;i<m_flangerSliders.count();i++) {
          m_flangerSliders.at(i)->setSliderDisable(false);
        }
        GET_DEVICE_DATA->dm90_flanger_Enable =1;

    }else{//关闭
        if(GET_DEVICE_DATA->dm90_flanger_Enable != enable_status)
        m_commin_fun.sendMessageForDevice(DM90PARAM_MIC_FLAN_START,1,0);
        for (int i=0;i<m_flangerSliders.count();i++) {
          m_flangerSliders.at(i)->setSliderDisable(true);
        }
        GET_DEVICE_DATA->dm90_flanger_Enable =0;
    }
}

