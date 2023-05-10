
#pragma once
#pragma execution_character_set("utf-8")
#include "pd400_yasuoqi_view.h"
#include <QDebug>
#include <QThread>
#include <QBitmap>
#include <QPainter>
#include <QPushButton>
#include <QElapsedTimer>

pd400_yasuoqi_view::pd400_yasuoqi_view(QWidget *parent) : QWidget(parent)
{

//    this->resize(950,630);

//    //设置标题栏隐藏
//    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    m_DM30Timer = new QTimer(this);
    connect(m_DM30Timer, SIGNAL(timeout()), this, SLOT(handleTimeout()));

}

void pd400_yasuoqi_view::createUI(){

    QLabel *back_image_msk = new QLabel(this);
    back_image_msk->resize(this->width(),this->height());
    back_image_msk->move(0,0);
    back_image_msk->setPixmap(QPixmap(":/image/images/pd400_setting_bgimg.png"));
    back_image_msk->setScaledContents(true);

    QLabel *back_image = new QLabel(this);
    back_image->resize(950*m_Magnification,630*m_Magnification);
    back_image->move(33*m_Magnification,44*m_Magnification);
    back_image->setPixmap(QPixmap(":/image/images/pd400_xianzhiqi_bgimg.png"));
    back_image->setScaledContents(true);


    QPalette pa;
    QColor niceBlue(255, 255, 255);
    pa.setColor(QPalette::WindowText,niceBlue);
    QFont font;
    font.setFamily("Source Han Sans CN Medium");
    font.setPixelSize(19*m_Magnification);

    m_yasuoqi_label = new QLabel(back_image);
    m_yasuoqi_label->setGeometry(60*m_Magnification,34*m_Magnification,188*m_Magnification,40*m_Magnification);
    m_yasuoqi_label->setText(tr("压缩器"));
    m_yasuoqi_label->setStyleSheet(QString("color:white;background-color:transparent;font-size:%1px;font-family:'Source Han Sans CN Medium'").arg(30*m_Magnification));

    QPushButton *arrow = new QPushButton(back_image);
    arrow->resize(23*m_Magnification,16*m_Magnification);
    arrow->move(21*m_Magnification,10*m_Magnification);
    arrow->setStyleSheet(
                "QPushButton{border-image:url(:/image/images/pd400_xianfu_arrow.png) 0 46 0 0;}"
                "QPushButton:hover{border-image:url(:/image/images/pd400_xianfu_arrow.png) 0 23 0 23;}"
                "QPushButton:pressed{border-image:url(:/image/images/pd400_xianfu_arrow.png) 0 0 0 46;}"
                );
    arrow->hide();
    arrow->connect(arrow,SIGNAL(clicked()),this,SLOT(hide_click()));

    QString font_stys = QString("color:white;background-color:transparent;font-size:%1px;font-family:'Source Han Sans CN Medium'").arg(15*m_Magnification);

    QVector<QString>titles={tr("限幅阈值"),tr("恢复时间"),tr("启动时间"),tr("压缩比")};
    QVector<QString>numbers={"-80dB","0dB","0ms","1000ms","0ms","200ms","1","20:1"};

//    QString xianfu_slider_stys=QString("QSlider::groove:horizontal{background: rgba(0, 0, 0,100);height:%1px;border: 0px solid #FFFFFF;border-image:url(:/image/images/pd400_right_slider_bgimg.png);}"
//                                       "QSlider::handle:horizontal{width:%2px;height:%2px;margin-top: -%1px;margin-left: 0px;margin-bottom: -%1px;margin-right: 0px;border-image:url(:/image/images/pd400_round.png);}"
//                                       "QSlider::sub-page:horizontal{background-color:rgba(0,0,0,100);border-radius: %1px;border-image:url(:/image/images/pd400_right_addslider_bgimg.png);}").arg(4*m_Magnification).arg(12*m_Magnification);

    for (int j=0;j<2;j++) {

        for (int k =0;k<2;k++) {
            QLabel *fanzhi_label= new QLabel(back_image);
            fanzhi_label->setGeometry(60*m_Magnification + k*448*m_Magnification,98*m_Magnification+112*j*m_Magnification,168*m_Magnification,24*m_Magnification);
            fanzhi_label->setText(titles.at(k+2*j));
            fanzhi_label->setStyleSheet(font_stys);
            m_labels.push_back(fanzhi_label);

            MySlider *xianfu_slider = new MySlider(back_image);
            xianfu_slider->setOrientation(Qt::Horizontal);

            qDebug()<<"kkkkkkkkkk"<<yaxianqi_slider_values.at(3);

            xianfu_slider->setGeometry(60*m_Magnification + k*448*m_Magnification,133*m_Magnification+112*j*m_Magnification,350*m_Magnification,12*m_Magnification);
//            xianfu_slider->setStyleSheet(xianfu_slider_stys);
            xianfu_slider->setObjectName(QString::number(100+k+2*j,10));
            m_sliders.push_back(xianfu_slider);


            QLabel* m_xianfu_current_value= new QLabel(back_image);
            m_xianfu_current_value->setGeometry(420*m_Magnification + k*448*m_Magnification,125*m_Magnification+112*j*m_Magnification,60*m_Magnification,24*m_Magnification);
            m_xianfu_current_value->setText("0dB");
            m_xianfu_current_value->setStyleSheet(font_stys);
            current_value_labels.push_back(m_xianfu_current_value);

            if(k+2*j==0){
                xianfu_slider->setMinimum(0);
                xianfu_slider->setMaximum(80);
                xianfu_slider->setValue(yaxianqi_slider_values.at(0));
                m_xianfu_current_value->setText(QString::number(yaxianqi_slider_values.at(0)-80,10)+"dB");
            }else if(k+2*j==1){
                xianfu_slider->setMinimum(0);
                xianfu_slider->setMaximum(1000);
                xianfu_slider->setValue(yaxianqi_slider_values.at(1));
                m_xianfu_current_value->setText(QString::number(yaxianqi_slider_values.at(1),10)+"ms");
            }else if(k+2*j==2){
                xianfu_slider->setMinimum(0);
                xianfu_slider->setMaximum(200);
                xianfu_slider->setValue(yaxianqi_slider_values.at(2));
                m_xianfu_current_value->setText(QString::number(yaxianqi_slider_values.at(2),10)+"ms");
            }else{
                xianfu_slider->setMinimum(1);
                xianfu_slider->setMaximum(20);
                xianfu_slider->setValue(yaxianqi_slider_values.at(3));
                m_xianfu_current_value->setText(QString::number(yaxianqi_slider_values.at(3),10)+":1");
            }

            connect(xianfu_slider,SIGNAL(valueChanged(int)),this,SLOT(slider_chanege_value(int)));
            connect(xianfu_slider,SIGNAL(sliderReleasedAt(int)),this,SLOT(slider_release_value(int)));

            for (int i= 0;i<2;i++) {
                QLabel *xianfu_num_label= new QLabel(back_image);
                xianfu_num_label->setGeometry(60*m_Magnification +320*i*m_Magnification + k*448*m_Magnification ,158*m_Magnification+112*j*m_Magnification,50*m_Magnification,24*m_Magnification);
                if(i + 2*k + 4*j == 3 || i + 2*k + 4*j == 5)xianfu_num_label->setGeometry(60*m_Magnification +300*i*m_Magnification + k*448*m_Magnification ,158*m_Magnification+112*j*m_Magnification,80*m_Magnification,24*m_Magnification);
                xianfu_num_label->setText(numbers.at(i + 2*k + 4*j));
                xianfu_num_label->setStyleSheet(font_stys);
            }
        }
    }

    //增加一个最大增益
    QLabel *fanzhi_label= new QLabel(back_image);
    fanzhi_label->setGeometry(60*m_Magnification ,98*m_Magnification+112*2*m_Magnification,168*m_Magnification,24*m_Magnification);
    fanzhi_label->setText(tr("最大增益"));
    fanzhi_label->setStyleSheet(font_stys);
    MySlider *maxslider_slider = new MySlider(back_image);
    maxslider_slider->setOrientation(Qt::Horizontal);
    maxslider_slider->setGeometry(60*m_Magnification,133*m_Magnification+112*2*m_Magnification,350*m_Magnification,12*m_Magnification);
    maxslider_slider->setMinimum(1);
    maxslider_slider->setMaximum(48);
//    maxslider_slider->setStyleSheet(xianfu_slider_stys);
    maxslider_slider->setObjectName(QString::number(104,10));
    m_sliders.push_back(maxslider_slider);

    QLabel* m_xianfu_current_value= new QLabel(back_image);
    m_xianfu_current_value->setGeometry(420*m_Magnification,125*m_Magnification+112*2*m_Magnification,60*m_Magnification,24*m_Magnification);
    m_xianfu_current_value->setText("0dB");
    m_xianfu_current_value->setStyleSheet(font_stys);
    current_value_labels.push_back(m_xianfu_current_value);
    for (int i= 0;i<2;i++) {
        QLabel *xianfu_num_label= new QLabel(back_image);
        xianfu_num_label->setGeometry(60*m_Magnification +310*i*m_Magnification ,158*m_Magnification+112*2*m_Magnification,50*m_Magnification,24*m_Magnification);
        if(i==0){
            xianfu_num_label->setText("1dB");
        }else{
            xianfu_num_label->setText("48dB");
        }
        xianfu_num_label->setStyleSheet(font_stys);
    }
    connect(maxslider_slider,SIGNAL(valueChanged(int)),this,SLOT(slider_chanege_value(int)));
    connect(maxslider_slider,SIGNAL(sliderReleasedAt(int)),this,SLOT(slider_release_value(int)));


    QVector<QString>title_btns={tr("重置"),tr("确认")};
    for (int i=0;i<2;i++) {
        custom_button *reset_confirm_btn = new custom_button(back_image);
        reset_confirm_btn->resize(76*m_Magnification,26*m_Magnification);
        reset_confirm_btn->move(338*m_Magnification +i*156*m_Magnification ,555*m_Magnification);
        reset_confirm_btn->setText(title_btns.at(i));
        if(i==0){
            QString reset_confirm_btn_stys=QString("QPushButton{border-image:url(:/image/images/pd400_xianfu_reset.png);font-family:'Source Han Sans CN Medium';color:white;font-size:%1px;}"
                                                   "QPushButton:hover{border-image:url(:/image/images/pd400_xianfu_reset.png);font-family:'Source Han Sans CN Medium';color:rgb(54,54,54);font-size:%1px;}"
                                                   "QPushButton:focus{outline: none;}").arg(15*m_Magnification);
            reset_confirm_btn->setStyleSheet(reset_confirm_btn_stys);
            reset_confirm_btn->connect(reset_confirm_btn,SIGNAL(clicked()),this,SLOT(reset_click()));
        }else{

            QString reset_confirm_btn_stys=QString("QPushButton{border-image:url(:/image/images/pd400_xianfu_confirm.png);font-family:'Source Han Sans CN Medium';color:white;font-size:%1px;}"
                                                   "QPushButton:hover{border-image:url(:/image/images/pd400_xianfu_confirm.png);font-family:'Source Han Sans CN Medium';color:rgb(54,54,54);font-size:%1px;}"
                                                   "QPushButton:focus{outline: none;}").arg(15*m_Magnification);
            reset_confirm_btn->setStyleSheet(reset_confirm_btn_stys);
            reset_confirm_btn->connect(reset_confirm_btn,SIGNAL(clicked()),this,SLOT(confirm_click()));
        }
        m_btns.push_back(reset_confirm_btn);
    }
}

