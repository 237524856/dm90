#ifndef MYPUSHBUTTON_H
#define MYPUSHBUTTON_H

#include <QPushButton>
#include <QSvgRenderer>
#if defined(Q_OS_WIN32)

#else

#endif
class mypushbutton : public QPushButton
{
    Q_OBJECT
public:
    mypushbutton();
    mypushbutton(QWidget* parent);

protected:


private:
    virtual void paintEvent(QPaintEvent *event);
    QSvgRenderer m_render;
};



#endif // MYPUSHBUTTON_H
