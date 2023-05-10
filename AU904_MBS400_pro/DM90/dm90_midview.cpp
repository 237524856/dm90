
#pragma once
#pragma execution_character_set("utf-8")
#include "dm90_midview.h"
#include "signalinstance.h"
#include "customView/custom_listview.h"
#include "dm90_default_datas.hpp"

dm90_midview::dm90_midview(QWidget *parent) : QWidget(parent)
{
    this->setAttribute(Qt::WA_StyledBackground,true);
    connect(this,SIGNAL(sendResultNumber(int)),&SignalInstance::GetInstance(),SIGNAL(changeResultTitle(int)));
}

void dm90_midview::createUI() {
    QWidget *backView = new QWidget(this);
    backView->resize(this->width(),this->height());
    backView->setAttribute(Qt::WA_StyledBackground,true);
    backView->setStyleSheet("background:rgb(17,17,17);border-radius: 15px;");

    QVector<QString>titles={tr("本地声音"),tr("效果")};
    QString update_btn_stys = QString("QPushButton {background-color:transparent;color:rgb(255,255,255);text-align:center;font-size:%1px;font-family:'Source Han Sans CN Medium'}"
                                      "QPushButton:hover{background-color:transparent;color:rgb(54,54,54);text-align:center;font-size:%2px;font-family:'Source Han Sans CN Medium'}"
                                      "QPushButton:pressed{background-color:transparent;color:rgb(255,171,33);text-align:center;font-size:%2px;font-family:'Source Han Sans CN Medium'}"
                                      "QPushButton:focus{outline: none;}").arg(15*m_Magnification).arg(15*m_Magnification);
    for (int i=0;i<titles.count();++i) {
        QPushButton *titleBtn = new QPushButton(this);
        titleBtn->resize(this->width()/2,30*m_Magnification);
        titleBtn->move(this->width()/2 *i,10);
        titleBtn->setText(titles.at(i));
        titleBtn->setStyleSheet(update_btn_stys);
        titleBtn->setObjectName(QString::number(i,10));
        connect(titleBtn, SIGNAL(clicked()), this, SLOT(titleBtnClick()));
        btns.push_back(titleBtn);
    }

    int value = btns.at(0)->fontMetrics().boundingRect(btns.at(0)->text()).width();
    m_titleStatus = new QLabel(this);
    m_titleStatus->move(btns.at(0)->x()+(btns.at(0)->width()-value)/2,10+30*m_Magnification);
    m_titleStatus->resize(value,2*m_Magnification);
    m_titleStatus->setStyleSheet("background-color:rgb(255,171,33)");

    m_pStackedWidget = new QStackedWidget(this);
    m_pStackedWidget->move(20*m_Magnification,46*m_Magnification);
    m_pStackedWidget->resize(this->width(),this->height()-46*m_Magnification);

    initLocalSoundView();
    initEffectView();
}
void dm90_midview::initLocalSoundView(){
    //添加本地声音视图===========
    QWidget *localSoundView = new QWidget();
    localSoundView->resize(m_pStackedWidget->width(),m_pStackedWidget->height());
    m_pStackedWidget->addWidget(localSoundView);

    QLabel *local_default_title = new QLabel(localSoundView);
    //user_title->move(20*m_Magnification,46*m_Magnification);
    local_default_title->resize(this->width()-40*m_Magnification,25*m_Magnification);
    local_default_title->setText(tr("    默认预设"));
    int fontSize;
    language_index==0?fontSize=13:fontSize=11;
    QString user_title_stys = QString("background-color:rgb(83,83,83);color:rgb(255,255,255);border-radius: 5px;font-size:%1px;font-family:'Source Han Sans CN Medium'").arg(fontSize*m_Magnification);
    local_default_title->setStyleSheet(user_title_stys);

    m_default_locaListView = new custom_listView("localSoundViewDefaultPreset",this->width(),210*m_Magnification,localSoundView);
    m_default_locaListView->move(local_default_title->x(),local_default_title->y()+local_default_title->height());
    m_default_locaListView->setObjectName("user_listView");
    connect(m_default_locaListView,SIGNAL(locallistViewClick(QString,QString,int)),this,SLOT(localDefault_listView_click(QString,QString,int)));

    //自定义预设
    QLabel *local_custom_title = new QLabel(localSoundView);
    local_custom_title->move(0,m_default_locaListView->y()+m_default_locaListView->height());
    local_custom_title->resize(this->width()-40*m_Magnification,25*m_Magnification);
    local_custom_title->setText(tr("  自定义预设"));
    local_custom_title->setStyleSheet(user_title_stys);
    //自定义预设后面的按钮
    QVector<QString>presetImgs = {"QPushButton{border-image:url(:/image/images/DM90/preset_sub)}"
                                  "QPushButton:hover{border-image:url(:/image/images/DM90/preset_sub_hover)}"
                                  "QPushButton:focus{outline: none;}",
                                  "QPushButton{border-image:url(:/image/images/DM90/preset_down)}"
                                  "QPushButton:hover{border-image:url(:/image/images/DM90/preset_down_hover)}"
                                  "QPushButton:focus{outline: none;}"};

    QVector<QString>presetbtnName={"local_sub","local_share"};
    for (int i=0;i<presetbtnName.count();i++) {
        QPushButton *btn = new QPushButton(local_custom_title);
        btn->resize(12*m_Magnification,12*m_Magnification);
        btn->move(90*m_Magnification+25*i*m_Magnification,(25*m_Magnification-btn->height())/2);
        btn->setStyleSheet(presetImgs.at(i));
        btn->setObjectName(presetbtnName.at(i));
        connect(btn,SIGNAL(clicked()),this,SLOT(localCustom_preset_click()));
    }
    //创建自定义的列表
    m_localCustom_itemList = new custom_itemListView("localSoundViewCustomPreset",m_default_locaListView->width(),m_default_locaListView->height()+70*m_Magnification,localSoundView);
    m_localCustom_itemList->move(0+local_custom_title->x(),local_custom_title->y()+local_custom_title->height());
    m_localCustom_itemList->refreshUI();
    connect(m_localCustom_itemList,SIGNAL(listViewClick(QString,QString,int)),this,SLOT(localCustom_listView_click(QString,QString,int)));
}
void dm90_midview::initEffectView(){

    //添加本地声音视图===========
    QWidget *localSoundView = new QWidget();
    localSoundView->resize(m_pStackedWidget->width(),m_pStackedWidget->height());
    m_pStackedWidget->addWidget(localSoundView);

    QLabel *local_default_title = new QLabel(localSoundView);
    //user_title->move(20*m_Magnification,46*m_Magnification);
    local_default_title->resize(this->width()-40*m_Magnification,25*m_Magnification);
    local_default_title->setText(tr("    默认预设"));
    int fontSize;
    language_index==0?fontSize=13:fontSize=11;
    QString user_title_stys = QString("background-color:rgb(83,83,83);color:rgb(255,255,255);border-radius: 5px;font-size:%1px;font-family:'Source Han Sans CN Medium'").arg(fontSize*m_Magnification);
    local_default_title->setStyleSheet(user_title_stys);

    m_default_effectListView = new custom_listView("effectViewDefaultPreset",this->width(),210*m_Magnification,localSoundView);
    m_default_effectListView->move(local_default_title->x(),local_default_title->y()+local_default_title->height());
    m_default_effectListView->setObjectName("user_listView");
    connect(m_default_effectListView,SIGNAL(listViewClick(QString,QString,int)),this,SLOT(effectDefault_listView_click(QString,QString,int)));

    //自定义预设
    QLabel *local_custom_title = new QLabel(localSoundView);
    local_custom_title->move(0,m_default_effectListView->y()+m_default_effectListView->height());
    local_custom_title->resize(this->width()-40*m_Magnification,25*m_Magnification);
    local_custom_title->setText(tr("  自定义预设"));
    local_custom_title->setStyleSheet(user_title_stys);

    //自定义预设后面的按钮
    QVector<QString>presetImgs = {"QPushButton{border-image:url(:/image/images/DM90/preset_sub)}"
                                  "QPushButton:hover{border-image:url(:/image/images/DM90/preset_sub_hover)}"
                                  "QPushButton:focus{outline: none;}",
                                  "QPushButton{border-image:url(:/image/images/DM90/preset_down)}"
                                  "QPushButton:hover{border-image:url(:/image/images/DM90/preset_down_hover)}"
                                  "QPushButton:focus{outline: none;}"};
    QVector<QString>presetbtnName={"effect_sub","effect_share"};
    for (int i=0;i<2;i++) {
        QPushButton *btn = new QPushButton(local_custom_title);
        btn->resize(12*m_Magnification,12*m_Magnification);
        btn->move(90*m_Magnification+25*i*m_Magnification,(25*m_Magnification-btn->height())/2);
        btn->setStyleSheet(presetImgs.at(i));
        btn->setObjectName(presetbtnName.at(i));
        connect(btn,SIGNAL(clicked()),this,SLOT(effectCustom_preset_click()));
    }
    m_effectCustom_itemList = new custom_itemListView("effectViewCustomtPreset",m_default_effectListView->width(),m_default_effectListView->height()+70*m_Magnification,localSoundView);
    m_effectCustom_itemList->move(0+local_custom_title->x(),local_custom_title->y()+local_custom_title->height());
    m_effectCustom_itemList->refreshUI();
    connect(m_effectCustom_itemList,SIGNAL(listViewClick(QString,QString,int)),this,SLOT(effectCustom_listView_click(QString,QString,int)));
}
//切换本地声音和效果
void dm90_midview::titleBtnClick(){

    QPushButton *btn = static_cast<QPushButton *>(sender());
    int value = btn->fontMetrics().boundingRect(btn->text()).width();
    m_titleStatus->move(btn->x()+(btn->width()-value)/2,10+30*m_Magnification);
    m_titleStatus->resize(value,2*m_Magnification);
    emit sendResultNumber(btn->objectName().toUInt());

    m_pStackedWidget->setCurrentIndex(btn->objectName().toUInt());

    if(btn->objectName().toUInt()==0){//本地声音

        m_localCustom_itemList->m_objectName = "localSoundViewCustomPreset";
        m_localCustom_itemList->refreshUI();

    }else{//效果
        m_effectCustom_itemList->m_objectName = "effectViewCustomtPreset";
        m_effectCustom_itemList->refreshUI();
    }
}

