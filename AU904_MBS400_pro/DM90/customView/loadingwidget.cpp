#include "loadingwidget.h"
#include "qmath.h"
#include <QDebug>

LoadingWidget::LoadingWidget(QWidget *parent) : QWidget(parent),_i(0),_interval(50),_index(0)
{
    //设置背景透明

    setDotColor(QColor(49, 177, 190));
    setDotCount(20);
    connect(&timer,&QTimer::timeout,this,&LoadingWidget::refresh);
    setMaxDiameter(30);
    setMinDiameter(5);
}

//********************************************** 设置部分 *************************************
//设置点的个数
void LoadingWidget::setDotCount(int count)
{
    _count=count;
}

//设置点的颜色
void LoadingWidget::setDotColor(const QColor & color)
{
    _dotColor=color;
}

//开始动画
void LoadingWidget::start()
{
    timer.setInterval(_interval);
    timer.start();
}

//设置最大直径
void LoadingWidget::setMaxDiameter(float max)
{
    _maxDiameter=max;
}

//设置最小直径
void LoadingWidget::setMinDiameter(float min)
{
    _minDiameter=min;
}
//********************************************** 绘制部分 *************************************
//刷新界面
void LoadingWidget::refresh()
{
    repaint();
}

void LoadingWidget::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event)
    caculate();
}

void LoadingWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.setPen(_dotColor);
    painter.setBrush(_dotColor);

    //绘制点
    paintDot(painter);
}

//计算绘制正方形区域
void LoadingWidget::caculate()
{
    _squareWidth=qMin(this->width(),this->height());
    float half=_squareWidth/2;
    _centerDistance=half-_maxDiameter/2-1;

    float gap=(_maxDiameter-_minDiameter)/(_count-1)/2;
    float angleGap=(float)360/_count;

    locationList.clear();
    radiiList.clear();

    for(int i=0;i<_count;i++)
    {
        radiiList<<_maxDiameter/2-i*gap;
        float radian=qDegreesToRadians(-angleGap*i);
        locationList.append(Location(half+_centerDistance*qCos(radian),half-_centerDistance*qSin(radian)));
    }
}

//绘制圆点
void LoadingWidget::paintDot(QPainter& painter)
{
    for(int i=0;i<_count;i++)
    {
        painter.setPen(_dotColor);
        //半径
        float radii=radiiList.at((_index+_count-i)%_count);

        //绘制圆点
        painter.drawEllipse(QPointF(locationList.at(i).x,locationList.at(i).y),radii,radii);
        //绘制正方形
        //painter.drawRect(locationList.at(i).x,locationList.at(i).y,radii,radii);
        //绘制文字
        //QFont font("Microsoft YaHei",radii*1.2,75);
        //painter.setFont(font);
        //painter.drawText(QPointF(locationList.at(i).x,locationList.at(i).y),u8"霞");
    }
    _index++;
}
