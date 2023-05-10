#include "dm90_custom_switch.h"


dm90_custom_switch::dm90_custom_switch(QWidget *parent) : QWidget(parent)
{
    this->setAttribute(Qt::WA_StyledBackground,true);
}
void dm90_custom_switch::createUI(){

    for (int i=0;i<2;i++) {

       custom_button *btn = new custom_button(this);
       btn->move(this->width()/2*i,0);
       btn->resize(this->width()/2,this->height());
       i==0?(btn->setText("OFF")):(btn->setText("ON"));
       QString btn_stys = QString("QPushButton{background-color:transparent ;font-family:'Source Han Sans CN Medium';color:white;font-size:%1px;border:0px}"
                                  "QPushButton:pressed{background-color:rgb(255,171,33) ;font-family:'Source Han Sans CN Medium';color:white;font-size:%1px;border:0px}"
                                  "QPushButton:checked{background-color:rgb(255,171,33) ;font-family:'Source Han Sans CN Medium';color:white;font-size:%1px;border:0px}"
                                  ).arg(10*m_Magnification);
       btn->setStyleSheet(btn_stys);
       btn->setObjectName(QString::number(i,10));
       btn->setCheckable(true);
       connect(btn,SIGNAL(clicked()),this,SLOT(switch_click()));
       m_switchBtns.push_back(btn);
    }
    m_switchBtns.at(0)->setChecked(true);
    m_switchBtns.at(1)->setChecked(false);

}


void dm90_custom_switch::refreshUI(){

    for (auto btn:m_switchBtns) {
        btn->setChecked(false);
    }
//    m_switchBtns.at(0)->setChecked(false);
//    m_switchBtns.at(1)->setChecked(false);
    if(GET_DEVICE_DATA->dm90_LED_OnOff==0){//打开
       m_switchBtns.at(0)->setChecked(true);
    }else{
       m_switchBtns.at(1)->setChecked(true);
    }
}

void dm90_custom_switch::setStyleBtn(int btn_style){

    m_switchBtns.at(0)->setChecked(false);
    m_switchBtns.at(1)->setChecked(false);
    m_switchBtns.at(btn_style)->setChecked(true);
}

void dm90_custom_switch::switch_click(){

    m_switchBtns.at(0)->setChecked(false);
    m_switchBtns.at(1)->setChecked(false);
    custom_button *btn = static_cast<custom_button *>(sender());
    btn->setChecked(true);
    emit lightswitchClick(btn->objectName().toUInt());
}
