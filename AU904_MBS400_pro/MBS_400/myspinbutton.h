#ifndef MYSPINBUTTON_H
#define MYSPINBUTTON_H

#include <QWidget>
#include <QSlider>
#include <QLabel>

#define whirlbuttonHeigth   175
#define whirlbuttonWidth    175
#define mybaseAngle 310

class MySpinButton : public QWidget
{
    Q_OBJECT
public:


    explicit MySpinButton(QWidget *parent = nullptr);
    ~MySpinButton();

    int getDegree();
    QSlider *m_pSlider;
    QString m_title_name;
    QLabel *m_low_left_mask_image;

    int m_whirlbuttonHeigth;//整个页面的高度
    int m_whirlbuttonWidth;
    int m_inner_circle_width;

    int m_max_number;
    int m_min_number;

    QString m_backimagestr;
    //指定那个视图旋转了
    int m_select_index;
    //如果相同则不需要再次发送
    int different_number;

    int different_low_number;
    int different_mid_number;
    int different_hi_number;
    //QLabel *m_title_label;

    void setDegree(int deg);
    void set_whirlbuttonHeigth(int);
    void set_whirlbuttonWidth(int);
    void set_backimage(QString image_str);
    void set_inner_circle_width(int innerwidth);


    void set_mask_view();

    void  paintEvent(QPaintEvent *event);

    void send_lowmidhigh_mess(int different_number);





signals:
    void updateDegree();
    void volume_click();
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseToPosion(QMouseEvent *event);
//    QSize sizeHint() const;
//    QSize minimumSizeHint() const;
private:
    double degree;
};

#endif // MYSPINBUTTON_H
