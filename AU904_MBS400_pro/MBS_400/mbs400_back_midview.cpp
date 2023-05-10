
#pragma once
#pragma execution_character_set("utf-8")
#include "mbs400_back_midview.h"
#include <QDebug>
#include "../common.h"

mbs400_back_midview::mbs400_back_midview(QWidget *parent) : QWidget(parent)
{

}
void mbs400_back_midview::createUI(){
    //上面部分
    QLabel *top_image = new QLabel(this);
    top_image->resize(this->width(),343);
    top_image->move(0,0);
    top_image->setPixmap(QPixmap(":/image/images/mbs400_mid_top.png"));

    m_mute_btn = new QCheckBox(this);
    m_mute_btn->resize(30,30);
    m_mute_btn->move(14 , 18);
    m_mute_btn->setStyleSheet(   "QCheckBox::indicator{width: 30px;height: 30px;color: rgb(255, 0, 0);}"
                                 "QCheckBox::indicator:unchecked{border-image:url(:/image/images/mbs400_newmute_btn.png) 0 60 0 0;}"
                                 "QCheckBox::indicator:unchecked:hover{border-image:url(:/image/images/mbs400_newmute_btn.png) 0 30 0 30;}"
                                 "QCheckBox::indicator:checked{border-image:url(:/image/images/mbs400_newmute_btn.png) 0 0 0 60;}");
    connect(m_mute_btn, SIGNAL(clicked(bool)), this, SLOT(mute_click_status(bool)));
    uint8_t mute_status =(background_data_pars.data_LR_lim_loud_mute>>16) & 0xff;
    uint8_t mute_status_value = getbit(mute_status,0);
    if(mute_status_value){
        m_mute_btn->setCheckState(Qt::Checked);
    }else{
        m_mute_btn->setCheckState(Qt::Unchecked);
    }


    QLabel *spin_back_image =new QLabel(this);
    spin_back_image->resize(175,156);
    spin_back_image->move(86,30);
    spin_back_image->setPixmap(QPixmap(":/image/images/mbs400_main_volumeimage.png"));


    //中间的声音的圆
    m_spin_view =new MySpinButton(this);
    m_spin_view->setGeometry(86,30,175,175);
    m_spin_view->show();
    m_spin_view->setDegree(background_data_pars.data_monitorVol);
    m_spin_view->set_whirlbuttonWidth(175);
    m_spin_view->set_whirlbuttonHeigth(175);
    m_spin_view->set_backimage(":/image/images/mbs400_main_volumebtn.png");
    m_spin_view->set_inner_circle_width(115);
    m_spin_view->m_select_index = 1;
    connect(m_spin_view,SIGNAL(volume_click()),this,SLOT(volume_change()));

    QLabel *top_name_label =new QLabel(this);
    top_name_label->resize(80,30);
    top_name_label->move(138,210);
    top_name_label->setText("Volume");
    QFont font;
    font.setFamily("Source Han Sans CN Medium");
    font.setPixelSize(18);
    top_name_label->setFont(font);
    QPalette pa;
    QColor niceBlue(175, 175, 175);
    pa.setColor(QPalette::WindowText,niceBlue);
    top_name_label->setPalette(pa);
    QLabel *top_value_label =new QLabel(this);
    top_value_label->resize(160,30);
    top_value_label->move(111,190);
    top_value_label->setText("0                             100");
    font.setPixelSize(15);
    top_value_label->setFont(font);
    top_value_label->setPalette(pa);
    QLabel *balance_label =new QLabel(this);
    balance_label->resize(70,40);
    balance_label->move(10,252);
    balance_label->setText("Balance");
    font.setPixelSize(18);
    balance_label->setFont(font);
    balance_label->setPalette(pa);
    QLabel *LR_label =new QLabel(this);
    LR_label->resize(340,30);
    LR_label->move(10,293);
    LR_label->setText("L                                                                                   R");
    font.setPixelSize(16);
    LR_label->setFont(font);
    LR_label->setPalette(pa);


    m_LR_slider = new QHorSlider(this);
    m_LR_slider->setOrientation(Qt::Horizontal);
    m_LR_slider->setMinimum(0);
    m_LR_slider->setMaximum(200);
    m_LR_slider->setSingleStep(1);

    m_LR_slider->setValue(background_data_pars.data_LR_lim_loud_mute & 0xff);
    m_LR_slider->setGeometry(37,300,265,22);
    m_LR_slider->setStyleSheet(
                "QSlider::groove:horizontal{background: rgba(0, 0, 0,100);height:6px;border: 0px solid #FFFFFF;border-image:url(:/image/images/mbs400_balance_back_left.png);}"
                "QSlider::handle:horizontal{width:  22px;height: 22px;margin-top: -8px;margin-left: 0px;margin-bottom: -8px;margin-right: 0px;border-image:url(:/image/images/mbs400_db_round.png);}"
                "QSlider::sub-page:horizontal{background-color:rgba(0,0,0,100);border-radius: 2px;border-image:url(:/image/images/mbs400_balance_left.png);}"
                );
    connect(m_LR_slider,SIGNAL(valueChanged(int)),this,SLOT(LR_chanege_value(int)));

    //中间部分
    QLabel *mid_image = new QLabel(this);
    mid_image->resize(this->width(),41);
    mid_image->move(0,355);
    mid_image->setPixmap(QPixmap(":/image/images/mbs400_mid_mid.png"));
    QLabel *equal_sound_label =new QLabel(this);
    equal_sound_label->resize(this->width()-10,41);
    equal_sound_label->move(10,355);
    equal_sound_label->setText("等响补偿       OFF                        ON");
    font.setPixelSize(16);
    equal_sound_label->setFont(font);
    equal_sound_label->setPalette(pa);


    m_switch_btn = new SwitchControl(this);
    m_switch_btn->resize(60,30);
    m_switch_btn->move(142,362);
    uint8_t switch_status =(background_data_pars.data_LR_lim_loud_mute>>16) & 0xff;
    uint8_t switch_status_value = getbit(switch_status,3);
    if(switch_status_value){
        m_switch_btn->setToggle(true);
    }else{
        m_switch_btn->setToggle(false);
    }
    connect(m_switch_btn, SIGNAL(toggled(bool)), this, SLOT(switch_btn(bool)));

    //最底下部分
    QLabel *bottom_image = new QLabel(this);
    bottom_image->resize(this->width(),189);
    bottom_image->move(0,408);
    bottom_image->setPixmap(QPixmap(":/image/images/mbs400_mid_bottom.png"));
    QLabel *high_title_label =new QLabel(this);

    high_title_label->resize(340/4,30);
    high_title_label->move(57,415);
    high_title_label->setText("0");
    font.setPixelSize(16);
    high_title_label->setFont(font);
    high_title_label->setPalette(pa);

    QLabel *mid_title_label =new QLabel(this);
    mid_title_label->resize(340/4,30);
    mid_title_label->move(57+340/4+30,415);
    mid_title_label->setText("0");
    font.setPixelSize(16);
    mid_title_label->setFont(font);
    mid_title_label->setPalette(pa);

    QLabel *low_title_label =new QLabel(this);
    low_title_label->resize(340/4,30);
    low_title_label->move(57+340/2+60,415);
    low_title_label->setText("0");
    font.setPixelSize(16);
    low_title_label->setFont(font);
    low_title_label->setPalette(pa);


    for (int i = 0;i< 3;i++) {
        //        QLabel *high_low_back_image =new QLabel(this);
        //        high_low_back_image->resize(92,83);
        //        high_low_back_image->move(15 + 112 *i,445);
        //high_low_back_image->setPixmap(QPixmap(":/image/images/mbs400_high_low_backimage.png"));


        QLabel *low_left_image =new QLabel(this);
        low_left_image->resize(46,83);
        low_left_image->move(18 + 112 *i,448);
        low_left_image->setPixmap(QPixmap(":/image/images/mbs400_half_cricle_right.png"));
        low_left_image->raise();

        QLabel *low_right_image =new QLabel(this);
        low_right_image->resize(46,83);
        low_right_image->move(13+46  + 112 *i,448);
        low_right_image->setPixmap(QPixmap(":/image/images/mbs400_half_cricle_left.png"));


        MySpinButton *high_low_spin_view =new MySpinButton(this);
        high_low_spin_view->setGeometry(15 + 112 * i,446,92,92);
        high_low_spin_view->show();
        high_low_spin_view->set_whirlbuttonWidth(92);
        high_low_spin_view->set_whirlbuttonHeigth(92);
        high_low_spin_view->set_backimage(":/image/images/mbs400_high_low_image.png");

        high_low_spin_view->set_inner_circle_width(54);
        high_low_spin_view->m_select_index = 2+i;
        high_low_spin_view->set_mask_view();
        if(i==0){
             int8_t high_value = background_data_pars.data_bass_mid_hid & 0xff;
             high_low_spin_view->setDegree((high_value+120)/2.4);
        }else if(i==1){
            int8_t mid_value = (background_data_pars.data_bass_mid_hid>>8) & 0xff;
             high_low_spin_view->setDegree((mid_value+120)/2.4);
        }else{
            int8_t mid_value = (background_data_pars.data_bass_mid_hid>>16) & 0xff;
             high_low_spin_view->setDegree((mid_value+120)/2.4);
        }
        m_high_low_spinArrs.push_back(high_low_spin_view);
    }
    QLabel *high_label =new QLabel(this);
    high_label->resize(340/4,30);
    high_label->move(15,529);
    high_label->setText("-12         +12");
    font.setPixelSize(16);
    high_label->setFont(font);
    high_label->setPalette(pa);
    QLabel *high_bottom_label =new QLabel(this);
    high_bottom_label->resize(340/4,30);
    high_bottom_label->move(44,553);
    high_bottom_label->setText("LOW");
    font.setPixelSize(16);
    high_bottom_label->setFont(font);
    high_bottom_label->setPalette(pa);

    QLabel *low_label =new QLabel(this);
    low_label->resize(340/4,30);
    low_label->move(40+340/4,529);
    low_label->setText("-12         +12");
    font.setPixelSize(16);
    low_label->setFont(font);
    low_label->setPalette(pa);
    QLabel *low_bottom_label =new QLabel(this);
    low_bottom_label->resize(340/4,30);
    low_bottom_label->move(69+340/4,553);
    low_bottom_label->setText("MID");
    font.setPixelSize(16);
    low_bottom_label->setFont(font);
    low_bottom_label->setPalette(pa);

    QLabel *hi_label =new QLabel(this);
    hi_label->resize(340/4,30);
    hi_label->move(68+340/2,529);
    hi_label->setText("-12         +12");
    font.setPixelSize(16);
    hi_label->setFont(font);
    hi_label->setPalette(pa);
    QLabel *hi_bottom_label =new QLabel(this);
    hi_bottom_label->resize(340/4,30);
    hi_bottom_label->move(94+340/2,553);
    hi_bottom_label->setText("HIGH");
    font.setPixelSize(16);
    hi_bottom_label->setFont(font);
    hi_bottom_label->setPalette(pa);

    m_simple_mask_view = new QLabel(this);
    m_simple_mask_view->resize(346,189);
    m_simple_mask_view->move(0,408);
    m_simple_mask_view->setPixmap(QPixmap(":/image/images/mbs400_high&low_mask.png"));
    m_simple_mask_view->setVisible(false);

}
//刷新ui界面
void mbs400_back_midview::notice_refresh_ui(){

    qDebug()<<"刷新所有的UI"<<m_receiveMsgs[0].index[8];
    uint8_t mute_status_value = getbit(m_receiveMsgs[0].index[8],0);
    if(mute_status_value){
        m_mute_btn->setCheckState(Qt::Checked);
    }else{
        m_mute_btn->setCheckState(Qt::Unchecked);
    }
    uint8_t switch_status_value = getbit(m_receiveMsgs[0].index[8],3);
    if(switch_status_value){
        m_switch_btn->setToggle(true);
    }else{
        m_switch_btn->setToggle(false);
    }
    m_LR_slider->blockSignals(true);
    int8_t lr_value = m_receiveMsgs[0].index[6];
    m_LR_slider->setValue(lr_value + 100);
    m_LR_slider->blockSignals(false);
    if(m_LR_slider->value() <=100){
        m_LR_slider->setStyleSheet(
                    "QSlider::groove:horizontal{background: rgba(0, 0, 0,100);height:6px;border: 0px solid #FFFFFF;border-image:url(:/image/images/mbs400_balance_back_left.png);}"
                    "QSlider::handle:horizontal{width:  22px;height: 22px;margin-top: -8px;margin-left: 0px;margin-bottom: -8px;margin-right: 0px;border-image:url(:/image/images/mbs400_db_round.png);}"
                    "QSlider::sub-page:horizontal{background-color:rgba(0,0,0,100);border-radius: 2px;border-image:url(:/image/images/mbs400_balance_left.png);}"
                    );
    }else{
        m_LR_slider->setStyleSheet(
                    "QSlider::groove:horizontal{background: rgba(0, 0, 0,100);height:6px;border: 0px solid #FFFFFF;border-image:url(:/image/images/mbs400_balance_back_right.png);}"
                    "QSlider::handle:horizontal{width:  22px;height: 22px;margin-top: -8px;margin-left: 0px;margin-bottom: -8px;margin-right: 0px;border-image:url(:/image/images/mbs400_db_round.png);}"
                    "QSlider::add-page:horizontal{background-color:rgba(0,0,0,100);border-radius: 2px;border-image:url(:/image/images/mbs400_balance_right.png);}"
                    );
            }
    //主声音
    m_spin_view->setDegree(m_receiveMsgs[1].index[6]);
    m_spin_view->different_number = m_receiveMsgs[1].index[6];

    //低中高的声音
    if(m_high_low_spinArrs.count()>=3){
        MySpinButton *low_btn = m_high_low_spinArrs.at(0);
        int8_t spi_low_value = m_receiveMsgs[2].index[6];
        qDebug()<<"spi_high_value-----"<<spi_low_value;
        low_btn->setDegree((spi_low_value+120)/2.4);

        MySpinButton *spi_mid_btn = m_high_low_spinArrs.at(1);
        int8_t spi_mid_value = m_receiveMsgs[2].index[7];
        spi_mid_btn->setDegree((spi_mid_value+120)/2.4);

        MySpinButton *spi_high_btn = m_high_low_spinArrs.at(2);
        int8_t spi_high_value = m_receiveMsgs[2].index[8];
        spi_high_btn->setDegree((spi_high_value+120)/2.4);

    }

}

