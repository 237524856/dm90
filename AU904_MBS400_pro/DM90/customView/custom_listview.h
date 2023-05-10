#ifndef CUSTOM_LISTVIEW_H
#define CUSTOM_LISTVIEW_H

#include <QWidget>
#include <QListWidgetItem>
#include "common.h"

class custom_listView : public QWidget
{
    Q_OBJECT
public:
    explicit custom_listView(const QString &objectName, const int &w,const int &h,QWidget *parent = nullptr);

    int m_select_number = 0;

    QListWidget *m_listWidget_default;

    //选中了默认列表的行
    int m_defaultHighCurrentItemIndex = 0;

    //取消按钮的选中状态
    void clearItemSelect();


protected:

      QListWidgetItem *m_item = NULL;
      QString m_objectName;
      common_funtion m_commClass;


public slots:

    void user_default_click(int);

    void default_click_item(QListWidgetItem *);

    void default_itemActivated(QListWidgetItem *);

    void default_itemEntered(QListWidgetItem *);
signals:

    void listViewClick(QString objectName,QString itemText,int itemIndex);

    void locallistViewClick(QString objectName,QString itemText,int itemIndex);

};

#endif // CUSTOM_LISTVIEW_H
