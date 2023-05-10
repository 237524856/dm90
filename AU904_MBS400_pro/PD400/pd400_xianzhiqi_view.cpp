
#pragma once
#pragma execution_character_set("utf-8")
#include "pd400_xianzhiqi_view.h"
#include <QBitmap>
#include <QPainter>
#include <QPushButton>
#include <QDebug>
#include <QThread>
#include <QElapsedTimer>

pd400_xianzhiqi_view::pd400_xianzhiqi_view(QWidget *parent) : QWidget(parent)
{

    //this->resize(950,630);
    m_DM30Timer = new QTimer(this);
    connect(m_DM30Timer, SIGNAL(timeout()), this, SLOT(handleTimeout()));

}


void pd400_xianzhiqi_view::createUI(){

    m_send_value=0;
    m_diffsend_value = 0;

    //设置标题栏隐藏
    //this->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
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

    //this->setStyleSheet("background-color:rgba(255,255,100)");
    QPalette pa;
    QColor niceBlue(255, 255, 255);
    pa.setColor(QPalette::WindowText,niceBlue);
    QFont font;
    font.setFamily("Source Han Sans CN Medium");
    font.setPixelSize(19*m_Magnification);

    m_xianzhiqi_label = new QLabel(back_image);
    m_xianzhiqi_label->setGeometry(60*m_Magnification,34*m_Magnification,168*m_Magnification,40*m_Magnification);
    m_xianzhiqi_label->setText(tr("限制器"));
    QString xianzhiqi_label_stys=QString("color:white;background-color:transparent;font-size:%1px;font-family:'Source Han Sans CN Medium'").arg(30*m_Magnification);
    m_xianzhiqi_label->setStyleSheet(xianzhiqi_label_stys);

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

    m_fanzhi_label= new QLabel(back_image);
    m_fanzhi_label->setGeometry(60*m_Magnification,98*m_Magnification,198,24*m_Magnification);
    m_fanzhi_label->setText(tr("限幅阈值"));
    m_fanzhi_label->setStyleSheet(QString("color:white;background-color:transparent;font-size:%1px;font-family:'Source Han Sans CN Medium'").arg(15*m_Magnification));


    MySlider *xianfu_slider = new MySlider(back_image);
    xianfu_slider->setOrientation(Qt::Horizontal);
    xianfu_slider->setMinimum(0);
    xianfu_slider->setMaximum(40);
    xianfu_slider->setValue(xianfuqi_slider_values.at(0));
    xianfu_slider->setGeometry(60*m_Magnification,133*m_Magnification,350*m_Magnification,12*m_Magnification);
//    QString xianfu_slider_stys=QString("QSlider::groove:horizontal{background: rgba(0, 0, 0,100);height:%1px;border: 0px solid #FFFFFF;border-image:url(:/image/images/pd400_right_slider_bgimg.png);}"
//                                       "QSlider::handle:horizontal{width:%2px;height:%2px;margin-top: -%1px;margin-left: 0px;margin-bottom: -%1px;margin-right: 0px;border-image:url(:/image/images/pd400_round.png);}"
//                                       "QSlider::sub-page:horizontal{background-color:rgba(0,0,0,100);border-radius: %1px;border-image:url(:/image/images/pd400_right_addslider_bgimg.png);}").arg(4*m_Magnification).arg(12*m_Magnification);
//    xianfu_slider->setStyleSheet(xianfu_slider_stys);
    connect(xianfu_slider,SIGNAL(valueChanged(int)),this,SLOT(xianfu_chanege_value(int)));
    connect(xianfu_slider,SIGNAL(sliderReleasedAt(int)),this,SLOT(xianfu_release_value(int)));
    m_sliders.push_back(xianfu_slider);


    m_xianfu_current_value= new QLabel(back_image);
    m_xianfu_current_value->setGeometry(420*m_Magnification,125*m_Magnification,50*m_Magnification,24*m_Magnification);
    m_xianfu_current_value->setText(QString::number(xianfuqi_slider_values.at(0)-40,10)+"dB");
    m_xianfu_current_value->setStyleSheet(QString("color:white;background-color:transparent;font-size:%1px;font-family:'Source Han Sans CN Medium'").arg(15*m_Magnification));


    QString font_stys = QString("color:white;background-color:transparent;font-size:%1px;font-family:'Source Han Sans CN Medium'").arg(15*m_Magnification);

    QVector<QString>fazhinums={"-40dB","0dB"};
    for (int i= 0;i<2;i++) {
        QLabel *xianfu_num_label= new QLabel(back_image);
        xianfu_num_label->setGeometry(60*m_Magnification +320*i*m_Magnification ,158*m_Magnification,50*m_Magnification,24*m_Magnification);
        xianfu_num_label->setText(fazhinums.at(i));
        xianfu_num_label->setStyleSheet(font_stys);
    }


    m_qidongtime_label= new QLabel(back_image);
    m_qidongtime_label->setGeometry(508*m_Magnification,98*m_Magnification,168*m_Magnification,24*m_Magnification);
    m_qidongtime_label->setText(tr("启动时间"));
    m_qidongtime_label->setStyleSheet(font_stys);

    MySlider *qidongtime_slider = new MySlider(back_image);
    qidongtime_slider->setOrientation(Qt::Horizontal);
    qidongtime_slider->setMinimum(0);
    qidongtime_slider->setMaximum(200);
    qidongtime_slider->setValue(xianfuqi_slider_values.at(1));
    qidongtime_slider->setGeometry(508*m_Magnification,133*m_Magnification,350*m_Magnification,12*m_Magnification);

//    qidongtime_slider->setStyleSheet(xianfu_slider_stys);
    connect(qidongtime_slider,SIGNAL(valueChanged(int)),this,SLOT(qidong_chanege_value(int)));
    connect(qidongtime_slider,SIGNAL(sliderReleasedAt(int)),this,SLOT(qidong_release_value(int)));
    m_sliders.push_back(qidongtime_slider);

    m_qidong_current_value = new QLabel(back_image);
    m_qidong_current_value->setGeometry(868*m_Magnification,125*m_Magnification,50*m_Magnification,24*m_Magnification);
    m_qidong_current_value->setText(QString::number(xianfuqi_slider_values.at(1),10)+"ms");
    m_qidong_current_value->setStyleSheet(font_stys);

    fazhinums={"0ms","200ms"};
    for (int i= 0;i<2;i++) {
        QLabel *qidong_num_label= new QLabel(back_image);
        qidong_num_label->setGeometry(508*m_Magnification +310*i*m_Magnification ,158*m_Magnification,50*m_Magnification,24*m_Magnification);
        qidong_num_label->setText(fazhinums.at(i));
        qidong_num_label->setStyleSheet(font_stys);
    }

    m_huifutime_label= new QLabel(back_image);
    m_huifutime_label->setGeometry(508*m_Magnification,210*m_Magnification,168*m_Magnification,24*m_Magnification);
    m_huifutime_label->setText(tr("恢复时间"));
    m_huifutime_label->setStyleSheet(font_stys);

    MySlider *huifutime_slider = new MySlider(back_image);
    huifutime_slider->setOrientation(Qt::Horizontal);
    huifutime_slider->setMinimum(0);
    huifutime_slider->setMaximum(500);
    huifutime_slider->setValue(xianfuqi_slider_values.at(2));
    huifutime_slider->setGeometry(508*m_Magnification,(133+112)*m_Magnification,350*m_Magnification,12*m_Magnification);
//    huifutime_slider->setStyleSheet(xianfu_slider_stys);
    connect(huifutime_slider,SIGNAL(valueChanged(int)),this,SLOT(huifu_chanege_value(int)));
    connect(huifutime_slider,SIGNAL(sliderReleasedAt(int)),this,SLOT(huifu_release_value(int)));
    m_sliders.push_back(huifutime_slider);

    m_huifu_current_value= new QLabel(back_image);
    m_huifu_current_value->setGeometry(868*m_Magnification,(125+112)*m_Magnification,50*m_Magnification,24*m_Magnification);
    m_huifu_current_value->setText(QString::number(xianfuqi_slider_values.at(2),10)+"ms");
    m_huifu_current_value->setStyleSheet(font_stys);

    fazhinums={"0ms","500ms"};
    for (int i= 0;i<2;i++) {
        QLabel *huifu_num_label= new QLabel(back_image);
        huifu_num_label->setGeometry(508*m_Magnification +310*i*m_Magnification ,(158+112)*m_Magnification,50*m_Magnification,24*m_Magnification);
        huifu_num_label->setText(fazhinums.at(i));
        huifu_num_label->setStyleSheet(font_stys);
    }

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




//确认
void pd400_xianzhiqi_view::confirm_click(){

    emit close_xianfuqi_view();
    QThread::msleep(40);
    m_DM30Timer->stop();
    this->hide();
}
//重置
void pd400_xianzhiqi_view::reset_click(){

    for (int i=0;i<3;i++) {
        m_sliders.at(i)->blockSignals(true);

        if(i==0){
            m_sliders.at(i)->setValue(40);
        }else if(i==1){
            m_sliders.at(i)->setValue(100);
        }else{
            m_sliders.at(i)->setValue(200);
        }
        m_sliders.at(i)->blockSignals(false);
    }
    m_xianfu_current_value->setText(QString::number(0,10)+"dB");
    m_qidong_current_value->setText(QString::number(100,10)+"ms");
    m_huifu_current_value->setText(QString::number(200,10)+"ms");
    //发送数据
    m_comm_fun.pd400_limiter_reset();
    //同步
    xianfuqi_slider_values.replace(0,40);
    xianfuqi_slider_values.replace(1,100);
    xianfuqi_slider_values.replace(2,200);
}

void pd400_xianzhiqi_view::handleTimeout(){

    if(m_slider_current_number==1){
        m_comm_fun.pd400_limiter(m_slider_current_value);
        PD400_send_value.insert(QString::number((PARAM_MIC_LIMIT_START & 0xff)+ 1,16),m_slider_current_value);
    }else if(m_slider_current_number==2){
        m_comm_fun.pd400_attack_limiter(m_slider_current_value);
        PD400_send_value.insert(QString::number((PARAM_MIC_LIMIT_START & 0xff) + 2,16),m_slider_current_value);
    }else if(m_slider_current_number==3){
        m_comm_fun.pd400_release_limiter(m_slider_current_value);
        PD400_send_value.insert(QString::number((PARAM_MIC_LIMIT_START & 0xff) + 3,16),m_slider_current_value);
    }
    m_DM30Timer->stop();
}
//阈值
void pd400_xianzhiqi_view::xianfu_chanege_value(int xianfu_value){

    m_xianfu_current_value->setText(QString::number(xianfu_value-40,10)+"dB");
    m_send_value= xianfu_value;
    xianfuqi_slider_values.replace(0,xianfu_value);
    m_slider_current_value = xianfu_value;
    m_slider_current_number = 1;
    m_comm_fun.pd400_limiter(xianfu_value);
    //pd400_send_value = xianfu_value;
    PD400_send_value.insert(QString::number((PARAM_MIC_LIMIT_START & 0xff)+ 1,16),xianfu_value);
    m_DM30Timer->stop();
    m_DM30Timer->start(40);
}
void pd400_xianzhiqi_view::xianfu_release_value(int xianfu_value){

//    QElapsedTimer time;
//    time.start();
//    while(time.elapsed() < 40)  {
//        m_comm_fun.pd400_limiter(xianfu_value);
//        pd400_send_value = xianfu_value;
//    }
}
//启动时间
void pd400_xianzhiqi_view::qidong_chanege_value(int qidong_value){
    m_qidong_current_value->setText(QString::number(qidong_value,10)+"ms");
    m_send_value= qidong_value;
    xianfuqi_slider_values.replace(1,qidong_value);
    m_slider_current_value = qidong_value;
    m_slider_current_number = 2;
    m_comm_fun.pd400_attack_limiter(qidong_value);
    //pd400_send_value = qidong_value;
    PD400_send_value.insert(QString::number((PARAM_MIC_LIMIT_START & 0xff) + 2,16),qidong_value);
    m_DM30Timer->stop();
    m_DM30Timer->start(40);
}
void pd400_xianzhiqi_view::qidong_release_value(int qidong_value){
//    QElapsedTimer time;
//    time.start();
//    while(time.elapsed() < 40)  {
//        m_comm_fun.pd400_attack_limiter(qidong_value);
//        pd400_send_value = qidong_value;
//    }
}

//恢复时间
void pd400_xianzhiqi_view::huifu_chanege_value(int huifu_value){

    // QThread::msleep(20);
    m_huifu_current_value->setText(QString::number(huifu_value,10)+"ms");
    m_send_value= huifu_value;
    xianfuqi_slider_values.replace(2,huifu_value);
    //qDebug()<<"xxxxxxx"<<huifu_value;
    m_slider_current_value = huifu_value;
    m_slider_current_number = 3;
    m_comm_fun.pd400_release_limiter(huifu_value);
    //pd400_send_value = huifu_value;
    PD400_send_value.insert(QString::number((PARAM_MIC_LIMIT_START & 0xff) + 3,16),huifu_value);
    m_DM30Timer->stop();
    m_DM30Timer->start(40);
}
void pd400_xianzhiqi_view::huifu_release_value(int huifu_value){

//    QElapsedTimer time;
//    time.start();
//    while(time.elapsed() < 40)  {
//        m_comm_fun.pd400_release_limiter(huifu_value);
//        pd400_send_value = huifu_value;
//    }
}

void pd400_xianzhiqi_view::paintEvent(QPaintEvent *event)
{
    //    Q_UNUSED(event);
    //    QBitmap bmp(this->size());
    //    bmp.fill();
    //    QPainter p(&bmp);
    //    p.setRenderHint(QPainter::Antialiasing);
    //    p.setRenderHint(QPainter::Antialiasing);
    //    p.setPen(Qt::NoPen);
    //    p.setBrush(Qt::black);
    //    p.drawRoundedRect(bmp.rect(),12,12);
    //    setMask(bmp);
}

void pd400_xianzhiqi_view::hide_click(){
    emit close_xianfuqi_view();

    this->hide();
}
//获取背景参数刷新UI
void pd400_xianzhiqi_view::refresh_ui(int limit,int limit_attack,int limit_release){

    for (int i=0;i<3;i++) {
        m_sliders.at(i)->blockSignals(true);
        if(i==0){
           //int change_limit = (limit-2000) /10 +40;
           m_sliders.at(i)->setValue(limit);

        }else if(i==1){
           m_sliders.at(i)->setValue(limit_attack);
        }else{
           m_sliders.at(i)->setValue(limit_release);
        }
        m_sliders.at(i)->blockSignals(false);
    }
    m_xianfu_current_value->setText(QString::number(limit-40,10)+"dB");
    m_qidong_current_value->setText(QString::number(limit_attack,10)+"ms");
    m_huifu_current_value->setText(QString::number(limit_release,10)+"ms");
}

void pd400_xianzhiqi_view::changelanguage(){

    m_xianzhiqi_label->setText(tr("限制器"));
    m_fanzhi_label->setText(tr("限幅阈值"));
    m_qidongtime_label->setText(tr("启动时间"));
    m_huifutime_label->setText(tr("恢复时间"));
    m_btns.at(0)->setText(tr("重置"));
    m_btns.at(1)->setText(tr("确认"));

}
