
#pragma once
#pragma execution_character_set("utf-8")
#include "custom_itemlistview.h"
#include "../dm90_default_datas.hpp"
#include <QDebug>
#include <QtAlgorithms>
#include "../dm90_midview.h"

custom_itemListView::custom_itemListView(const QString &objectName,const int &w,const int &h,QWidget *parent) : QWidget(parent)
{

    this->resize(w,h);

    m_objectName = objectName;

    m_parent = parent;

    if(m_objectName == "highViewCustomPreset"){//高级页面

    }else if(m_objectName == "localSoundViewCustomPreset"){



    }else if(m_objectName == "effectViewCustomtPreset"){


    }

    m_listWidget_default = new QListWidget(this);
    m_listWidget_default->resize(this->width()-40*m_Magnification,this->height());

    m_listWidget_default->setFocusPolicy(Qt::NoFocus);
    QString listWidget_style = QString("QListWidget{background-color:rgb(11,11,11);border:0px solid red; color:rgb(27,27,27);font-size:%1px;}"

                                       "QScrollBar:vertical{width:8px;background-color:rgb(45,45,45);border-radius:4px;margin-top:20px;margin-bottom:20px;}"

                                       "QScrollBar::handle:vertical{background-color:rgb(245,179,53);width:8px;border-radius:4px;}"

                                       "QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical{background:rgba(45,45,45,100%);border-radius:4px;}"

                                       "QScrollBar::sub-line:vertical{height:0px;width:0px;subcontrol-position:top;}").arg(12*m_Magnification).arg(34*m_Magnification);
    m_listWidget_default->setStyleSheet(listWidget_style);

    m_listWidget_default->setMouseTracking(true);
    m_listWidget_default->verticalScrollBar()->setContextMenuPolicy(Qt::NoContextMenu);
    m_listWidget_default->horizontalScrollBar()->setContextMenuPolicy(Qt::NoContextMenu);
    connect(m_listWidget_default,SIGNAL(itemEntered(QListWidgetItem*)),this,SLOT(default_itemEntered(QListWidgetItem *)));
}


