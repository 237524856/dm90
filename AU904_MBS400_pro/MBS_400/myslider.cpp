#include "myslider.h"

#include <QDebug>
#include <QEvent>
#include <QCoreApplication>
#include <QThread>
#include "../common.h"

MySlider::MySlider(QWidget *parent) : QSlider(parent)
{
    m_value=0;
    m_mousePress = false;
    m_isMoving=false;
    this->setDisabled(false);
    this->setCursor(Qt::PointingHandCursor);
    this->setPageStep(1);
    m_slider_timer = new QTimer(this);
    connect(m_slider_timer, SIGNAL(timeout()), this, SLOT(OnMouseLeave()));
    m_slider_timer->stop();

    QString slider_stys=QString("QSlider::groove:horizontal{background: rgba(56, 56, 56,255);height:%3px;border: 0px solid #FFFFFF;border-radius: %1px;}"
                                "QSlider::handle:horizontal{width:%2px;height:%2px;margin-top: -%3px;margin-left: 0px;margin-bottom: -%3px;margin-right: 0px;border-image:url(:/image/images/pd400_round.png);}"
                                "QSlider::sub-page:horizontal{background-color:rgba(255,171,33,255);border-radius: %1px;}").arg(2*m_Magnification).arg(12*m_Magnification).arg(4*m_Magnification);
    this->setStyleSheet(slider_stys);


}
void MySlider::mousePressEvent(QMouseEvent *event){
    //    this.x:控件原点到界面边缘的x轴距离；
    //    globalPos.x：鼠标点击位置到屏幕边缘的x轴距离；
    //    pos.x：鼠标点击位置到本控件边缘的距离；
    //    this.width:本控件的宽度;
    //注意应先调用父类的鼠标点击处理事件，这样可以不影响拖动的情况
    //QSlider::mousePressEvent(event);
    switch (event->button()) {
    case Qt::LeftButton:
    {
        m_isMoving = false;
        m_mousePress = true;
        //获取鼠标的位置，这里并不能直接从ev中取值（因为如果是拖动的话，鼠标开始点击的位置没有意义了）
        m_pos = event->pos().x() / (double)width();
        double value = m_pos * (maximum() - minimum()) + minimum();
        //value + 0.5 四舍五入
        if(value>maximum()){
            value=maximum();
        }
        if(value<minimum()){
            value=minimum();
        }
        m_value=value+0.5;
        setValue(m_value);

        emit sliderCustomChangeValue(m_value);
        //emit sliderMoved( m_value );
        //向父窗口发送自定义事件event type，这样就可以在父窗口中捕获这个事件进行处理
        //qDebug()<<"mousePressEvent"<<value;
        QEvent evEvent(static_cast<QEvent::Type>(QEvent::User + 1));
        QCoreApplication::sendEvent(parentWidget(), &evEvent);
    }
        break;
    case Qt::RightButton:     // 0x00000002
    {
        //鼠标右键按下处理
    }
        break;
    default:
        break;
    }

}


void MySlider::mouseMoveEvent(QMouseEvent *event){


//    switch (event->button()) {
//    case Qt::LeftButton:
//    {
//        //鼠标左键按下处理

//    }
//        break;
//    case Qt::RightButton:     // 0x00000002
//    {
//        //鼠标右键按下处理
//    }
//        break;
//    default:
//        break;
//    }

    if(m_mousePress){

        QSlider::mouseMoveEvent(event);

        //qDebug()<<"mouseMoveEvent";
        m_pos = event->pos().x() / (double)width();
        double value = m_pos * (maximum() - minimum()) + minimum();
        if(value>maximum()){
            value=maximum();
        }
        if(value<minimum()){
            value=minimum();
        }
        //value + 0.5 四舍五入
        if(m_mousePress){
            m_value=value + 0.5;
            m_isMoving=true;
            //emit sliderMoved(m_value);
            emit sliderCustomChangeValue(m_value);
        }
        this->setValue(value + 0.5);

        //qDebug()<<"mouseMoveEvent"<<m_pos<<"mywww"<<value;
        //向父窗口发送自定义事件event type，这样就可以在父窗口中捕获这个事件进行处理
        QEvent evEvent(static_cast<QEvent::Type>(QEvent::User + 1));
        QCoreApplication::sendEvent(parentWidget(), &evEvent);
    }
}

void MySlider::OnMouseLeave(){
    m_DM30micsilder_runing = false;
    m_DM30phsilder_runing = false;
    m_PD400phsilder_runing = false;
    m_PD400mixsilder_runing = false;
    m_PD400gainsilder_runing = false;
    m_slider_timer->stop();
}
void MySlider::wheelEvent(QWheelEvent *event){
    QSlider::wheelEvent(event);
    m_slider_timer->stop();
    m_slider_timer->start(80);
//    if(event->delta() > 0){                    // 当滚轮远离使用者时
//            this->value();                // 进行增加
//        }else{                                     // 当滚轮向使用者方向旋转时
//            m_value -= 1;               // 进行减少
//        }
//    qDebug()<<"gunlllllllllllll"<<m_value;
}
void MySlider::keyReleaseEvent(QKeyEvent *event){

    QSlider::keyReleaseEvent(event);
    m_DM30micsilder_runing = false;
    m_DM30phsilder_runing = false;
    m_PD400phsilder_runing = false;
    m_PD400mixsilder_runing = false;
    m_PD400gainsilder_runing = false;
};
void MySlider::mouseReleaseEvent(QMouseEvent *event){

    if(m_mousePress){

        QSlider::mouseReleaseEvent(event);
        //qDebug()<<"mouseReleaseEvent"<<m_value;
        m_mousePress = false;
        m_isMoving=false;
        emit sliderReleasedAt(m_value);//抛出有用信号
    }
//qDebug()<<"gunlllllllllllll"<<m_value;

}

void MySlider::setSliderDisable(bool is_disabled){

    if(is_disabled){//不可点

        QString slider_stys=QString("QSlider::groove:horizontal{background: rgba(56, 56, 56,255);height:%3px;border: 0px solid #FFFFFF;border-radius: %1px;}"
                                    "QSlider::handle:horizontal{width:%2px;height:%2px;margin-top: -%3px;margin-left: 0px;margin-bottom: -%3px;margin-right: 0px;border-image:url(:/image/images/pd400_round.png);}"
                                    "QSlider::sub-page:horizontal{background-color:rgba(171,171,171,255);border-radius: %1px;}").arg(2*m_Magnification).arg(12*m_Magnification).arg(4*m_Magnification);
        this->setStyleSheet(slider_stys);
        this->setDisabled(true);

    }else{//可点

        QString slider_stys=QString("QSlider::groove:horizontal{background: rgba(56, 56, 56,255);height:%3px;border: 0px solid #FFFFFF;border-radius: %1px;}"
                                    "QSlider::handle:horizontal{width:%2px;height:%2px;margin-top: -%3px;margin-left: 0px;margin-bottom: -%3px;margin-right: 0px;border-image:url(:/image/images/pd400_round.png);}"
                                    "QSlider::sub-page:horizontal{background-color:rgba(255,171,33,255);border-radius: %1px;}").arg(2*m_Magnification).arg(12*m_Magnification).arg(4*m_Magnification);
        this->setStyleSheet(slider_stys);
        this->setDisabled(false);
    }


}
