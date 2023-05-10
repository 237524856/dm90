#ifndef DM90_LEFTVIEW_H
#define DM90_LEFTVIEW_H

#include <QWidget>
#include <QLabel>
#include "common.h"
#include <QVector>
#include "custom_button.h"

class dm90_leftview : public QWidget
{
    Q_OBJECT
public:
    explicit dm90_leftview(QWidget *parent = nullptr);

    enum POLARSATAUS{
        HEART,
        OMNI,
        GLYPH
    };


    //创建UI
    void createUI();
    //刷新UI
    void refreshUI();

protected:
    QVector<custom_button *>m_pointBtns;
    QVector<QLabel *>m_btnTitles;

    //发送数据得公共累
    send_receive_massage_control m_commin_fun;

public slots:
    void changezhixiangClick();

signals:

};

#endif // DM90_LEFTVIEW_H