void custom_itemListView::refreshUI(){    

    m_itemCounts =0;
    m_items.clear();
    m_itemObjectNames.clear();
    //m_listWidget_default->clear();
    while (m_listWidget_default->count() > 0) {
        QListWidgetItem *item = m_listWidget_default->takeItem(0);
        delete item;
    }

    //获取本地的选中状态
    QString filename = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+"/listrow.dat";
    QVector<QStringList> getlistRowSaveDatas = m_commClass.getDM90CustomPreDatas(filename);

    int viewIndex = 0;
    if(m_objectName == "highViewCustomPreset"){//高级页面
        //先从本地把数组取出来，显示
        filename = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+"/dm90HighView.dat";
        viewIndex = 0;

    }else if(m_objectName == "localSoundViewCustomPreset"){
        //先从本地把数组取出来，显示
        filename = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+"/localSoundView.bin";
        //选中了那个按钮
        viewIndex = 1;

    }else if(m_objectName == "effectViewCustomtPreset"){

        //先从本地把数组取出来，显示
        filename = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+"/effectSoundView.bin";
        viewIndex = 2;
    }
    //获取本地数据
    QVector<QStringList> my_slider_map = m_commClass.getDM90CustomPreDatas(filename);
    if(my_slider_map.count()<=0){
        QStringList list = getlistRowSaveDatas.at(viewIndex);
        QString str = list.join("");
        if(str.toInt()<100){

            if(m_objectName == "localSoundViewCustomPreset"){
                dm90_midview *midView =(dm90_midview *) (this->parentWidget()->parentWidget()->parentWidget());
                midView->m_defaultTtemRow = str.toInt();
                midView->m_itemtext = m_localSound_default_data.at(str.toInt()).split(",").last();
            }else if(m_objectName == "effectViewCustomtPreset"){
                dm90_midview *midView =(dm90_midview *) (this->parentWidget()->parentWidget()->parentWidget());
                midView->m_defaultTtemRow = str.toInt();
                midView->m_itemtext = m_effectSound_default_data.at(str.toInt()).split(",").last();
            }
        }
        return;
    }
    for (int i =0;i<my_slider_map.count();i++) {
        custom_item *Citem = new custom_item();
        Citem->setObjectName(QString::number(i,10));
        m_itemObjectNames.push_back(Citem->objectName());
        Citem->m_item_isSelect = false;
        m_items.push_back(Citem);

        if(m_objectName == "highViewCustomPreset"){
            //安装QLineEdit事件过滤
            m_parent->installEventFilter(Citem->m_item_title);
            m_listWidget_default->installEventFilter(Citem->m_item_title);
            Citem->installEventFilter(Citem->m_item_title);
        }else{

            //安装QLineEdit事件过滤
            m_parent->parentWidget()->parentWidget()->parentWidget()->installEventFilter(Citem->m_item_title);
            m_listWidget_default->installEventFilter(Citem->m_item_title);
            Citem->installEventFilter(Citem->m_item_title);
        }
        connect(Citem,SIGNAL(item_click(QString)),this,SLOT(listItemClick(QString)));
        connect(Citem,SIGNAL(item_enter(QString)),this,SLOT(listItemEnter(QString)));
        connect(Citem,SIGNAL(item_btnClick(QString,QString)),this,SLOT(item_btnClick_row(QString,QString)));
        connect(Citem->m_item_title,SIGNAL(lineViewBlockText(QString)),this,SLOT(reviseItemText(QString)));

        //获取本地数据的标题
        Citem->setTitleName(my_slider_map.at(i).last().simplified());
        QListWidgetItem* item = new QListWidgetItem;
        item->setSizeHint(QSize(m_listWidget_default->width()-50, 34*m_Magnification));

        m_listWidget_default->addItem(item);
        m_listWidget_default->setItemWidget(item,Citem);
    }
    m_itemCounts =my_slider_map.count();
    //选中了那个按钮
    QStringList list = getlistRowSaveDatas.at(viewIndex);
    QString str = list.join("");
    if(str.toInt()>=100){//选中自定义的
        m_items.at(str.toInt()-100)->setClickSelect(true);
        m_items.at(str.toInt()-100)->m_item_isSelect = true;
        if(m_objectName == "highViewCustomPreset"){

            highView_customTempPreData = my_slider_map.at(str.toInt()-100);
        }

    }else{

        QString defaultListTitle;//默认列表的标题
        if(m_objectName == "highViewCustomPreset"){

        }else if(m_objectName == "localSoundViewCustomPreset"){
            dm90_midview *midView =(dm90_midview *) (this->parentWidget()->parentWidget()->parentWidget());
            midView->m_defaultTtemRow = str.toInt();
            midView->m_itemtext = m_localSound_default_data.at(str.toInt()).split(",").last();
        }else if(m_objectName == "effectViewCustomtPreset"){
            dm90_midview *midView =(dm90_midview *) (this->parentWidget()->parentWidget()->parentWidget());
            midView->m_defaultTtemRow = str.toInt();
            midView->m_itemtext = m_effectSound_default_data.at(str.toInt()).split(",").last();
        }
    }
}
//选中了当前的ITEM
void custom_itemListView::listItemClick(QString itemName){

    for (int i=0;i<m_items.count();i++) {
        m_items.at(i)->setStyleSheet("background:transparent;border-radius: 0px;");
        m_items.at(i)->m_item_isSelect = false;
    }
    //获取本地的数据
    QString filename = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+"/listrow.dat";
    QVector<QStringList> getlistRowSaveDatasK = m_commClass.getDM90CustomPreDatas(filename);

    int viewIndex =0;
    if(m_objectName == "highViewCustomPreset"){//高级页面

        viewIndex =0;

    }else if(m_objectName == "localSoundViewCustomPreset"){

        viewIndex =1;

    }else if(m_objectName == "effectViewCustomtPreset"){

        viewIndex =2;
    }
    QStringList list = getlistRowSaveDatasK.at(viewIndex);
    QString select_str = list.join("");

    if(select_str.toInt()>=100){
        QString filename;
        if(m_objectName == "highViewCustomPreset"){
            filename = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+"/dm90HighView.dat";
            QVector<QStringList> getSaveDatas = m_commClass.getDM90CustomPreDatas(filename);
            getSaveDatas.replace(select_str.toInt()-100,highView_customTempPreData);
            m_commClass.saveDM90CustomPreDatas(filename,getSaveDatas);//保存
        }else if(m_objectName == "localSoundViewCustomPreset"){
            filename = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+"/localSoundView.bin";
            QVector<QStringList> getSaveDatas = m_commClass.getDM90CustomPreDatas(filename);
            QStringList localList = localstructChangeforList();
            //添加最后的标题
            localList.push_back(getSaveDatas.at(select_str.toInt()-100).last());
            getSaveDatas.replace(select_str.toInt()-100,localList);//替换
            m_commClass.saveDM90CustomPreDatas(filename,getSaveDatas);//保存

        }else if(m_objectName == "effectViewCustomtPreset"){
            filename = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+"/effectSoundView.bin";
            QVector<QStringList> getSaveDatas = m_commClass.getDM90CustomPreDatas(filename);
            QStringList effectList = effectstructChangeforList();
            //添加最后的标题
            effectList.push_back(getSaveDatas.at(select_str.toInt()-100).last());
            getSaveDatas.replace(select_str.toInt()-100,effectList);//替换
            m_commClass.saveDM90CustomPreDatas(filename,getSaveDatas);//保存
        }
    }

    m_click_row = m_enter_row = m_itemObjectNames.indexOf(itemName);

    filename = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+"/listrow.dat";
    QVector<QStringList> getSaveDatas = m_commClass.getDM90CustomPreDatas(filename);
    QStringList changeList(QString::number(m_enter_row+100,10));
    getSaveDatas.replace(viewIndex,changeList);
    //保存数据
    m_commClass.saveDM90CustomPreDatas(filename,getSaveDatas);


    //发送信号
    emit listViewClick(m_objectName,itemName,m_enter_row);
    clearAllItemEditStatus();

}
void custom_itemListView::listItemEnter(QString itemName){

    m_enter_row = m_itemObjectNames.indexOf(itemName);
}

