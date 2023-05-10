
#pragma once
#pragma execution_character_set("utf-8")
#include "networking_class.h"
#include "common.h"

//这句一定要写,因为self是静态变量,必须要初始化值
networking_class *networking_class::self = NULL;



networking_class *networking_class::getInstance(){

    //判断是否为第一次调用

    if(self == NULL){
        //防止多进程同时调用创建2个对象的问题
                //原理是因为是静态变量只会定义一次
                static QMutex mutex;

                //上锁  QMutexLocker会锁住mutex，当QMutexLocker 被释放的时候会自动解锁
                //locker是局部变量，所以getInstance（）函数结束之后会自动解锁
                QMutexLocker locker(&mutex);

                //在判断一次,防止其它进程抢先new过了
                if (self == NULL) {
                    self = new networking_class;
                }
    }
    return self;
}
QString networking_class::network_updateversion(QString geturl){

    //设置url
    QString url = geturl;
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

            QString version_str = jo["f_ver"].toString();
            return version_str;

//            QString name_str = jo["f_name"].toString();
//            if(name_str == "glob"){
//                //判断2个版本号是不是一样的，如果是一样则不升级，不一样的升级
//                if(DFU_version != jo["proj_ver"].toString()){//弹出升级框
//                    QMessageBox message(QMessageBox::NoIcon, "提示", "软件有新的版本，确定要更新吗?", QMessageBox::Yes | QMessageBox::No, NULL);
//                    if(message.exec() == QMessageBox::Yes)
//                    {
//                        download_file_len = jo["f_len"].toInt();
//                        network_download(QUrl("xxxxxx"),"xxx");
//                    }

//                }else{
//                    qDebug() << "999999999"<<name_str;
//                }
//            }
        }

    }
    const char *result = responseByte.data();
    QString str = responseByte;
    qDebug()<<"lllllllllllllllllllllllllUSB连接失败";
    return "101";

}
void networking_class::network_download(QUrl url,QString download_file){

    m_filename = download_file;
    qDebug() <<"ggxxxxxxxxx"<<m_filename;
    m_file =new QFile(m_filename);
    if(!m_file->open(QIODevice::WriteOnly)){
        delete m_file;
        m_file=0;
        return;
    }
    startRequest(url);
}
void networking_class::startRequest(QUrl url){
    QNetworkRequest requestInfo;
    requestInfo.setUrl(QUrl(url));
    m_reply =  m_pHttpMgr->get(requestInfo);
    connect(m_reply,&QNetworkReply::readyRead,this,&networking_class::httpReadyRead);
    connect(m_reply,&QNetworkReply::downloadProgress,this,&networking_class::updateDataReadProgress);
    connect(m_reply,&QNetworkReply::finished,this,&networking_class::httpFinished);
}
void networking_class::httpReadyRead(){
    if(m_file)
        m_file->write(m_reply->readAll());
    //qDebug() <<"111111xxxxxxxxx";
}
void networking_class::updateDataReadProgress(qint64 bytesRead,qint64 totalBytes){

    //m_progress_view->setMaximum(93080064);\
    //进度条的显示
//    qDebug() <<"22222xxxxxxxxx"<<bytesRead<<totalBytes;
}
//下载完成之后的回调
void networking_class::httpFinished(){
    if (m_file) {
        m_file->close();
        delete m_file;
        m_file = nullptr;
    }
    m_file->deleteLater();
    m_file = 0;
    emit network_finish_download();


}
