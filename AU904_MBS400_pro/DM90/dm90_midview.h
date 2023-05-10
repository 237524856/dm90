#ifndef DM90_MIDVIEW_H
#define DM90_MIDVIEW_H

#include <QWidget>
#include <QLabel>
#include "common.h"
#include "customView/custom_listview.h"
#include "customView/custom_itemlistview.h"

class dm90_midview : public QWidget
{
    Q_OBJECT
public:
    explicit dm90_midview(QWidget *parent = nullptr);

    void createUI();
    //创建本地声音的列表
    void initLocalSoundView();
    //创建效果的列表
    void initEffectView();


    //默认列表
    custom_listView *m_default_locaListView;

    custom_listView *m_default_effectListView;

    //自定义列表
    custom_itemListView *m_localCustom_itemList;

    custom_itemListView *m_effectCustom_itemList;

    //默认item的标题
    QString m_itemtext;
    //默认ITEM的行
    int m_defaultTtemRow=0;

    QVector<QPushButton *>btns;

protected:

    QLabel *m_titleStatus;

    //本地声音，效果的切换
    QStackedWidget *m_pStackedWidget;   

    //本地声音赋值
    void localGetMainDatasource();
    //效果声音赋值
    void effectGetMainDatasource();

    //发送数据的类
    send_receive_massage_control m_commin_fun;

    common_funtion m_commClass;


public slots:

    void titleBtnClick();
    //本地声音自定义预设的增删享
    void localCustom_preset_click();

    void effectCustom_preset_click();

    //本地声音
    void localDefault_listView_click(QString,QString,int);

    //点击了本地声音的自定义列表
    void localCustom_listView_click(QString,QString,int);

    //效果默认音效点击事件
    void effectDefault_listView_click(QString,QString,int);

    //效果自定义音效点击事件
    void effectCustom_listView_click(QString,QString,int);

    //本地声音增加ITEM事件
    void localAddItem(int,int);

    //效果增加ITEM事件
    void effectAddItem(int,int);



signals:

    void sendResultNumber(int);

    //刷新本地声音默认预设的UI
    void refreshLocalSoundUI(bool);

};

#endif // DM90_MIDVIEW_H