//点击了本地声音的默认列表数据
void dm90_midview::localDefault_listView_click(QString titleName,QString itemtext,int select){

    m_itemtext = itemtext;
    m_defaultTtemRow = select;
    QString user_soundStr = m_localSound_default_data.at(select);
    //刷新界面
    QStringList user_default_list = user_soundStr.split(",");
    qDebug()<<"eeeenterkkk===="<<user_default_list.count();
    QByteArray device_datas_pack;

    for (int i=0;i<user_default_list.count()-1;i++) {
        uint16_t a  =user_default_list.at(i).toInt();
        device_datas_pack.push_back(a&0xff);
        device_datas_pack.push_back((a>>8)&0xff);
    }    
    GET_LOCAL_DATA = (pc_localSound_dataSource*)device_datas_pack.data();
    localGetMainDatasource();
    //关闭自定义的选中
    m_localCustom_itemList->clearSelectStatus();

    //发送数据====一共要发送4包数据
    int addrs = 0x2028;
    QVector<int>values;
    for (int i=0;i<26;i++) {
        uint16_t default_value  =user_default_list.at(i).toInt();
        values.push_back(default_value);
    }
    int frame_len = 15 + 2 *(values.count()-2);
    m_commin_fun.sendCotiMessageForDevice(frame_len,addrs,values.count(),values);

    //第二包
    addrs+=26;
    values.clear();
    for (int i=0;i<21;i++) {
        uint16_t default_value  =user_default_list.at(i+26).toInt();
        values.push_back(default_value);
    }
    frame_len = 15 + 2 *(values.count()-2);
    m_commin_fun.sendCotiMessageForDevice(frame_len,addrs,values.count(),values);

    //第三包
    addrs = 0x2089;
    values.clear();
    for (int i=0;i<24;i++) {
        uint16_t default_value  =user_default_list.at(i+26+21).toInt();
        values.push_back(default_value);
    }
    frame_len = 15 + 2 *(values.count()-2);
    m_commin_fun.sendCotiMessageForDevice(frame_len,addrs,values.count(),values);

    //第四包
    addrs = 0x20BD;
    values.clear();
    for (int i=0;i<9;i++) {
        uint16_t default_value  =user_default_list.at(i+26+21+24).toInt();
        values.push_back(default_value);
    }
    frame_len = 15 + 2 *(values.count()-2);
    m_commin_fun.sendCotiMessageForDevice(frame_len,addrs,values.count(),values);
    //刷新对应的界面
    emit refreshLocalSoundUI(true);

}
//点击了本地声音的自定义列表数据
void dm90_midview::localCustom_listView_click(QString,QString,int select){

    //取消默认预设的点击
    m_default_locaListView->clearItemSelect();

    //获取本地的音效，并且更新对应的UI界面
    QString filename = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+"/localSoundView.bin";
    QVector<QStringList> my_slider_map = m_commClass.getDM90CustomPreDatas(filename);

    QStringList user_custom_list = my_slider_map.at(select);
    QByteArray device_datas_pack;

    for (int i=0;i<user_custom_list.count()-1;i++) {
        uint16_t a  =user_custom_list.at(i).toInt();
        device_datas_pack.push_back(a&0xff);
        device_datas_pack.push_back((a>>8)&0xff);
    }
    GET_LOCAL_DATA = (pc_localSound_dataSource*)device_datas_pack.data();
    localGetMainDatasource();

    //发送数据====一共要发送4包数据
    int addrs = 0x2028;
    QVector<int>values;
    for (int i=0;i<26;i++) {
        uint16_t default_value  =user_custom_list.at(i).toInt();
        values.push_back(default_value);
    }
    int frame_len = 15 + 2 *(values.count()-2);
    m_commin_fun.sendCotiMessageForDevice(frame_len,addrs,values.count(),values);

    //第二包
    addrs+=26;
    values.clear();
    for (int i=0;i<21;i++) {
        uint16_t default_value  =user_custom_list.at(i+26).toInt();
        values.push_back(default_value);
    }
    frame_len = 15 + 2 *(values.count()-2);
    m_commin_fun.sendCotiMessageForDevice(frame_len,addrs,values.count(),values);

    //第三包
    addrs = 0x2089;
    values.clear();
    for (int i=0;i<24;i++) {
        uint16_t default_value  =user_custom_list.at(i+26+21).toInt();
        values.push_back(default_value);
    }
    frame_len = 15 + 2 *(values.count()-2);
    m_commin_fun.sendCotiMessageForDevice(frame_len,addrs,values.count(),values);

    //第四包
    addrs = 0x20BD;
    values.clear();
    for (int i=0;i<9;i++) {
        uint16_t default_value  =user_custom_list.at(i+26+21+24).toInt();
        values.push_back(default_value);
    }
    frame_len = 15 + 2 *(values.count()-2);
    m_commin_fun.sendCotiMessageForDevice(frame_len,addrs,values.count(),values);

    //刷新麦克风页面
    emit refreshLocalSoundUI(true);

}
//效果页面的默认音效*******************************************
void dm90_midview::effectDefault_listView_click(QString,QString itemtext,int select){

    m_itemtext = itemtext;
    m_defaultTtemRow = select;
    QString user_soundStr = m_effectSound_default_data.at(select);
    QStringList user_default_list = user_soundStr.split(",");
    //qDebug()<<"eeeenter===="<<user_default_list.count();
    QByteArray device_datas_pack;
    for (int i=0;i<user_default_list.count()-1;i++) {
        uint16_t a  =user_default_list.at(i).toInt();
        device_datas_pack.push_back(a&0xff);
        device_datas_pack.push_back((a>>8)&0xff);
    }
    GET_EFFECT_DATA = (pc_effectSound_dataSource*)device_datas_pack.data();
    effectGetMainDatasource();
    //刷新麦克风页面
    emit refreshLocalSoundUI(true);
    //关闭自定义的选中
    m_effectCustom_itemList->clearSelectStatus();

    //发送数据====一共要发送2包数据
    int addrs = 0x20A1;
    QVector<int>values;
    for (int i=0;i<22;i++) {
        uint16_t default_value = user_default_list.at(i).toInt();
        values.push_back(default_value);
    }
    int frame_len = 15 + 2 *(values.count()-2);
    m_commin_fun.sendCotiMessageForDevice(frame_len,addrs,values.count(),values);
    //第二包
    addrs+=22;
    values.clear();
    for (int i=0;i<6;i++) {
        uint16_t default_value  =user_default_list.at(i+22).toInt();
        values.push_back(default_value);
    }
    frame_len = 15 + 2 *(values.count()-2);
    m_commin_fun.sendCotiMessageForDevice(frame_len,addrs,values.count(),values);
}