void custom_itemListView::default_itemEntered(QListWidgetItem *item){

    //qDebug()<<"vvvvvvvvvvvvvvvvvvvvv"<<m_listWidget_default->row(item);
}

//增加新的ITEM-----------
void custom_itemListView::addItem(int defaultList_row,QString title,int change_index,int change_value){

    for (int i=0;i<m_items.count();i++) {//如果有选中状态的，不增加ITEM

        qDebug()<<"aaaaa"<<m_items.count()<<m_items.at(i)->m_item_title->text()<<m_items.at(i)->m_item_isSelect;
        if(m_items.at(i)->m_item_isSelect){
            return;
        }
    }
    //清除状态
    clearSelectStatus();

    dm90_midview *midView =(dm90_midview *) (this->parentWidget()->parentWidget()->parentWidget());

    m_itemCounts ++;
    custom_item *Citem = new custom_item();
    Citem->setObjectName(QString::number(m_itemCounts,10));
    m_itemObjectNames.push_back(Citem->objectName());
    Citem->setClickSelect(true);
    Citem->m_item_isSelect =true;

    m_items.push_back(Citem);

    if(m_objectName == "highViewCustomPreset"){
        //安装QLineEdit事件过滤
        m_parent->installEventFilter(Citem->m_item_title);
        m_listWidget_default->installEventFilter(Citem->m_item_title);
        Citem->installEventFilter(Citem->m_item_title);
    }else{

        //安装QLineEdit事件过滤
        m_parent->parentWidget()->parentWidget()->parentWidget()->installEventFilter(Citem->m_item_title);
        m_listWidget_default->installEventFilter(Citem->m_item_title);
        Citem->installEventFilter(Citem->m_item_title);
    }
    connect(Citem,SIGNAL(item_click(QString)),this,SLOT(listItemClick(QString)));
    connect(Citem,SIGNAL(item_enter(QString)),this,SLOT(listItemEnter(QString)));
    connect(Citem,SIGNAL(item_btnClick(QString,QString)),this,SLOT(item_btnClick_row(QString,QString)));
    connect(Citem->m_item_title,SIGNAL(lineViewBlockText(QString)),this,SLOT(reviseItemText(QString)));

    //修改标题
    QString itemText = title;
    int viewIndex = 0;
    if(m_objectName == "highViewCustomPreset"){//高级页面

        //保存对应的数据,保存之前需要把原来的取出来
        QString filename = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+"/dm90HighView.dat";
        QVector<QStringList> getSaveDatas = m_commClass.getDM90CustomPreDatas(filename);
        itemText = titleAddNumber(getSaveDatas,title);

        //获取默认的特效,并保存之
        QStringList list = m_high_default_data.at(defaultList_row).split(",");
        list.replace(change_index,QString::number(change_value,10));
        list.last() = itemText;
        getSaveDatas.push_back(list);
        highView_customTempPreData = list;
        m_commClass.saveDM90CustomPreDatas(filename,getSaveDatas);//保存
        viewIndex = 0;

    }else if(m_objectName == "localSoundViewCustomPreset"){
        //保存对应的数据,保存之前需要把原来的取出来
        QString filename = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+"/localSoundView.bin";
        QVector<QStringList> getSaveDatas = m_commClass.getDM90CustomPreDatas(filename);
        itemText = titleAddNumber(getSaveDatas,title);
        //获取默认的特效,并保存之
        QString user_soundStr = m_localSound_default_data.at(defaultList_row);
        QStringList user_default_list = user_soundStr.split(",");
        //替换数据
        user_default_list.replace(change_index,QString::number(change_value,10));
        user_default_list.last() = itemText;
        getSaveDatas.push_back(user_default_list);
        m_commClass.saveDM90CustomPreDatas(filename,getSaveDatas);
        viewIndex = 1;
        midView->m_default_locaListView->clearItemSelect();
    }else if(m_objectName == "effectViewCustomtPreset"){

        //保存对应的数据,保存之前需要把原来的取出来
        QString filename = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+"/effectSoundView.bin";
        QVector<QStringList> getSaveDatas = m_commClass.getDM90CustomPreDatas(filename);
        itemText = titleAddNumber(getSaveDatas,title);
        //获取默认的特效,并保存之
        QString user_soundStr = m_effectSound_default_data.at(defaultList_row);
        QStringList user_default_list = user_soundStr.split(",");
        //替换数据
        user_default_list.replace(change_index,QString::number(change_value,10));
        user_default_list.last() = itemText;
        getSaveDatas.push_back(user_default_list);
        m_commClass.saveDM90CustomPreDatas(filename,getSaveDatas);
        viewIndex = 2;
        midView->m_default_effectListView->clearItemSelect();
    }
    //获取默认的特效的下标,并保存之
    QString filename = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+"/listrow.dat";
    QVector<QStringList> getlistRowSaveDatas = m_commClass.getDM90CustomPreDatas(filename);
    QStringList changeList(QString::number(m_items.count()+99,10));
    getlistRowSaveDatas.replace(viewIndex,changeList);
    m_commClass.saveDM90CustomPreDatas(filename,getlistRowSaveDatas);
    m_click_row = m_items.count()-1;

    Citem->setTitleName(itemText.simplified());
    QListWidgetItem* item = new QListWidgetItem;
    item->setSizeHint(QSize(this->width()-100, 34*m_Magnification));
    m_listWidget_default->addItem(item);
    m_listWidget_default->setItemWidget(item,Citem);
}

