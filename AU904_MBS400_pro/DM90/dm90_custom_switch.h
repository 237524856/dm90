#ifndef DM90_CUSTOM_SWITCH_H
#define DM90_CUSTOM_SWITCH_H

#include <QWidget>
#include <QPushButton>
#include <QVector>
#include "common.h"
#include "../custom_button.h"

class dm90_custom_switch : public QWidget
{
    Q_OBJECT
public:
    explicit dm90_custom_switch(QWidget *parent = nullptr);
    void createUI();
    //刷新UI界面的方法
    void refreshUI();

    //设置样式
    void setStyleBtn(int);

private:

    QVector<custom_button *>m_switchBtns;



public slots:
    void switch_click();

signals:

    void lightswitchClick(int);

};

#endif // DM90_CUSTOM_SWITCH_H
