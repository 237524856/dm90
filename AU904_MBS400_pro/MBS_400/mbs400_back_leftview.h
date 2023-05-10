#ifndef MBS400_BACK_LEFTVIEW_H
#define MBS400_BACK_LEFTVIEW_H

#include <QWidget>
#include <QLabel>
#include <QSlider>
#include <QVector>
#include <QListWidget>
#include <QCheckBox>

class mbs400_back_leftview : public QWidget
{
    Q_OBJECT
public:
    explicit mbs400_back_leftview(QWidget *parent = nullptr);


    void new_createUI(void);
    QVector<QSlider *>m_reset_sliders;

    //设置Slider的当前值
    QVector<int>m_right_sliderValues;
    //设置为默认值
    QVector<double>m_reset_sliderValues;

    QStringList m_model_list;
    QStringList m_user_model_list;

    //要删除数据的下标
    int m_delete_datasource_index;

    //刷新用户列表的方法
    void refresh_user_list();




signals:
    void click_userlist_row(int);


protected:
    QListWidget *m_listWidget_default;
    QListWidget *m_listWidget_custom;
    QLabel *m_perset_bottom_label;
    QLabel *m_usercustom_label;
    QCheckBox *m_user_arrow_btn;
    QCheckBox *m_arrow_btn;
private:
    QSlider *volume_slider;

private slots:
    void perset_btn_click();
    void user_btn_click();
    void preset_btn_click();
    void user_custom_click(int);

};

#endif // MBS400_BACK_LEFTVIEW_H
