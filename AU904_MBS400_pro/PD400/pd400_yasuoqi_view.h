#ifndef PD400_YASUOQI_VIEW_H
#define PD400_YASUOQI_VIEW_H

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

enum slider_name{
    YASUO_SLIDER = 0,
    HUIFU_SLIDER,
    QIDONG_SLIDER,
    YASUOBI_SLIDER
};

class pd400_yasuoqi_view : public QWidget
{
    Q_OBJECT
public:
    explicit pd400_yasuoqi_view(QWidget *parent = nullptr);

    void createUI();
    void paintEvent(QPaintEvent *event);


    QVector<QLabel *>current_value_labels;

    slider_name slider_status;
    //所有存放sliders的数组
    QVector<MySlider *>m_sliders;

    QLabel *m_yasuoqi_label;
    QVector<QLabel *>m_labels;
    QVector<QPushButton *>m_btns;

    void refresh_UI(QVector<int> *slider_values);

    void changelanguage();

    //刷新界面
    void refresh_ui(int comp_threshold,int comp_attack,int comp_release,int comp_rate,int maxgain);

    //计时器
    QTimer *m_DM30Timer;
    //页面中滑动条当前的值
    int m_slider_current_value=0;
    int m_slider_current_number = 0;

public slots:

    void hide_click();

    void slider_chanege_value(int value);

    void slider_release_value(int);

    //点击重设
    void reset_click();
    void confirm_click();

    //计时器的方法
    void handleTimeout();


signals:

    void close_yasuoqi_view();

protected:
    send_receive_massage_control m_comm_fun;

};

#endif // PD400_YASUOQI_VIEW_H
