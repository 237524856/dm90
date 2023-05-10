#pragma once
#pragma execution_character_set("utf-8")

#include "pd400_check_updateview.h"
#include <QLabel>
#include <QPushButton>
#include <QDebug>
#include <QApplication>
#include <QFile>
#include <QFileDialog>
#include <QStandardPaths>
#include "../common.h"
#include "HID/utils/CRC.h"
#include <QThread>
#include <QMessageBox>
#include "../networking_class.h"

pd400_check_updateView::pd400_check_updateView(QWidget *parent) : QWidget(parent)
{

    connect(networking_class::getInstance(),SIGNAL(network_finish_download()),this,SLOT(finish_downloadfile()));
}
void pd400_check_updateView::createUI(){

    //    QLabel *back_bg_image = new QLabel(this);
    //    back_bg_image->move(0,0);
    //    back_bg_image->resize(this->width(),this->height());
    //    back_bg_image->setPixmap(QPixmap(":/image/images/pd400_setting_bgimg.png"));


    QLabel *connect_bg_image = new QLabel(this);
    connect_bg_image->move(this->width()/2 - 250*m_Magnification/2,(this->height()-180*m_Magnification)/2);
    connect_bg_image->resize(250*m_Magnification,180*m_Magnification);
    connect_bg_image->setPixmap(QPixmap(":/image/images/pd400_update_check.png"));
    connect_bg_image->setScaledContents(true);

    m_title_label = new QLabel(connect_bg_image);
    m_title_label->move(0,30*m_Magnification);
    m_title_label->resize(250*m_Magnification,40*m_Magnification);
    m_title_label->setAlignment(Qt::AlignCenter);
    QString title_label_stys=QString("background-color:rgb(0,0,0);color:white;font-size:%1px;font-family:'Source Han Sans CN Medium'").arg(15*m_Magnification);
    m_title_label->setStyleSheet(title_label_stys);
    m_title_label->setWordWrap(true);
    QString dangqian_str= tr("当前版本");
    QString yijingshi_str= tr("已是最新版本");

    m_title_label->setText(dangqian_str+yijingshi_str);
    //m_title_label->setText(dangqian_str+version_DFU_str+yijingshi_str);

    m_title_massage = new QLabel(connect_bg_image);
    m_title_massage->move(0,60*m_Magnification);
    m_title_massage->resize(250*m_Magnification,40*m_Magnification);
    m_title_massage->setAlignment(Qt::AlignCenter);
    m_title_massage->setStyleSheet(title_label_stys);
    m_title_massage->setWordWrap(true);
    m_title_massage->setAlignment(Qt::AlignCenter);
    m_title_massage->setText(version_DFU_str+tr("版本可用于该设备，可以安装"));

    m_cancel_btn = new custom_button(connect_bg_image);
    m_cancel_btn->resize(76*m_Magnification,26*m_Magnification);
    m_cancel_btn->move(20*m_Magnification ,120*m_Magnification);
    m_cancel_btn->setText(tr("取消"));
    QString cancel_btn_stys=QString("QPushButton{border-image:url(:/image/images/pd400_xianfu_reset.png);font-family:'Source Han Sans CN Medium';color:white;font-size:%1px;}"
                                    "QPushButton:hover{border-image:url(:/image/images/pd400_xianfu_reset.png);font-family:'Source Han Sans CN Medium';color:rgb(54,54,54);font-size:%1px;}"
                                    "QPushButton:focus{outline: none;}").arg(14*m_Magnification);
    m_cancel_btn->setStyleSheet(cancel_btn_stys);
    m_cancel_btn->connect(m_cancel_btn,SIGNAL(clicked()),this,SLOT(cancel_click()));

    m_confirm_update_btn = new custom_button(connect_bg_image);
    m_confirm_update_btn->resize(90*m_Magnification,26*m_Magnification);
    m_confirm_update_btn->move((250-20-90)*m_Magnification,120*m_Magnification);
    m_confirm_update_btn->setText(tr("更新"));
    QString confirm_update_btn_stys=QString("QPushButton{border-image:url(:/image/images/pd400_xianfu_confirm.png);font-family:'Source Han Sans CN Medium';color:white;font-size:%1px;}"
                                            "QPushButton:hover{border-image:url(:/image/images/pd400_xianfu_confirm.png);font-family:'Source Han Sans CN Medium';color:rgb(54,54,54);font-size:%1px;}"
                                            "QPushButton:focus{outline: none;}").arg(14*m_Magnification);
    m_confirm_update_btn->setStyleSheet(confirm_update_btn_stys);
    connect(m_confirm_update_btn,SIGNAL(clicked()),this,SLOT(confirm_update_click()));
}

