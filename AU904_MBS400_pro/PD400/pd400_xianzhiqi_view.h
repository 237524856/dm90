#ifndef PD400_XIANZHIQI_VIEW_H
#define PD400_XIANZHIQI_VIEW_H


#include "qdialog.h"
#include <QPushButton>
#include <QLabel>
#include <QMouseEvent>
#include <QLineEdit>
#include <QWidget>
#include <QVector>
#include <QTimer>
#include "../common.h"
#include "../send_receive_massage_control.h"
#include "../MBS_400/myslider.h"
#include "custom_button.h"
#include "custom_checkbox.h"

class pd400_xianzhiqi_view : public QWidget
{
    Q_OBJECT
public:
    explicit pd400_xianzhiqi_view(QWidget *parent = nullptr);

    void createUI();

    void paintEvent(QPaintEvent *event);
    QLabel *m_xianzhiqi_label;
    QLabel *m_fanzhi_label;
    QLabel *m_qidongtime_label;
    QLabel *m_huifutime_label;
    QVector<QPushButton *>m_btns;


    //限幅的当前值
    QLabel *m_xianfu_current_value;

    QLabel *m_qidong_current_value;

    QLabel *m_huifu_current_value;

    QVector<MySlider *>m_sliders;


    int m_send_value;
    int m_diffsend_value;
    int m_times;

    void changelanguage();

    //刷新界面
    void refresh_ui(int limit,int limit_attack,int limit_release);

    //计时器
    QTimer *m_DM30Timer;
    //页面中滑动条当前的值
    int m_slider_current_value=0;
    int m_slider_current_number = 0;

public slots:

    void hide_click();

    void xianfu_chanege_value(int xianfu_value);
    void xianfu_release_value(int);

    void qidong_chanege_value(int qidong_value);
    void qidong_release_value(int);

    void huifu_chanege_value(int huifu_value);
    void huifu_release_value(int);
    //点击重设
    void reset_click();
    void confirm_click();
    //计时器的方法
    void handleTimeout();




signals:

    void close_xianfuqi_view();

protected:
    send_receive_massage_control m_comm_fun;

};

#endif // PD400_XIANZHIQI_VIEW_H
