#include "myspinbutton.h"
#include <QPainter>
#include <QMouseEvent>
#include <math.h>
#include <QDebug>
#include <QGraphicsDropShadowEffect>
#include "../common.h"


MySpinButton::MySpinButton(QWidget *parent) :
    QWidget(parent),
    m_title_name("Treble")
{
    setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    degree = mybaseAngle;
    this->setAttribute(Qt::WA_StyledBackground,true);
    m_select_index = 0;//表示没有选中
    different_number = 0;

    different_low_number =0;
    different_mid_number =0;
    different_hi_number =0;
    //this->setStyleSheet("background-color: rgb(255,255, 0)");
    m_max_number = 0;
    m_min_number = 0;

}
void MySpinButton::set_mask_view(){

    m_low_left_mask_image =new QLabel(this);
    m_low_left_mask_image->resize(46,83);
    m_low_left_mask_image->move(3,2);
    m_low_left_mask_image->setPixmap(QPixmap(":/image/images/mbs400_half_cricle_right.png"));
    m_low_left_mask_image->hide();


}
MySpinButton::~MySpinButton()
{
}

void MySpinButton::setDegree(int deg)
{

    if(deg> 100) deg = 100;
    if(deg < 0) deg = 0;
    this->degree = deg*2.8-50;
//    if(m_select_index==1){

//    }else{
//        if(deg> 240) deg = 240;
//        if(deg < 0) deg = 0;

//        this->degree = deg*2.8-50;
//        //send_mess_value =(int)(50+degree)/2.8*2.4 -120;
//    }
    this->update();
}
void MySpinButton::set_whirlbuttonHeigth(int heigthvalue){

    m_whirlbuttonHeigth = heigthvalue;
    this->update();
}
void MySpinButton::set_whirlbuttonWidth(int widthvalue){

    m_whirlbuttonWidth = widthvalue;
    this->update();
}
void MySpinButton::set_backimage(QString image_str){
    m_backimagestr = image_str;
    this->update();
}
void MySpinButton::set_inner_circle_width(int innerwidth){
    m_inner_circle_width = innerwidth;
    this->update();

}
void MySpinButton::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    //反走样
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    QTransform tranform1;
    int originY = m_whirlbuttonHeigth/2;
    int originX = m_whirlbuttonWidth/2;
    tranform1.translate(originX,originY);
    tranform1.rotate(degree);
    tranform1.translate(-(originX),-(originY));
    painter.setWorldTransform(tranform1);

    painter.setRenderHint(QPainter:: Antialiasing, true);  //设置渲染,启动反锯齿
    QPixmap map;
    map.load(QString(m_backimagestr));
