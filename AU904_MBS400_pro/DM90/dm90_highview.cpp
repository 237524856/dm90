#pragma once
#pragma execution_character_set("utf-8")
#include "dm90_highview.h"
#include "custom_checkbox.h"
#include <QDebug>
#include "dm90_default_datas.hpp"
#include <QVector>


dm90_highView::dm90_highView(QWidget *parent) : QWidget(parent)
{

    this->setAttribute(Qt::WA_StyledBackground,true);
}

void dm90_highView::createUI(){

    //左边的视图
    QWidget *leftView = new QWidget(this);
    leftView->move(10*m_Magnification,0*m_Magnification);
    leftView->resize(70*m_Magnification,600*m_Magnification);
    leftView->setAttribute(Qt::WA_StyledBackground,true);
    leftView->setStyleSheet("background:rgb(17,17,17);border-radius: 15px;");

    //中间的视图===============
    QWidget *midView = new QWidget(this);
    midView->move(90*m_Magnification,0*m_Magnification);
    midView->resize(180*m_Magnification,600*m_Magnification);
    midView->setAttribute(Qt::WA_StyledBackground,true);
    midView->setStyleSheet("background:rgb(17,17,17);border-radius: 15px;");

    QLabel *titleLbl = new QLabel(midView);
    titleLbl->move(20*m_Magnification,10*m_Magnification);
    titleLbl->resize(this->width(),16*m_Magnification);
    titleLbl->setText(tr("  效果"));
    QString updateLabel_stys = QString("background-color:transparent;color:rgb(255,255,255);font-size:%1px;font-family:'Source Han Sans CN Medium'").arg(16*m_Magnification);
    titleLbl->setStyleSheet(updateLabel_stys);
    //默认预设
    QLabel *user_title = new QLabel(midView);
    user_title->move(20*m_Magnification,36*m_Magnification);
    user_title->resize(midView->width()-40*m_Magnification,25*m_Magnification);
    user_title->setText(tr("    默认预设"));
    int fontSize;
    language_index==0?fontSize=13:fontSize=11;
    QString user_title_stys = QString("background-color:rgb(83,83,83);color:rgb(255,255,255);border-radius: 5px;font-size:%1px;font-family:'Source Han Sans CN Medium'").arg(fontSize*m_Magnification);
    user_title->setStyleSheet(user_title_stys);

    m_user_listView= new custom_listView("highViewDefaultPreset",midView->width(),210*m_Magnification,this);
    m_user_listView->move(midView->x()+user_title->x(),user_title->y()+user_title->height());
    m_user_listView->setObjectName("user_listView");
    connect(m_user_listView,SIGNAL(listViewClick(QString,QString,int)),this,SLOT(user_listView_click(QString,QString,int)));

    //自定义预设
    QLabel *custom_title = new QLabel(midView);
    custom_title->move(20*m_Magnification,m_user_listView->y()+m_user_listView->height());
    custom_title->resize(midView->width()-40*m_Magnification,25*m_Magnification);
    custom_title->setText(tr("  自定义预设"));
    custom_title->setStyleSheet(user_title_stys);
    //自定义预设后面的按钮
    QVector<QString>presetImgs = {
        "QPushButton{border-image:url(:/image/images/DM90/preset_sub)}"
        "QPushButton:hover{border-image:url(:/image/images/DM90/preset_sub_hover)}"
        "QPushButton:focus{outline: none;}",
        "QPushButton{border-image:url(:/image/images/DM90/preset_down)}"
        "QPushButton:hover{border-image:url(:/image/images/DM90/preset_down_hover)}"
        "QPushButton:focus{outline: none;}"};
    QVector<QString>presetbtnName={"sub","share"};
    for (int i=0;i<2;i++) {
        QPushButton *btn = new QPushButton(custom_title);
        btn->resize(12*m_Magnification,12*m_Magnification);
        btn->move(95*m_Magnification+25*i*m_Magnification,(25*m_Magnification-btn->height())/2);
        btn->setStyleSheet(presetImgs.at(i));
        btn->setObjectName(presetbtnName.at(i));
        connect(btn,SIGNAL(clicked()),this,SLOT(custom_preset_click()));
    }
    //自定义预设列表
    m_usercustom_itemList = new custom_itemListView("highViewCustomPreset",m_user_listView->width(),m_user_listView->height()+80*m_Magnification,this);
    m_usercustom_itemList->move(midView->x()+user_title->x(),custom_title->y()+custom_title->height());
    connect(m_usercustom_itemList,SIGNAL(listViewClick(QString,QString,int)),this,SLOT(custom_listView_click(QString,QString,int)));

    //右边的上视图
    QWidget *rightTopView = new QWidget(this);
    rightTopView->move(280*m_Magnification,0*m_Magnification);
    rightTopView->resize(730*m_Magnification,190*m_Magnification);
    rightTopView->setAttribute(Qt::WA_StyledBackground,true);
    rightTopView->setStyleSheet("background:rgb(17,17,17);border-radius: 15px;");

    QLabel *outputLbl = new QLabel(rightTopView);
    outputLbl->move(20*m_Magnification,10*m_Magnification);
    outputLbl->resize(this->width(),16*m_Magnification);
    outputLbl->setText(tr("3.5毫米输出"));
    outputLbl->setStyleSheet(updateLabel_stys);

    QVector<QString>outputTitle={tr("耳机输出"),tr("监听"),tr("高音"),tr("低音")};
    QVector<QString>leftTitle={"0",tr("麦克风"),"-12dB","-12dB"};
    QVector<QString>rightTitle={"100",tr("电脑"),"12dB","12dB"};
    QVector<QString>outputObjectName ={"mic","monitor","high","low"};

    //最小值
    QVector<int>slider_minValue={0,0,0,0};
    //最大值
    QVector<int>slider_maxValue={100,200,24,24};
    for (int i=0;i<outputTitle.count();++i) {

        QWidget *signalView_unit = new QWidget(rightTopView);
        signalView_unit->setAttribute(Qt::WA_StyledBackground,true);
        signalView_unit->resize(rightTopView->width()/2,rightTopView->height()-100*m_Magnification);
        if(i>=2){
            signalView_unit->move(rightTopView->width()/2*(i-2),20*m_Magnification+signalView_unit->height());
        }else{
            signalView_unit->move(rightTopView->width()/2*i,30*m_Magnification);
        }

        //signalView_unit->setStyleSheet("background-color:red");
        QLabel *mic_output_lab = new QLabel(signalView_unit);
        mic_output_lab->move(20*m_Magnification,14*m_Magnification);
        mic_output_lab->resize(150*m_Magnification,14*m_Magnification);
        mic_output_lab->setText(outputTitle.at(i));
        QString mic_output_lab_stys = QString("background-color:transparent;color:rgb(255,255,255);font-size:%1px;font-family:'Source Han Sans CN Medium'").arg(12*m_Magnification);
        mic_output_lab->setStyleSheet(mic_output_lab_stys);

        int slider_y      = 0;
        int slider_x      = 0;
        int slider_height = 0;
        int slider_widtht = 0;

        if(i==0){

            MySlider *output_slider = new MySlider(signalView_unit);
            output_slider->setOrientation(Qt::Horizontal);
            output_slider->setMinimum(0);
            output_slider->setMaximum(slider_maxValue.at(i));
            output_slider->setValue(xianfuqi_slider_values.at(0));
            output_slider->setGeometry(20*m_Magnification,mic_output_lab->y()+mic_output_lab->height()+10*m_Magnification,280*m_Magnification,12*m_Magnification);
            output_slider->setObjectName(outputObjectName.at(i));
            connect(output_slider,SIGNAL(valueChanged(int)),this,SLOT(output_chanege_value(int)));
            connect(output_slider,SIGNAL(sliderReleasedAt(int)),this,SLOT(mix_release_value(int)));
            m_highHorSliders.push_back(output_slider);
            slider_y = output_slider->y();
            slider_x = output_slider->x();
            slider_height = output_slider->height();
            slider_widtht = output_slider->width();

        }else{

            SliderWidget *m_mic_slider = new SliderWidget(signalView_unit);
            i==1?m_mic_slider->SetValue(100):m_mic_slider->SetValue(12);
            i==1?m_mic_slider->SetRange(0,200):m_mic_slider->SetRange(0,24);
            m_mic_slider->setGeometry(20*m_Magnification,mic_output_lab->y()+mic_output_lab->height()+10*m_Magnification,280*m_Magnification,12*m_Magnification);
            connect(m_mic_slider,SIGNAL(valueChanged(int)),this,SLOT(mic_chanege_value(int)));
            connect(m_mic_slider,SIGNAL(sliderReleasedAt(int)),this,SLOT(mix_release_value(int)));
            m_mic_slider->setObjectName(outputObjectName.at(i));
            m_mixhighlowSliders.push_back(m_mic_slider);
            slider_y = m_mic_slider->y();
            slider_x = m_mic_slider->x();
            slider_height = m_mic_slider->height();
            slider_widtht = m_mic_slider->width();
        }

        QLabel *current_value = new QLabel(signalView_unit);
        current_value->move(signalView_unit->width()-50*m_Magnification,slider_y-2*m_Magnification);
        current_value->resize(40*m_Magnification,14*m_Magnification);
        current_value->setText("-80dB");
        QString currentLabel_stys = QString("background-color:transparent;color:rgb(255,255,255);font-size:%1px;font-family:'Source Han Sans CN Medium'").arg(12*m_Magnification);
        current_value->setStyleSheet(currentLabel_stys);
        m_currentValueLabels.push_back(current_value);

        QLabel *left_value = new QLabel(signalView_unit);
        left_value->resize(80*m_Magnification,14*m_Magnification);
        left_value->move(slider_x,slider_y+slider_height+5*m_Magnification);
        left_value->setText(leftTitle.at(i));
        left_value->setStyleSheet(currentLabel_stys);

        QLabel *right_value = new QLabel(signalView_unit);
        right_value->resize(80*m_Magnification,16*m_Magnification);
        right_value->move(slider_x+slider_widtht-80*m_Magnification,slider_y+slider_height+5*m_Magnification);
        right_value->setText(rightTitle.at(i));
        right_value->setStyleSheet(currentLabel_stys);
        right_value ->setAlignment(Qt::AlignRight);
    }
    //右边的下 视图
    m_EQView = new dm90_highEQView(this);
    m_EQView->move(rightTopView->x(),rightTopView->height()+10*m_Magnification);
    m_EQView->resize(730*m_Magnification,this->height()-rightTopView->height()-10*m_Magnification);
    connect(m_EQView,SIGNAL(block_highEQSlider(int)),this,SLOT(eqViewSlider_value(int)));
    connect(m_EQView,SIGNAL(block_sliderRelease(int)),this,SLOT(eqViewSliderRelease_value(int)));
    connect(m_EQView,SIGNAL(block_disable_usbplay(int)),this,SLOT(disable_usbplay(int)));

    m_EQView->createUI();
    //m_highEQStackedWidget->addWidget(EQView);
}
//刷新整个UI界面
void dm90_highView::refresh(){

    //水平进度条
    for (int i =0;i<m_highHorSliders.count();i++) {
        MySlider *current_slider = m_highHorSliders.at(i);
        current_slider->blockSignals(true);
        switch (i) {
        case HPOUTPUT_SLIDER:{//耳机输出
            current_slider->setValue(GET_DEVICE_DATA->dm90_HpOutputGain);
            m_currentValueLabels.at(i)->setText(QString::number(GET_DEVICE_DATA->dm90_HpOutputGain,10));
        }
            break;

        default:
            break;
        }
        current_slider->blockSignals(false);
    }

    for (int i=1;i<m_mixhighlowSliders.count()+1;i++) {
        SliderWidget *current_slider = m_mixhighlowSliders.at(i-1);
        current_slider->blockSignals(true);
        switch (i) {
        case MIX_SLIDER:{//监听
            int show_number = dm90_deviceDataSource::getInstance()->loadingChangeNegativeNumber(GET_DEVICE_DATA->dm90_pc_mix);
            current_slider->SetValue(show_number+100);
            m_currentValueLabels.at(i)->setText(QString::number(show_number,10));
        }
            break;

        case MICHIGHGAIN_SLIDER:{//高音

            int show_number = dm90_deviceDataSource::getInstance()->loadingChangeNegativeNumber(GET_DEVICE_DATA->dm90_tone_highGain);
            current_slider->SetValue(show_number+12);
            m_currentValueLabels.at(i)->setText(QString::number(show_number,10)+"dB");
        }
            break;

        case MICLOWGAIN_SLIDER:{//低音

            int show_number = dm90_deviceDataSource::getInstance()->loadingChangeNegativeNumber(GET_DEVICE_DATA->dm90_tone_lowGain);
            current_slider->SetValue(show_number+12);
            m_currentValueLabels.at(i)->setText(QString::number(show_number,10)+"dB");
        }
            break;

        default:
            break;
        }
        current_slider->blockSignals(false);
    }
    //垂直进度条
    m_EQView->refresh();
}