void pd400_yasuoqi_view::refresh_UI(QVector<int> *slider_values){
    //int qq = slider_values->at(0);
}

void pd400_yasuoqi_view::handleTimeout(){

    if(m_slider_current_number==1){
        m_comm_fun.pd400_comp(m_slider_current_value);
        PD400_send_value.insert(QString::number((PARAM_MIC_COMP_START & 0xff) +1,16),m_slider_current_value);
    }else if(m_slider_current_number==2){
        m_comm_fun.pd400_release_comp(m_slider_current_value);
        PD400_send_value.insert(QString::number((PARAM_MIC_COMP_START & 0xff) +4,16),m_slider_current_value);
    }else if(m_slider_current_number==3){
        m_comm_fun.pd400_attack_comp(m_slider_current_value);
        PD400_send_value.insert(QString::number((PARAM_MIC_COMP_START & 0xff) +3,16),m_slider_current_value);
    }else if(m_slider_current_number==4){
        m_comm_fun.pd400_rate_comp(m_slider_current_value);
        PD400_send_value.insert(QString::number(PARAM_MIC_COMP_END & 0xff,16),m_slider_current_value);
    }else if(m_slider_current_number==5){
        m_comm_fun.pd400_maxgain_comp(m_slider_current_value);
        PD400_send_value.insert(QString::number((PARAM_MIC_COMP_START & 0xff) +2,16),m_slider_current_value);
    }
    m_DM30Timer->stop();
}

