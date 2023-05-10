#ifndef PD400_SETTING_VIEW_H
#define PD400_SETTING_VIEW_H

#include <QWidget>
#include <QVector>
#include <QLabel>
#include <QPushButton>
#include <QMouseEvent>
#include "custom_button.h"
#include "custom_checkbox.h"
class pd400_setting_view : public QWidget
{
    Q_OBJECT
public:
    explicit pd400_setting_view(QWidget *parent = nullptr);

    void mousePressEvent(QMouseEvent *qevent);            //鼠标按下事件
    void mouseReleaseEvent(QMouseEvent *qevent);         //鼠标释放事件
    void mouseMoveEvent(QMouseEvent *qevent);             //鼠标移动事件

    custom_button *m_check_btn;

    QLabel *m_version_label;

    QLabel *m_DSP_label;

    QLabel *m_SN_label;

    QLabel *m_device_name_label;

    void createUI();

    QVector<QLabel *>seting_labels;


public slots:

    //检查更新
    void check_click();
    //恢复原厂设置
    void reset_click();


signals:

    //切换语言
    void change_languge(int);
    //检查更新得信号
    void check_version();
    //鼠标点击的事件
    void setting_mouse_press();
    //设置恢复出厂设置
    void reset_clickBlock();
};

#endif // PD400_SETTING_VIEW_H
