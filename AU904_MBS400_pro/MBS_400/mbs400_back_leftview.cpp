#pragma once
#pragma execution_character_set("utf-8")
#include "mbs400_back_leftview.h"
#include <QFontDatabase>
#include <QDebug>
#include <QPushButton>
#include <qtreeview.h>
#include <QStandardItemModel>
#include <QApplication>
#include <QMessageBox>
#include <QStandardPaths>
#include "../common.h"
mbs400_back_leftview::mbs400_back_leftview(QWidget *parent) : QWidget(parent)
{

    m_delete_datasource_index = -1;

}
void mbs400_back_leftview:: new_createUI(void){

    QLabel *back_left_image = new QLabel(this);
    back_left_image->resize(this->width(),this->height());
    back_left_image->move(0,0);
    back_left_image->setPixmap(QPixmap(":/image/images/preset_back_image.png"));

    QLabel *back_image = new QLabel(this);
    back_image->resize(160,544);
    back_image->move(5,43);
    back_image->setPixmap(QPixmap(":/image/images/preset_image.png"));

    //创建下拉框
    QLabel *perset_image = new QLabel(this);
    perset_image->resize(152,28);
    perset_image->move(8,5);
    perset_image->setPixmap(QPixmap(":/image/images/mbs400_perset_back_image.png"));
    QFont font;
    font.setFamily("Source Han Sans CN Medium");
    font.setPixelSize(18);
    QPalette pa;
    QColor niceBlue(175, 175, 175);
    pa.setColor(QPalette::WindowText,niceBlue);
    QLabel *perset_name = new QLabel(this);
    perset_name->resize(40,28);
    perset_name->move(18,5);
    font.setPixelSize(16);
    perset_name->setText("预设");
    perset_name->setPalette(pa);
    perset_name->setFont(font);
    m_arrow_btn= new QCheckBox(this);
    m_arrow_btn->resize(22,12);
    m_arrow_btn->move(117 , 12);
    m_arrow_btn->setStyleSheet(  "QCheckBox::indicator{width:22px;height: 12px;color: rgb(255, 0, 0);}"
                                 "QCheckBox::indicator:unchecked{border-image:url(:/image/images/mbs400_perset_arrow_down.png);}"
                                 "QCheckBox::indicator:checked{border-image:url(:/image/images/mbs400_perset_arrow_up.png);}");
    connect(m_arrow_btn,SIGNAL(clicked()),this,SLOT(perset_btn_click()));
    m_listWidget_default = new QListWidget(this);
    m_listWidget_default->resize(135,510);
    m_listWidget_default->move(18 ,43);
    QString modele_name = "Rock 摇滚,CUSTOMIZE 63,CUSTOMIZE 53,Volume 43,EQ 33,Nostalgically 怀旧,Rock 摇滚,CUSTOMIZE 63,CUSTOMIZE 53,Volume 43,EQ 33,Nostalgically 怀旧,Rock 摇滚,Rock 摇滚,CUSTOMIZE 63,CUSTOMIZE 53,"
                          "Volume 43,EQ 33,Nostalgically 怀旧,Rock 摇滚,CUSTOMIZE 63,CUSTOMIZE 53,Volume 43,EQ 33,Nostalgically 怀旧,Rock 摇滚,Rock 摇滚,CUSTOMIZE 63,CUSTOMIZE 53,Volume 43,EQ 33,Nostalgically 怀旧,Rock 摇滚,CUSTOMIZE 63,CUSTOMIZE 53,Volume 43,EQ 33,Nostalgically 怀旧,Rock 摇滚,Rock 摇滚,CUSTOMIZE 63,CUSTOMIZE 53,"
                          "Volume 43,EQ 33,Nostalgically 怀旧,Rock 摇滚,CUSTOMIZE 63,CUSTOMIZE 53,Volume 43,EQ 33,Nostalgically 怀旧,Rock 摇滚";

    m_model_list = modele_name.split(",");
    QString konge_str = " ";
    for (int n = 0;n<m_model_list.count();n ++) {
        m_listWidget_default->insertItem(0,m_model_list.at(n));
    }
    m_listWidget_default->setStyleSheet(

                "QListWidget{background-color:rgb(11,11,11);border:0px solid red; color:rgb(27,27,27);font-size:13px}"

                "QListWidget::Item{color:rgb(175, 175, 175);height:46px;}"

                "QScrollBar:vertical{width:8px;background-color:rgb(45,45,45);border-radius:4px;margin-top:20px;margin-bottom:20px;}"

                "QScrollBar::handle:vertical{background-color:rgb(245,179,53);width:8px;border-radius:4px;}"

                "QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical{background:rgba(45,45,45,100%);border-radius:4px;}"

                "QScrollBar::sub-line:vertical{height:0px;width:0px;subcontrol-position:top;}"
                );

    //创建最底下的按钮
    QLabel *perset_bottom_label = new QLabel(this);
    perset_bottom_label->resize(165,26);
    perset_bottom_label->move(0,560);
    //bottom_label->setStyleSheet("background-color:red");
    //预设底部的按钮
    QVector<QString> presetbuttons = { "QPushButton{border-image:url(:/image/images/msb400_perset_edi.png) 0 40 0 0;}"
                                       "QPushButton:hover{border-image:url(:/image/images/msb400_perset_edi.png) 0 20 0 20;}"
                                       "QPushButton:pressed{border-image:url(:/image/images/msb400_perset_edi.png) 0 0 0 40;}",

                                       "QPushButton{border-image:url(:/image/images/msb400_perset_delete.png) 0 40 0 0;}"
                                       "QPushButton:hover{border-image:url(:/image/images/msb400_perset_delete.png) 0 20 0 20;}"
                                       "QPushButton:pressed{border-image:url(:/image/images/msb400_perset_delete.png) 0 0 0 40;}",

                                       "QPushButton{border-image:url(:/image/images/msb400_perset_share.png) 0 40 0 0;}"
                                       "QPushButton:hover{border-image:url(:/image/images/msb400_perset_share.png) 0 20 0 20;}"
                                       "QPushButton:pressed{border-image:url(:/image/images/msb400_perset_share.png) 0 0 0 40;}",

                                       "QPushButton{border-image:url(:/image/images/msb400_perset_load.png) 0 40 0 0;}"
                                       "QPushButton:hover{border-image:url(:/image/images/msb400_perset_load.png) 0 20 0 20;}"
                                       "QPushButton:pressed{border-image:url(:/image/images/msb400_perset_load.png) 0 0 0 40;}",
                                     };

    for (int i=0;i<presetbuttons.count();i++) {
        QPushButton *preset_btn = new QPushButton(perset_bottom_label);
        preset_btn->resize(20,20);
        preset_btn->move(16 +37*i ,3);
        preset_btn->setStyleSheet(presetbuttons.at(i));
        preset_btn->setObjectName(QString::number(i+10,10));
        connect(preset_btn,SIGNAL(clicked()),this,SLOT(preset_btn_click()));
    }
    m_perset_bottom_label=perset_bottom_label;
    m_perset_bottom_label->setVisible(false);

    m_usercustom_label = new QLabel(this);
    m_usercustom_label->resize(152,28);
    m_usercustom_label->move(8,553);
    m_usercustom_label->setPixmap(QPixmap(":/image/images/mbs400_perset_back_image.png"));
    QLabel *user_setting_name = new QLabel(m_usercustom_label);
    user_setting_name->resize(60,28);
    user_setting_name->move(10,0);
    font.setPixelSize(15);
    user_setting_name->setText("用户预设");
    user_setting_name->setPalette(pa);
    user_setting_name->setFont(font);

    m_user_arrow_btn = new QCheckBox(m_usercustom_label);
    m_user_arrow_btn->resize(22,12);
    m_user_arrow_btn->move(108 , 7);
    m_user_arrow_btn->setCheckState(Qt::Checked);
    m_user_arrow_btn->setStyleSheet(    "QCheckBox::indicator{width:22px;height: 12px;color: rgb(255, 0, 0);}"
                                        "QCheckBox::indicator:unchecked{border-image:url(:/image/images/mbs400_perset_arrow_down.png);}"
                                        "QCheckBox::indicator:checked{border-image:url(:/image/images/mbs400_perset_arrow_up.png);}");
    connect(m_user_arrow_btn,SIGNAL(clicked()),this,SLOT(user_btn_click()));
    //用户自定义的音效，首先获取内存文件的音效
    QString filename = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+"/yangyu.dat";
    QFile file(filename);
    //读之前先要取出来
    if(file.open(QIODevice::ReadOnly)){
        //只读模式
        QDataStream in(&file);
        QVector<QStringList>datasource;
        in >>  datasource;
        if(datasource.count()> 0){
            m_slider_map =  datasource;
        }
        file.close();
    }

    m_listWidget_custom = new QListWidget(this);
    m_listWidget_custom->resize(135,483);
    m_listWidget_custom->move(18 ,70);
    //获取到内存的数据放到用户自定义的列表
    if(m_slider_map.count()>0){
        QVector<QStringList>::iterator datasource_it;
        for ( datasource_it = m_slider_map.begin(); datasource_it != m_slider_map.end(); ++datasource_it )
        {
            QString str_title = datasource_it->at(datasource_it->count()-1);
            m_listWidget_custom->insertItem(0,str_title);
        }
    }
    m_listWidget_custom->setFocusPolicy(Qt::NoFocus);
    m_listWidget_custom->setStyleSheet(

                "QListWidget{background-color:rgb(11,11,11);border:0px solid red; color:rgb(27,27,27);font-size:13px}"

                "QListWidget::Item{color:rgb(175, 175, 175);height:46px;}"

                "QListWidget::item:selected{background-color:rgb(45,45,45);border:none;}"

                "QScrollBar:vertical{width:8px;background-color:rgb(45,45,45);border-radius:4px;margin-top:20px;margin-bottom:20px;}"

                "QScrollBar::handle:vertical{background-color:rgb(245,179,53);width:8px;border-radius:4px;}"

                "QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical{background:rgba(45,45,45,100%);border-radius:4px;}"

                "QScrollBar::sub-line:vertical{height:0px;width:0px;subcontrol-position:top;}"
                );
    m_listWidget_custom->setVisible(false);
    connect(m_listWidget_custom,SIGNAL(currentRowChanged(int)),this,SLOT(user_custom_click(int)));
}