void pd400_check_updateView::set_diff_status(int status){

    m_current_status = status;

    if(status ==1){//固件可以更新
        refreshUI();

    }else{
        this->show();
        QString v_str = version_str;
        QString dangqian_str= tr("当前版本");
        QString yijingshi_str= tr("已是最新版本");
        m_title_label->setText(dangqian_str+yijingshi_str);
        //        m_title_label->setText(tr("当前版本(V1.0)已是最新版本"));
        m_cancel_btn->hide();
        m_title_massage->hide();
        m_confirm_update_btn->move((250-76)*m_Magnification/2,100*m_Magnification);
        m_confirm_update_btn->setObjectName("NOupdata");
        m_confirm_update_btn->setText(tr("确定"));
    }
};

void pd400_check_updateView::refreshUI(){

    //    if(!read_bin_file()) {
    //        this->hide();
    //        return;
    //    };
    this->show();

    m_title_label->setText(tr("固件更新"));
    m_cancel_btn->show();
    m_title_massage->show();
    m_confirm_update_btn->move((250-20-76)*m_Magnification,120*m_Magnification);
    m_confirm_update_btn->setObjectName("updata");
    m_title_massage->setText(version_DFU_str+tr("版本可用于该设备，可以安装"));
    m_cancel_btn->setText(tr("取消"));
    m_confirm_update_btn->setText(tr("更新"));
}

void pd400_check_updateView::cancel_click(){

    this->hide();
}
void pd400_check_updateView::download_bin_file(QUrl url,QString file){

    this->hide();
    networking_class::getInstance()->network_download(url,file);//网络请求
}
void pd400_check_updateView::confirm_update_click(){

    this->hide();
    QPushButton *btn = dynamic_cast<QPushButton *>(sender());
    if(btn->objectName()=="updata"){
        //开始固件升级
        emit updata_DFU();
    }else{

    }
    //if(m_current_status==1){//需要更新
    //if(m_confirm_update_btn->objectName()=="updata")
    //networking_class::getInstance()->network_download(QUrl("http://159.75.118.131/dnfile_test.aspx?f_sn=13"));//网络请求
    //connect(networking_class::getInstance(),SIGNAL(network_finish_download()),this,SLOT(finish_downloadfile()));
    //}
}
//下载完成的函数