void pd400_yasuoqi_view::slider_chanege_value(int value){

    MySlider *slider = dynamic_cast<MySlider *>(sender());
    int slider_index =slider->objectName().toInt()-100;
    QLabel *current_value_label = current_value_labels.at(slider_index);
    if(slider_index==0){
        current_value_label->setText(QString::number(value-80,10)+"dB");
        yaxianqi_slider_values.replace(0,value);
        m_slider_current_value = value;
        m_slider_current_number = 1;
        //发送数据
        m_comm_fun.pd400_comp(value);
        //pd400_send_value = value;
        PD400_send_value.insert(QString::number((PARAM_MIC_COMP_START & 0xff) +1,16),value);
    }else if(slider_index==1){//恢复时间
        current_value_label->setText(QString::number(value,10)+"ms");
        yaxianqi_slider_values.replace(2,value);
        m_slider_current_value = value;
        m_slider_current_number = 2;
        //发送数据
        m_comm_fun.pd400_release_comp(value);
        //pd400_send_value = value;
        PD400_send_value.insert(QString::number((PARAM_MIC_COMP_START & 0xff) +4,16),value);

    }else if(slider_index==2){//启动时间
        current_value_label->setText(QString::number(value,10)+"ms");
        yaxianqi_slider_values.replace(1,value);
        m_slider_current_value = value;
        m_slider_current_number = 3;
        //发送数据
        m_comm_fun.pd400_attack_comp(value);
        //pd400_send_value = value;
        PD400_send_value.insert(QString::number((PARAM_MIC_COMP_START & 0xff) +3,16),value);

    }else if(slider_index==3){//压缩比
        current_value_label->setText(QString::number(value,10)+":1");
        yaxianqi_slider_values.replace(3,value);
        m_slider_current_value = value;
        m_slider_current_number = 4;
        //发送数据       
        m_comm_fun.pd400_rate_comp(value);
        //pd400_send_value = value;
        PD400_send_value.insert(QString::number(PARAM_MIC_COMP_END & 0xff,16),value);

    }else if(slider_index==4){
       current_value_label->setText(QString::number(value,10)+"dB");
       yaxianqi_slider_values.replace(4,value);
       m_slider_current_value = value;
       m_slider_current_number = 5;
       m_comm_fun.pd400_maxgain_comp(value);
      // pd400_send_value = value;
       PD400_send_value.insert(QString::number((PARAM_MIC_COMP_START & 0xff) +2,16),value);
    }
    m_DM30Timer->stop();
    m_DM30Timer->start(40);
   qDebug()<<"xxxx"<<slider->objectName()<<value;
}

