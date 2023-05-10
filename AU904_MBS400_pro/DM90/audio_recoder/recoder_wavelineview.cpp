#include "recoder_wavelineview.h"

#include "recoderaudio.h"
#include <QAudioDeviceInfo>
#include <QAudioInput>
#include <qdebug.h>
#include "common.h"

#define AUDIOPATH QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+"/maonoAudio.pcm"

recoder_waveLineView::recoder_waveLineView(QWidget *parent) : QWidget(parent)
{

    //initChart();
    // audioSample();
    
}

void recoder_waveLineView::stopRecording(){

    m_audioInput->stop();    // 停止录制
    m_device->close();       // 关闭IO设备m_device
    m_device->m_outputFile.close();

    m_outAudio->stop();
    m_palyAudio->close();

}
/**
 * @brief 初始化绘制图表
 */
bool recoder_waveLineView::initChart()
{    
    //获取音频设备得名字
    QVector<QString> aDeviceListI;
    QList<QAudioDeviceInfo> audioDeviceListIs;
    QList<QAudioDeviceInfo> audioDeviceListI = QAudioDeviceInfo::availableDevices(QAudio::AudioInput);
    foreach (QAudioDeviceInfo devInfo, audioDeviceListI)
    {
        QString strName = devInfo.deviceName();
        if (devInfo.isNull()) continue;
        if (strName[0] == 65533) continue;
        bool bFound = false;
        foreach (QString dev, aDeviceListI) {
            if (strName == dev){
                bFound = true;
            }
        }
        if (bFound == true) continue;
        if(strName.contains("DM90",Qt::CaseSensitive)|| strName.contains("DM70",Qt::CaseSensitive) ){
            qDebug() << "包含了";
            audioDeviceListIs.push_back(devInfo);
        }
        qDebug()<<"yangyukkk-----------"<<strName;
        aDeviceListI.push_back(strName);


    }

     // 获取默认的音频输入设备(麦克风)

    if(audioDeviceListIs.count()>0){
       m_inputDevice = audioDeviceListIs.at(0);
    }else{
        return false;
    }


    m_chartView = new QChartView(this);
    m_chartView->resize(this->width(),this->height());//
    m_chartView->move(0,30*m_Magnification);
//    chartView->setStyleSheet("background:rgb(255,255,0)");

    QChart* chart = m_chartView->chart();             // 这里直接获取QChartView包含的chart，不自己创建
    chart->setBackgroundVisible(false);
    chart->legend()->hide();                            // 隐藏折线图的图例

    //chart->setTitle(QString("来自麦克风的数据：%1").arg(m_inputDevice.deviceName()));// 设置图表的标题和输入设备名称


    m_series = new QLineSeries();                       // 创建一个折线图对象
    chart->addSeries(m_series);

    m_axisX = new QValueAxis;                 // 创建X轴
    m_axisX->setRange(0, 10000);                          // 设置显示同时10000个音频数据点
    m_axisX->setLabelFormat("%g");
    m_axisX->setTitleText("采样");
    m_axisX->setTitleVisible(false);
    m_axisX->setLabelsVisible(false); //隐藏下面的数字
    m_axisX->setGridLineVisible(false);//设置网格线可见
    m_axisX->setLineVisible(false);//设置刻度线可见

    m_axisY= new QValueAxis;                 // 创建Y轴
    m_axisY->setRange(0, 255);                            // 因为显示的使uchar类型数据，所以Y轴设置为0~255就可以
    // axisY->setTitleText("输入音频电平");
    //m_axisY->setMinorGridLineColor(Qt::green);
    m_axisY->setGridLineVisible(false);
    m_axisY->setLabelsVisible(false);
    m_axisY->setLineVisible(false);

    chart->addAxis(m_axisX, Qt::AlignBottom);           // 在chart中添加创建的X轴，指定在图表底部
    m_series->attachAxis(m_axisX);                      // 将X轴关联到创建的折线图上
    chart->addAxis(m_axisY, Qt::AlignLeft);             // 添加创建的Y轴，指定在左侧
    m_series->attachAxis(m_axisY);
    m_series->setUseOpenGL(true);                     // 开启OpenGL加速，仅QLineSeries和QScatterSeries支持使用OpenGL进行加速
    m_series->setColor(Qt::red);


    return true;
}

