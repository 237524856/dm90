#include "custom_lineeditview.h"
#include <QDebug>
#include "common.h"

custom_lineEditView::custom_lineEditView(QWidget *parent) : QLineEdit(parent)
{

    setTextMargins(20, 1, 1 , 1);
}


void custom_lineEditView::focusInEvent(QFocusEvent *e){

    //qDebug()<<"focusInEvent--xxx----";

    QString title_stys = QString("background-color:rgb(255,255,255);color:rgb(175, 175, 175);border-radius: 5px;font-size:%1px;"
                                 "font-family:'Source Han Sans CN Medium';").arg(12*m_Magnification);
    setStyleSheet(title_stys);

    QLineEdit::focusInEvent(e);

}
void custom_lineEditView::focusOutEvent(QFocusEvent *e){

    //qDebug()<<"focusOutEvent---xx---"<<this->text();

    emit lineViewBlockText(this->text());

    QString user_title_stys = QString("background-color:transparent;color:rgb(175, 175, 175);border-radius: 5px;font-size:%1px;"
                                      "font-family:'Source Han Sans CN Medium';border-width:0;border-style:outset").arg(12*m_Magnification);
    this->setStyleSheet(user_title_stys);

    this->setEnabled(false);

    this->lower();

    QLineEdit::focusOutEvent(e);
}

bool custom_lineEditView::eventFilter(QObject *watched, QEvent *event){
    if(event->type() == QEvent::MouseButtonPress){
       if(this != watched){
           if(this->isEnabled()){
                this->setEnabled(false);
           }
       }
    }

    return QWidget::eventFilter(watched,event);
}