//下载特效---------------------------------
void custom_itemListView::downloadItem(QStringList listData){

    QString filename;
    if(m_objectName == "highViewCustomPreset"){
        filename = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+"/dm90HighView.dat";
    }else if(m_objectName == "localSoundViewCustomPreset"){
        filename = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+"/localSoundView.bin";
    }else if(m_objectName == "effectViewCustomtPreset"){
        filename = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+"/effectSoundView.bin";
    }
    QVector<QStringList> getSaveDatas = m_commClass.getDM90CustomPreDatas(filename);
    getSaveDatas.push_back(listData);
    m_commClass.saveDM90CustomPreDatas(filename,getSaveDatas);

    //刷新界面
    refreshUI();
}

bool custom_itemListView::subtractItem(){

    if(m_items.count()<=0)return false;
    // m_listWidget_default->removeItemWidget()
    QString filename;
    if(m_objectName == "highViewCustomPreset"){
        filename = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+"/dm90HighView.dat";
    }else if(m_objectName == "localSoundViewCustomPreset"){
        filename = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+"/localSoundView.bin";
    }else if(m_objectName == "effectViewCustomtPreset"){
        filename = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+"/effectSoundView.bin";
    }
    QVector<QStringList> getSaveDatas = m_commClass.getDM90CustomPreDatas(filename);
    QStringList list = getSaveDatas.at(m_click_row);
    QString item_text = list.last();


    QMessageBox msg;
    msg.setWindowTitle(tr("提示"));
    msg.setText(tr("确认要删除特效")+item_text.simplified());
    QPushButton *yesButton = msg.addButton(tr("是"),QMessageBox::ActionRole);

    int a =10;

    connect(yesButton, &QPushButton::clicked, [&]() {
        a=11;
        m_listWidget_default->takeItem(m_click_row);
        m_items.removeAt(m_click_row);
        m_itemObjectNames.removeAt(m_click_row);
        //同时要删除本地缓存的数据，保存对应的数据,保存之前需要把原来的取出来
        getSaveDatas.removeAt(m_click_row);
        bool result = m_commClass.saveDM90CustomPreDatas(filename,getSaveDatas);
        m_click_row = 0;
        qDebug()<<"rrrrrrrrrr"<<result<<a;
        return result;
    });
    QPushButton *noButton = msg.addButton(tr("否"),QMessageBox::ActionRole);
    msg.addButton(QMessageBox::No);
    msg.button(QMessageBox::No)->setHidden(true);
    msg.setDefaultButton(yesButton);
    msg.setDefaultButton(noButton);
    msg.exec();

    return true;
}

