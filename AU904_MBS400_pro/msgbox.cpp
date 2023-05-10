
#pragma once
#pragma execution_character_set("utf-8")
#include "msgbox.h"


MsgBox::MsgBox(int style,QString text)
{
    this->resize(201,125);

  //获取主界面的宽度
    int width = this->width();
    int height = this->height();

    QLabel *back_image = new QLabel(this);
    back_image->resize(width,height);
    back_image->move(0,0);
    back_image->setPixmap(QPixmap(":/image/images/mbs400_dialog_backimage.png"));

    setStyleSheet("QWidget{background-color: rgba(255,255,255,0);}");
  //初始化为未按下鼠标左键

  //设置标题栏隐藏
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);


    askLabel = new QLabel(this);
    askLabel->setGeometry(0,9,width,42);
    askLabel->setAlignment(Qt::AlignCenter);


    m_lenedit = new QLineEdit(this);
    m_lenedit->setGeometry(18,57,171,20);
    m_lenedit->setStyleSheet("border-image:url(:/image/images/mbs400_text_backimage.png)");
    m_lenedit->setPlaceholderText("标题");
    m_lenedit->setTextMargins(5,0,0,0);

    okBtn=new QPushButton(this);
//    if(style==3)
        okBtn->setGeometry(width -100,height-35,100,35);

//    else
//        okBtn->setGeometry(135,119,85,30);

    okBtn->setStyleSheet("QPushButton{border-image:url(:/image/images/mbs400_dialog_configbtn.png)}");

    cancleBtn=new QPushButton(this);
    cancleBtn->setGeometry(0,height-35,100,35);
    cancleBtn->setText(QString("取消"));
    //cancleBtn->setStyleSheet("QPushButton{border:1px groove gray;border-radius:4px;border-color: rgb(139,159,185);}QPushButton:hover{background-color: rgb(119,146,183);}QPushButton:pressed{background-color: rgb(139,159,185);}");
    if(style == 3){

        m_lenedit->show();
        askLabel->setText("保存设置\n\n请为此输入名称");
        okBtn->setText(QString("保存"));
    }
       // cancleBtn->show();
    else if(style == 2){
        m_lenedit->hide();
        askLabel->setText("提示\n\n确认删除预设音效吗？");
        okBtn->setText(QString("确认"));
    }else if(style == 1){
        m_lenedit->show();
        askLabel->setText("修改名称\n\n请为此输入名称");
        okBtn->setText(QString("确认"));
    }else if(style == 4){
        m_lenedit->hide();
        askLabel->setText("提示\n\n已经存在相同的音效");
        okBtn->setText(QString("确认"));
    }
        //cancleBtn->hide();

    connect(okBtn,SIGNAL(clicked()),this,SLOT(okBtn_press()));
    //connect(closeBtn,SIGNAL(clicked()),this,SLOT(closeBtn_press()));
    connect(cancleBtn,SIGNAL(clicked()),this,SLOT(cancleBtn_press()));
}

MsgBox::~MsgBox(void)
{
}



void MsgBox::mousePressEvent(QMouseEvent *qevent)
{
    if(qevent->button() == Qt::LeftButton)
    {

    }
}
void MsgBox::mouseMoveEvent(QMouseEvent *qevent)
{
}
void MsgBox::mouseReleaseEvent(QMouseEvent *qevent)
{
    //设置鼠标为未被按下
    mouse_press = false;
}

void MsgBox::okBtn_press()
{
    this->accept();
}
void MsgBox::cancleBtn_press()
{
     this->reject();
}
void MsgBox::closeBtn_press()
{
    close();
}