void mbs400_back_midview::volume_change(){

    //qDebug()<<"xxxxxxxxxxxx";
    m_mute_btn->setCheckState(Qt::Unchecked);
}

void mbs400_back_midview::LR_chanege_value(int value){
    if(value <=100){
        m_LR_slider->setStyleSheet(
                    "QSlider::groove:horizontal{background: rgba(0, 0, 0,100);height:6px;border: 0px solid #FFFFFF;border-image:url(:/image/images/mbs400_balance_back_left.png);}"
                    "QSlider::handle:horizontal{width:  22px;height: 22px;margin-top: -8px;margin-left: 0px;margin-bottom: -8px;margin-right: 0px;border-image:url(:/image/images/mbs400_db_round.png);}"
                    "QSlider::sub-page:horizontal{background-color:rgba(0,0,0,100);border-radius: 2px;border-image:url(:/image/images/mbs400_balance_left.png);}"
                    );
    }else{
        m_LR_slider->setStyleSheet(
                    "QSlider::groove:horizontal{background: rgba(0, 0, 0,100);height:6px;border: 0px solid #FFFFFF;border-image:url(:/image/images/mbs400_balance_back_right.png);}"
                    "QSlider::handle:horizontal{width:  22px;height: 22px;margin-top: -8px;margin-left: 0px;margin-bottom: -8px;margin-right: 0px;border-image:url(:/image/images/mbs400_db_round.png);}"
                    "QSlider::add-page:horizontal{background-color:rgba(0,0,0,100);border-radius: 2px;border-image:url(:/image/images/mbs400_balance_right.png);}"
                    );

    }
    //处理数据
    int8_t send_value = value-100;
    LR_Balance = send_value;

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

}
//是否开启等响补偿的函数
void mbs400_back_midview::switch_btn(bool is_open){

    if(is_open==0){//关闭

        DAC_Mute_LH_Enable = clrbit(DAC_Mute_LH_Enable,3);
    }else{//开启

        DAC_Mute_LH_Enable = setbit(DAC_Mute_LH_Enable,3);
    }

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
}
void mbs400_back_midview::mute_click_status(bool status){

    if(status==0){

        DAC_Mute_LH_Enable = clrbit(DAC_Mute_LH_Enable,0);
    }else{

        DAC_Mute_LH_Enable = setbit(DAC_Mute_LH_Enable,0);
    }

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

    qDebug()<<status;
}
