
#pragma once
#pragma execution_character_set("utf-8")
#include "custom_listview.h"
#include "../dm90_default_datas.hpp"
#include <QDebug>

custom_listView::custom_listView(const QString &objectName,const int &w,const int &h,QWidget *parent) : QWidget(parent)
{

    this->resize(w,h);

    m_listWidget_default = new QListWidget(this);
    m_listWidget_default->resize(this->width()-40*m_Magnification,this->height());
    m_objectName = objectName;

    //获取本地的选中状态
    QString filename = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+"/listrow.dat";
    QVector<QStringList> getSaveDatas = m_commClass.getDM90CustomPreDatas(filename);

    if(objectName == "highViewDefaultPreset"){

        for (int n = 0;n<m_high_default_data.count();n ++) {
            QString user_default_str = m_high_default_data.at(n);
            QStringList user_default_list = user_default_str.split(",");
            user_default_list.removeAt(0);
            m_listWidget_default->insertItem(user_default_list.count(),user_default_list.at(user_default_list.count()-1));            
        }
        QStringList list = getSaveDatas.at(0);
        QString str = list.join("");
        if(str.toInt()<100){
            m_listWidget_default->setCurrentRow(str.toInt());
            m_select_number = m_defaultHighCurrentItemIndex = str.toInt();
        }

    }else if(objectName == "localSoundViewDefaultPreset"){

        for (int n = 0;n<m_localSound_default_data.count();n ++) {
            QString user_default_str = m_localSound_default_data.at(n);
            QStringList user_default_list = user_default_str.split(",");
            m_listWidget_default->insertItem(user_default_list.count(),user_default_list.at(user_default_list.count()-1));            
        }
        QStringList list = getSaveDatas.at(1);
        QString str = list.join("");
        if(str.toInt()<100){
            m_listWidget_default->setCurrentRow(str.toInt());
            m_select_number = str.toInt();
        }

    }else if(objectName == "effectViewDefaultPreset"){
        for (int n = 0;n<m_effectSound_default_data.count();n ++) {
            QString user_default_str = m_effectSound_default_data.at(n);
            QStringList user_default_list = user_default_str.split(",");
            m_listWidget_default->insertItem(user_default_list.count(),user_default_list.at(user_default_list.count()-1));
        }
        QStringList list = getSaveDatas.at(2);
        QString str = list.join("");
        if(str.toInt()<100){
            m_listWidget_default->setCurrentRow(str.toInt());
            m_select_number = str.toInt();
        }

    }
    m_listWidget_default->setFocusPolicy(Qt::NoFocus);
    QString listWidget_style = QString("QListWidget{background-color:rgb(11,11,11);border:0px solid red; color:rgb(27,27,27);font-size:%1px;}"

                                       "QListWidget::Item{color:rgb(175, 175, 175);height:%2px;}"

                                       "QListWidget::item:selected{background-color:rgb(45,45,45);border:none;}"

                                       "QScrollBar:vertical{width:8px;background-color:rgb(45,45,45);border-radius:4px;margin-top:20px;margin-bottom:20px;}"

                                       "QScrollBar::handle:vertical{background-color:rgb(245,179,53);width:8px;border-radius:4px;}"

                                       "QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical{background:rgba(45,45,45,100%);border-radius:4px;}"

                                       "QScrollBar::sub-line:vertical{height:0px;width:0px;subcontrol-position:top;}").arg(12*m_Magnification).arg(34*m_Magnification);
    m_listWidget_default->setStyleSheet(listWidget_style);
    m_listWidget_default->verticalScrollBar()->setContextMenuPolicy(Qt::NoContextMenu);
    m_listWidget_default->horizontalScrollBar()->setContextMenuPolicy(Qt::NoContextMenu);

    connect(m_listWidget_default,SIGNAL(currentRowChanged(int)),this,SLOT(user_default_click(int)));
    connect(m_listWidget_default,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(default_click_item(QListWidgetItem *)));
    connect(m_listWidget_default,SIGNAL(itemActivated(QListWidgetItem*)),this,SLOT(default_itemActivated(QListWidgetItem *)));
    connect(m_listWidget_default,SIGNAL(itemEntered(QListWidgetItem*)),this,SLOT(default_itemEntered(QListWidgetItem *)));
}
//当前的row改变了
void custom_listView::user_default_click(int number){

    m_select_number = number;
    int ViewIndex = 0;
    if(m_objectName == "highViewDefaultPreset"){
        ViewIndex=0;
        m_defaultHighCurrentItemIndex = number;
    }else if(m_objectName == "localSoundViewDefaultPreset"){
        ViewIndex=1;
    }else if(m_objectName == "effectViewDefaultPreset"){
        ViewIndex=2;
    }
    QString filename = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+"/listrow.dat";
    QVector<QStringList> getSaveDatas = m_commClass.getDM90CustomPreDatas(filename);
    QStringList changeList(QString::number(number,10));
    getSaveDatas.replace(ViewIndex,changeList);
    //保存数据
    m_commClass.saveDM90CustomPreDatas(filename,getSaveDatas);
}
void custom_listView::default_click_item(QListWidgetItem *item){

    if(m_objectName == "localSoundViewDefaultPreset"){
        emit locallistViewClick(this->objectName(),item->text(),m_select_number);
    }else{
        emit listViewClick(this->objectName(),item->text(),m_select_number);
        m_item = item;
    }
}

void custom_listView::default_itemActivated(QListWidgetItem *){


}

void custom_listView::default_itemEntered(QListWidgetItem *){


    //qDebug()<<"eeeenter====";
}
void custom_listView::clearItemSelect(){

    //    if(m_objectName == "highViewDefaultPreset"){
    if(m_item){
        m_item->setSelected(false);
    }
    //清空所有的
    for (int j = 0; j < m_listWidget_default->count(); j++)
    {
        QListWidgetItem *item= m_listWidget_default->item(j);
        item->setSelected(false);
    }

    //}

}