void dm90_highView::disable_usbplay(int status){

    qDebug()<<"测试"<<status;
    m_user_listView->clearItemSelect();
    if(status==0){//没选中
        if(highView_customTempPreData.count()>10)highView_customTempPreData.replace(0,QString::number(0,10));
        m_usercustom_itemList->addItem(m_user_listView->m_defaultHighCurrentItemIndex,m_defaultCurrentItemText,0,0);
    }else{//选中
        if(highView_customTempPreData.count()>10)highView_customTempPreData.replace(0,QString::number(1,10));
        m_usercustom_itemList->addItem(m_user_listView->m_defaultHighCurrentItemIndex,m_defaultCurrentItemText,0,1);
    }

}
//输出进度条
void dm90_highView::output_chanege_value(int currentValue){

    m_HcurrentSliderNum = 1;
    m_currentValueLabels.at(0)->setText(QString::number(currentValue,10));
    m_DM90delayTimer->stop();
    m_DM90delayTimer->start(20);
}
//高低音进度条
void dm90_highView::mic_chanege_value(int currentValue){

    SliderWidget *slider = static_cast<SliderWidget *>(sender());
    if(slider->objectName()=="monitor"){
        m_currentValueLabels.at(1)->setText(QString::number(currentValue-100,10));
        m_HcurrentSliderNum = 2;
    }else if(slider->objectName()=="high"){
        m_currentValueLabels.at(2)->setText(QString::number(currentValue-12,10)+"dB");
        m_HcurrentSliderNum = 3;
    }else{
        m_currentValueLabels.at(3)->setText(QString::number(currentValue-12,10)+"dB");
        m_HcurrentSliderNum = 4;
    }
    m_DM90delayTimer->stop();
    m_DM90delayTimer->start(20);
}
void dm90_highView::mix_release_value(int){

    SliderWidget *slider = static_cast<SliderWidget *>(sender());

    if(slider->objectName()=="high"){
        //取消默认预设的点击
        m_user_listView->clearItemSelect();
        int send_number = dm90_deviceDataSource::getInstance()->loadingChangeDeviceNumber(m_mixhighlowSliders.at(1)->GetValue()-12);
        m_usercustom_itemList->addItem(m_user_listView->m_defaultHighCurrentItemIndex,m_defaultCurrentItemText,1,send_number);

    }else if(slider->objectName()=="low"){
        //取消默认预设的点击
        m_user_listView->clearItemSelect();
        int send_number = dm90_deviceDataSource::getInstance()->loadingChangeDeviceNumber(m_mixhighlowSliders.at(2)->GetValue()-12);
        m_usercustom_itemList->addItem(m_user_listView->m_defaultHighCurrentItemIndex,m_defaultCurrentItemText,2,send_number);
    }

}
void dm90_highView::eqViewSlider_value(int value){

    m_HcurrentSliderNum = 5;
    m_eqViewSliderNumber = value;
    m_DM90delayTimer->stop();
    m_DM90delayTimer->start(20);
}

