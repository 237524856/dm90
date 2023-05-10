#ifndef SLIDERWIDGET_H
#define SLIDERWIDGET_H

#include <QWidget>
#include <QMouseEvent>

class SliderWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SliderWidget(QWidget *parent = 0);

    void SetRange(const int &nMin, const int &nMax);
    void GetRange(int *pMin, int *pMax);

    void SetValue(const int &nValue);
    int GetValue() const;

    void SetVertical(const bool &isVertical);
    //禁止点击事件
    void SetDisable(const bool&isopen);

protected:
    void paintEvent(QPaintEvent *e);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    void keyPressEvent(QKeyEvent *event);
    void wheelEvent(QWheelEvent *event);

private:
    //将坐标转换成当前值
    void CalcPoint2Value(QMouseEvent *event);

    //获取滑块大小
    int GetThumbSize();

    //获取区域大小
    QRect GetClientRect();

    void UpdateValue(const int &nValue);

signals:
    void valueChanged(int curVal);

    void sliderReleasedAt(int curVal);//当鼠标释放时，抛出包含鼠标X坐标位置信息的信号



private:
    int m_nMin;
    int m_nMax;
    int m_nValue;

    bool m_IsVertical; //是否是垂直显示

    bool m_isdisable;//是否能点击
};

#endif // SLIDERWIDGET_H
