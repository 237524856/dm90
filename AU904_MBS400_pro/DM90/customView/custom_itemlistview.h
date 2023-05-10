#ifndef CUSTOM_ITEMLISTVIEW_H
#define CUSTOM_ITEMLISTVIEW_H

#include <QObject>
#include <QWidget>
#include "custom_item.h"
#include "common.h"
#include "../common_funtion.h"


class custom_itemListView : public QWidget
{
    Q_OBJECT
public:
    explicit custom_itemListView(const QString &objectName,const int &w,const int &h,QWidget *parent = nullptr);


    void refreshUI();

    //高级页面的ITEMS
    QVector <custom_item *>m_items;
    //本地声音页面的ITEMS
    QVector <custom_item *>m_localItems;
    //效果页面的ITEMS
    QVector <custom_item *>m_effectItems;

    QVector <QString>m_itemObjectNames;

    int m_itemCounts=0;
    //选中的行
    int m_enter_row = 0;

    //点击了行
    int m_click_row = 0;

    //编辑的行
    int m_item_editRor =0;

    QString m_objectName;

    //增加了ITEM
    void addItem(int defaultList_row,QString title,int change_index,int change_value);

    //下载特效
    void downloadItem(QStringList);

    //减少了ITREM
    bool subtractItem();
    //分享了ITEM
    void shareItem();

    //清空所有ITEM的编辑状态
    void clearAllItemEditStatus();

    //清空所有ITEM的背景颜色
    //void clearAllItembackgroundColor();

    //清空所有ITEM的选中状态
    void clearSelectStatus();

    //判断是否是数字的方法
    bool isDigitStr(QString);
    //标题增加数字
    QString titleAddNumber(QVector<QStringList>,QString);

    //本地声音的缓存结构体转为LIST
    QStringList localstructChangeforList();
    //效果声音的缓存结构体转为LIST
    QStringList effectstructChangeforList();

public slots:

    void listItemClick(QString);

    void listItemEnter(QString);

    //修改了ITEM的label
    void  reviseItemText(QString);

    //编辑了ITEM
    void item_btnClick_row(QString,QString);

    void default_itemEntered(QListWidgetItem *);

protected:

    QListWidget *m_listWidget_default;   

    QWidget *m_parent;

    common_funtion m_commClass;


signals:

    void listViewClick(QString objectName,QString itemText,int itemIndex);

};

#endif // CUSTOM_ITEMLISTVIEW_H

////高级自定义预设
//QVector <custom_item *>m_highListItems;
////本地声音自定义预设
//QVector <custom_item *>m_localListItems;
////效果预设
//QVector <custom_item *>m_effectListItems;
