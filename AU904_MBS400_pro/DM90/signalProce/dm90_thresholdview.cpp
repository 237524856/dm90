#pragma once
#pragma execution_character_set("utf-8")
#include "dm90_thresholdview.h"
#include "custom_checkbox.h"
#include "custom_button.h"
#include "../window_dm90.h"

dm90_thresholdview::dm90_thresholdview(QWidget *parent) : QWidget(parent)
{

    this->setAttribute(Qt::WA_StyledBackground,true);

    m_showLabelNumber_arrs = {"42us","83us","167us","333us","667us","1.3ms","2.6ms","5.3ms","10.7ms","21.3ms","42.7ms","85.3ms","170.7ms","341.3ms","682.7ms","1.4s"};

}

void dm90_thresholdview::mousePressEvent(QMouseEvent *qevent){

    // this->hide();
}
void dm90_thresholdview::createUI(){

    QWidget *mianView = new QWidget(this);
    mianView->move(40*m_Magnification,50*m_Magnification);
    mianView->setAttribute(Qt::WA_StyledBackground,true);
    mianView->resize(this->width()-80*m_Magnification,this->height()-100*m_Magnification);
    mianView->setStyleSheet("background:rgb(17,17,17);border-radius: 15px;");

    m_highEQ_btn= new custom_checkbox(mianView);
    m_highEQ_btn->setText(tr("  门限"));
    m_highEQ_btn->resize(92*m_Magnification,40*m_Magnification);
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
    QVector<QString>threshouldTitle={tr("噪声门阈值"),tr("持续时间"),tr("启动时间"),tr("恢复时间")};
    QVector<QString>highlowLeft={"-75dB","42us","42us","42us"};
    QVector<QString>highlowRight={"-28dB","1.4s","1.4s","1.4s"};
    QVector<QString>highLowObjectName ={"high","low"};

    QVector<int>slider_maxValue={47,15,15,15};

    for (int j=0;j<2;j++) {

        for (int i=0;i<2;i++) {

            QWidget *threshould_unit = new QWidget(mianView);
            threshould_unit->setAttribute(Qt::WA_StyledBackground,true);
            threshould_unit->resize(mianView->width()/2,100*m_Magnification);
            threshould_unit->move(mianView->width()/2*i,m_highEQ_btn->height()+m_highEQ_btn->height()+20*m_Magnification +threshould_unit->height()*j);


            QLabel *threshould_lab = new QLabel(threshould_unit);
            threshould_lab->move(40*m_Magnification,14*m_Magnification);
            threshould_lab->resize(120*m_Magnification,16*m_Magnification);
            threshould_lab->setText(threshouldTitle.at(i+2*j));
            QString highLow_lab_stys = QString("background-color:transparent;color:rgb(255,255,255);font-size:%1px;font-family:'Source Han Sans CN Medium'").arg(12*m_Magnification);
            threshould_lab->setStyleSheet(highLow_lab_stys);

            MySlider *threshould_slider = new MySlider(threshould_unit);
            threshould_slider->setOrientation(Qt::Horizontal);
            threshould_slider->setMinimum(0);
            threshould_slider->setMaximum(slider_maxValue.at(i+2*j));
            threshould_slider->setValue(xianfuqi_slider_values.at(0));
            threshould_slider->setGeometry(40*m_Magnification,threshould_lab->y()+threshould_lab->height()+10*m_Magnification,310*m_Magnification,12*m_Magnification);
            threshould_slider->setObjectName(QString::number(i+2*j,10));
            connect(threshould_slider,SIGNAL(valueChanged(int)),this,SLOT(threshould_chanege_value(int)));
            connect(threshould_slider,SIGNAL(sliderReleasedAt(int)),this,SLOT(threshould_release_value(int)));
            m_threshouldSliders.push_back(threshould_slider);

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

void dm90_thresholdview::refreshUI(){

    //按钮的使能状态
    qDebug()<<"tttttttttxxx"<<GET_DEVICE_DATA->dm90_threshold_Enable;

    if(GET_DEVICE_DATA->dm90_threshold_Enable){//打开
        m_highEQ_btn->setCheckState(Qt::Checked);
    }else{//关闭
        m_highEQ_btn->setCheckState(Qt::Unchecked);
    }

    for (int i =0;i<m_threshouldSliders.count();i++) {

        MySlider *current_slider = m_threshouldSliders.at(i);
        current_slider->blockSignals(true);

        switch (i) {

        case THRESHOLD_SLIDER:
        {
            int show_number = dm90_deviceDataSource::getInstance()->loadingChangeNegativeNumber(GET_DEVICE_DATA->dm90_threshold_value);
            current_slider->setValue(show_number+75);
            m_showValueLabels.at(i)->setText(QString::number(show_number,10)+"dB");

        }
            break;

        case HOLD_SLIDER:
        {
            current_slider->setValue(GET_DEVICE_DATA->dm90_threshold_Hold);
//            qDebug()<<"ttttttttt"<<GET_DEVICE_DATA->dm90_threshold_Hold;
            m_showValueLabels.at(i)->setText(m_showLabelNumber_arrs.at(GET_DEVICE_DATA->dm90_threshold_Hold));
        }
            break;


        case ATTACK_SLIDER:{

            current_slider->setValue(GET_DEVICE_DATA->dm90_threshold_Attack);
            m_showValueLabels.at(i)->setText(m_showLabelNumber_arrs.at(GET_DEVICE_DATA->dm90_threshold_Attack));
        }
            break;

        case RELEASE_SLIDER:{

            current_slider->setValue(GET_DEVICE_DATA->dm90_threshold_Release);
            m_showValueLabels.at(i)->setText(m_showLabelNumber_arrs.at(GET_DEVICE_DATA->dm90_threshold_Release));
        }
            break;
        default:
            break;
        }
        current_slider->blockSignals(false);
    }
}


void dm90_thresholdview::threshould_chanege_value(int value){

    m_DM90delayTimer->stop();
    m_DM90delayTimer->start(20);
    MySlider *threshould_slider = static_cast<MySlider *>(sender());
    int slider_number = threshould_slider->objectName().toInt();
    m_currentSliderNum = slider_number;
    switch (slider_number) {
    case THRESHOLD_SLIDER:
    {

        m_showValueLabels.at(slider_number)->setText(QString::number(value-75,10)+"dB");

    }
        break;

    case HOLD_SLIDER:
    {

        m_showValueLabels.at(slider_number)->setText(m_showLabelNumber_arrs.at(value));
    }
        break;


    case ATTACK_SLIDER:{

        m_showValueLabels.at(slider_number)->setText(m_showLabelNumber_arrs.at(value));
    }
        break;

    case RELEASE_SLIDER:{

        m_showValueLabels.at(slider_number)->setText(m_showLabelNumber_arrs.at(value));
    }
        break;

    default:
        break;

    }
}
//延时发送数据的方法==================================
void dm90_thresholdview::thresholdStarDelayTimerFun(){

    if(Qwdget_timer_index==5){

        window_dm90* mainWindow = (window_dm90*)(this->parentWidget()->parentWidget());//获取列表视图
        //发送数据
        MySlider *current_slider = m_threshouldSliders.at(m_currentSliderNum);
        switch (m_currentSliderNum) {

        case THRESHOLD_SLIDER:
        {
            int send_Value = dm90_deviceDataSource::getInstance()->loadingChangeDeviceNumber(current_slider->value()-75);
            m_commin_fun.sendMessageForDevice(DM90PARAM_MIC_GATE_START+1,1,send_Value);
            GET_DEVICE_DATA->dm90_threshold_value = send_Value;
            mainWindow->m_midView->m_localCustom_itemList->addItem(mainWindow->m_midView->m_defaultTtemRow,mainWindow->m_midView->m_itemtext,1,send_Value);

        }
            break;
        case HOLD_SLIDER:
        {
            m_commin_fun.sendMessageForDevice(DM90PARAM_MIC_GATE_START+4,1,current_slider->value());
            GET_DEVICE_DATA->dm90_threshold_Hold = current_slider->value();
            mainWindow->m_midView->m_localCustom_itemList->addItem(mainWindow->m_midView->m_defaultTtemRow,mainWindow->m_midView->m_itemtext,4,current_slider->value());
        }
            break;

        case ATTACK_SLIDER:{

            m_commin_fun.sendMessageForDevice(DM90PARAM_MIC_GATE_START+2,1,current_slider->value());
            GET_DEVICE_DATA->dm90_threshold_Attack = current_slider->value();
            mainWindow->m_midView->m_localCustom_itemList->addItem(mainWindow->m_midView->m_defaultTtemRow,mainWindow->m_midView->m_itemtext,2,current_slider->value());
        }
            break;

        case RELEASE_SLIDER:{
            m_commin_fun.sendMessageForDevice(DM90PARAM_MIC_GATE_START+3,1,current_slider->value());
            GET_DEVICE_DATA->dm90_threshold_Release = current_slider->value();
            mainWindow->m_midView->m_localCustom_itemList->addItem(mainWindow->m_midView->m_defaultTtemRow,mainWindow->m_midView->m_itemtext,3,current_slider->value());
        }
            break;
        default:
            break;
        }
        //发完数据之后停止计时器
        m_DM90delayTimer->stop();
    }
}


void dm90_thresholdview::threshould_release_value(int){


}
//重置按钮=====================
void dm90_thresholdview::reset_click(){

    //刷新当前UI界面
    GET_DEVICE_DATA->dm90_threshold_Enable = 0;
    GET_DEVICE_DATA->dm90_threshold_value = 1400;
    GET_DEVICE_DATA->dm90_threshold_Hold = 9;
    GET_DEVICE_DATA->dm90_threshold_Attack =11;
    GET_DEVICE_DATA->dm90_threshold_Release = 11;
    refreshUI();//刷新界面
    //发送数据
    QVector<int>addrs;
    QVector<int>value = {0,1400,11,11,9};
    for (int i=0;i<value.count();i++) {
        addrs.push_back(DM90PARAM_MIC_GATE_START+i);
    }
   m_commin_fun.sendRandomMessageForDevice(0x17+4,addrs,value);
}

void dm90_thresholdview::confirm_click(){

    QWidget* pWidget = this->parentWidget();
    pWidget->hide();
    this->hide();
    Qwdget_timer_index = 1;
    int send_Value = dm90_deviceDataSource::getInstance()->loadingChangeDeviceNumber(m_threshouldSliders.at(0)->value()-75);
    GET_DEVICE_DATA->dm90_threshold_value = send_Value;
    confirmClick(2);
}
//使能按钮
void dm90_thresholdview::isopen_enable_click(int enable_status){

    qDebug()<<"tttttttttgggggg"<<enable_status;
    if(enable_status){//打开
        if(GET_DEVICE_DATA->dm90_threshold_Enable != enable_status-1)
        m_commin_fun.sendMessageForDevice(DM90PARAM_MIC_GATE_START,1,1);
        for (int i=0;i<m_threshouldSliders.count();i++) {
          m_threshouldSliders.at(i)->setSliderDisable(false);
        }
        GET_DEVICE_DATA->dm90_threshold_Enable =1;

    }else{//关闭
        if(GET_DEVICE_DATA->dm90_threshold_Enable != enable_status)
        m_commin_fun.sendMessageForDevice(DM90PARAM_MIC_GATE_START,1,0);
        for (int i=0;i<m_threshouldSliders.count();i++) {
          m_threshouldSliders.at(i)->setSliderDisable(true);
        }
        GET_DEVICE_DATA->dm90_threshold_Enable =0;
    }

}
