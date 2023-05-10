#include "playaudio.h"
#include <QLineSeries>
#include <QPointF>
#include <qdebug.h>
#include <qvalueaxis.h>
#include <QTimer>

Playaudio::Playaudio(QXYSeries *series,const QByteArray &data,QObject *parent) : QIODevice(parent), m_series(series), m_data(data)
{


    const QList<QAbstractAxis*> axiss = m_series->attachedAxes();   // 获取关联的X轴
    for(auto axis : axiss)
    {
        if(axis->alignment() == Qt::AlignBottom)  // 判断是否是X轴
        {
            QValueAxis * axisX = (QValueAxis *)m_series->attachedAxes().at(0);
            int count = axisX->max() - axisX->min();   // 获取轴范围
            m_buffer.reserve(count);                   // 初始化绘制点的数组大小
            for(int i = 0; i < count; i++)
            {
                m_buffer.append(QPointF(i, 127));
            }
        }
    }
    m_audio_data = data;
    m_timer_play = new QTimer();
    connect(m_timer_play, SIGNAL(timeout()), this, SLOT(test_playAudio()));
    //QTimer::singleShot(200, this, SLOT(test_playAudio()));

}
void Playaudio::starDrawWAVcurve(){

    m_timer_play->start(32);
}


void Playaudio::stopDrawWAVcurve(){

    m_timer_play->stop();
}

/**
 * @brief          readData是纯虚函数，需要重写才能实例化
 * @param data
 * @param maxlen
 * @return
 */
int number =0;
QByteArray c ;
qint64 Playaudio::readData(char *data, qint64 maxlen)
{
//    Q_UNUSED(data)
//    Q_UNUSED(maxlen)
//    return 0;

    QByteArray d;
    if (m_data.size() >= maxlen) {
        d = m_data.mid(0, int(maxlen));
        memcpy(data, d.data(), size_t(d.size()));
        m_data.remove(0, int(maxlen));

    } else {
        d = m_data;
        memcpy(data, d.data(), size_t(d.size()));
        m_data.clear();
    }
     qDebug()<<"yangyu-----------"<<maxlen<<d.size();
    return d.size();
}

void Playaudio::test_playAudio(){

    if(m_buffer.isEmpty())return;

//    if (m_data.size() > 0) {
        int readSize = 400;
//        int chunks = 5;
//        while (chunks) {
            QByteArray samples = m_audio_data.mid(0, readSize);
            int len = samples.size();
            const char *data = samples.data();

            if(m_buffer.isEmpty()) return;                        // 如果未初始化数组则不显示

            static const int resolution = 4;                         // 每四个数据显示一个
            const int availableSamples = int(len) / resolution;      // 需要显示的数据个数
            int start = 0;
            if(availableSamples < m_buffer.count())                  // 如果需要显示的数据个数 < 数组个数，则每次的数据都需要往前移动
            {
                start = m_buffer.count() - availableSamples;         // 需要向前移动的数据个数
                for(int i = 0; i < start; i++)
                {
                    m_buffer[i].setY(m_buffer.at(i + availableSamples).y());
                }
            }
            for(int i = start; i < m_buffer.count(); i++)           // 每隔4个取出1个数据加入绘制的数组
            {
                m_buffer[i].setY(uchar(*data));
                data += resolution;
            }
            m_series->setColor(Qt::green);
            m_series->replace(m_buffer);                           // 绘制数组中的数据

            m_audio_data.remove(0, len);
//            if (len != readSize) break;

//            chunks--;
//        }
//    }else{
//        qDebug()<<"yangyu------888-----";
//    }

}
/**
 * @brief        重写IO接口，获取输入设备录制的音频数据
 * @param data   音频数据
 * @param len    数据长度
 * @return
 */
qint64 Playaudio::writeData(const char *data, qint64 len)
{
    Q_UNUSED(data);
    Q_UNUSED(len);
    return 0;
}
