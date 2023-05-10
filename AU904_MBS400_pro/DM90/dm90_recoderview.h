#ifndef DM90_RECODERVIEW_H
#define DM90_RECODERVIEW_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include "custom_checkbox.h"
#include "audio_recoder/recoder_wavelineview.h"

class dm90_recoderview : public QWidget
{
    Q_OBJECT
public:
    explicit dm90_recoderview(QWidget *parent = nullptr);

    void createUI();

public:
    //计时器
    QTimer *m_recoderTimer;

    QLabel *m_times;

    int m_time_num = 0;

    enum btnStatus{

        recoder_status =0,
        paly_status
    };

public slots:

    void handleTimeout();

    //按钮的方法
    void star_recoder_play(int);

protected:

    qint64 m_audio_time; //音效的时长

    bool m_isRecoderAudio = false;

public:

    recoder_waveLineView *m_waveView;
    QVector<custom_checkbox *>m_audio_btns;//录音播放的按钮



signals:

};

#endif // DM90_RECODERVIEW_H