//效果页面的自定义音效
void dm90_midview::effectCustom_listView_click(QString,QString itemtext,int select){

    //取消默认预设的点击
    m_default_effectListView->clearItemSelect();
    //获取本地的音效，并且更新对应的UI界面
    QString filename = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+"/effectSoundView.bin";
    QVector<QStringList> my_slider_map = m_commClass.getDM90CustomPreDatas(filename);

    QStringList user_custom_list = my_slider_map.at(select);
    QByteArray device_datas_pack;
    for (int i=0;i<user_custom_list.count()-1;i++) {
        uint16_t a  =user_custom_list.at(i).toInt();
        device_datas_pack.push_back(a&0xff);
        device_datas_pack.push_back((a>>8)&0xff);
    }
    GET_EFFECT_DATA = (pc_effectSound_dataSource*)device_datas_pack.data();
    effectGetMainDatasource();
    //刷新麦克风页面
    emit refreshLocalSoundUI(true);

    //发送数据====一共要发送2包数据
    int addrs = 0x20A1;
    QVector<int>values;
    for (int i=0;i<22;i++) {
        uint16_t default_value  =user_custom_list.at(i).toInt();
        values.push_back(default_value);
    }
    int frame_len = 15 + 2 *(values.count()-2);
    m_commin_fun.sendCotiMessageForDevice(frame_len,addrs,values.count(),values);
    //第二包
    addrs+=22;
    values.clear();
    for (int i=0;i<6;i++) {
        uint16_t default_value  =user_custom_list.at(i+22).toInt();
        values.push_back(default_value);
    }
    frame_len = 15 + 2 *(values.count()-2);
    m_commin_fun.sendCotiMessageForDevice(frame_len,addrs,values.count(),values);

}
//本地声音
void dm90_midview::localCustom_preset_click(){

    QPushButton *clickBtn = dynamic_cast<QPushButton *>(sender());

    if(clickBtn->objectName()=="local_sub"){//删除预设

        if(m_localCustom_itemList->subtractItem()){//删除成功，回到原音
            //选中原音
            m_default_locaListView->m_listWidget_default->item(0)->setSelected(true);
            //保存本地的音效ROW
            QString filename = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+"/listrow.dat";
            QVector<QStringList> getlistRowSaveDatas = m_commClass.getDM90CustomPreDatas(filename);
            QStringList changeList("0");
            getlistRowSaveDatas.replace(1,changeList);
            //保存数据
            m_commClass.saveDM90CustomPreDatas(filename,getlistRowSaveDatas);
        }

    }else if(clickBtn->objectName()=="local_share"){//下载

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
                if(downloadData.count() != 81){
                    QMessageBox::information(nullptr,tr("提示"),tr("导入的文件无效"),tr("是"));
                    return;
                }
                m_localCustom_itemList->downloadItem(downloadData);
            }
        }
    }
}