//修改ITEM的标题
void custom_itemListView::reviseItemText(QString text){

    if(m_items.count()<=0)return;

    QString filename;
    if(m_objectName == "highViewCustomPreset"){
        filename = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+"/dm90HighView.dat";
    }else if(m_objectName == "localSoundViewCustomPreset"){
        filename = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+"/localSoundView.bin";
    }else if(m_objectName == "effectViewCustomtPreset"){
        filename = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+"/effectSoundView.bin";
    }
    QVector<QStringList> getSaveDatas = m_commClass.getDM90CustomPreDatas(filename);
    QStringList list = getSaveDatas.at(m_item_editRor);
    list.replace(list.count()-1,text);
    getSaveDatas.replace(m_item_editRor,list);

    for (auto list :getSaveDatas) {
        //        qDebug()<<"cccccccc"<<m_item_editRor<<list.last();
    }
    m_commClass.saveDM90CustomPreDatas(filename,getSaveDatas);

}

void custom_itemListView::item_btnClick_row(QString itemText,QString btnObjectName){

    m_item_editRor = m_enter_row;

    if(btnObjectName =="share"){//分享
        QString location_desktop = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);

        QString filename;
        QString title;
        if(m_objectName == "highViewCustomPreset"){
            filename = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+"/dm90HighView.dat";
        }else if(m_objectName == "localSoundViewCustomPreset"){
            filename = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+"/localSoundView.bin";
        }else if(m_objectName == "effectViewCustomtPreset"){
            filename = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+"/effectSoundView.bin";
        }
        QVector<QStringList> getSaveDatas = m_commClass.getDM90CustomPreDatas(filename);
        //添加最后的标题
        title = getSaveDatas.at(m_item_editRor).last();

        QString file_save_name = QFileDialog::getSaveFileName(this, "分享音效", location_desktop+"/"+title);//获取需要保存成的文件名
        if (!file_save_name.isEmpty()){

            QFile *file = new QFile;
            file->setFileName(file_save_name+ ".bin");
            bool ok = file->open(QIODevice::WriteOnly);
            QStringList strlist = file_save_name.split("/");
            QString mysave_name = strlist.at(strlist.count()-1);
            if (ok)
            {
                QDataStream out(file);
                qDebug() << "xxxx" << getSaveDatas.at(m_item_editRor);
                //mydatasource.replace(mydatasource.count()-1,mysave_name.mid(0,mysave_name.count()));
                out << getSaveDatas.at(m_item_editRor);
                file->close();
                delete(file);
            }
        }
    }else if(btnObjectName=="edit"){

    }else if(btnObjectName=="save"){//保存数据===========

        //if(m_click_row != m_item_editRor)return;

        QString filename;
        QString title;
        if(m_objectName == "highViewCustomPreset"){
            filename = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+"/dm90HighView.dat";
            QVector<QStringList> getSaveDatas = m_commClass.getDM90CustomPreDatas(filename);
            //添加最后的标题
            title = getSaveDatas.at(m_item_editRor).last();

        }else if(m_objectName == "localSoundViewCustomPreset"){
            filename = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+"/localSoundView.bin";
            QVector<QStringList> getSaveDatas = m_commClass.getDM90CustomPreDatas(filename);
            //添加最后的标题
            title = getSaveDatas.at(m_item_editRor).last();

        }else if(m_objectName == "effectViewCustomtPreset"){
            filename = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+"/effectSoundView.bin";
            QVector<QStringList> getSaveDatas = m_commClass.getDM90CustomPreDatas(filename);
            //添加最后的标题
            title = getSaveDatas.at(m_item_editRor).last();
        }

        int btnStatus = QMessageBox::information(nullptr,tr("提示"),tr("确认保存")+title+tr("音效吗?"),tr("是"),tr("否"));
        if(0 == btnStatus){//是

            QString filename;
            if(m_objectName == "highViewCustomPreset"){
                filename = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+"/dm90HighView.dat";
                QVector<QStringList> getSaveDatas = m_commClass.getDM90CustomPreDatas(filename);
                getSaveDatas.replace(m_item_editRor,highView_customTempPreData);
                m_commClass.saveDM90CustomPreDatas(filename,getSaveDatas);//保存

            }else if(m_objectName == "localSoundViewCustomPreset"){
                filename = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+"/localSoundView.bin";
                QVector<QStringList> getSaveDatas = m_commClass.getDM90CustomPreDatas(filename);
                QStringList localList = localstructChangeforList();
                //添加最后的标题
                localList.push_back(getSaveDatas.at(m_item_editRor).last());
                getSaveDatas.replace(m_item_editRor,localList);//替换
                m_commClass.saveDM90CustomPreDatas(filename,getSaveDatas);//保存

            }else if(m_objectName == "effectViewCustomtPreset"){
                filename = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+"/effectSoundView.bin";
                QVector<QStringList> getSaveDatas = m_commClass.getDM90CustomPreDatas(filename);
                QStringList effectList = effectstructChangeforList();
                //添加最后的标题
                effectList.push_back(getSaveDatas.at(m_item_editRor).last());
                getSaveDatas.replace(m_item_editRor,effectList);//替换
                m_commClass.saveDM90CustomPreDatas(filename,getSaveDatas);//保存
            }

        }else{//否

        }

    }

}

