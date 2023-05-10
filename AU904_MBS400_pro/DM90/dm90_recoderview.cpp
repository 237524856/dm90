#pragma once
#pragma execution_character_set("utf-8")
#include "dm90_recoderview.h"
#include <QDebug>
#include "common.h"

dm90_recoderview::dm90_recoderview(QWidget *parent) : QWidget(parent)
{

    this->setAttribute(Qt::WA_StyledBackground,true);
    //this->setStyleSheet("background:red");
}
void dm90_recoderview::createUI(){

    //按钮
    QVector<QString> btnImage = {":/image/images/DM90/pd400_recoder_star.png",
                                 ":/image/images/DM90/pd400_recoder_play.png"};
    QVector<QString> btnImage_select = {":/image/images/DM90/DM90_recoder_stop.png",
                                        ":/image/images/DM90/DM90_recoder_playStop.png"};

    QVector<QString> btnImage_disable =  { ":/image/images/DM90/pd400_recoder_disable.png",
                                           ":/image/images/DM90/pd400_play_disable.png"};
    for (int i =0;i<2;i++) {
        custom_checkbox *btn = new custom_checkbox(this);
        btn->resize(24*m_Magnification,24*m_Magnification);
        btn->move(20*m_Magnification + (btn->width()+30*m_Magnification)*i,60*m_Magnification);

        QString mainBtn_stys=QString("QCheckBox::indicator{width: %1px;height: %1px;color: rgb(255, 0, 0);}"
                                     "QCheckBox::indicator:unchecked{border-image:url(%2);}"
                                     "QCheckBox::indicator:unchecked:disabled{border-image:url(%4);}"
                                     "QCheckBox::indicator:unchecked:hover{border-image:url(%2);}"
                                     "QCheckBox::indicator:unchecked:hover:disabled{border-image:url(%4);}"
                                     "QCheckBox::indicator:checked{border-image:url(%3);}"
                                     "QCheckBox::indicator:checked:disabled{border-image:url(%4);}"
                                     "QCheckBox:focus{outline: none;}"
                                     ).arg(24*m_Magnification).arg(btnImage.at(i)).arg(btnImage_select.at(i)).arg(btnImage_disable.at(i));
        btn->setStyleSheet(mainBtn_stys);
        btn->setObjectName(QString::number(i,10));
        connect(btn,SIGNAL(stateChanged(int)),this,SLOT(star_recoder_play(int)));
        m_audio_btns.push_back(btn);
    }

    m_times = new QLabel(this);
    m_times->resize(80*m_Magnification,24*m_Magnification);
    m_times->move(this->width()-20*m_Magnification-m_times->width(),60*m_Magnification);
    m_times->setText(tr("00:00"));
    QString timeLabel_stys = QString("background-color:transparent;color:rgb(255,255,255);font-size:%1px;font-family:'Source Han Sans CN Medium'").arg(16*m_Magnification);
    m_times->setStyleSheet(timeLabel_stys);

    m_recoderTimer = new QTimer(this);
    connect(m_recoderTimer, SIGNAL(timeout()), this, SLOT(handleTimeout()));

    //音频波线
    m_waveView= new recoder_waveLineView(this);
    m_waveView->move((60+24*2)*m_Magnification,0);
    m_waveView->resize(this->width()-m_waveView->x()-100*m_Magnification,this->height());

    bool result = m_waveView->initChart();
    if(result){
         m_waveView->playAudioSample();
         m_waveView->audioSample();
    }

    QLabel *paly_label = new QLabel(this);
    paly_label->move(20*m_Magnification,5*m_Magnification);
    paly_label->resize(100*m_Magnification,40*m_Magnification);
    paly_label->setText(tr("录音"));
    QString Label_stys = QString("background-color:transparent;color:rgb(255,255,255);font-size:%1px;font-family:'Source Han Sans CN Medium'").arg(20*m_Magnification);
    paly_label->setStyleSheet(Label_stys);
    //m_waveView->audioSample();
}

void dm90_recoderview::handleTimeout(){
    m_time_num ++;
    QString str ;

    if(m_time_num>=60 && m_time_num<600){
        str = QString("0%3:%1%2").arg((int)(m_time_num%60)/10).arg(m_time_num%10).arg((int)m_time_num/60);
    }else if(m_time_num>=600){
        str = QString("%3:%1%2").arg((int)(m_time_num%60)/10).arg(m_time_num%10).arg((int)m_time_num/60);
    }else{
        str = QString("00:%1%2").arg((int)m_time_num/10).arg(m_time_num%10);
    }

    m_times->setText(str);
    if(m_time_num>=m_audio_time && !m_isRecoderAudio){//播放结束后的操作
       m_recoderTimer->stop();
       m_audio_btns.at(1)->setChecked(false);
       m_audio_btns.at(0)->setDisabled(false);
       m_time_num = 0;
    }
}

void dm90_recoderview::star_recoder_play(int selectIndex){

    custom_checkbox *btn = static_cast<custom_checkbox *>(sender());
    int btn_number = btn->objectName().toInt();
    switch (btn_number) {

    case recoder_status:{

        if(selectIndex){//选中
            m_audio_time=0;
            m_time_num = 0;
            m_recoderTimer->start(1000);

            m_waveView->audioSample();
             m_waveView->playAudioSample();
            m_waveView->starRecord();//开始录音

            //屏蔽播放按钮
            m_audio_btns.at(1)->setDisabled(true);
            m_isRecoderAudio = true;

        }else{//录音结束
            m_waveView->stopRecord();
            m_recoderTimer->stop();
            m_audio_btns.at(1)->setDisabled(false);
            m_isRecoderAudio = false;
            m_time_num = 0;
        }
    }
        break;

    case paly_status:{//选中

        if(selectIndex){//播放录音

            //两种情况
            if(m_time_num==0){
                m_waveView->audioSample();
                m_waveView->playAudioSample();
                m_audio_time = m_waveView->starPlayRecord();
                m_waveView->starDrawCurve();
                m_recoderTimer->start(1000);
            }else{
                m_waveView->continuePlayRecoder();
                m_waveView->starDrawCurve();
                m_recoderTimer->start(1000);
            }

            //m_time_num = 0;

            //屏蔽录音按钮
            m_audio_btns.at(0)->setDisabled(true);

        }else{//暂停播放录音

            m_waveView->suspendRecoder();
            m_waveView->stopDrawCurve();
            m_audio_btns.at(0)->setDisabled(false);
            //计时器停止
            m_recoderTimer->stop();
        }
    }
        break;

    }


}
