#ifndef MBS400_BACK_RIGHTVIEW_H
#define MBS400_BACK_RIGHTVIEW_H

#include <QWidget>
#include <QListWidget>
#include <QVector>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include "qhorslider.h"
#include "qverslider.h"
#include <QMap>

class mbs400_back_rightview : public QWidget
{
    Q_OBJECT
public:
    explicit mbs400_back_rightview(QWidget *parent = nullptr);
    void new_createUI(void);
    //EQ的Slider
    QVector<QVerSlider *>m_EQ_sliders;
    //limiter&high&low的slider
    QVector<QHorSlider *>m_limiter_highlow_slider;

    //设置所有Slider的当前值
    QVector<int>m_right_sliderValues;
    //设置为默认值
    QVector<double>m_reset_sliderValues;
    //寄存器的地址
    QVector<uint16_t>m_EQaddrs;

    QStringList m_data_list;

    QHorSlider*m_high_slider;

    QHorSlider*m_low_slider;

    int isDigitStr(QString src);
    //刷新高级中的UI数据
    void refresh_data_forui(int);

    //收到刷新UI的通知
    void notice_refresh_ui();


protected:
    QListWidget *m_listWidget_default;


private:
    //简单模式的挡板
    QLabel *m_simple_mask_view;
    //高级模式的挡板
    QLabel *m_high_mask_view;
    //保存数据的按钮
    QPushButton *m_save_btn;

    QVector<QPushButton *>m_refresh_save_arrs;


signals:
    void switch_status(bool isopen);
    void user_message();
private slots:
    void refresh_save_click();
    void verticalSlider_setValue(int);
    void switch_btn(bool isopen);
    //linmiter事件
    void limiter_slider_click(int);
    //high_pass事件
    void high_pass_click(int);
    //low_pass事件
    void low_pass_click(int);


};

#endif // MBS400_BACK_RIGHTVIEW_H
