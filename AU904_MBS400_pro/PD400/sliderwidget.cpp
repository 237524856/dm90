#include "sliderwidget.h"
#include <QPainter>
#include <QDebug>

SliderWidget::SliderWidget(QWidget *parent)
    : QWidget(parent)
    , m_nMin(0)
    , m_nMax(200)
    , m_nValue(0)
    , m_IsVertical(false)
    , m_isdisable(false)
{
    setFocusPolicy(Qt::StrongFocus);
    setCursor(Qt::PointingHandCursor);
}

void SliderWidget::SetRange(const int &nMin, const int &nMax)
{
    Q_ASSERT(nMin <= nMax);

    m_nMin = nMin;
    m_nMax = nMax;

    update();
}

void SliderWidget::SetValue(const int &nValue)
{
    if(m_nValue == nValue)
        return;

    m_nValue = nValue;

    if(m_nValue < m_nMin)
        m_nValue = m_nMin;

    if(m_nValue > m_nMax)
        m_nValue = m_nMax;

    update();
}

int SliderWidget::GetValue() const
{
    return m_nValue;
}

void SliderWidget::SetVertical(const bool &isVertical)
{
    m_IsVertical = isVertical;


    update();
}

void SliderWidget::SetDisable(const bool&isopen){

    m_isdisable = isopen;
    this->setDisabled(isopen);
    update();
}

void SliderWidget::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e)

    QPainter painter(this);

    int nThumbSize = GetThumbSize();                //滑块大小
    const int &nSliderBarSize = nThumbSize / 2;     //滑动条背景大小

    int nMiddleValue = (m_nMin + m_nMax) / 2;       //滑动条中间值

    QRect bkRect;
    QRect posBkRect;
    QPoint thumbPoint;

    QRect rect = GetClientRect();

    if(m_IsVertical)//垂直
    {
        int x = (rect.width() - nSliderBarSize) / 2;
        int y =  rect.bottom();
        int height = rect.height();

        bkRect = QRect(x,y-height,nSliderBarSize,height);

        int nPos = height * (m_nValue - m_nMin) / (m_nMax - m_nMin);
        int nMiddlePos = height / 2;

        int nPosY = 0;          //划过背景Y坐标
        int nThumbY = 0;        //滑块Y坐标

        if(nMiddleValue <= m_nValue)
        {
            nPosY = y - nMiddlePos;
            nThumbY = y - nPos - nThumbSize / 2;
            height = nMiddlePos - nPos;
        }
        else
        {        
            nPosY = y - nPos;
            nThumbY = y - nPos - nThumbSize / 2;
            height = nPos - nMiddlePos;
        }

        posBkRect = QRect(x,nPosY,nSliderBarSize, height);

        x = (rect.width() - nThumbSize) / 2;
        thumbPoint = QPoint(x, nThumbY);
    }
    else//水平
    {
        int x = rect.left();
        int y =  (rect.height() - nSliderBarSize) / 2;
        int width = rect.width();

        bkRect = QRect(x,y,width,nSliderBarSize);

        int nPos = width * (m_nValue - m_nMin) / (m_nMax - m_nMin);
        int nMiddlePos = width / 2;

        int nPosX = 0;          //划过背景X坐标
        int nThumbX = 0;        //滑块x坐标

        if(nMiddleValue <= m_nValue)
        {
            nPosX = x + nMiddlePos;
            nThumbX = x+ nPos - nThumbSize/2 ;
            width = nPos - nMiddlePos;
        }
        else
        {
            nPosX = x + nPos;
            nThumbX = x + nPos - nThumbSize / 2;
            width = nMiddlePos - nPos;
        }

        posBkRect = QRect(nPosX,y, width,nSliderBarSize);

        y =  (rect.height() - nThumbSize) / 2;
        thumbPoint = QPoint(nThumbX, y);
    }

    //绘制背景
    {
        painter.setBrush(QColor(56,56,56));
        painter.setPen(Qt::NoPen);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.drawRoundedRect(bkRect,5,5);
    }

    //绘制划过背景
    {
        if(m_isdisable){
           painter.setBrush(QColor(171, 171, 171));
        }else{
           painter.setBrush(QColor(234, 152, 0));
        }

        painter.drawRect(posBkRect);
    }

    //绘制滑块
    {
        QPen pen;
        pen.setWidth(2);
        pen.setColor(QColor(255, 255, 255));
        painter.setBrush(Qt::white);
        painter.setPen(pen);
        painter.drawEllipse(thumbPoint.x(), thumbPoint.y(),nThumbSize,nThumbSize);
    }
}

void SliderWidget::mousePressEvent(QMouseEvent *event)
{
    CalcPoint2Value(event);
}

void SliderWidget::mouseMoveEvent(QMouseEvent *event)
{
    CalcPoint2Value(event);
}

void SliderWidget::mouseReleaseEvent(QMouseEvent *event){

    emit sliderReleasedAt(m_nValue);
}


void SliderWidget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Left:
    case Qt::Key_Down:
    {
        UpdateValue(m_nValue-1);
    }
    break;
    case Qt::Key_Right:
    case Qt::Key_Up:
    {
        UpdateValue(m_nValue+1);
    }
    break;
    default:
        break;
    }
}

void SliderWidget::wheelEvent(QWheelEvent *event)
{
    if(event->delta() < 0)
    {
        UpdateValue(m_nValue-1);
    }
    else
    {
        UpdateValue(m_nValue+1);
    }
}

void SliderWidget::CalcPoint2Value(QMouseEvent *event)
{
    int nThumbSize = GetThumbSize();

    int nValue = m_nValue;

    if(m_IsVertical)
    {
        nValue = m_nMax - (qreal)(event->pos().y() - nThumbSize / 2)/(GetClientRect().height() - nThumbSize)*(m_nMax - m_nMin);
    }
    else
    {
        nValue = (qreal)(event->pos().x() - nThumbSize / 2)/(GetClientRect().width() - nThumbSize)*(m_nMax - m_nMin) + m_nMin;
    }

    if(nValue != m_nValue)
    {
        UpdateValue(nValue);
    }
}

int SliderWidget::GetThumbSize()
{
    return m_IsVertical ? this->width() - 4 :this->height() - 4;
}

QRect SliderWidget::GetClientRect()
{
    int nThumbSize = GetThumbSize();

    QRect rect  = this->rect();

    int offset = nThumbSize*0.65;

    if(m_IsVertical)
    {
        rect.adjust(0, offset, 0, -offset);
    }
    else
    {
        rect.adjust(offset, 0, -offset, 0);
    }

    return rect;
}

void SliderWidget::UpdateValue(const int &nValue)
{
    SetValue(nValue);

    emit valueChanged(m_nValue);
}
