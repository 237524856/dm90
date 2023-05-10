
#pragma once
#pragma execution_character_set("utf-8")
#include "dm90_limiterview.h"
#include "custom_checkbox.h"
#include "custom_button.h"
#include "../window_dm90.h"

dm90_limiterView::dm90_limiterView(QWidget *parent) : QWidget(parent)
{

    this->setAttribute(Qt::WA_StyledBackground,true);


    m_attack_arrs = {"8us","16us","167us","64us","128us","256us","512us","1.0ms","2.0ms","4.1ms","8.2ms","16.4ms","32.8ms","65.5ms","131ms","262ms"};

    m_release_arrs = {"2.0ms","4.1ms","8.2ms","16.4ms","32.8ms","65.5ms","131.1ms","262.1ms","524.3ms","1.0s","2.1s","4.2s","8.4s","16.8s","33.6s","67.1s"};
}

void dm90_limiterView::createUI(){

    QWidget *mianView = new QWidget(this);
    mianView->move(40*m_Magnification,50*m_Magnification);
    mianView->setAttribute(Qt::WA_StyledBackground,true);
    mianView->resize(this->width()-80*m_Magnification,this->height()-100*m_Magnification);
    mianView->setStyleSheet("background:rgb(17,17,17);border-radius: 15px;");


    m_highEQ_btn= new custom_checkbox(mianView);
    m_highEQ_btn->setText(tr("  限制器"));
    language_index==0?m_highEQ_btn->resize(120*m_Magnification,40*m_Magnification):m_highEQ_btn->resize(150*m_Magnification,40*m_Magnification);
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
    QVector<QString>threshouldTitle={tr("阈值"),tr("启动时间"),tr("恢复时间")};
    QVector<QString>highlowLeft={"-25.50dB","8us","2.0ms"};
    QVector<QString>highlowRight={"0dB","262ms","67.1s"};

    //最小值
    QVector<int>slider_minValue={0,0,0};
    //最大值
    QVector<int>slider_maxValue={255,15,15};

    for (int j=0;j<2;j++) {

        for (int i=0;i<2;i++) {

         if(i+2*j != 3){
             QWidget *threshould_unit = new QWidget(mianView);
             threshould_unit->setAttribute(Qt::WA_StyledBackground,true);
             threshould_unit->resize(mianView->width()/2,100*m_Magnification);
             threshould_unit->move(mianView->width()/2*i,m_highEQ_btn->height()+m_highEQ_btn->height() +threshould_unit->height()*j);


             QLabel *threshould_lab = new QLabel(threshould_unit);
             threshould_lab->move(40*m_Magnification,14*m_Magnification);
             threshould_lab->resize(100*m_Magnification,14*m_Magnification);
             threshould_lab->setText(threshouldTitle.at(i+2*j));
             QString highLow_lab_stys = QString("background-color:transparent;color:rgb(255,255,255);font-size:%1px;font-family:'Source Han Sans CN Medium'").arg(12*m_Magnification);
             threshould_lab->setStyleSheet(highLow_lab_stys);

             MySlider *limiter_slider = new MySlider(threshould_unit);
             limiter_slider->setOrientation(Qt::Horizontal);
             limiter_slider->setMinimum(slider_minValue.at(i+2*j));
             limiter_slider->setMaximum(slider_maxValue.at(i+2*j));
             limiter_slider->setValue(xianfuqi_slider_values.at(0));
             limiter_slider->setObjectName(QString::number(i+2*j,10));
             limiter_slider->setGeometry(40*m_Magnification,threshould_lab->y()+threshould_lab->height()+10*m_Magnification,310*m_Magnification,12*m_Magnification);
             connect(limiter_slider,SIGNAL(valueChanged(int)),this,SLOT(limiter_chanege_value(int)));
             connect(limiter_slider,SIGNAL(sliderReleasedAt(int)),this,SLOT(limiter_release_value(int)));
             m_limiterSliders.push_back(limiter_slider);

             QLabel *current_value = new QLabel(threshould_unit);
             current_value->move(limiter_slider->width()+limiter_slider->x()+10*m_Magnification,limiter_slider->y()-2*m_Magnification);
             current_value->resize(60*m_Magnification,14*m_Magnification);
             current_value->setText("-80dB");
             QString currentLabel_stys = QString("background-color:transparent;color:rgb(255,255,255);font-size:%1px;font-family:'Source Han Sans CN Medium'").arg(12*m_Magnification);
             current_value->setStyleSheet(currentLabel_stys);
             m_showValueLabels.push_back(current_value);

             QLabel *left_value = new QLabel(threshould_unit);
             left_value->resize(90*m_Magnification,14*m_Magnification);
             left_value->move(limiter_slider->x(),limiter_slider->y()+limiter_slider->height()+5*m_Magnification);
             left_value->setText(highlowLeft.at(i+2*j));
             left_value->setStyleSheet(currentLabel_stys);

             QLabel *right_value = new QLabel(threshould_unit);
             right_value->resize(50*m_Magnification,16*m_Magnification);
             right_value->move(limiter_slider->x()+limiter_slider->width()-right_value->width(),limiter_slider->y()+limiter_slider->height()+5*m_Magnification);
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


void dm90_limiterView::refreshUI(){

    if(GET_DEVICE_DATA->dm90_Limit_Enable){//打开
        m_highEQ_btn->setCheckState(Qt::Checked);
    }else{//关闭
        m_highEQ_btn->setCheckState(Qt::Unchecked);
    }

    for (int i =0;i<m_limiterSliders.count();i++) {

        MySlider *current_slider = m_limiterSliders.at(i);
        current_slider->blockSignals(true);
        switch (i) {

        case LIMITER_SLIDER:{

            double show_number = dm90_deviceDataSource::getInstance()->loadingChangeNegativeNumber(GET_DEVICE_DATA->dm90_Limit_Threshold);
            current_slider->setValue(show_number*10+255);
            m_showValueLabels.at(i)->setText(QString::number(show_number,'f',1)+"dB");

        }
            break;

        case LIMITER_ATTACK:{
            if(GET_DEVICE_DATA->dm90_Limit_Attack<=m_attack_arrs.count()){
                current_slider->setValue(GET_DEVICE_DATA->dm90_Limit_Attack);
                m_showValueLabels.at(i)->setText(m_attack_arrs.at(GET_DEVICE_DATA->dm90_Limit_Attack));
            }

        }
            break;

        case LIMITER_RELEASE:{

            if(GET_DEVICE_DATA->dm90_Limit_Release<=m_release_arrs.count()){
                current_slider->setValue(GET_DEVICE_DATA->dm90_Limit_Release);
                m_showValueLabels.at(i)->setText(m_release_arrs.at(GET_DEVICE_DATA->dm90_Limit_Release));
            }


        }
            break;

        default:
            break;

        }
        current_slider->blockSignals(false);
    }

}

void dm90_limiterView::limiter_chanege_value(int value){

    m_DM90delayTimer->stop();
    m_DM90delayTimer->start(20);
    MySlider *detonator_slider = static_cast<MySlider *>(sender());
    int slider_number = detonator_slider->objectName().toUInt();
    m_currentSliderNum = slider_number;

    switch (slider_number) {
    case LIMITER_SLIDER:{

        double show_number = (double)(value -255)/10;
        m_showValueLabels.at(slider_number)->setText(QString::number(show_number,'f',1)+"dB");
    }
        break;

    case LIMITER_ATTACK:{

        m_showValueLabels.at(slider_number)->setText(m_attack_arrs.at(value));
    }
        break;

    case LIMITER_RELEASE:{

        m_showValueLabels.at(slider_number)->setText(m_release_arrs.at(value));
    }
        break;

    default:
        break;

    }

}

void dm90_limiterView::limiterStarDelayTimerFun(){


    if(Qwdget_timer_index==8){

        window_dm90* mainWindow = (window_dm90*)(this->parentWidget()->parentWidget());//获取列表视图
        //发送数据
        MySlider *current_slider = m_limiterSliders.at(m_currentSliderNum);
        switch (m_currentSliderNum) {
        case LIMITER_SLIDER:{

            int send_Value = dm90_deviceDataSource::getInstance()->loadingChangeDeviceNumber(double(current_slider->value()-255)/10);
            m_commin_fun.sendMessageForDevice(DM90PARAM_MIC_LIMIT_START+1,1,send_Value);
            GET_DEVICE_DATA->dm90_Limit_Threshold = send_Value;
            mainWindow->m_midView->m_localCustom_itemList->addItem(mainWindow->m_midView->m_defaultTtemRow,mainWindow->m_midView->m_itemtext,76+1,send_Value);
        }
            break;

        case LIMITER_ATTACK:{

            m_commin_fun.sendMessageForDevice(DM90PARAM_MIC_LIMIT_START+2,1,current_slider->value());
            GET_DEVICE_DATA->dm90_Limit_Attack = current_slider->value();
            mainWindow->m_midView->m_localCustom_itemList->addItem(mainWindow->m_midView->m_defaultTtemRow,mainWindow->m_midView->m_itemtext,76+2,current_slider->value());
        }
            break;

        case LIMITER_RELEASE:{

            m_commin_fun.sendMessageForDevice(DM90PARAM_MIC_LIMIT_START+3,1,current_slider->value());
            GET_DEVICE_DATA->dm90_Limit_Release = current_slider->value();
            mainWindow->m_midView->m_localCustom_itemList->addItem(mainWindow->m_midView->m_defaultTtemRow,mainWindow->m_midView->m_itemtext,76+3,current_slider->value());
        }
            break;

        default:
            break;

        }
    }
    //发完数据之后停止计时器
    m_DM90delayTimer->stop();
}

void dm90_limiterView::limiter_release_value(int){


}
//重置按钮=====================
void dm90_limiterView::reset_click(){

    //刷新当前UI界面
    GET_DEVICE_DATA->dm90_Limit_Enable = 0;
    GET_DEVICE_DATA->dm90_Limit_Threshold = 1800;
    GET_DEVICE_DATA->dm90_Limit_Attack = 9;
    GET_DEVICE_DATA->dm90_Limit_Release = 8;
    refreshUI();//刷新界面
    //发送数据
    QVector<int>addrs;
    QVector<int>value = {0,1800,9,8};
    for (int i=0;i<value.count();i++) {
        addrs.push_back(DM90PARAM_MIC_LIMIT_START+i);
    }
   m_commin_fun.sendRandomMessageForDevice(0x13+4,addrs,value);

}

void dm90_limiterView::confirm_click(){
    this->hide();
    QWidget* pWidget = this->parentWidget();
    pWidget->hide();
    Qwdget_timer_index = 1;
    int send_Value = dm90_deviceDataSource::getInstance()->loadingChangeDeviceNumber(double(m_limiterSliders.at(0)->value()-255)/10);
    GET_DEVICE_DATA->dm90_Limit_Threshold = send_Value;
    confirmClick(5);
}
void dm90_limiterView::isopen_enable_click(int enable_status){

    if(enable_status){//打开
        if(GET_DEVICE_DATA->dm90_Limit_Enable != enable_status-1)
        m_commin_fun.sendMessageForDevice(DM90PARAM_MIC_LIMIT_START,1,1);
        for (int i=0;i<m_limiterSliders.count();i++) {
          m_limiterSliders.at(i)->setSliderDisable(false);
        }
        GET_DEVICE_DATA->dm90_Limit_Enable =1;

    }else{//关闭
        if(GET_DEVICE_DATA->dm90_Limit_Enable != enable_status)
        m_commin_fun.sendMessageForDevice(DM90PARAM_MIC_LIMIT_START,1,0);
        for (int i=0;i<m_limiterSliders.count();i++) {
          m_limiterSliders.at(i)->setSliderDisable(true);
        }
        GET_DEVICE_DATA->dm90_Limit_Enable =0;
    }
}
