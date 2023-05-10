#ifndef PD400_CHECK_UPDATEVIEW_H
#define PD400_CHECK_UPDATEVIEW_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include "custom_button.h"
#include "custom_checkbox.h"

enum DEVICE_MODE{
    DM30 = 1,
    PD400
};

class pd400_check_updateView : public QWidget
{
    Q_OBJECT
public:
    explicit pd400_check_updateView(QWidget *parent = nullptr);

    //选择产品的型号
    DEVICE_MODE m_model_codel;

    QLabel *m_title_label;

    QLabel *m_title_massage;

    custom_button *m_cancel_btn;
    custom_button *m_confirm_update_btn;

    int m_current_status;

    void createUI();

    void refreshUI();
    //根据不同的状态显示不同的页面
    void set_diff_status(int);

    //读取BIN文件
    bool read_bin_file();

    void download_bin_file(QUrl url,QString file);

    void set_bindfu_file(QString path);

    QList<QByteArray> m_dataSourceList;

    //版本号
    int version_V1;
    int version_V2;
    int version_V3;

signals:
    void updata_DFU();
    void finish_download_dspfile();

public slots:

    void cancel_click();

    void confirm_update_click();
    //下载完成得槽
    void finish_downloadfile();

protected:
    //升级的BIN文件的路径文件
    QString m_dfubin_file;

};

#endif // PD400_CHECK_UPDATEVIEW_H