//效果页面的删除分享
void dm90_midview::effectCustom_preset_click(){

    QPushButton *clickBtn = dynamic_cast<QPushButton *>(sender());
    if(clickBtn->objectName()=="effect_sub"){//删除预设

        if(m_effectCustom_itemList->subtractItem()){//删除成功，回到原音
            //选中原音
            m_default_effectListView->m_listWidget_default->item(0)->setSelected(true);
            //保存本地的音效ROW
            QString filename = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+"/listrow.dat";
            QVector<QStringList> getlistRowSaveDatas = m_commClass.getDM90CustomPreDatas(filename);
            QStringList changeList("0");
            getlistRowSaveDatas.replace(2,changeList);
            //保存数据
            m_commClass.saveDM90CustomPreDatas(filename,getlistRowSaveDatas);
        }

    }else if(clickBtn->objectName()=="effect_share"){//下载

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
                if(downloadData.count() != 29){

                    int btnStatus = QMessageBox::information(nullptr,tr("提示"),tr("导入的文件无效"),tr("是"));
                    return;
                }
                m_effectCustom_itemList->downloadItem(downloadData);

            }

        }
    }
}

void dm90_midview::localAddItem(int index,int value){

    m_default_locaListView->clearItemSelect();
    m_localCustom_itemList->addItem(m_defaultTtemRow,m_itemtext,index,value);
}

