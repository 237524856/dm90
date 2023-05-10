#ifndef DM90_LIGHTVIEW_H
#define DM90_LIGHTVIEW_H

#include <QWidget>
#include <QListWidget>
#include "common.h"
#include "dm90_lightrgbimage.h"
#include "dm90_custom_switch.h"


class dm90_lightView : public QWidget
{
    Q_OBJECT
public:
    explicit dm90_lightView(QWidget *parent = nullptr);

    void createUI();

    //获取背景参数刷新当前的UI
    void refresh();

    //同步界面刷新设备的数据
    void refreshColorItem(const int onoff,const int model,const int H,const int S,const int V);

    //刷新列表增加的颜色
    void listRefreshColor();

    //进度条计时器的发送方法
    void lightStarDelayTimerFun();

    //取消列表的选中状态
    void cancelListItemSelectStatus();

    //判断是否有一样的颜色，没有则添加“+”
    void exitSamecolorAndappendItem();
    void DM70exitSamecolorAndappendItem();

    //右击删除添加的ITEM
    void removeItemStatus(int);

    void change_mode(bool sendMessage,int result_num);


    QLabel *m_bgLab;

    QListWidget *m_RGBlistWidge;

    QWidget *m_speedview;

    enum resultRGB{
        result_fix = 0,
        result_loop,
        result_breathing
    };

    QLabel *m_briCurrentValue;
    QLabel *m_speedCurrentValue;

    //rgb颜色
    int m_R;
    int m_G;
    int m_B;

    //呼吸下RGB颜色
    int m_breatheR=0;
    int m_breatheG=0;
    int m_breatheB=0;
    //HSV颜色
    int m_H=0;
    double m_S;
    double m_V;

    //呼吸颜色
    bool m_isbreathColorFront = false;
    int m_breathTimeStar = 0;

    //循环颜色
    bool m_isloopColorFront = false;
    int m_loopTimeStar = 0;

    //当前选中的行
    int m_currentSelectItemIndex = 0;

    //列表中的HSV颜色集合
    QVector<QVector<int>>m_listHSV;


public slots:

    void rgb_click(int,int,int);

    void change_result(int);

    void brightness_change_value(int);

    void brightness_release_value(int);

    void speed_change_value(int);

    void speed_release_value(int);

    void reset_click();
    //选中颜色按钮
    void onListRGBItemClicked(QListWidgetItem*);

    //灯光调节的按钮的事件
    void lightadjSwitchClick(int);



    //呼吸计时器的曹
    void breatheTimeout();


public:

    //头顶的开关
    dm90_custom_switch *m_cus_switch;

    //亮度
    MySlider *m_brightness_slider;

    //速度
    MySlider *m_speed_slider;

    //下拉框
    QComboBox *m_LedCbo_sex;

    //蒙版
    QWidget *m_mask_view;

    QWidget *m_RGBmask_view;

    //产品的背景图
    QLabel * m_devicebgImage;


    //呼吸的计时器
    QTimer *m_breatheTimer;

    //当前选中的滑动号码
    int m_currentSliderNum =-1;   

    //0 固定，1 循环，2 呼吸
    int m_mode_number=0;

    send_receive_massage_control m_commin_fun;


    uint32_t HSVtoRGB(const bool&isgetrgb,uint16_t h, uint16_t s, uint16_t v);

    bool IsEquals(double val1, double val2);
    void BGR2HSV(uint16_t R, uint16_t G, uint16_t B);

    void appendItem(QString);



signals:
};

#endif // DM90_LIGHTVIEW_H