void pd400_yasuoqi_view::slider_release_value(int value){


    MySlider *slider = dynamic_cast<MySlider *>(sender());
    int slider_index =slider->objectName().toInt()-100;
    QLabel *current_value_label = current_value_labels.at(slider_index);
    QElapsedTimer time;
    time.start();
    while(time.elapsed() < 40)  {//等待200MS接收数据
        if(slider_index==0){

            //发送数据
            m_comm_fun.pd400_comp(value);
            pd400_send_value = value;
        }else if(slider_index==1){//恢复时间

            //发送数据
            m_comm_fun.pd400_release_comp(value);
            pd400_send_value = value;
        }else if(slider_index==2){//启动时间

            //发送数据
            m_comm_fun.pd400_attack_comp(value);
            pd400_send_value = value;
        }else if(slider_index==3){

            //发送数据
            m_comm_fun.pd400_rate_comp(value);
            pd400_send_value = value;
        }else if(slider_index==4){

            m_comm_fun.pd400_maxgain_comp(value);
            pd400_send_value = value;
         }
        //qDebug()<<"kkkkk"<<slider->objectName()<<value;
    }

}

void pd400_yasuoqi_view::confirm_click(){

    emit close_yasuoqi_view();
    QThread::msleep(40);
    m_DM30Timer->stop();
    this->hide();
}