void custom_itemListView::shareItem(){

}

void custom_itemListView::clearAllItemEditStatus(){

    for (auto item : m_items) {
        item->m_item_title->clearFocus();
    }
}
void custom_itemListView::clearSelectStatus(){

    for (int i=0;i<m_items.count();i++) {
        m_items.at(i)->setStyleSheet("background:transparent;border-radius: 0px;");
        m_items.at(i)->m_item_isSelect = false;
    }
}

QStringList custom_itemListView::localstructChangeforList()
{
    QStringList localList;
    //给列表赋值
    //门限
    localList.push_back(QString::number(GET_DEVICE_DATA->dm90_threshold_Enable,10));
    localList.push_back(QString::number(GET_DEVICE_DATA->dm90_threshold_value,10));
    localList.push_back(QString::number(GET_DEVICE_DATA->dm90_threshold_Attack,10));
    localList.push_back(QString::number(GET_DEVICE_DATA->dm90_threshold_Release,10));
    localList.push_back(QString::number(GET_DEVICE_DATA->dm90_threshold_Hold,10));
    localList.push_back(QString::number(GET_DEVICE_DATA->dm90_HPF_Enable,10));
    localList.push_back(QString::number(GET_DEVICE_DATA->dm90_HPF_Freq,10));

    for (int i=0;i<8;i++) {

        localList.push_back(QString::number(GET_DEVICE_DATA->dm90micEQdatasource[i].Enable,10));
        localList.push_back(QString::number(GET_DEVICE_DATA->dm90micEQdatasource[i].Type,10));
        localList.push_back(QString::number(GET_DEVICE_DATA->dm90micEQdatasource[i].Freq,10));
        localList.push_back(QString::number(GET_DEVICE_DATA->dm90micEQdatasource[i].Gain,10));
        localList.push_back(QString::number(GET_DEVICE_DATA->dm90micEQdatasource[i].Q,10));

    }
    //齿音消除器
    localList.push_back(QString::number(GET_DEVICE_DATA->dm90_Dental_Enable,10));
    localList.push_back(QString::number(GET_DEVICE_DATA->dm90_Dental_Threshold,10));
    localList.push_back(QString::number(GET_DEVICE_DATA->dm90_Dental_Freq,10));
    localList.push_back(QString::number(GET_DEVICE_DATA->dm90_Dental_Q,10));
    localList.push_back(QString::number(GET_DEVICE_DATA->dm90_Dental_Attack,10));
    localList.push_back(QString::number(GET_DEVICE_DATA->dm90_Dental_Release,10));
    localList.push_back(QString::number(GET_DEVICE_DATA->dm90_Dental_MinGain,10));
    localList.push_back(QString::number(GET_DEVICE_DATA->dm90_Dental_DetectPeriod,10));

    //去爆音器
    localList.push_back(QString::number(GET_DEVICE_DATA->dm90_Detonator_Enable,10));
    localList.push_back(QString::number(GET_DEVICE_DATA->dm90_Detonator_Threshold,10));
    localList.push_back(QString::number(GET_DEVICE_DATA->dm90_Detonator_Freq,10));
    localList.push_back(QString::number(GET_DEVICE_DATA->dm90_Detonator_Q,10));
    localList.push_back(QString::number(GET_DEVICE_DATA->dm90_Detonator_Attack,10));
    localList.push_back(QString::number(GET_DEVICE_DATA->dm90_Detonator_Release,10));
    localList.push_back(QString::number(GET_DEVICE_DATA->dm90_Detonator_MinGain,10));
    localList.push_back(QString::number(GET_DEVICE_DATA->dm90_Detonator_DetectPeriod,10));

    //语音变调
    localList.push_back(QString::number(GET_DEVICE_DATA->dm90_VoiceSandhi_Enable,10));
    localList.push_back(QString::number(GET_DEVICE_DATA->dm90_VoiceSandhi_Rate,10));

    //镶边器/移相器
    localList.push_back(QString::number(GET_DEVICE_DATA->dm90_flanger_Enable,10));
    localList.push_back(QString::number(GET_DEVICE_DATA->dm90_flanger_Level,10));
    localList.push_back(QString::number(GET_DEVICE_DATA->dm90_flanger_Delay,10));
    localList.push_back(QString::number(GET_DEVICE_DATA->dm90_flanger_FreqModu,10));
    localList.push_back(QString::number(GET_DEVICE_DATA->dm90_flanger_DeptModu,10));
    localList.push_back(QString::number(GET_DEVICE_DATA->dm90_flanger_FeedRate,10));

    //压缩器(Comp)
    localList.push_back(QString::number(GET_DEVICE_DATA->dm90_Comp_Enable,10));
    localList.push_back(QString::number(GET_DEVICE_DATA->dm90_Comp_Threshold,10));
    localList.push_back(QString::number(GET_DEVICE_DATA->dm90_Comp_Attack,10));
    localList.push_back(QString::number(GET_DEVICE_DATA->dm90_Comp_Release,10));
    localList.push_back(QString::number(GET_DEVICE_DATA->dm90_Comp_Rate,10));

    //限幅器(Limit)
    localList.push_back(QString::number(GET_DEVICE_DATA->dm90_Limit_Enable,10));
    localList.push_back(QString::number(GET_DEVICE_DATA->dm90_Limit_Threshold,10));
    localList.push_back(QString::number(GET_DEVICE_DATA->dm90_Limit_Attack,10));
    localList.push_back(QString::number(GET_DEVICE_DATA->dm90_Limit_Release,10));

    return localList;
};
QStringList custom_itemListView::effectstructChangeforList(){

    QStringList localList;

    //和声
    localList.push_back(QString::number(GET_DEVICE_DATA->dm90_harmony_Enable,10));
    localList.push_back(QString::number(GET_DEVICE_DATA->dm90_harmony_Level,10));
    localList.push_back(QString::number(GET_DEVICE_DATA->dm90_harmony_Delay,10));
    localList.push_back(QString::number(GET_DEVICE_DATA->dm90_harmony_FreqModu1,10));
    localList.push_back(QString::number(GET_DEVICE_DATA->dm90_harmony_DeptModu1,10));
    localList.push_back(QString::number(GET_DEVICE_DATA->dm90_harmony_FreqModu2,10));
    localList.push_back(QString::number(GET_DEVICE_DATA->dm90_harmony_DeptModu2,10));

    //混响(Reverb)
    localList.push_back(QString::number(GET_DEVICE_DATA->dm90_Reverb_Mode,10));
    localList.push_back(QString::number(GET_DEVICE_DATA->dm90_Reverb_Enable,10));
    localList.push_back(QString::number(GET_DEVICE_DATA->dm90_Reverb_PreDelay,10));
    localList.push_back(QString::number(GET_DEVICE_DATA->dm90_Reverb_DryLevel,10));
    localList.push_back(QString::number(GET_DEVICE_DATA->dm90_Reverb_WetLevel,10));
    localList.push_back(QString::number(GET_DEVICE_DATA->dm90_Reverb_Spread,10));
    localList.push_back(QString::number(GET_DEVICE_DATA->dm90_Reverb_RoomSize,10));
    localList.push_back(QString::number(GET_DEVICE_DATA->dm90_Reverb_Time,10));
    localList.push_back(QString::number(GET_DEVICE_DATA->dm90_Reverb_Damping,10));
    localList.push_back(QString::number(GET_DEVICE_DATA->dm90_Reverb_EarlyLevel,10));
    localList.push_back(QString::number(GET_DEVICE_DATA->dm90_Reverb_TailLevel,10));
    localList.push_back(QString::number(GET_DEVICE_DATA->dm90_Reverb_LpfEn,10));
    localList.push_back(QString::number(GET_DEVICE_DATA->dm90_Reverb_LpfBw,10));
    localList.push_back(QString::number(GET_DEVICE_DATA->dm90_Reverb_HpfEn,10));
    localList.push_back(QString::number(GET_DEVICE_DATA->dm90_Reverb_HpfBw,10));

    //回声(Echo) 延迟的一种算法
    localList.push_back(QString::number(GET_DEVICE_DATA->dm90_Echo_Mode,10));
    localList.push_back(QString::number(GET_DEVICE_DATA->dm90_Echo_Enable,10));
    localList.push_back(QString::number(GET_DEVICE_DATA->dm90_Echo_Level,10));
    localList.push_back(QString::number(GET_DEVICE_DATA->dm90_Echo_Feedback,10));
    localList.push_back(QString::number(GET_DEVICE_DATA->dm90_Echo_Delay,10));
    localList.push_back(QString::number(GET_DEVICE_DATA->dm90_Echo_MaxDelay,10));

    return localList;
}