void dm90_highView::eqViewSliderRelease_value(int current_slider_index){

    double showCurrent = ((double)m_EQView->m_silders.at(m_eqViewSliderNumber)->GetValue()-120)/10;
    int sendValue = showCurrent*10+2000;
    m_user_listView->clearItemSelect();
    m_usercustom_itemList->addItem(m_user_listView->m_defaultHighCurrentItemIndex,m_defaultCurrentItemText,3+m_eqViewSliderNumber,sendValue);

}
//计时器的方法
void dm90_highView::highStarDelayTimerFun(){

    if(Qwdget_timer_index==2){

        switch (m_HcurrentSliderNum) {

        case 1:{//耳机输出
            m_commin_fun.sendMessageForDevice(DM90PARAM_GAIN_END,1,m_highHorSliders.at(0)->value());
            GET_DEVICE_DATA->dm90_HpOutputGain = m_highHorSliders.at(0)->value();
        }
            break;
        case 2:{//监听比例
            int send_number = dm90_deviceDataSource::getInstance()->loadingChangeDeviceNumber(m_mixhighlowSliders.at(0)->GetValue()-100);
            m_commin_fun.sendMessageForDevice(DM90PARAM_LR_MONITOR,1,send_number);
            GET_DEVICE_DATA->dm90_pc_mix = send_number;
        }
            break;
        case 3:{//高音
            int send_number = dm90_deviceDataSource::getInstance()->loadingChangeDeviceNumber(m_mixhighlowSliders.at(1)->GetValue()-12);
            m_commin_fun.sendMessageForDevice(DM90PARAM_MIC_TONE_END,1,send_number);
            if(highView_customTempPreData.count()>10)highView_customTempPreData.replace(1,QString::number(send_number,10));
        }
            break;
        case 4:{//低音
            int send_number = dm90_deviceDataSource::getInstance()->loadingChangeDeviceNumber(m_mixhighlowSliders.at(2)->GetValue()-12);
            m_commin_fun.sendMessageForDevice(DM90PARAM_MIC_TONE_START,1,send_number);
            if(highView_customTempPreData.count()>10)highView_customTempPreData.replace(2,QString::number(send_number,10));
        }
            break;
        case 5:{//高级均衡器

            double showCurrent = ((double)m_EQView->m_silders.at(m_eqViewSliderNumber)->GetValue()-120)/10;
            //发送数据
            int sendValue = showCurrent*10+2000;
            m_commin_fun.sendMessageForDevice(0x205A+5*m_eqViewSliderNumber,1,sendValue);
            qDebug()<<"llllll"<<m_eqViewSliderNumber;
            if(highView_customTempPreData.count()>10)highView_customTempPreData.replace(3+m_eqViewSliderNumber,QString::number(sendValue,10));
        }
            break;

        default:
            break;
        }

        m_DM90delayTimer->stop();
    }
}
void dm90_highView::custom_preset_click(){

    QPushButton *btn = dynamic_cast<QPushButton *>(sender());

    if(btn->objectName()=="sub"){//删除

        if(m_usercustom_itemList->subtractItem()){//删除成功，回到原音

            user_listView_click("user_listView","原声",0);
            //选中原音
            m_user_listView->m_listWidget_default->item(0)->setSelected(true);
            //保存本地的音效ROW
            QString filename = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+"/listrow.dat";
            QVector<QStringList> getlistRowSaveDatas = m_commClass.getDM90CustomPreDatas(filename);
            QStringList changeList("0");
            getlistRowSaveDatas.replace(0,changeList);
            //保存数据
            m_commClass.saveDM90CustomPreDatas(filename,getlistRowSaveDatas);
        }

    }else if(btn->objectName()=="share"){//下载按钮-----------------

        QString AppDataLocation = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
        QString filename = QFileDialog::getOpenFileName(
            this,
            tr("下载音效"),
            AppDataLocation,
            "Document files (*.bin)");
        if (!filename.isEmpty()) {
            QFile file(filename);
            if (file.open(QIODevice::ReadOnly)) {
                QDataStream in(&file);
                QStringList downloadData;
                in >> downloadData;

                //获取路径的名字并进行转换音效名字
                QStringList strlist = filename.split("/");
                QString mysave_name = strlist.at(strlist.count()-1);
                QString title = mysave_name.mid(0,mysave_name.length()-4).trimmed();
                if(title.length()<=0){
                    return;
                }
                downloadData.replace(downloadData.count()-1,title);
                file.close();

                //添加数据，并且保存它
                if(downloadData.count() != 14){
                    int btnStatus = QMessageBox::information(nullptr,tr("提示"),tr("导入的文件无效"),tr("是"));
                    return;
                }
                m_usercustom_itemList->downloadItem(downloadData);
            }
        }
    }
}
//点击默认特效的方法=============================
void dm90_highView::user_listView_click(QString list_objectName,QString itemtext,int user_index){

    if(list_objectName == "user_listView"){

        m_defaultCurrentItemText = itemtext;
        //底下的自定义预设失去焦点
        m_usercustom_itemList->clearSelectStatus();
        //m_allItemesIndex = user_index;

        QString user_default_str = m_high_default_data.at(user_index);
        QStringList string_list = user_default_str.split(",");
        string_list.removeAt(0);
        string_list.removeAt(string_list.count()-1);

        //赋值
        GET_DEVICE_DATA->dm90_tone_highGain = string_list.at(0).toInt();
        GET_DEVICE_DATA->dm90_tone_lowGain  = string_list.at(1).toInt();
        for (int i =0;i<m_EQView->m_silders.count();i++) {

            GET_DEVICE_DATA->dm90UsbPalyEQdatasource[i].Gain = string_list.at(i+2).toInt();
        }
        refresh();
        //发送数据
        QVector<int>addrs;
        QVector<int>value;
        for (int i=0;i<string_list.count();i++) {
            if(i==0){//高音
                addrs.push_back(DM90PARAM_MIC_TONE_END);
            }else if(i==1){//低音
                addrs.push_back(DM90PARAM_MIC_TONE_START);
            }else{
                addrs.push_back(0x205A+5*(i-2));
            }
        }
        for (int k=0;k<string_list.count();k++) {

            value.push_back(string_list.at(k).toInt());
        }
        qDebug()<<"ppppppp"<<string_list<<value;
        int frame_len = 7 + 4 *value.count();
        m_commin_fun.sendRandomMessageForDevice(frame_len,addrs,value);
    }

}
//点击自定义特效的方法=============================
void dm90_highView::custom_listView_click(QString list_objectName,QString itemtext,int user_index){

    if(list_objectName=="highViewCustomPreset"){

        //取消默认预设的点击
        m_user_listView->clearItemSelect();
        //获取本地的数据，刷新对应的UI
        QString filename = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+"/dm90HighView.dat";
        QVector<QStringList> my_slider_map = m_commClass.getDM90CustomPreDatas(filename);
        QStringList current_listDataSource = my_slider_map.at(user_index);
        GET_DEVICE_DATA->dm90_tone_highGain = current_listDataSource.at(1).toInt();
        GET_DEVICE_DATA->dm90_tone_lowGain  = current_listDataSource.at(2).toInt();
        for (int i =0;i<m_EQView->m_silders.count();i++) {
            GET_DEVICE_DATA->dm90UsbPalyEQdatasource[i].Gain = current_listDataSource.at(i+3).toInt();
        }        
        //赋值给缓值
        highView_customTempPreData = current_listDataSource;
        QStringList string_list = current_listDataSource;
        if(string_list.at(0)=="0"){//使能关闭
            if(highView_customTempPreData.count()>10)highView_customTempPreData.replace(0,QString::number(0,10));
            for (int i=0;i<10;i++) {
               GET_DEVICE_DATA->dm90UsbPalyEQdatasource[i].Enable =0;
            }
        }else{//打开
            if(highView_customTempPreData.count()>10)highView_customTempPreData.replace(0,QString::number(1,10));
            for (int i=0;i<10;i++) {
               GET_DEVICE_DATA->dm90UsbPalyEQdatasource[i].Enable =1;
            }
        }
        string_list.removeAt(0);
        string_list.removeAt(string_list.count()-1);

        refresh();
        //发送数据
        QVector<int>addrs;
        QVector<int>value;
        for (int i=0;i<string_list.count();i++) {
            if(i==0){//高音
                addrs.push_back(DM90PARAM_MIC_TONE_END);
            }else if(i==1){//低音
                addrs.push_back(DM90PARAM_MIC_TONE_START);
            }else{
                addrs.push_back(0x205A+5*(i-2));
            }
        }
        for (int k=0;k<string_list.count();k++) {

            value.push_back(string_list.at(k).toInt());
        }
        int frame_len = 7 + 4 *value.count();
        m_commin_fun.sendRandomMessageForDevice(frame_len,addrs,value);


    }
}


