
#pragma once
#pragma execution_character_set("utf-8")
#include "custom_item.h"
#include "common.h"
#include <QDebug>
#include <QPushButton>

custom_item::custom_item(QWidget *parent) : QWidget(parent)
{
    this->setAttribute(Qt::WA_StyledBackground,true);
    //this->setFocusPolicy(Qt::ClickFocus);
    //this->setStyleSheet("background:rgb(255,0,0);border-radius: 0px;");

    m_item_title = new custom_lineEditView(this);
    QString user_title_stys = QString("background-color:transparent;color:rgb(175, 175, 175);border-radius: 5px;font-size:%1px;"
                                      "font-family:'Source Han Sans CN Medium';border-width:0;border-style:outset").arg(12*m_Magnification);
    m_item_title->move(0*m_Magnification,0);
    m_item_title->resize(140*m_Magnification,34*m_Magnification);
    m_item_title->setObjectName("custom_lineEditView");
    m_item_title->setStyleSheet(user_title_stys);
    m_item_title->setEnabled(false);

    connect(m_item_title, &QLineEdit::returnPressed, [=]() {  //输入工况点后，回车，enter，触发信号，进行创建 对应个数的spro文件

    });

    QVector<QString>itemBtnName = {"share","edit","save"};
    QVector<QString>imgs = {"QPushButton{border-image:url(:/image/images/DM90/preset_share)}"
                            "QPushButton:hover{border-image:url(:/image/images/DM90/preset_share_hover)}"
                            "QPushButton:focus{outline: none;}",
                            "QPushButton{border-image:url(:/image/images/DM90/preset_edit)}"
                            "QPushButton:hover{border-image:url(:/image/images/DM90/preset_edit_hover)}"
                            "QPushButton:focus{outline: none;}",
                            "QPushButton{border-image:url(:/image/images/DM90/preset_save)}"
                            "QPushButton:hover{border-image:url(:/image/images/DM90/preset_save_hover)}"
                            "QPushButton:focus{outline: none;}"};
    for (int i=0;i<3;i++) {
        QPushButton *btn = new QPushButton(this);
        btn->resize(12*m_Magnification,12*m_Magnification);
        btn->move(70*m_Magnification+25*i*m_Magnification,(25*m_Magnification-btn->height())/2+6);
        btn->hide();
        btn->setStyleSheet(imgs.at(i));
        btn->setObjectName(itemBtnName.at(i));
        backImgs.push_back(btn);

        connect(btn,SIGNAL(clicked()),this,SLOT(itemBtn_click()));
    }
}

void custom_item::setTitleName(const QString &str){

    m_item_title->setText(str);
}
void custom_item::mousePressEvent(QMouseEvent *qevent){

    if(qevent->type()==QEvent::MouseButtonDblClick) {

    }else{

        emit item_click(this->objectName());
        m_item_isSelect =true;
        this->setStyleSheet("background:rgb(45,45,45);border-radius: 0px;");
    }

}


void custom_item::setClickSelect(bool isClick){

    if(isClick){
        this->setStyleSheet("background:rgb(45,45,45);border-radius: 0px;");
    }else{

    }

}

void custom_item::mouseReleaseEvent(QMouseEvent *qevent){

}
void custom_item::mouseMoveEvent(QMouseEvent *qevent){

}

void custom_item::enterEvent(QEvent *){

    for (int i=0;i<backImgs.count();i++) {
        backImgs.at(i)->show();
    }
    if(!m_item_isSelect)
        this->setStyleSheet("background:rgb(22,51,83);border-radius: 0px;");

    emit item_enter(this->objectName());

}
void custom_item::leaveEvent(QEvent *){

    for (int i=0;i<backImgs.count();i++) {
        backImgs.at(i)->hide();
    }
    if(!m_item_isSelect)
        this->setStyleSheet("background:transparent;border-radius: 0px;");

}

void custom_item::itemBtn_click(){

    QPushButton *btn = dynamic_cast<QPushButton *>(sender());

    emit item_btnClick(m_item_title->text(),btn->objectName());

    if(btn->objectName()=="share"){

    }else if(btn->objectName()=="edit"){

        m_item_title->setEnabled(true);
        m_item_title->raise();
        m_item_title->setFocus();
        qDebug()<<"itemBtn_click-----";

    }else if(btn->objectName()=="save"){//保存按钮


    }

}