/**
 * @brief 采样音频波形显示
 */
void recoder_waveLineView::audioSample()
{
    // 存储音频流参数信息的对象
    m_audioFormat.setSampleRate(8000);                        // 设置音频采样率
    m_audioFormat.setChannelCount(1);                         // 设置通道数
    m_audioFormat.setSampleSize(8);                           // 设置样本大小
    m_audioFormat.setCodec("audio/pcm");                      // 设置编解码器
    m_audioFormat.setByteOrder(QAudioFormat::LittleEndian);   // 采用小端模式
    m_audioFormat.setSampleType(QAudioFormat::UnSignedInt);   // 设置样本数据类型

    m_audioInput = new QAudioInput(m_inputDevice, m_audioFormat, this);  // 打开音频录音设备
    m_device = new recoderAudio(m_series, this);             // 将录制的音频数据输出到XYSeriesIODevice
    //m_device->open(QIODevice::WriteOnly);
    //m_audioInput->start(m_device);                               // 开始录制
}
//开始播放音乐
void recoder_waveLineView::playAudioSample(){

    m_audioFormat.setSampleRate(8000);                        // 设置音频采样率
    m_audioFormat.setChannelCount(1);                         // 设置通道数
    m_audioFormat.setSampleSize(8);                           // 设置样本大小
    m_audioFormat.setCodec("audio/pcm");                      // 设置编解码器
    m_audioFormat.setByteOrder(QAudioFormat::LittleEndian);   // 采用小端模式
    m_audioFormat.setSampleType(QAudioFormat::UnSignedInt);   // 设置样本数据类型

    m_outAudio = new QAudioOutput(m_audioFormat, this);
    outputFile.setFileName(AUDIOPATH);
    outputFile.open(QIODevice::ReadOnly);
    QByteArray audio_data = outputFile.readAll();

    m_palyAudio = new Playaudio(m_series,audio_data,this);
    // m_palyAudio->m_audio_data = audio_data;

    //    m_palyAudio->open(QIODevice::ReadOnly);
    //    m_outAudio->start(m_palyAudio);

    //    int readSize = m_outAudio->periodSize();
    //    int chunks = m_outAudio->bytesFree() / readSize;

    //    qDebug() <<"play begin!"<<readSize<<chunks;

}
void recoder_waveLineView::write_click(){

    stopRecording();
}
void recoder_waveLineView::starRecord(){


    m_device->open(QIODevice::WriteOnly);
    m_audioInput->start(m_device);
    //打开本地文件夹
    m_device->m_outputFile.setFileName(AUDIOPATH);
    m_device->m_outputFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Truncate);
}
void recoder_waveLineView::stopRecord(){

    m_audioInput->stop();    // 停止录制
    m_device->close();       // 关闭IO设备m_device
    m_device->m_outputFile.close();

}
qint64 recoder_waveLineView::starPlayRecord(){

    m_palyAudio->open(QIODevice::ReadOnly);
    m_outAudio->start(m_palyAudio);

    outputFile.setFileName(AUDIOPATH);
    outputFile.open(QIODevice::ReadOnly);
    QByteArray audio_data = outputFile.readAll();
    m_palyAudio->m_audio_data  = audio_data;
    m_palyAudio->m_data = audio_data;

    qint64 fileSize = outputFile.size();
    qint64 time = fileSize / (8000.0 * 1.0);

    return time;

    qDebug() <<"play begin------!"<<time;
}

void recoder_waveLineView::suspendRecoder(){

    m_outAudio->suspend();
}
void recoder_waveLineView::continuePlayRecoder(){

    qDebug() <<"ccccccccplay begin------!";
    m_outAudio->resume();
}

void recoder_waveLineView::starDrawCurve(){

   m_palyAudio->starDrawWAVcurve();
}

void recoder_waveLineView::stopDrawCurve(){

    m_palyAudio->stopDrawWAVcurve();
}

void recoder_waveLineView::stopPlayRecord(){

    m_outAudio->stop();
    outputFile.close();
}