void mbs400_back_leftview::perset_btn_click(){

    QCheckBox* btn = dynamic_cast<QCheckBox*>(sender());
    qDebug()<<btn->isChecked();
    m_perset_bottom_label->setVisible(false);
    if(btn->isChecked()){
        m_listWidget_default->setVisible(false);
        m_usercustom_label->move(8,38);
        m_listWidget_custom->setVisible(false);

    }else{
        m_listWidget_default->setVisible(true);
        m_usercustom_label->move(8,553);
        m_listWidget_custom->setVisible(false);
        m_user_arrow_btn->setCheckState(Qt::Checked);
    }
}
void mbs400_back_leftview::user_btn_click(){
    QCheckBox* btn = dynamic_cast<QCheckBox*>(sender());
    if(btn->isChecked()){
        m_listWidget_custom->setVisible(false);
        m_perset_bottom_label->setVisible(false);

    }else{
        m_listWidget_custom->setVisible(true);
        m_perset_bottom_label->setVisible(true);
        m_listWidget_default->setVisible(false);
        m_usercustom_label->move(8,38);
        m_arrow_btn->setCheckState(Qt::Checked);
    }
}

void mbs400_back_leftview::refresh_user_list(){
    //清空列表的所有元素
    m_listWidget_custom->clear();
    if(m_slider_map.count()>0){
        QVector<QStringList>::iterator datasource_it;
        for ( datasource_it = m_slider_map.begin(); datasource_it != m_slider_map.end(); ++datasource_it )
        {
            QString str_title = datasource_it->at(datasource_it->count()-1);
            m_listWidget_custom->insertItem(0,str_title);
        }
    }
}