//  QRect rec = QRect(originX-34,originY-34,68,68);
    QRect rec = QRect(originX-m_inner_circle_width/2,originY-m_inner_circle_width/2,m_inner_circle_width,m_inner_circle_width);
    painter.drawPixmap(rec,map);

    QColor niceBlue(0, 255, 0);
    //    //painter.setPen(thinPen);

    if(m_select_index == 1){
       painter.setPen(QPen(QColor(245,179,53),2));
    }else{
       painter.setPen(QPen(QColor(245,179,53),1.5));
    }

    QRectF rect(originX-(m_whirlbuttonWidth-10)/2 ,originY-(m_whirlbuttonHeigth-10)/2, m_whirlbuttonWidth-10, m_whirlbuttonHeigth-10);

    double myqq;
    if(degree < mybaseAngle){
        myqq = degree + (360-mybaseAngle);
    }else{

        myqq = (360-mybaseAngle)+degree-360;

    }
    painter.drawArc(rect, 179 * 16, (myqq) * 16);

    unsigned rx = 30,ry = 8;


    if(m_select_index == 1){//VOLUME声音的发送



        int send_mess_value;
        if(degree>230){
            send_mess_value =(int)(50-(360-degree))/2.8;
        }else{
            send_mess_value =(int)(50+degree)/2.8;
        }
        if(different_number==send_mess_value){
            return;
        }
        emit volume_click();
        PACKDATA send = {  };
        send.command = 0x4b;
        send.index[0] = 0xfc;

        send.index[4] = 0x77;
        send.index[6] =(uint8_t) send_mess_value;

        m_sendMsgs.push_back(send);

        different_number=send_mess_value;
        //qDebug()<<"kkk--------------------"<<send_mess_value<<degree;
    }else if(m_select_index == 2){//低音
        int send_mess_value;
        if(degree>230){
            send_mess_value =(int)(50-(360-degree))/2.8 *2.4 -120;
        }else{
            send_mess_value =(int)(50+degree)/2.8*2.4 -120;
        }
        if(different_low_number==send_mess_value){
            return;
        }
//        qDebug()<<degree<<"hhhhhhh";
        BassEQGain = send_mess_value;
        send_lowmidhigh_mess(send_mess_value);
        different_low_number=send_mess_value;

    }else if(m_select_index == 3){//中音
        int send_mess_value;
        if(degree>230){
            send_mess_value =(int)(50-(360-degree))/2.8 *2.4 -120;
        }else{
            send_mess_value =(int)(50+degree)/2.8*2.4 -120;
        }
        if(different_mid_number==send_mess_value){
            return;
        }
        MidEQGain = send_mess_value;
        send_lowmidhigh_mess(send_mess_value);
        different_mid_number=send_mess_value;

    }else if(m_select_index == 4){//高音
        int send_mess_value;
        if(degree>230){
            send_mess_value =(int)(50-(360-degree))/2.8 *2.4 -120;
        }else{
            send_mess_value =(int)(50+degree)/2.8*2.4 -120;
        }
        if(different_hi_number==send_mess_value){
            return;
        }
        TrebleEQGain = send_mess_value;
        send_lowmidhigh_mess(send_mess_value);
        different_hi_number=send_mess_value;
    }


}
void MySpinButton::send_lowmidhigh_mess(int different_number){

    if(degree>=90 &&degree<=230){
        m_low_left_mask_image->show();
    }else{
        m_low_left_mask_image->hide();
    }

    PACKDATA send = {  };
    send.command = 0x4b;
    send.index[0] = 0xFB;

    send.index[4] = 0x77;
    send.index[6] = BassEQGain;
    send.index[7] = MidEQGain;
    send.index[8] = TrebleEQGain;

    m_sendMsgs.push_back(send);

    qDebug()<<"kkk-----myspinbutton---------------"<<different_number;
}
//QSize MySpinButton::sizeHint() const
//{
//    QSize size = QSize(whirlbuttonHeigth,whirlbuttonWidth);
//    return size;
//}
//QSize MySpinButton::minimumSizeHint() const
//{
//    QSize size = QSize(whirlbuttonHeigth,whirlbuttonWidth);
//    return size;
//}
void MySpinButton::mousePressEvent(QMouseEvent *event)
{
    m_min_number = 1;
    m_max_number = 1;
    mouseToPosion(event);
    this->update();
}
int MySpinButton :: getDegree()
{
    return(degree);
}
void MySpinButton::mouseMoveEvent(QMouseEvent * event)
{

    mouseToPosion(event);
    this->update();

}
void MySpinButton::mouseReleaseEvent(QMouseEvent * event)
{
    mouseToPosion(event);
    this->updateDegree();
}
void MySpinButton:: mouseToPosion(QMouseEvent * event)
{
    QPoint currentPoint = event->pos();
    double pi = 3.1415927;
    double  x = currentPoint.x() - m_whirlbuttonHeigth/2;
    double  y = currentPoint.y() - m_whirlbuttonWidth/2;



    if(x<0 &&y<0){

    }else if(x>0 &&y <0){

    }else if(x>0 && y>=0){
        int max_value = atan(y/x)*180/pi;
        if(max_value>51){
            m_max_number=2;
        }else{
            m_max_number = 1;
        }

        //qDebug()<<"xxxxx"<<x<<"yyyyy"<<y<<kk<<endl;


        }else if (x<0 && y> 0) {//第四区域

            int min_value = atan(y/x)*180/pi;
            if(min_value <-51){
                m_min_number=2;
            }else{
                m_min_number = 1;
            }
            //qDebug()<<"xxxxx"<<x<<"yyyyy"<<y<<min_value<<endl;
        }

if(m_max_number>1)return;
if(m_min_number>1)return;
// qDebug()<<"xxxxx"<<x<<"yyyyy"<<y<<degree<<endl;


    if((x ==0)&&(y<=0)){
        degree = 90;
    }

    else if((x==0)&&(y>0)){
        degree = 270;
    }

    else if(x>0){
        degree = atan(y/x)*180/pi + 180;
        //qDebug()<<"kkk----------111----------"<<endl;

    }

    else if(y>0){
        degree = atan(y/x)*180/pi + 360;
        //qDebug()<<"kkk----------22222----------"<<endl;
    }

    else{
        degree = atan(y/x)*180/pi;
        //qDebug()<<"kkk----------3333----------"<<endl;
    }

    if (degree >= 230){
        if(x >= 0){
            degree = 230;
            //qDebug()<<"kkk--------------------"<<endl;
        }

        else{
           // degree = -10;
            //qDebug()<<"jjjjj----2222222------------"<<degree<<endl;
        }

    }

//qDebug()<<"xxxxx"<<x<<y<<atan(y/x)*180/pi<<endl;
    if (degree <= mybaseAngle){
        if(y >0 && x < 0)
        degree = mybaseAngle;
    }

}
