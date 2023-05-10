
#pragma once
#pragma execution_character_set("utf-8")
#include "mbs400_back_rightview.h"
#include "myspinbutton.h"
#include "switchcontrol.h"
#include <QDebug>
#include <QFileDialog>
#include <QApplication>
#include "../common.h"
#include <QFile>
#include <QStandardPaths>

mbs400_back_rightview::mbs400_back_rightview(QWidget *parent) : QWidget(parent)
{

    m_reset_sliderValues = {120,120,120,120,120,120,120,120,120,120};
    m_EQaddrs = {0x0087,0x0089,0x008B,0x008D,0x008F,0x0091,0x0093,0x0095,0x0097,0x0099};
}
void mbs400_back_rightview::new_createUI(){

    QLabel *back_right_image = new QLabel(this);
    back_right_image->resize(this->width(),this->height());
    back_right_image->move(0,0);
    back_right_image->setPixmap(QPixmap(":/image/images/mbs400_high_backimage.png"));

    QLabel *hightitle_image = new QLabel(this);
    hightitle_image->resize(460,28);
    hightitle_image->move(6,6);
    hightitle_image->setPixmap(QPixmap(":/image/images/mbs400_high_mode_titleback.png"));

    QFont font;
    font.setFamily("Source Han Sans CN Medium");
    font.setPixelSize(18);
    QPalette pa;
    QColor niceBlue(175, 175, 175);
    pa.setColor(QPalette::WindowText,niceBlue);

    QLabel *high_name = new QLabel(this);
    high_name->resize(460,28);
    high_name->move(26,6);
    font.setPixelSize(16);
    high_name->setText("高级");
    high_name->setPalette(pa);
    high_name->setFont(font);

    QLabel *off_on_name = new QLabel(this);
    off_on_name->resize(130,28);
    off_on_name->move(this->width()-140,6);
    font.setPixelSize(16);
    off_on_name->setText("OFF                  ON");
    off_on_name->setPalette(pa);
    off_on_name->setFont(font);


    SwitchControl *switch_btn = new SwitchControl(this);
    switch_btn->resize(60,30);
    switch_btn->move(365,6);
    connect(switch_btn, SIGNAL(toggled(bool)), this, SLOT(switch_btn(bool)));
    QLabel *EQ_name = new QLabel(this);
    EQ_name->resize(40,28);
    EQ_name->move(26,60);
    font.setPixelSize(16);
    EQ_name->setText("EQ");
    EQ_name->setPalette(pa);
    EQ_name->setFont(font);

    QVector<QString> fun_btn_names = { "QPushButton{border-image:url(:/image/images/mbs400_refresh_btn.png) 0 56 0 0;}"
                                       "QPushButton:hover{border-image:url(:/image/images/mbs400_refresh_btn.png) 0 28 0 28;}"
                                       "QPushButton:pressed{border-image:url(:/image/images/mbs400_refresh_btn.png) 0 0 0 56;}",

                                       "QPushButton{border-image:url(:/image/images/mbs400_saveaudio_btn.png) 0 56 0 0;}"
                                       "QPushButton:hover{border-image:url(:/image/images/mbs400_saveaudio_btn.png) 0 28 0 28;}"
                                       "QPushButton:pressed{border-image:url(:/image/images/mbs400_saveaudio_btn.png) 0 0 0 56;}",
                                     };
    for (int i=0;i<fun_btn_names.count();i++) {
        QPushButton *refresh_save_btn = new QPushButton(this);
        refresh_save_btn->resize(28,28);
        refresh_save_btn->move(350 + 48*i ,57);
        refresh_save_btn->setStyleSheet(fun_btn_names.at(i));
        connect(refresh_save_btn,SIGNAL(clicked()),this,SLOT(refresh_save_click()));
        if(i ==1){
            m_save_btn = refresh_save_btn;
        }
        i==0 ?(refresh_save_btn->setObjectName("refresh_btn")) :(refresh_save_btn->setObjectName("save_btn"));
        //refresh_save_btn->setEnabled(false);
        m_refresh_save_arrs.push_back(refresh_save_btn);
    }

    QVector<QString>db_label_names = {"+12dB","0dB","-12dB"};
    for (int k=0;k<2;k++) {
        for (int i=0;i<db_label_names.count();i ++) {
            QLabel *db_label = new QLabel(this);
            db_label->resize(50,38);
            db_label->move(15 + k*420 ,95 + 108 * i);
            db_label->setText(db_label_names.at(i));
            font.setPixelSize(12);
            db_label->setFont(font);
            db_label->setPalette(pa);
        }
    }

    //创建垂直的滑动条
    for (int i=0;i<10;i++) {
        QVerSlider *verticalSlider = new QVerSlider(this);
        verticalSlider->setOrientation(Qt::Vertical);
        verticalSlider->setMinimum(0);
        verticalSlider->setMaximum(240);
        verticalSlider->setSingleStep(1);
        verticalSlider->setValue(m_right_sliderValues.at(i));
        verticalSlider->setGeometry(50 + 39*i,108,30,226);
        verticalSlider->setObjectName(QString::number(i+10,10));
        if(verticalSlider->value()>120){
            verticalSlider->setStyleSheet(
                        "QSlider::groove:Vertical {background: rgba(255, 255, 0,0);width:4px;border-image:url(:/image/images/mbs400_high_equp.png);}"
                        "QSlider::handle:Vertical {border-radius: 5px;width: 18px;height: 18px;margin-top: 0px;margin-left: -7px;margin-bottom: 0px;margin-right: -7px;border-image:url(:/image/images/mbs400_db_round.png);}"
                        "QSlider::sub-page:Vertical {background: rgba(255, 171, 33,0%);border-radius:2px;border-image:url(:/image/images/mbs400_high_eqback.png);}"
                        );
        }else{//问题
            verticalSlider->setStyleSheet(
                        "QSlider::groove:Vertical {background: rgba(255, 255, 0,0);width:4px;border-image:url(:/image/images/mbs400_high_eqdown.png);}"
                        "QSlider::handle:Vertical {border-radius: 5px;width: 18px;height: 18px;margin-top: 0px;margin-left: -7px;margin-bottom: 0px;margin-right: -7px;border-image:url(:/image/images/mbs400_db_round.png);} "
                        "QSlider::add-page:Vertical {background: rgba(255, 171, 33,0%);border-radius:2px;border-image:url(:/image/images/mbs400_high_eqback.png);}"
                        );
        }
        m_EQ_sliders.push_back(verticalSlider);
        if(i != 9){
            QLabel *sacle_image = new QLabel(this);
            sacle_image->resize(10,221);
            sacle_image->move(80 + 39*i,108);
            sacle_image->setPixmap(QPixmap(":/image/images/mbs400_scale.png"));
        }
        connect(verticalSlider,SIGNAL(valueChanged(int)),this,SLOT(verticalSlider_setValue(int)));
    }

    QVector<QString>bottom_names = {"100","160","250","400","630","1k","2k","4k","8k","16k"};
    for (int i=0;i<bottom_names.count();i++) {
        QLabel *bottom_label = new QLabel(this);
        bottom_label->resize(30,30);
        bottom_label->move(35 + 42*i  ,335);
        font.setPixelSize(13);
        bottom_label->setFont(font);
        //        pa.setColor(QPalette::WindowText,QColor(181,181,181));
        bottom_label->setPalette(pa);
        bottom_label->setText(bottom_names[i]);
    }
    QLabel *limiter_label = new QLabel(this);
    limiter_label->resize(60,50);
    limiter_label->move(35  ,390);
    limiter_label->setPalette(pa);
    font.setPixelSize(14);
    limiter_label->setFont(font);
    limiter_label->setAlignment(Qt::AlignTop);
    limiter_label->setText("Limiter");

    QHorSlider*limiter_slider = new QHorSlider(this);
    limiter_slider->setOrientation(Qt::Horizontal);
    limiter_slider->setMinimum(-240);
    limiter_slider->setMaximum(0);
    limiter_slider->setSingleStep(1);
    limiter_slider->setValue(((background_data_pars.data_LR_lim_loud_mute>>8) & 0xff)-240);
    limiter_slider->setGeometry(120,390,320,22);
    limiter_slider->setStyleSheet(
                "QSlider::groove:horizontal{height:6px;border-image:url(:/image/images/mbs400_limiter.png);}"
                "QSlider::handle:horizontal{width:22px;height:22px;margin-top: -8px;margin-left: 0px;margin-bottom: -8px;margin-right: 0px;border-image:url(:/image/images/mbs400_db_round.png);}"
                "QSlider::sub-page:horizontal{background-color:rgb(243,152,0);border-radius: 2px;border-image:url(:/image/images/mbs400_limiter_origer.png);}"
                );
    connect(limiter_slider,SIGNAL(valueChanged(int)),this,SLOT(limiter_slider_click(int)));
    m_limiter_highlow_slider.push_back(limiter_slider);

    QLabel *limiter_label_number =new QLabel(this);
    limiter_label_number->resize(340,30);
    limiter_label_number->move(120,418);
    limiter_label_number->setAlignment(Qt::AlignTop);
    limiter_label_number->setText("-24                     -18                        -12                        -6                     0");
    font.setPixelSize(12);
    limiter_label_number->setFont(font);
    limiter_label_number->setPalette(pa);

    QLabel *high_label = new QLabel(this);
    high_label->resize(80,250);
    high_label->move(35  ,449);
    high_label->setPalette(pa);
    font.setPixelSize(14);
    high_label->setFont(font);
    high_label->setAlignment(Qt::AlignTop);
    high_label->setText("High-pass\nFilter");

    m_high_slider = new QHorSlider(this);
    m_high_slider->setOrientation(Qt::Horizontal);
    m_high_slider->setMinimum(20);
    m_high_slider->setMaximum(1000);
    m_high_slider->setSingleStep(1);
    m_high_slider->setValue(50);
    m_high_slider->setGeometry(120,449,320,22);
    m_high_slider->setStyleSheet(
                "QSlider::groove:horizontal{height:6px;border-image:url(:/image/images/mbs400_limiter.png);}"
                "QSlider::handle:horizontal{width:22px;height:22px;margin-top: -8px;margin-left: 0px;margin-bottom: -8px;margin-right: 0px;border-image:url(:/image/images/mbs400_db_round.png);}"
                );
    connect(m_high_slider,SIGNAL(valueChanged(int)),this,SLOT(high_pass_click(int)));
    m_limiter_highlow_slider.push_back(m_high_slider);
    QLabel *high_label_number =new QLabel(this);
    high_label_number->resize(340,30);
    high_label_number->move(120,479);
    high_label_number->setAlignment(Qt::AlignTop);
    high_label_number->setText("20               50                  120                  200               500               1k");
    font.setPixelSize(12);
    high_label_number->setFont(font);
    high_label_number->setPalette(pa);

    QLabel *low_label = new QLabel(this);
    low_label->resize(80,250);
    low_label->move(35  ,515);
    low_label->setPalette(pa);
    font.setPixelSize(14);
    low_label->setFont(font);
    low_label->setAlignment(Qt::AlignTop);
    low_label->setText("Low-pass\nFilter");
    m_low_slider = new QHorSlider(this);
    m_low_slider->setOrientation(Qt::Horizontal);
    m_low_slider->setMinimum(1000);
    m_low_slider->setMaximum(20000);
    m_low_slider->setSingleStep(1);
    m_low_slider->setValue(50);
    m_low_slider->setGeometry(120,515,320,22);
    m_low_slider->setStyleSheet(
                "QSlider::groove:horizontal{height:6px;border-image:url(:/image/images/mbs400_limiter.png);}"
                "QSlider::handle:horizontal{width:22px;height:22px;margin-top: -8px;margin-left: 0px;margin-bottom: -8px;margin-right: 0px;border-image:url(:/image/images/mbs400_db_round.png);}"
                );
    connect(m_low_slider,SIGNAL(valueChanged(int)),this,SLOT(low_pass_click(int)));
    m_limiter_highlow_slider.push_back(m_low_slider);
    QLabel *low_label_number =new QLabel(this);
    low_label_number->resize(340,30);
    low_label_number->move(120,541);
    low_label_number->setAlignment(Qt::AlignTop);
    low_label_number->setText("1k      2k     4k                              10k                                               20k");
    font.setPixelSize(12);
    low_label_number->setFont(font);
    low_label_number->setPalette(pa);

    m_high_mask_view = new QLabel(this);
    m_high_mask_view->resize(471,593-36);
    m_high_mask_view->move(0,36);
    m_high_mask_view->setPixmap(QPixmap(":/image/images/mbs400_eq_mask.png"));
    // m_high_mask_view->setVisible(true);
}