void dm90_midview::effectAddItem(int index,int value){

    qDebug()<<"dddddd"<<m_itemtext;
    m_default_effectListView->clearItemSelect();
    if(m_itemtext.length()<=0)return;
    m_effectCustom_itemList->addItem(m_defaultTtemRow,m_itemtext,index,value);

}
//本地声音预设值赋给主设备的结构体
void dm90_midview::localGetMainDatasource(){

    //给主结构体赋值
    //门限
    GET_DEVICE_DATA->dm90_threshold_Enable = GET_LOCAL_DATA->dm90_threshold_Enable;
    GET_DEVICE_DATA->dm90_threshold_value = GET_LOCAL_DATA->dm90_threshold_value;
    GET_DEVICE_DATA->dm90_threshold_Attack = GET_LOCAL_DATA->dm90_threshold_Attack;
    GET_DEVICE_DATA->dm90_threshold_Release = GET_LOCAL_DATA->dm90_threshold_Release;
    GET_DEVICE_DATA->dm90_threshold_Hold = GET_LOCAL_DATA->dm90_threshold_Hold;
    GET_DEVICE_DATA->dm90_HPF_Enable = GET_LOCAL_DATA->dm90_HPF_Enable;
    GET_DEVICE_DATA->dm90_HPF_Freq = GET_LOCAL_DATA->dm90_HPF_Freq;

    for (int i=0;i<8;i++) {
       GET_DEVICE_DATA->dm90micEQdatasource[i].Enable = GET_LOCAL_DATA->dm90micEQdatasource[i].Enable;
       GET_DEVICE_DATA->dm90micEQdatasource[i].Type = GET_LOCAL_DATA->dm90micEQdatasource[i].Type;
       GET_DEVICE_DATA->dm90micEQdatasource[i].Freq = GET_LOCAL_DATA->dm90micEQdatasource[i].Freq;
       GET_DEVICE_DATA->dm90micEQdatasource[i].Gain = GET_LOCAL_DATA->dm90micEQdatasource[i].Gain;
       GET_DEVICE_DATA->dm90micEQdatasource[i].Q = GET_LOCAL_DATA->dm90micEQdatasource[i].Q;
    }
    //齿音消除器
    GET_DEVICE_DATA->dm90_Dental_Enable = GET_LOCAL_DATA->dm90_Dental_Enable;
    GET_DEVICE_DATA->dm90_Dental_Threshold = GET_LOCAL_DATA->dm90_Dental_Threshold;
    GET_DEVICE_DATA->dm90_Dental_Freq = GET_LOCAL_DATA->dm90_Dental_Freq;
    GET_DEVICE_DATA->dm90_Dental_Q = GET_LOCAL_DATA->dm90_Dental_Q;
    GET_DEVICE_DATA->dm90_Dental_Attack = GET_LOCAL_DATA->dm90_Dental_Attack;
    GET_DEVICE_DATA->dm90_Dental_Release = GET_LOCAL_DATA->dm90_Dental_Release;
    GET_DEVICE_DATA->dm90_Dental_MinGain = GET_LOCAL_DATA->dm90_Dental_MinGain;
    GET_DEVICE_DATA->dm90_Dental_DetectPeriod = GET_LOCAL_DATA->dm90_Dental_DetectPeriod;

    //去爆音器
    GET_DEVICE_DATA->dm90_Detonator_Enable = GET_LOCAL_DATA->dm90_Detonator_Enable;
    GET_DEVICE_DATA->dm90_Detonator_Threshold = GET_LOCAL_DATA->dm90_Detonator_Threshold;
    GET_DEVICE_DATA->dm90_Detonator_Freq = GET_LOCAL_DATA->dm90_Detonator_Freq;
    GET_DEVICE_DATA->dm90_Detonator_Q = GET_LOCAL_DATA->dm90_Detonator_Q;
    GET_DEVICE_DATA->dm90_Detonator_Attack = GET_LOCAL_DATA->dm90_Detonator_Attack;
    GET_DEVICE_DATA->dm90_Detonator_Release = GET_LOCAL_DATA->dm90_Detonator_Release;
    GET_DEVICE_DATA->dm90_Detonator_MinGain = GET_LOCAL_DATA->dm90_Detonator_MinGain;
    GET_DEVICE_DATA->dm90_Detonator_DetectPeriod = GET_LOCAL_DATA->dm90_Detonator_DetectPeriod;

    //语音变调
    GET_DEVICE_DATA->dm90_VoiceSandhi_Enable = GET_LOCAL_DATA->dm90_VoiceSandhi_Enable;
    GET_DEVICE_DATA->dm90_VoiceSandhi_Rate = GET_LOCAL_DATA->dm90_VoiceSandhi_Rate;
    //镶边器/移相器
    GET_DEVICE_DATA->dm90_flanger_Enable = GET_LOCAL_DATA->dm90_flanger_Enable;
    GET_DEVICE_DATA->dm90_flanger_Level = GET_LOCAL_DATA->dm90_flanger_Level;
    GET_DEVICE_DATA->dm90_flanger_Delay = GET_LOCAL_DATA->dm90_flanger_Delay;
    GET_DEVICE_DATA->dm90_flanger_FreqModu = GET_LOCAL_DATA->dm90_flanger_FreqModu;
    GET_DEVICE_DATA->dm90_flanger_DeptModu = GET_LOCAL_DATA->dm90_flanger_DeptModu;
    GET_DEVICE_DATA->dm90_flanger_FeedRate = GET_LOCAL_DATA->dm90_flanger_FeedRate;

    //压缩器(Comp)
    GET_DEVICE_DATA->dm90_Comp_Enable = GET_LOCAL_DATA->dm90_Comp_Enable;
    GET_DEVICE_DATA->dm90_Comp_Threshold = GET_LOCAL_DATA->dm90_Comp_Threshold;
    GET_DEVICE_DATA->dm90_Comp_Attack = GET_LOCAL_DATA->dm90_Comp_Attack;
    GET_DEVICE_DATA->dm90_Comp_Release = GET_LOCAL_DATA->dm90_Comp_Release;
    GET_DEVICE_DATA->dm90_Comp_Rate = GET_LOCAL_DATA->dm90_Comp_Rate;

    //限幅器(Limit)
    GET_DEVICE_DATA->dm90_Limit_Enable = GET_LOCAL_DATA->dm90_Limit_Enable;
    GET_DEVICE_DATA->dm90_Limit_Threshold = GET_LOCAL_DATA->dm90_Limit_Threshold;
    GET_DEVICE_DATA->dm90_Limit_Attack = GET_LOCAL_DATA->dm90_Limit_Attack;
    GET_DEVICE_DATA->dm90_Limit_Release = GET_LOCAL_DATA->dm90_Limit_Release;
}
void dm90_midview::effectGetMainDatasource(){
    //和声
    GET_DEVICE_DATA->dm90_harmony_Enable = GET_EFFECT_DATA->dm90_harmony_Enable;
    GET_DEVICE_DATA->dm90_harmony_Level = GET_EFFECT_DATA->dm90_harmony_Level;
    GET_DEVICE_DATA->dm90_harmony_Delay = GET_EFFECT_DATA->dm90_harmony_Delay;
    GET_DEVICE_DATA->dm90_harmony_FreqModu1 = GET_EFFECT_DATA->dm90_harmony_FreqModu1;
    GET_DEVICE_DATA->dm90_harmony_DeptModu1 = GET_EFFECT_DATA->dm90_harmony_DeptModu1;
    GET_DEVICE_DATA->dm90_harmony_FreqModu2 = GET_EFFECT_DATA->dm90_harmony_FreqModu2;
    GET_DEVICE_DATA->dm90_harmony_DeptModu2 = GET_EFFECT_DATA->dm90_harmony_DeptModu2;
    //混响(Reverb)
    GET_DEVICE_DATA->dm90_Reverb_Mode = GET_EFFECT_DATA->dm90_Reverb_Mode;
    GET_DEVICE_DATA->dm90_Reverb_Enable = GET_EFFECT_DATA->dm90_Reverb_Enable;
    GET_DEVICE_DATA->dm90_Reverb_PreDelay = GET_EFFECT_DATA->dm90_Reverb_PreDelay;
    GET_DEVICE_DATA->dm90_Reverb_DryLevel = GET_EFFECT_DATA->dm90_Reverb_DryLevel;
    GET_DEVICE_DATA->dm90_Reverb_WetLevel = GET_EFFECT_DATA->dm90_Reverb_WetLevel;
    GET_DEVICE_DATA->dm90_Reverb_Spread = GET_EFFECT_DATA->dm90_Reverb_Spread;
    GET_DEVICE_DATA->dm90_Reverb_RoomSize = GET_EFFECT_DATA->dm90_Reverb_RoomSize;
    GET_DEVICE_DATA->dm90_Reverb_Time = GET_EFFECT_DATA->dm90_Reverb_Time;
    GET_DEVICE_DATA->dm90_Reverb_Damping = GET_EFFECT_DATA->dm90_Reverb_Damping;
    GET_DEVICE_DATA->dm90_Reverb_EarlyLevel = GET_EFFECT_DATA->dm90_Reverb_EarlyLevel;
    GET_DEVICE_DATA->dm90_Reverb_TailLevel = GET_EFFECT_DATA->dm90_Reverb_TailLevel;
    GET_DEVICE_DATA->dm90_Reverb_LpfEn = GET_EFFECT_DATA->dm90_Reverb_LpfEn;
    GET_DEVICE_DATA->dm90_Reverb_LpfBw = GET_EFFECT_DATA->dm90_Reverb_LpfBw;
    GET_DEVICE_DATA->dm90_Reverb_HpfEn = GET_EFFECT_DATA->dm90_Reverb_HpfEn;
    GET_DEVICE_DATA->dm90_Reverb_HpfBw = GET_EFFECT_DATA->dm90_Reverb_HpfBw;

    //回声(Echo) 延迟的一种算法
    GET_DEVICE_DATA->dm90_Echo_Mode = GET_EFFECT_DATA->dm90_Echo_Mode;
    GET_DEVICE_DATA->dm90_Echo_Enable = GET_EFFECT_DATA->dm90_Echo_Enable;
    GET_DEVICE_DATA->dm90_Echo_Level = GET_EFFECT_DATA->dm90_Echo_Level;
    GET_DEVICE_DATA->dm90_Echo_Feedback = GET_EFFECT_DATA->dm90_Echo_Feedback;
    GET_DEVICE_DATA->dm90_Echo_Delay = GET_EFFECT_DATA->dm90_Echo_Delay;
    GET_DEVICE_DATA->dm90_Echo_MaxDelay = GET_EFFECT_DATA->dm90_Echo_MaxDelay;
}