void mbs400_back_leftview::preset_btn_click(void){
    QPushButton* btn = dynamic_cast<QPushButton*>(sender());
    //qDebug()<<btn->objectName();

    if(m_delete_datasource_index == -1 && (btn->objectName()=="10" || btn->objectName()=="11" || btn->objectName()=="12")){
        MsgBox *msgBox=new MsgBox(2,QString("请输入卡口名或路口名"));//1为警告框
        msgBox->askLabel->setText("提示\n\n您还没选中音效");
        int nRes=msgBox->exec();
        delete msgBox;
        return;
    }

    if(btn->objectName()=="10"){//编辑
        if (m_slider_map.count() <=0) return;
        MsgBox *msgBox=new MsgBox(1,QString("请输入卡口名或路口名"));//1为警告框
        int nRes=msgBox->exec();
        if(nRes==QDialog::Accepted)//更改名称
        {
            if(msgBox->m_lenedit->text().length() <=0){
                return;
            }
            QStringList item =  m_slider_map.at(m_slider_map.count()-m_delete_datasource_index-1);
            QString item_title = item.at(item.count()-1);
            //判断数组中有没相同得标题
            QString save_str = msgBox->m_lenedit->text();
            for(int i = 0;i<m_slider_map.count();i++){
                QStringList item_base =  m_slider_map.at(i);
                for (int k = 0;k<item_base.count();k++) {
                    QString item_title_base = item_base.at(k);
                    if(msgBox->m_lenedit->text()==item_title_base){//如果有一样的则不保存

                        MsgBox *msgBox=new MsgBox(4,QString("请输入卡口名或路口名"));//1为警告框
                        msgBox->exec();
                        delete msgBox;
                        return;
                    }
                }
            }
            item.replace(item.count()-1,save_str);
            m_slider_map.replace(m_slider_map.count()-m_delete_datasource_index-1,item);
            //放到缓存区
            QString filename = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+"/yangyu.dat";
            QFile file(filename);
            //refresh_user_list();
            if(file.open(QIODevice::WriteOnly)) //只写模式
            {
                QDataStream out(&file);
                out << m_slider_map;
                file.close();
                m_delete_datasource_index =-1;
                refresh_user_list();
            }
        }

    }else if(btn->objectName()=="11"){//删除
        MsgBox *msgBox=new MsgBox(2,QString("请输入卡口名或路口名"));//1为警告框
        int nRes=msgBox->exec();
        if(nRes==QDialog::Accepted)//删除文件
        {
            m_slider_map.remove(m_slider_map.count()-m_delete_datasource_index-1);
            //删除之后写入文件
            qDebug()<<"删除成功"<<m_delete_datasource_index<<m_slider_map;//6
            QString filename = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+"/yangyu.dat";
            QFile file(filename);
            refresh_user_list();
            if(file.open(QIODevice::WriteOnly)) //只写模式
            {
                QDataStream out(&file);
                out << m_slider_map;
                file.close();
                //refresh_user_list();
            }
            m_delete_datasource_index = -1;
        }


    }else if(btn->objectName()=="12"){//分享
        qDebug() << "分享";
        QString location_desktop = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
        //QString location_documents = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
        QStringList mydatasource = m_slider_map.at(m_slider_map.count() - m_delete_datasource_index - 1);
        QString save_title = mydatasource.at(mydatasource.count()-1);
        QString file_save_name = QFileDialog::getSaveFileName(this, "分享音效", location_desktop+"/"+save_title);//获取需要保存成的文件名
        if (file_save_name.isEmpty())
        {
            return;
        }
        else {

            QFile *file = new QFile;
            file->setFileName(file_save_name+ ".dat");
            bool ok = file->open(QIODevice::WriteOnly);

            QStringList strlist = file_save_name.split("/");
            QString mysave_name = strlist.at(strlist.count()-1);
            if (ok)
            {
                QDataStream out(file);

                qDebug() << "xxxx" << mydatasource;
                mydatasource.replace(mydatasource.count()-1,mysave_name.mid(0,mysave_name.count()));
                out << mydatasource;
                file->close();
                delete(file);
            }
            else {

            }

        }
    }else{//下载

        QString AppDataLocation = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
        QString filename = QFileDialog::getOpenFileName(
            this,
            "下载音效",
            AppDataLocation,
            "Document files (*.dat)");
        //判断下是否选中文件
        if (!filename.isEmpty()) {
            QFile file(filename);
            if (file.open(QIODevice::ReadOnly)) {
                QDataStream in(&file);
                QStringList datasource;
                in >> datasource;
                //获取路径的名字并经行转换
                QStringList strlist = filename.split("/");
                QString mysave_name = strlist.at(strlist.count()-1);
                QString title = mysave_name.mid(0,mysave_name.length()-4);
                datasource.replace(datasource.count()-1,title);

                QString select_file_name = datasource.at(datasource.count() - 1);

                //                qDebug()<<"读取"<<datasource.at(datasource.count()-1);
                file.close();
                for (int i = 0; i < m_slider_map.count(); i++) {
                    QStringList item_base = m_slider_map.at(i);
                    //qDebug() << "读取xx" << item_base.at(item_base.count() - 1);
                    if (select_file_name == item_base.at(item_base.count() - 1)) {
                        MsgBox *msgBox = new MsgBox(2, QString("请输入卡口名或路口名"));//1为警告框
                        msgBox->askLabel->setText("提示\n\n您已有相同的音效");
                        int nRes = msgBox->exec();
                        delete msgBox;
                        return;
                    }
                }
                //添加数据，并且保存它
                m_slider_map.push_back(datasource);
                refresh_user_list();
                m_massge_control->save_for_file();

            }
        }}
}
//点击了行
void mbs400_back_leftview::user_custom_click(int row){
    qDebug() << "获取的行号：" << row ;
    if(row == -1)return;
    m_delete_datasource_index = row;
    emit click_userlist_row(m_delete_datasource_index);
}
