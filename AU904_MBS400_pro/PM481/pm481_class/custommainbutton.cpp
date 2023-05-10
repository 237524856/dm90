#include "custommainbutton.h"
#include <QDebug>

customMainButton::customMainButton(QWidget *parent) : QPushButton(parent)
{

}
void customMainButton::mousePressEvent(QMouseEvent *qevent){

    switch (qevent->button()) {
    case Qt::LeftButton:
    {
        //鼠标左键按下处理
        emit custom_click();
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
void customMainButton::mouseReleaseEvent(QMouseEvent *qevent){

}
void customMainButton::mouseMoveEvent(QMouseEvent *qevent){

}

//#if defined(Q_OS_WIN32)
    void customMainButton::enterEvent(QEvent *){
//#else
//   void customMainButton::enterEvent(QEnterEvent *){
//#endif
    emit custom_enter(this->objectName());
     //qDebug()<<"jiruuuuuuuuuuuuuuuuuuuu";

}
void customMainButton::leaveEvent(QEvent *){

    emit custom_leave(this->objectName());
    //qDebug()<<"learrrrrrrrrrrrrrrrrrrrrr";

}