QString custom_itemListView::titleAddNumber(QVector<QStringList>getSaveDatas,QString itemText){

    QVector<QString>savetitles;
    for (int i =0;i<getSaveDatas.count();i++) {
        QString  saveStr = getSaveDatas.at(i).last();
        if(saveStr.contains(itemText.simplified(),Qt::CaseSensitive)){
            savetitles.push_back(saveStr);
            qDebug() << "包含了";
        }else{
        }
    }
    QVector<int>savetitleNumber;
    if(savetitles.count()>0){
        for (int i=0;i<savetitles.count();i++) {
            QString currentTitle = savetitles.at(i);
            QString ch1 = currentTitle.mid(currentTitle.length()-1,1);
            if(isDigitStr(ch1)){//是数字
                QString ch2 = currentTitle.mid(currentTitle.length()-2,1);
                if(isDigitStr(ch2)){
                    savetitleNumber.push_back((ch2+ch1).toUInt());
                }else{
                    savetitleNumber.push_back(ch1.toUInt());
                }
            }
        }
    }else{
        itemText = itemText+"1";
    }
    if(savetitleNumber.count()>0){
        std::sort(savetitleNumber.begin(), savetitleNumber.end());
        int showNumber = savetitleNumber.last()+1;
        itemText = itemText+QString::number(showNumber,10);
    }
    return itemText;
}

bool custom_itemListView::isDigitStr(QString src){
    QByteArray ba =src.toLatin1();//QString 转换为 char*
    const char *s = ba.data();

    while(*s && *s>='0' && *s<='9') s++;

    if (*s)
    { //不是纯数字
        return false;
    }
    else
    { //纯数字
        return true;
    }
}


