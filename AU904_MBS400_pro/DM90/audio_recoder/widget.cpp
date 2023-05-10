#include "widget.h"
#include "ui_widget.h"
#include "xyseriesiodevice.h"
#include <QAudioDeviceInfo>
#include <QAudioInput>
#include <QtCharts>
#include <qdebug.h>
#define AUDIOPATH "c:/yang/yangyu.pcm"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    //ui->setupUi(this);

    this->setFixedSize(1000,700);
    this->setWindowTitle(QString("QtCharts绘图-动态数据的绘制（麦克风输入）Demo - V%1").arg(APP_VERSION));
    initChart();
    //audioSample();

   // QTimer::singleShot(10000, this, SLOT(stopRecording()));

    //创建2个按钮
    QPushButton *btn = new QPushButton(this);
    btn->move(100,600);
    btn->resize(100,60);
    btn->setText("播放录音");
    connect(btn,SIGNAL(clicked()),this,SLOT(read_click()));

    QPushButton *btn2 = new QPushButton(this);
    btn2->move(300,600);
    btn2->resize(100,60);
    btn2->setText("写入录音");
    connect(btn2,SIGNAL(clicked()),this,SLOT(write_click()));

}

Widget::~Widget()
{
//    m_audioInput->stop();    // 停止录制
//    m_device->close();       // 关闭IO设备m_device
//    m_device->m_outputFile.close();

    m_outAudio->stop();
    m_palyAudio->close();

    delete ui;
}
void Widget::stopRecording(){

    m_audioInput->stop();    // 停止录制
    m_device->close();       // 关闭IO设备m_device
    m_device->m_outputFile.close();

    m_outAudio->stop();
    m_palyAudio->close();


}
/**
 * @brief 初始化绘制图表
 */
void Widget::initChart()
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
            if(strName.contains("DM90",Qt::CaseSensitive)){
                    qDebug() << "包含了";
                    audioDeviceListIs.push_back(devInfo);
                }
            qDebug()<<"yangyukkk-----------"<<strName;
            aDeviceListI.push_back(strName);


        }
    m_inputDevice = audioDeviceListIs.at(0);   // 获取默认的音频输入设备(麦克风)


    QChartView* chartView = new QChartView(this);
    chartView->resize(800,400);
    chartView->setStyleSheet("background:rgb(255,255,0)");

    QChart* chart = chartView->chart();             // 这里直接获取QChartView包含的chart，不自己创建

    chart->legend()->hide();                            // 隐藏折线图的图例
    chart->setTitle(QString("来自麦克风的数据：%1").arg(m_inputDevice.deviceName()));// 设置图表的标题和输入设备名称


    m_series = new QLineSeries();                       // 创建一个折线图对象
    chart->addSeries(m_series);

    QValueAxis *axisX = new QValueAxis;                 // 创建X轴
    axisX->setRange(0, 10000);                          // 设置显示同时10000个音频数据点
    axisX->setLabelFormat("%g");
    axisX->setTitleText("采样");
    axisX->setTitleVisible(false);
    axisX->setLabelsVisible(false); //隐藏下面的数字
    axisX->setGridLineVisible(false);//设置网格线可见
    axisX->setLineVisible(false);//设置刻度线可见


    QValueAxis *axisY = new QValueAxis;                 // 创建Y轴
    axisY->setRange(0, 255);                            // 因为显示的使uchar类型数据，所以Y轴设置为0~255就可以
   // axisY->setTitleText("输入音频电平");
    axisY->setMinorGridLineColor(Qt::red);
    axisY->setGridLineVisible(false);
    axisY->setLabelsVisible(false);
    axisY->setLineVisible(false);

    chart->addAxis(axisX, Qt::AlignBottom);           // 在chart中添加创建的X轴，指定在图表底部
    m_series->attachAxis(axisX);                      // 将X轴关联到创建的折线图上
    chart->addAxis(axisY, Qt::AlignLeft);             // 添加创建的Y轴，指定在左侧
    m_series->attachAxis(axisY);
    m_series->setUseOpenGL(true);                     // 开启OpenGL加速，仅QLineSeries和QScatterSeries支持使用OpenGL进行加速

    m_series->setColor(Qt::red);
}

/**
 * @brief 采样音频波形显示
 */
void Widget::audioSample()
{
                              // 存储音频流参数信息的对象
    m_audioFormat.setSampleRate(8000);                        // 设置音频采样率
    m_audioFormat.setChannelCount(1);                         // 设置通道数
    m_audioFormat.setSampleSize(8);                           // 设置样本大小
    m_audioFormat.setCodec("audio/pcm");                      // 设置编解码器
    m_audioFormat.setByteOrder(QAudioFormat::LittleEndian);   // 采用小端模式
    m_audioFormat.setSampleType(QAudioFormat::UnSignedInt);   // 设置样本数据类型

    m_audioInput = new QAudioInput(m_inputDevice, m_audioFormat, this);  // 打开音频录音设备
    m_device = new XYSeriesIODevice(m_series, this);             // 将录制的音频数据输出到XYSeriesIODevice
    m_device->open(QIODevice::WriteOnly);
    m_audioInput->start(m_device);                               // 开始录制
}
//开始播放音乐
void Widget::read_click(){

    m_audioFormat.setSampleRate(8000);                        // 设置音频采样率
    m_audioFormat.setChannelCount(1);                         // 设置通道数
    m_audioFormat.setSampleSize(8);                           // 设置样本大小
    m_audioFormat.setCodec("audio/pcm");                      // 设置编解码器
    m_audioFormat.setByteOrder(QAudioFormat::LittleEndian);   // 采用小端模式
    m_audioFormat.setSampleType(QAudioFormat::UnSignedInt);   // 设置样本数据类型

    m_outAudio = new QAudioOutput(m_audioFormat, this);

   // m_device = new XYSeriesIODevice(m_series, this);             // 将录制的音频数据输出到XYSeriesIODevice
    //m_device->open(QIODevice::ReadOnly);

    outputFile.setFileName(AUDIOPATH);
    outputFile.open(QIODevice::ReadOnly);
    QByteArray audio_data = outputFile.readAll();

    m_palyAudio = new Playaudio(m_series,audio_data,this);
   // m_palyAudio->m_audio_data = audio_data;
    m_palyAudio->open(QIODevice::ReadOnly);


    m_outAudio->start(m_palyAudio);


    int readSize = m_outAudio->periodSize();
    int chunks = m_outAudio->bytesFree() / readSize;

    qDebug() <<"play begin!"<<readSize<<chunks;

}
void Widget::write_click(){

    stopRecording();
}
