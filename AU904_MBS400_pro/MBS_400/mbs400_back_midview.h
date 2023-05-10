#ifndef MBS400_BACK_MIDVIEW_H
#define MBS400_BACK_MIDVIEW_H

#include <QWidget>
#include <QLabel>
#include <QCheckBox>
#include "QSlider"
#include "qhorslider.h"
#include "myspinbutton.h"
#include "switchcontrol.h"

class mbs400_back_midview : public QWidget
{
    Q_OBJECT
public:
    explicit mbs400_back_midview(QWidget *parent = nullptr);

    void createUI();
    //简单模式的挡板
    QLabel *m_simple_mask_view;
    //收到刷新UI的通知
    void notice_refresh_ui();

signals:


private slots:
    void LR_chanege_value(int value);
    void switch_btn(bool);
    void mute_click_status(bool);
    void volume_change();

private:
    //左右均衡的Slider
    QHorSlider *m_LR_slider;
    QCheckBox *m_mute_btn;
    MySpinButton *m_spin_view;
    SwitchControl *m_switch_btn;
    QVector<MySpinButton *> m_high_low_spinArrs;


};

#endif // MBS400_BACK_MIDVIEW_H
