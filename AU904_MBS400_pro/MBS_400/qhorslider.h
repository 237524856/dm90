#ifndef QHORSLIDER_H
#define QHORSLIDER_H

#include <QObject>
#include <QSlider>


class QHorSlider : public QSlider
{
    Q_OBJECT


public:
    QHorSlider(QWidget *parent = nullptr);
    ~QHorSlider();

signals:
    void ValueChanged();

protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);

    bool            m_bLButtonDown;

};

#endif // QHORSLIDER_H
