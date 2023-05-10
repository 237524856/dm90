
#pragma once
#pragma execution_character_set("utf-8")
#include "dfu_0206_view.h"
#include <QStandardPaths>
#include <QApplication>
#include <QDebug>
#include <QMessageBox>

dfu_0206_view::dfu_0206_view(QWidget *parent) : QWidget(parent)
{

}
void dfu_0206_view::startNewExe(QString file_name){

    QString path = qApp->applicationFilePath();
    //qDebug()<<"xxx"<<path;
    QStringList tempStringList =  path.split("/");
    tempStringList.removeLast();
    QString fileName = tempStringList.join("/");
    QString fileName1 = tempStringList.join("/");
    fileName += "/ktTool.exe";
    //QString program = QStringLiteral(fileName);
    myProcess = new QProcess(this);
    QStringList arguments;
    //arguments<<QString("ktTool");
    qDebug()<<QString::number(m_pid,16);
    arguments.append("0x352F");
    arguments.append(QString::number(m_pid,16));
    arguments.append("-s");
    arguments.append("-b");
    //arguments.append(fileName1 +"\\DM30_KT0206_v4.9.bin");
    arguments.append(file_name);

    myProcess->start(fileName,arguments);

    if(!myProcess->waitForStarted())
    {
        qDebug()<<"failure!";
    }else
    {
        qDebug()<<"succ!";}
    //myProcess->waitForFinished();
    connect(myProcess,
            SIGNAL(readyReadStandardOutput()),
            this,
            SLOT(onReadData()));
    connect(myProcess, SIGNAL(finished(int)), this, SLOT(readResult(int)));
}
//发送数据完成
void dfu_0206_view::readResult(int exitCode){
    // QByteArray kk =myProcess->readAll();
    QByteArray qByteRead = myProcess->readAllStandardOutput();

    qDebug()<<"ggggggggxddddxx"<<exitCode<<qByteRead<<this->m_total_lengh;
    if(m_total_lengh<=10){
      emit dsp_finish_progress(-1);
    }else{
       emit dsp_finish_progress(1);
    }
    //QString  tmpStr = QString::fromLocal8Bit(qByteRead);
}
//C:\Users\yu.yang.MAONO\Desktop\shengji\MBS400_22.3.17B4.bin
void dfu_0206_view::onReadData(){

    QByteArray qByteRead = myProcess->readAllStandardOutput();
    QString  tmpStr = QString::fromLocal8Bit(qByteRead);
    QStringList tem_list = tmpStr.split("\r");
    if(tem_list.count() > 3){
        QString str = tem_list.at(tem_list.count()-3);
        QStringList tem_list1 = str.split("/");
        if(tem_list1.count()>=2){
            QString str1 = tem_list1.at(0);
            str1 = str1.mid(1,str1.count()-1);

            QString str2 = tem_list1.last();
            str2 = str2.mid(0,str2.count()-2);
            this->m_total_lengh = str2.toUInt();
            emit dsp_progress_value(str1.toInt());
            qDebug()<<"gogogogoogo"<<str2.toInt()<<str1.toInt();
        }
    }else{
         //emit dsp_finish_progress(99);
    }
    QString  tmpSt1r = QString::fromLocal8Bit(qByteRead);
        qDebug()<<"gogogogoogo"<<tmpSt1r;
}
