#ifndef NETWORKING_CLASS_H
#define NETWORKING_CLASS_H

#include <QObject>
#include <QDebug>

//#include <QtSerialPort/QSerialPort>
#include <QMutex>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkCookie>
#include <QNetworkRequest>
#include <QEventLoop>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QStandardPaths>
#include <QMessageBox>
#include <QApplication>

class networking_class :public QObject{

    Q_OBJECT
public:
    //通过它获取self指针,这个函数是静态调用,不需要创建对象就可以访问
    static networking_class *getInstance();
    void hello(){
        qDebug()<<"xxxxxxxgggggggggx";
    }
    QNetworkAccessManager *m_pHttpMgr;
    QNetworkReply *m_reply;

    QFile *m_file;
    QString m_filename;

    //文件的长度
    int download_file_len;
    QString network_updateversion(QString geturl);
    void network_download(QUrl url,QString download_file);
    void startRequest(QUrl url);

signals:
    void network_finish_download();

private slots:
    void httpFinished();
    void httpReadyRead();
    void updateDataReadProgress(qint64,qint64);

private:
    networking_class(){//构造函数,写在private下,不允许new生成单例，可以加入你的代码,也可以没有代码

        m_pHttpMgr = new QNetworkAccessManager();
        qDebug()<<"lllllllllllllllllllllllll";
    }
    static networking_class *self;//静态指针

};


#endif // NETWORKING_CLASS_H
