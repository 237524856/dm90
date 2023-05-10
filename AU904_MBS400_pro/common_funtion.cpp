#include "common_funtion.h"
#include <QStandardPaths>
#include <QFile>
#include <QFileDialog>
#include <QApplication>
#include <QDebug>
#include <QString>
common_funtion::common_funtion(QWidget *parent) : QWidget(parent)
{

}
//是否是第一次打开
bool common_funtion::is_frist_open(){

     QString AppDataLocation = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
     //QString AppDataLocation = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
     QDir *dir=new QDir;
        if(dir->exists(AppDataLocation))
        {
            //dir->rmdir(AppDataLocation);

        }else{
            dir->mkdir(AppDataLocation);
        }

     delete dir;
     qDebug()<<"file_ssss";

     QString filename = AppDataLocation+"/ma.dat";
     QFile file(filename);

     if(file.open(QIODevice::ReadOnly)) //只读模式,如果成功证明有此文件
     {
         return  true;

     }else{

         if(file.open(QIODevice::WriteOnly)){
             QDataStream out(&file);
             out << "m_slider_map";
             file.close();
             //qDebug()<<"xxxxfile_ssss";
         }
         QString filename_datas = AppDataLocation+"/yangyu.dat";
         QFile file_datas(filename_datas);
         if(file_datas.open(QIODevice::WriteOnly)){
             QDataStream out(&file);
             out << "";
             file.close();
         }

          return false;
     }
}
//保存当前语言的状态
bool common_funtion::set_save_language_status(int current_language){

    QString AppDataLocation = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    //QString AppDataLocation = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    QDir *dir=new QDir;
       if(dir->exists(AppDataLocation))
       {
           //dir->rmdir(AppDataLocation);

       }else{
           dir->mkdir(AppDataLocation);
       }

    delete dir;
    qDebug()<<"file_ssss";

    QString filename = AppDataLocation+"/language.dat";
    QFile file(filename);
    if(file.open(QIODevice::ReadWrite)) //只读模式,如果成功证明有此文件
    {
        QDataStream out(&file);
        out << QString::number(current_language,10);
        file.close();
        return  true;
    }else{
       return  false;
    }
}

//获取当前语言状态
int common_funtion::get_save_language_status(){
    QString filename = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+"/language.dat";
    QString AppDataLocation = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

    QString language_index="";
    QFile file(filename);
    if(file.open(QIODevice::ReadOnly)){
        QDataStream in(&file);

        in >>  language_index;
    }else{
        QDir *dir=new QDir;
           if(dir->exists(AppDataLocation))
           {
               //dir->rmdir(AppDataLocation);

           }else{
               dir->mkdir(AppDataLocation);
           }

        delete dir;
        return -1;
    }
    if(language_index.length()>0){

        return  language_index.toUInt();
    }else{
       return -1;
    }

}

QString common_funtion::add_image_forstatus(QString images,double image_width){
    QString imagesname =  "QPushButton{border-image:";
            imagesname.append(images);
            QString str1=QString("QPushButton:focus{outline: none;}QPushButton{border-image:%1 0 %2 0 0;}QPushButton:hover{border-image:%3 0 %4 0 %5;}QPushButton:pressed{border-image:%6 0 0 0 %7;}").arg(images).arg(image_width*2)
                    .arg(images).arg(image_width).arg(image_width).arg(images).arg(image_width*2);
    return str1;
}

QString common_funtion::add_checkBox_forstatus(QString images,int image_width){

    QString str1=QString("QCheckBox::indicator{width: %4px;height: %4px;color: rgb(255, 0, 0);}"
                         "QCheckBox:focus{outline: none;}"
                         "QCheckBox::indicator:unchecked{border-image:%1 0 %2 0 0;}"
                         "QCheckBox::indicator:unchecked:hover{border-image:%3 0 %4 0 %5;}"
                         "QCheckBox::indicator:checked{border-image:%6 0 0 0 %7;}").arg(images).arg(image_width*2)
            .arg(images).arg(image_width).arg(image_width).arg(images).arg(image_width*2);
    return str1;
}

//获取DM90自定义预设的音效数据=========
QVector<QStringList> common_funtion::getDM90CustomPreDatas(QString file_path){

    QVector<QStringList> my_slider_map;
    QFile file(file_path);
    //读之前先要取出来
    if(file.open(QIODevice::ReadOnly)){
        //只读模式
        QDataStream in(&file);
        QVector<QStringList>datasource;
        in >>  datasource;
        if(datasource.count()> 0){
              my_slider_map =  datasource;
        }
        file.close();
    }

    return my_slider_map;
}
//保存DM90自定义预设的音效数据=========
bool common_funtion::saveDM90CustomPreDatas(const QString &file_path,QVector<QStringList> saveDatas){

    QFile file(file_path);
    if(file.open(QIODevice::WriteOnly)) //只写模式
    {
        QDataStream out(&file);
        out << saveDatas;
        file.close();
        return true;
        qDebug()<<"写入成功"<<saveDatas;
    }else{

        return false;
    }


}
//获取DM90 列表的选中下标
bool common_funtion::getDM90ListRowSelectStatus(){

    QString AppDataLocation = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QString filename = AppDataLocation+"/listrow.dat";
    QFile file(filename);
    if(file.open(QIODevice::ReadOnly)) //只读模式,如果成功证明有此文件
    {
        file.close();
        return  true;

    }else{//保存数据

        if(file.open(QIODevice::WriteOnly)) //只写模式
        {
            QDataStream out(&file);
            QStringList list1("100");
            QStringList list2("0");
            QStringList list3("0");
            QVector<QStringList> listRows = {list1,list2,list3};
            out << listRows;
            file.close();
            qDebug()<<"写入成功--"<<listRows;
            return true;

        }else{

            return false;
        }


         return false;
    }
}
