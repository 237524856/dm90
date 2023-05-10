#ifndef RECODERAUDIO_H
#define RECODERAUDIO_H

#include <QIODevice>
#include <QChartGlobal>
#include <QVector>
#include <QElapsedTimer>
#include <QFile>
#include <QPointF>

QT_CHARTS_BEGIN_NAMESPACE
class QXYSeries;
QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

class recoderAudio : public QIODevice
{
    Q_OBJECT
public:
    explicit recoderAudio(QXYSeries *series,QObject *parent = nullptr);



    QFile m_outputFile;   // 保存输出音效的文件.

public:
    qint64 readData(char *data, qint64 maxlen) override;
    qint64 writeData(const char *data, qint64 len) override;

public:
    QXYSeries* m_series= nullptr;      // 用于显示音频数据的对象
    QVector<QPointF> m_buffer;

};

#endif // RECODERAUDIO_H
