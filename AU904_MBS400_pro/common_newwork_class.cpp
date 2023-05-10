
#pragma once
#pragma execution_character_set("utf-8")
#include "common_newwork_class.h"

#include <QUrl>
#include <QStandardPaths>
#include <QMessageBox>
#include <QApplication>
#include "common.h"

#if defined(Q_OS_WIN32)
#include <windows.h>
#else

#endif
#include <QCoreApplication>

common_newwork_class::common_newwork_class(QWidget *parent) : QWidget(parent)
{

    m_pHttpMgr = new QNetworkAccessManager();
}

void common_newwork_class::createUI(){

    QLabel *back_image_msk = new QLabel(this);
    back_image_msk->resize(this->width(),this->height());
    back_image_msk->move(0,0);
    back_image_msk->setPixmap(QPixmap(":/image/images/pd400_setting_bgimg.png"));
    back_image_msk->setScaledContents(true);

    QLabel *back_image = new QLabel(this);
    back_image->resize(320*m_Magnification,100*m_Magnification);
    back_image->move((this->width()-320*m_Magnification)/2,(this->height()-100*m_Magnification)/2);
    back_image->setPixmap(QPixmap(":/image/images/pd400_progress_bgimg.png"));
    back_image->setScaledContents(true);

    m_load_label = new QLabel(back_image);
    m_load_label->setGeometry(10*m_Magnification,15*m_Magnification,168*m_Magnification,30*m_Magnification);
    m_load_label->setText(tr("软件升级中..."));
    QString load_label_stys=QString("color:white;background-color:transparent;font-size:%1px;font-family:'Source Han Sans CN Medium'").arg(14*m_Magnification);
    m_load_label->setStyleSheet(load_label_stys);

    m_progress_view = new QProgressBar(back_image);
    m_progress_view->move(12*m_Magnification,45*m_Magnification);
    m_progress_view->resize((270-4)*m_Magnification,16*m_Magnification);
    m_progress_view->setMinimum(0);  // 最小值
    m_progress_view->setMaximum(100);  // 最大值
    m_progress_view->setValue(0);  // 当前进度
    QString progress_view_stys=QString("QProgressBar {border-radius: %1px;background-color:rgb(27,27,27);}"
                                       "QProgressBar::chunk{background-color:rgb(255,171,33);}").arg(8*m_Magnification);
    m_progress_view->setStyleSheet(progress_view_stys);
    m_progress_view->setTextVisible(false);

    QLabel *progress_image = new QLabel(back_image);
    progress_image->resize(270*m_Magnification,16*m_Magnification);
    progress_image->move(10*m_Magnification,45*m_Magnification);
    progress_image->setPixmap(QPixmap(":/image/images/pd400_progress_image.png"));
    progress_image->setScaledContents(true);

    m_current_value = new QLabel(back_image);
    m_current_value->setGeometry(285*m_Magnification,45*m_Magnification,30*m_Magnification,16*m_Magnification);
    m_current_value->setText(QString::number(m_progress_view->value(),10)+"%");
    QString current_value_stys=QString("color:white;background-color:transparent;font-size:%1px;font-family:'Source Han Sans CN Medium'").arg(13*m_Magnification);
    m_current_value->setStyleSheet(current_value_stys);
}
void common_newwork_class::network_updateversion(){

    //设置url
    QString url = "http://159.75.118.131/upinfo_test.aspx?proj_name=Maono Link";
    QNetworkRequest requestInfo;
    requestInfo.setUrl(QUrl(url));

    //添加事件循环机制，返回后再运行后面的
    QEventLoop eventLoop;
    QNetworkReply *reply =  m_pHttpMgr->get(requestInfo);
    connect(reply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
    eventLoop.exec();       //block until finish
    //错误处理
    if (reply->error() == QNetworkReply::NoError)
    {
        qDebug() << "request protobufHttp NoError";
    }
    else
    {
        qDebug()<<"request protobufHttp handle errors here";
        QVariant statusCodeV = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
        //statusCodeV是HTTP服务器的相应码，reply->error()是Qt定义的错误码，可以参考QT的文档
        qDebug( "request protobufHttp found error ....code: %d %d\n", statusCodeV.toInt(), (int)reply->error());
        qDebug(qPrintable(reply->errorString()));
    }
    //请求返回的结果
    QByteArray responseByte = reply->readAll();
    //qDebug() <<"xxxxxxxxx"<<responseByte;
    //判断字符串转化为QJsonDocument  是否出现了错误
    QJsonParseError jsonError;//Qt5新类
    QJsonDocument json = QJsonDocument::fromJson(responseByte, &jsonError);//Qt5新类

    if (jsonError.error == QJsonParseError::NoError)
    {
        QJsonArray rootObj = json.array();
        for (int i=0;i<rootObj.count();i++) {
            QJsonObject jo = rootObj.at(i).toObject();
            QString name_str = jo["f_name"].toString();
            if(name_str == "Maono Link.exe"){

                //判断2个版本号是不是一样的，如果是一样则不升级，不一样的升级
                qDebug() << "999999999"<<name_str<<jo["f_ver"].toString();
                QStringList app_ver_arrs = m_appversion_str.split(".");
                int app_version = app_ver_arrs.at(0).toInt()*1000+app_ver_arrs.at(1).toInt()*100+app_ver_arrs.at(2).toInt();

                QStringList net_app_ver_arrs = jo["f_ver"].toString().split(".");
                int net_app_version = net_app_ver_arrs.at(0).toInt()*1000+net_app_ver_arrs.at(1).toInt()*100+net_app_ver_arrs.at(2).toInt();

                if(net_app_version > app_version){//弹出升级框


                    int btnStatus = QMessageBox::information(nullptr,tr("提示"),tr("软件有新的版本，确定要更新吗?"),tr("是"),tr("否"));

                    if(0 == btnStatus)//点击了按钮1（按钮索引位置为0，后面的依次增加）
                    {
                        download_file_len = jo["f_len"].toInt();
                        m_progress_view->setMaximum(download_file_len);  // 最大值
                        network_download();
                        qDebug() << "选中了确定"<<download_file_len;
                        m_load_label->setText(tr("软件升级中..."));
                        this->show();
                    }
                    else if(1 == btnStatus)//点击了按钮2（按钮索引位置为1）
                    {
                        qDebug() << "选中了否";
                    }


                }else{
                    QString dangqian_str= tr("当前版本");
                    QString yijingshi_str= tr("已是最新版本");

                    QMessageBox msg;
                    msg.setWindowTitle(tr("提示"));
                    msg.setText( dangqian_str+m_appversion_str+yijingshi_str);
                    QPushButton *noButton = msg.addButton(tr("是"),QMessageBox::ActionRole);
                    msg.addButton(QMessageBox::No);
                    msg.button(QMessageBox::No)->setHidden(true);
                    msg.setDefaultButton(noButton);
                    msg.exec();
                }

            }
        }

    }
    //    const char *result = responseByte.data();
    //    QString str = responseByte;
    //return str;
}
void common_newwork_class::network_download(){

    m_filename = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+"/Maono Link_download.exe";
    m_file =new QFile(m_filename);
    if(!m_file->open(QIODevice::WriteOnly)){
        delete m_file;
        m_file=0;
        return;
    }
    startRequest(QUrl("http://159.75.118.131/dnfile_test.aspx?f_sn=21"));
}
void common_newwork_class::startRequest(QUrl url){
    QNetworkRequest requestInfo;
    requestInfo.setUrl(QUrl(url));
    m_reply =  m_pHttpMgr->get(requestInfo);
    connect(m_reply,&QNetworkReply::readyRead,this,&common_newwork_class::httpReadyRead);
    connect(m_reply,&QNetworkReply::downloadProgress,this,&common_newwork_class::updateDataReadProgress);
    connect(m_reply,&QNetworkReply::finished,this,&common_newwork_class::httpFinished);
}

void common_newwork_class::httpReadyRead(){

    if(m_file)
        m_file->write(m_reply->readAll());
    //qDebug() <<"111111xxxxxxxxx";
}
void common_newwork_class::updateDataReadProgress(qint64 bytesRead,qint64 totalBytes){

    //m_progress_view->setMaximum(93080064);
    m_progress_view->setValue(bytesRead);
    double progress_number = (double)bytesRead/(double)download_file_len *100;
    m_current_value->setText(QString::number(progress_number,'f',0)+"%");
    //进度条的显示
    qDebug() <<"22222xxxxxxxxx"<<bytesRead<<progress_number;

}
//下载完成之后的回调
void common_newwork_class::httpFinished(){

    this->hide();
    if (m_file) {
        m_file->close();
        delete m_file;
        m_file = nullptr;
    }
    m_file->deleteLater();
    m_file = 0;

    qDebug() <<"333333xxxxxxxxx"<<m_filename;

#if defined(Q_OS_WIN32)
    //打开新的程序
    QString workpath =m_filename;

    std::wstring wlpstrstd = workpath.toStdWString();
    LPCWSTR lpcwStr = wlpstrstd.c_str();

    SHELLEXECUTEINFO ShExecInfo = { 0 };
    ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
    ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
    ShExecInfo.hwnd = NULL;
    ShExecInfo.lpVerb = NULL;
    ShExecInfo.lpFile = lpcwStr;
    //ShExecInfo.lpParameters = _T("");
    //ShExecInfo.lpDirectory = lpcwStr1;
    ShExecInfo.nShow =  SW_SHOWDEFAULT ;
    ShExecInfo.hInstApp = NULL;
    ShellExecuteEx(&ShExecInfo);

    WaitForSingleObject(ShExecInfo.hProcess, INFINITE);

    DWORD dwExitCode = -1;
    GetExitCodeProcess(ShExecInfo.hProcess, &dwExitCode);

    QApplication* app;
    app->exit(0);
    qDebug() <<"333333xxxxxxxxx"<<m_filename;

#else

#endif


}