void pd400_check_updateView::finish_downloadfile(){

    if(m_model_codel == DM30){
        m_dm30downloaddfu_number +=1;

        if(m_dm30downloaddfu_number <=1){//下载完MCU之后下载DSP
            //判断dsp的版本号
            QString ver_str = networking_class::getInstance()->network_updateversion("http://159.75.118.131/upinfo_test.aspx?proj_name=DM30_dsp");
            QStringList ver_arrs = ver_str.split(".");
            if(ver_arrs.count()>=3){
                int net_dsp_version = ver_arrs.at(0).toInt()*10000+ver_arrs.at(1).toInt()*1000+ver_arrs.at(2).toInt();
                int dsp_version = dsp_version_x*10000+dsp_version_y*1000+dsp_version_z;
                if(net_dsp_version > dsp_version){//下载升级
                    m_dm30file_dsp = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+"/DM30_dsp"+ver_str+".bin";
                    //this->set_bindfu_file(file_dsp);
                    this->download_bin_file(QUrl("http://159.75.118.131/dnfile_test.aspx?f_sn=15"),m_dm30file_dsp);
                    is_needto_updatadsp = true;
                    is_dm30updata_runing =true;
                    qDebug() <<"22222xxxxxxxxx"<<"下载完成";
                }else{//下载完MCU版本，DSP版本不需要下载
                    read_bin_file();
                }
                //qDebug()<<"xx"<<net_dsp_version<<dsp_version;
            }
        }else{//两种情况
            qDebug()<<"number---xx"<<m_dm30downloaddfu_number;
            if(m_dm30downloaddfu_number>=100){//MCU是最新版本不需要下载,下载DSP版本
                emit finish_download_dspfile();
            }else{//mcu ,dsp同时下载完成
               read_bin_file();
            }
        }
    }else if(m_model_codel == PD400){
        m_dm30downloaddfu_number +=1;
        if(m_dm30downloaddfu_number <=1){//下载完MCU之后下载DSP
            //判断dsp的版本号
            QString ver_str = networking_class::getInstance()->network_updateversion("http://159.75.118.131/upinfo_test.aspx?proj_name=PD400_dsp");
            QStringList ver_arrs = ver_str.split(".");
            if(ver_arrs.count()>=3){
                int net_dsp_version = ver_arrs.at(0).toInt()*10000+ver_arrs.at(1).toInt()*1000+ver_arrs.at(2).toInt();
                int dsp_version = dsp_version_x*10000+dsp_version_y*1000+dsp_version_z;
                if(net_dsp_version > dsp_version){//下载升级
                    m_PD400file_dsp = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+"/PD400_dsp"+ver_str+".bin";
                    //this->set_bindfu_file(file_dsp);
                    this->download_bin_file(QUrl("http://159.75.118.131/dnfile_test.aspx?f_sn=18"),m_PD400file_dsp);
                    is_pd400needto_updatadsp = true;
                    is_pd400updata_runing =true;
                    qDebug() <<"22222xxxxxxxxx"<<"下载完成";
                }else{//下载完MCU版本，DSP版本不需要下载
                    read_bin_file();
                }
                //qDebug()<<"xx"<<net_dsp_version<<dsp_version;
            }
        }else{//两种情况
            qDebug()<<"number---xx"<<m_dm30downloaddfu_number;
            if(m_dm30downloaddfu_number>=100){//MCU是最新版本不需要下载,下载DSP版本
                emit finish_download_dspfile();
            }else{//mcu ,dsp同时下载完成
               read_bin_file();
            }
        }
    }

}
void pd400_check_updateView::set_bindfu_file(QString path){

    m_dfubin_file = path;
}
bool pd400_check_updateView::read_bin_file(){

    m_dataSourceList.clear();
    //读取本地bin文件的方法
    //    QString path = qApp->applicationFilePath();
    //    qDebug()<<"xxx"<<path;
    //    QStringList tempStringList =  path.split("/");
    //    tempStringList.removeLast();
    //    QString fileName = tempStringList.join("/");
    //    fileName += "/PD400_DFU v0.0.6.bin";

    //读取网络下载的数据
    //QString fileName = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+"/pd400.bin";
    QString fileName = m_dfubin_file;
    //    QString AppDataLocation = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    //    QString fileName = QFileDialog::getOpenFileName(
    //        this,
    //        "选择升级文件",
    //        AppDataLocation,
    //        "Document files (*.bin)");

    if (fileName.isEmpty())//如果未选择文件便确认，即返回
        return false;
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly)){
        QMessageBox::information(NULL, "Title", "打开错误+"+file.errorString(),
                                 QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        return false;
        qDebug()<<"打开错误"<<file.errorString();
    }

    //文件打开错误显示错误信息

    QByteArray total_arry =file.readAll();//读取文件
    if(total_arry.count()<=0)return false;


    QByteArray datasource = total_arry.mid(11, total_arry.count());
    QByteArray sendmess;

    uint16_t my_value_front =0;
    uint8_t len_L = total_arry.at(2) & 0xff;
    uint8_t len_H = total_arry.at(1) & 0xff;
    uint16_t my_value = ((len_L <<8) | len_H) &0xffff;

    m_dataSourceList << total_arry.mid(0, my_value);

    int test_num = 0;
    //qDebug()<<my_value_front<<"kkkllllkxxxxxx";
    while (my_value !=  total_arry.size()) {

        total_arry = total_arry.mid(my_value, total_arry.size()-my_value);

        len_L = total_arry.at(2) & 0xff;
        len_H = total_arry.at(1) & 0xff;
        //qDebug()<<my_value<<"kkkk"<<total_arry.at(2)<<total_arry.at(1);
        my_value = ((len_L <<8) | len_H) &0xffff;
        sendmess = datasource.mid(my_value_front, my_value);
        my_value_front += my_value;
        m_dataSourceList << sendmess;
        test_num ++;
        //qDebug()<<my_value<<"kkkk"<<my_value_front<<test_num<<total_arry.size();
    }
    //qDebug()<<m_dataSourceList.at(m_dataSourceList.count()-1)<<"kkkkxxxxxx";
    file.close();

    //QThread::msleep(1000);
    //获取版本号
    version_V1 = m_dataSourceList.at(1).at(79);
    version_V2 = m_dataSourceList.at(1).at(78);
    version_V3 = m_dataSourceList.at(1).at(77);

    version_DFU_str =QString("V")+QString::number(version_V3,10) +"."+QString::number(version_V2,10)+"."+QString::number(version_V1,10);
    //判断版本号决定是否需要更新
    int device_version = device_version_x*100+device_version_y*10+device_version_z;
    int dfu_version = version_V3*100+version_V2*10+version_V1;

    set_diff_status(1);
    //    if(dfu_version > device_version){//需要升级
    //        set_diff_status(1);
    //    }else{//不需要升级

    //        set_diff_status(2);
    //    }
    qDebug()<<version_V1<<version_V2<<version_V3<<version_DFU_str;
    //开始发送数据
    //emit updata_DFU();

    return true;
}
