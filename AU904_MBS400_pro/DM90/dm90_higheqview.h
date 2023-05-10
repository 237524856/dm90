#ifndef DM90_HIGHEQVIEW_H
#define DM90_HIGHEQVIEW_H

#include <QWidget>
#include <QLabel>
#include "common.h"
#include "MBS_400/qverslider.h"

class dm90_highEQView : public QWidget
{
    Q_OBJECT
public:
    explicit dm90_highEQView(QWidget *parent = nullptr);

    void createUI();

    //获取背景参数刷新当前的UI
    void refresh();

    //显示当前的全部值
    QVector<QLabel *>m_current_values;
    QVector<SliderWidget *>m_silders;


public slots:

    //各种进度条的方法
    void verticalSlider_setValue(int);
    void verticalSlider_released(int);

    //重置
    void reset_click();

    void isopen_highEQ_click(int);

private:

    //蒙版
    QWidget *m_maskView;

    custom_checkbox *highEQ_btn;

    send_receive_massage_control m_commin_fun;




signals:

    void block_highEQSlider(int);
    //鼠标释放的方法，返回当前SLIDER的下标
     void block_sliderRelease(int);

    //点击了使能方法
     void block_disable_usbplay(int);

};

#endif // DM90_HIGHEQVIEW_H
