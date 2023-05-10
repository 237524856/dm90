#ifndef COMMON_NEWWORK_CLASS_H
#define COMMON_NEWWORK_CLASS_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkCookie>
#include <QNetworkRequest>
#include <QEventLoop>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QProgressBar>
#include <QLabel>
#include <QProcess>

class common_newwork_class : public QWidget
{
    Q_OBJECT
public:
    explicit common_newwork_class(QWidget *parent = nullptr);


    QNetworkAccessManager *m_pHttpMgr;
    QNetworkReply *m_reply;

    QFile *m_file;

    QLabel *m_load_label;


    QProgressBar *m_progress_view;

    QLabel *m_current_value;
    //网络请求,更新版本号
    void network_updateversion();

    //下载执行文件
    void network_download();

    void startRequest(QUrl url);

    //文件的长度
    int download_file_len;

    QString m_filename;

    QProcess *myProcess;

    void createUI();
private slots:
    void httpFinished();
    void httpReadyRead();
    void updateDataReadProgress(qint64,qint64);

signals:

};

#endif // COMMON_NEWWORK_CLASS_H
