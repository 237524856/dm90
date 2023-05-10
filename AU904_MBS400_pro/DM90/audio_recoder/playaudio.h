#ifndef PLAYAUDIO_H
#define PLAYAUDIO_H

#include <QObject>
#include <QChartGlobal>
#include <QVector>
#include <QElapsedTimer>
#include <QFile>
#include <QTimer>
#include <QPointF>

QT_CHARTS_BEGIN_NAMESPACE
class QXYSeries;
QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

class Playaudio : public QIODevice
{
    Q_OBJECT
public:
    explicit Playaudio(QXYSeries *series,const QByteArray &data,QObject *parent = nullptr);

    QFile m_outputFile;   // class member.

    int readSize;
    int chunks ;

    QByteArray m_data;//播放的数据
    QByteArray m_audio_data;

public:
    qint64 readData(char *data, qint64 maxlen) override;
    qint64 writeData(const char *data, qint64 len) override;

    //画曲线计时器
    QTimer *m_timer_play;
    //开始画曲线
    void starDrawWAVcurve();

    //暂停画曲线
    void stopDrawWAVcurve();

public:
    QXYSeries* m_series= nullptr;      // 用于显示音频数据的对象
    QVector<QPointF> m_buffer;




public slots:

    void test_playAudio();
signals:

};

#endif // PLAYAUDIO_H