//获取完背景参数刷新所有的UI数据
void mbs400_back_rightview::notice_refresh_ui(){

    for (int i =0;i<m_EQ_sliders.count();i++) {

        int8_t spi_low_value = m_receiveMsgs[3+i].index[6];
        QVerSlider *eq_slider = m_EQ_sliders.at(i);
        eq_slider->blockSignals(true);
        eq_slider->setValue(spi_low_value+120);
        eq_slider->blockSignals(false);

        if(eq_slider->value()>120){
            eq_slider->setStyleSheet(
                        "QSlider::groove:Vertical {background: rgba(255, 255, 0,0);width:4px;border-image:url(:/image/images/mbs400_high_equp.png);}"
                        "QSlider::handle:Vertical {border-radius: 5px;width: 18px;height: 18px;margin-top: 0px;margin-left: -7px;margin-bottom: 0px;margin-right: -7px;border-image:url(:/image/images/mbs400_db_round.png);}"
                        "QSlider::sub-page:Vertical {background: rgba(255, 171, 33,0%);border-radius:2px;border-image:url(:/image/images/mbs400_high_eqback.png);}"
                        );
        }else{//问题
            eq_slider->setStyleSheet(
                        "QSlider::groove:Vertical {background: rgba(255, 255, 0,0);width:4px;border-image:url(:/image/images/mbs400_high_eqdown.png);}"
                        "QSlider::handle:Vertical {border-radius: 5px;width: 18px;height: 18px;margin-top: 0px;margin-left: -7px;margin-bottom: 0px;margin-right: -7px;border-image:url(:/image/images/mbs400_db_round.png);} "
                        "QSlider::add-page:Vertical {background: rgba(255, 171, 33,0%);border-radius:2px;border-image:url(:/image/images/mbs400_high_eqback.png);}"
                        );
        }
    }
    //刷新高低通

    uint16_t hpf_value = m_receiveMsgs[13].index[9] <<8 | m_receiveMsgs[13].index[8];
    m_high_slider->setValue(hpf_value);

    uint16_t lpf_value = m_receiveMsgs[14].index[9] <<8 | m_receiveMsgs[14].index[8];
    m_low_slider->setValue(lpf_value);
}
int mbs400_back_rightview::isDigitStr(QString src)
{
    QByteArray ba = src.toLatin1();//QString 转换为 char*
     const char *s = ba.data();

    while(*s && *s>='0' && *s<='9') s++;

    if (*s)
    { //不是纯数字
        return -1;
    }
    else
    { //纯数字
        return 0;
    }
}
void mbs400_back_rightview::refresh_save_click(){

    QPushButton *btn = dynamic_cast<QPushButton *>(sender());
    if(btn->objectName() == "save_btn"){
        MsgBox *msgBox=new MsgBox(3,QString("请输入卡口名或路口名"));//1为警告框
        int nRes=msgBox->exec();
        if(nRes==QDialog::Accepted)//保存文件
        {
            qDebug()<<"按下了确认键"<<msgBox->m_lenedit->text();
            if(msgBox->m_lenedit->text().length() <=0){

                return;
            }
            QString filename = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+"/yangyu.dat";
            QFile file(filename);
            //保存EQ数据之前需要清空上一次的数据
            m_data_list.clear();
            foreach(QSlider *eqslider,m_EQ_sliders){
                m_data_list << QString::number(eqslider->value(),10);
            }
            //保存limiter&high&low的slider数据
            foreach(QSlider *slider,m_limiter_highlow_slider){
                m_data_list << QString::number(slider->value(),10);
            }

           // qDebug()<<"按下了确认键"<<m_slider_map;

            //读之前先要取出来
            if(file.open(QIODevice::ReadOnly)){
                //只读模式
                QDataStream in(&file);
                QVector<QStringList>datasource;
                in >>  datasource;
                if(datasource.count()> 0){
                   m_slider_map =  datasource;
                }
                file.close();
                //qDebug()<<"2222"<<datasource;//6
            }
            //添加到MAP中

            QVector<int>compar_strs;
            QString save_str = msgBox->m_lenedit->text();
            for(int i = 0;i<m_slider_map.count();i++){
                       QStringList item_base =  m_slider_map.at(i);
                       for (int k = 0;k<item_base.count();k++) {
                           QString item_title_base = item_base.at(item_base.count()-1);

                           QStringList list = item_title_base.split("-");
                           QString number_str = list.at(list.count()-1);

                            if(isDigitStr(list.at(list.count()-1)) == 0){//纯数字
                               // qDebug()<<"my写入成功"<<number_str;//6
                                list.removeAt(list.count()-1);
                                QString str = list.join("-");

                                if(msgBox->m_lenedit->text()==str){
                                     compar_strs.push_back(number_str.toInt());
                                }
                            }
                       }}
             QVector<int>baseNumbers;
            if(compar_strs.count()>0){
                auto max = std::max_element(std::begin(compar_strs), std::end(compar_strs));
                int biggest = *max;
                //找出缺失的数字,并且补齐它
                for (int i = 0;i<biggest+1;i++) {
                    baseNumbers.push_back(i+1);
                }
                for (int i = 0;i<baseNumbers.count();i++) {
                    for (int k = 0;k<compar_strs.count();k++) {
                        if(baseNumbers.at(i) == compar_strs.at(k)){
                           baseNumbers.remove(i);
                        }
                    }
                }
            }
            if(baseNumbers.count()>0){//补齐
                int add_number = baseNumbers.front();
                save_str = msgBox->m_lenedit->text()+"-"+QString::number(add_number,10);
            }else{//没有补齐的情况
                for(int i = 0;i<m_slider_map.count();i++){
                           QStringList item_base =  m_slider_map.at(i);
                           for (int k = 0;k<item_base.count();k++) {
                               QString item_title_base = item_base.at(item_base.count()-1);
                               if(item_title_base == msgBox->m_lenedit->text()) save_str = msgBox->m_lenedit->text()+"-1";
                           }
                }

            }
            m_data_list<<save_str;
             qDebug()<<"xxxxxxxxaaaa"<<m_data_list;
            m_slider_map.push_back(m_data_list);
            if(file.open(QIODevice::WriteOnly)) //只写模式
            {
                QDataStream out(&file);
                out << m_slider_map;
                file.close();
                emit user_message();
               // qDebug()<<"写入成功"<<m_slider_map;//6
            }

        }
        else if(nRes==QDialog::Rejected)
        {
            //处理按下取消或者关闭按钮的语句
            qDebug()<<"按下了取消键";
        }

    }else{//重置均衡
        for (int i= 0;i<m_EQ_sliders.count();i++) {

            QSlider *verticalSlider = m_EQ_sliders.at(i);
            verticalSlider->setValue(120);
        }
        //qDebug()<<btn->objectName()+"-----refresh------"<<endl;
    }
    uint8_t a= 0xFF;
    uint8_t qq = a &= 0x20;

    int8_t jj = 0xFF;
    qDebug()<<QString::number(jj,10)<<"-----refresh------";
}
//进度条的方法
void mbs400_back_rightview::verticalSlider_setValue(int value){
    QSlider *myslider = dynamic_cast<QSlider *>(sender());
    m_save_btn->setDown(true);

    if(value > 120){
        myslider->setStyleSheet(
                    "QSlider::groove:Vertical {background: rgba(255, 255, 0,0);width:4px;border-image:url(:/image/images/mbs400_high_equp.png);}"
                    "QSlider::handle:Vertical {border-radius: 5px;width: 18px;height: 18px;margin-top: 0px;margin-left: -7px;margin-bottom: 0px;margin-right: -7px;border-image:url(:/image/images/mbs400_db_round.png);} "
                    "QSlider::sub-page:Vertical {background: rgba(255, 171, 33,0%);border-radius:2px;border-image:url(:/image/images/mbs400_high_eqback.png);}"
                    );
    }else{
        myslider->setStyleSheet(
                    "QSlider::groove:Vertical {background: rgba(255, 255, 0,0);width:4px;border-image:url(:/image/images/mbs400_high_eqdown.png);}"
                    "QSlider::handle:Vertical {border-radius: 5px;width: 18px;height: 18px;margin-top: 0px;margin-left: -7px;margin-bottom: 0px;margin-right: -7px;border-image:url(:/image/images/mbs400_db_round.png);} "
                    "QSlider::add-page:Vertical {background: rgba(255, 171, 33,0%);border-radius:2px;border-image:url(:/image/images/mbs400_high_eqback.png);}");
    }
    //发送数据
    uint16_t addrs = m_EQaddrs.at(myslider->objectName().toInt()-10);//地址
    int16_t sendvalue =value-120;
    qDebug()<<sendvalue;
    PACKDATA send = {  };
    send.command = 0x4b;
    send.index[0] = addrs & 0xFF;
    send.index[1] = (addrs>> 8)&0xFF;
    send.index[4] = 0x77;

    send.index[6] = sendvalue&0XFF;
    send.index[7] = (sendvalue>>8)&0XFF;
    send.index[9] = 0x01;
    m_sendMsgs.push_back(send);
}
void mbs400_back_rightview::limiter_slider_click(int value){

    uint8_t send_value= abs(value);
    LimiterLevel=send_value;

    PACKDATA send = {  };
    send.command = 0x4b;
    send.index[0] = 0xE0;

    send.index[4] = 0x77;
    send.index[5] = 0;
    send.index[6] = LR_Balance;
    send.index[7] = LimiterLevel;
    send.index[8] = DAC_Mute_LH_Enable;
    send.index[9] = Reserved;

    m_sendMsgs.push_back(send);


    qDebug()<<send_value;
}
//高通滤波器
void mbs400_back_rightview::high_pass_click(int value){

    uint16_t send_value= value;


    HPF_Freq_low = send_value & 0XFF;
    HPF_Freq_high = (send_value>>8)& 0XFF;

    PACKDATA send_Enable = {  };
    send_Enable.command = 0x4b;
    send_Enable.index[0] = 0x9d;
    send_Enable.index[4] = 0x77;

    send_Enable.index[8] = 2;
    send_Enable.index[9] = 1;
    m_sendMsgs.push_back(send_Enable);

    PACKDATA send = {  };
    send.command = 0x4b;
    send.index[0] = 0x9C;
    send.index[4] = 0x77;

    send.index[6] = 0;
    send.index[7] = 0;
    send.index[8] = HPF_Freq_low;
    send.index[9] = HPF_Freq_high;
    m_sendMsgs.push_back(send);
    qDebug()<<send_value;
}
void mbs400_back_rightview::low_pass_click(int value){

    uint16_t send_value= value;
    LPF_Freq_low = send_value & 0XFF;
    LPF_Freq_high = (send_value>>8)& 0XFF;

    PACKDATA send_Enable = {  };
    send_Enable.command = 0x4b;
    send_Enable.index[0] = 0x9b;
    send_Enable.index[4] = 0x77;

    send_Enable.index[8] = 1;
    send_Enable.index[9] = 1;
    m_sendMsgs.push_back(send_Enable);

    PACKDATA send = {  };
    send.command = 0x4b;
    send.index[0] = 0x9A;
    send.index[4] = 0x77;

    send.index[6] = 0;
    send.index[7] = 0;
    send.index[8] = LPF_Freq_low;
    send.index[9] = LPF_Freq_high;
    m_sendMsgs.push_back(send);

    qDebug()<<send_value;
}
//开关的点击事件
void mbs400_back_rightview::switch_btn(bool is_open){
    qDebug()<<is_open;
    emit switch_status(is_open);
    if(is_open){
        m_high_mask_view->setVisible(false);

    }else{
        m_high_mask_view->setVisible(true);
    }
}
//刷新数据
void mbs400_back_rightview::refresh_data_forui(int row){


    QStringList string_list = m_slider_map.at(m_slider_map.count()-row-1);
    if(string_list.count()<13)return;
    for (int i =0;i<string_list.count();i++) {

        if(i<10){
            QVerSlider *eq_slider = m_EQ_sliders.at(i);
            eq_slider->blockSignals(true);
            eq_slider->setValue(string_list.at(i).toInt());
            eq_slider->blockSignals(false);
            //开始发送数据
            uint16_t addrs = m_EQaddrs.at(i);//地址
            int16_t sendvalue =eq_slider->value()-120;

            PACKDATA send = {};
            send.command = 0x4b;
            send.index[0] = addrs & 0xFF;
            send.index[1] = (addrs>> 8)&0xFF;
            send.index[4] = 0x77;

            send.index[6] = sendvalue&0XFF;
            send.index[7] = (sendvalue>>8)&0XFF;
            send.index[9] = 0x01;
            m_sendMsgs.push_back(send);

           qDebug()<<"ceshi========================================="<<m_sendMsgs.count()<<i;

        }else if(i>=10 && i<13 ){
            QHorSlider *LHL_slider =m_limiter_highlow_slider.at(i-10);
            LHL_slider->blockSignals(true);
            LHL_slider->setValue(string_list.at(i).toInt());
            LHL_slider->blockSignals(false);
            //发送数据
            if(i == 10){
                uint8_t send_value= abs(LHL_slider->value());
                LimiterLevel=send_value;

                PACKDATA send = {  };
                send.command = 0x4b;
                send.index[0] = 0xE0;

                send.index[4] = 0x77;
                send.index[5] = 0;
                send.index[6] = LR_Balance;
                send.index[7] = LimiterLevel;
                send.index[8] = DAC_Mute_LH_Enable;
                send.index[9] = Reserved;

                m_sendMsgs.push_back(send);
            }else if(i ==11){
                high_pass_click(LHL_slider->value());

            }else{
               low_pass_click(LHL_slider->value());
            }


        }
        //qDebug()<<"xxxxxxx"<<string_list.at(i).toInt()<<string_list.at(i);

    }


}
