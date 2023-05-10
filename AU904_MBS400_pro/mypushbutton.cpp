#include "mypushbutton.h"
#include <QSvgRenderer>
#include "qpainter.h"

mypushbutton::mypushbutton()
{

}

mypushbutton::mypushbutton(QWidget* parent) :
    QPushButton(parent)
{

this->m_render.load(QString(":/image/images/Mainwindows/21519.svg"));
 #if defined(Q_OS_WIN32)
    //this->m_render.load(QString(":/image/images/Mainwindows/21515.svg"));

#else
    //this->setStyleSheet("QPushButton{border-image:url(:/image/images/Mainwindows/Main_backimage.png)}");
#endif
}

void mypushbutton::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    this->m_render.render(&painter, QRectF(0, 0, this->size().width() ,this->size().height()));

    painter.drawEllipse(QPoint(200,50), 50, 50);
}
#if defined(Q_OS_WIN32)

#else

#endif
