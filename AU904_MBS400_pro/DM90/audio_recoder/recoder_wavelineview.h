#ifndef RECODER_WAVELINEVIEW_H
#define RECODER_WAVELINEVIEW_H

#include <QWidget>
#include <QChartGlobal>
#include <QAudioDeviceInfo>
#include <QAudioOutput>
#include <QFile>
#include "playaudio.h"
#include "recoderaudio.h"
#include <QtCharts>


QT_CHARTS_BEGIN_NAMESPACE    // QtCharts命名空间
class QLineSeries;           // 在头文件中声明要用到的QtCharts类，而不直接引入头文件
class QChart;
QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE      // 引入QtCharts命名空间

class XYSeriesIODevice;
class QAudioInput;

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE



class recoder_waveLineView : public QWidget
{
    Q_OBJECT
public:
    explicit recoder_waveLineView(QWidget *parent = nullptr);

public:
    bool initChart();         // 初始化绘制图表
    void audioSample();       // 采样绘制录音音频波形

    void playAudioSample();

    void starRecord();//开始录音
    void stopRecord();//停止录音

    void suspendRecoder();//暂停播放录音
    void continuePlayRecoder();//继续播放录音

    qint64 starPlayRecord();//播放录音
    void stopPlayRecord();//停止播放录音

    //开始画曲线
    void starDrawCurve();

    //停止画线
    void stopDrawCurve();



    //获取音效的时长
    //qint64 getAudioTime();

public slots:

    void stopRecording() ;
    //void read_click();
    void write_click();


public:
    QAudioDeviceInfo m_inputDevice;                // 音频输入设备信息对象
    recoderAudio* m_device = nullptr;          // IO接口，用于获取音频数据并显示
    Playaudio *m_palyAudio;

    QLineSeries* m_series = nullptr;               // 折线图对象
    QAudioInput* m_audioInput = nullptr;           // 录音设备对象

    QAudioOutput *m_outAudio;                      // 播放设备对象
    QFile outputFile;

    QChartView* m_chartView;
    //音频的格式
    QAudioFormat m_audioFormat;

    QValueAxis *m_axisX;

    QValueAxis *m_axisY;

};

#endif // RECODER_WAVELINEVIEW_H