//重置
void pd400_yasuoqi_view::reset_click(){

    for (int i=0;i<5;i++) {
        m_sliders.at(i)->blockSignals(true);

        if(i==0){
           m_sliders.at(i)->setValue(80);
        }else if(i==1){
           m_sliders.at(i)->setValue(200);
        }else if(i==2){
           m_sliders.at(i)->setValue(100);
        }else if(i==3 || i==4){
           m_sliders.at(i)->setValue(1);
        }
        m_sliders.at(i)->blockSignals(false);
    }
    current_value_labels.at(0)->setText("0dB");
    current_value_labels.at(1)->setText("200ms");
    current_value_labels.at(2)->setText("100ms");
    current_value_labels.at(3)->setText("1:1");
    current_value_labels.at(4)->setText("1dB");
    //发数据
    m_comm_fun.pd400_comp_reset();
    //同步
    yaxianqi_slider_values.replace(0,80);
    yaxianqi_slider_values.replace(1,100);
    yaxianqi_slider_values.replace(2,200);
    yaxianqi_slider_values.replace(3,1);
    yaxianqi_slider_values.replace(4,1);

}
void pd400_yasuoqi_view::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QBitmap bmp(this->size());
    bmp.fill();
    QPainter p(&bmp);
    p.setRenderHint(QPainter::Antialiasing);
    p.setRenderHint(QPainter::Antialiasing);
    p.setPen(Qt::NoPen);
    p.setBrush(Qt::black);
    p.drawRoundedRect(bmp.rect(),12,12);
    setMask(bmp);
}
void pd400_yasuoqi_view::hide_click(){

//    emit close_yasuoqi_view();
//    this->hide();
}

void pd400_yasuoqi_view::refresh_ui(int comp_threshold,int comp_attack,int comp_release,int comp_rate,int maxgain){

    //qDebug()<<"jjjkkkkk"<<m_sliders.count()<<m_sliders.at(1)->objectName()<<comp_attack<<comp_threshold;
    for (int i=0;i<5;i++) {
        m_sliders.at(i)->blockSignals(true);

        if(i==0){
            //int change_limit = (comp_threshold-2000) /10 +40;
            m_sliders.at(i)->setValue(comp_threshold);
        }else if(i==1){
           m_sliders.at(1)->setValue(comp_attack);
        }else if(i==2){
            m_sliders.at(i)->setValue(comp_release);
        }else if(i==3){
            m_sliders.at(i)->setValue(comp_rate);
        }else if(i==4){
            m_sliders.at(i)->setValue(maxgain);
        }
        m_sliders.at(i)->blockSignals(false);
    }
    current_value_labels.at(0)->setText(QString::number(comp_threshold-80,10)+"dB");
    current_value_labels.at(1)->setText(QString::number(comp_attack,10)+"ms");
    current_value_labels.at(2)->setText(QString::number(comp_release,10)+"ms");
    current_value_labels.at(3)->setText(QString::number(comp_rate,10)+":1");
    current_value_labels.at(4)->setText(QString::number(maxgain,10)+"dB");
}

void pd400_yasuoqi_view::changelanguage(){

//    m_yasuoqi_label->setText(tr("压缩器"));
//    QVector<QString>titles={tr("限幅阈值"),tr("恢复时间"),tr("启动时间"),tr("压缩比")};
//    for (int i=0;i<4;i++) {
//        m_labels.at(i)->setText(titles.at(i));
//    }
//    m_btns.at(0)->setText(tr("重置"));
//    m_btns.at(1)->setText(tr("确认"));
}
