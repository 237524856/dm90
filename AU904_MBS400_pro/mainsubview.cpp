#include "mainsubview.h"
#include <QDebug>

mainSubView::mainSubView(QWidget *parent) : QWidget(parent)
{
    this->setAttribute(Qt::WA_StyledBackground,true);
}
void mainSubView::mousePressEvent(QMouseEvent* event){

    QMouseEvent *k = dynamic_cast<QMouseEvent *>(event);
    qDebug() <<"press"<<k->x()<<k->y();
    m_press_x = k->x();
    //return __super::mousePressEvent(event);
}

void mainSubView::mouseMoveEvent(QMouseEvent *event){

    QMouseEvent *k = dynamic_cast<QMouseEvent *>(event);

    if(k->x()-m_press_x>0){//向右边滑动了
        is_next =2;
    }else if(k->x()-m_press_x<0){//向左边滑动了
        is_next=1;
    }
}

void mainSubView::mouseReleaseEvent(QMouseEvent *event){

    if(is_next==1){//下一张


    }else if(is_next==2){//上一张

    }
}
