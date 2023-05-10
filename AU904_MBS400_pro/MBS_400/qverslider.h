#ifndef QVERSLIDER_H
#define QVERSLIDER_H

#include <QWidget>
#include <QSlider>


class QVerSlider : public QSlider
{
    Q_OBJECT
public:
    explicit QVerSlider(QWidget *parent = nullptr);

    void setSliderDisable(bool);

signals:
    void ValueChanged();

protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);

    bool            m_bLButtonDown;
};

#endif // QVERSLIDER_H
