#include "dm90_lightrgbimage.h"
#include <QDebug>
#include <QScreen>
#include <QDesktopWidget>
#include <QApplication>
#include <QPainter>
#include <QPixmap>
#include <QCursor>

dm90_lightRGBimage::dm90_lightRGBimage(QWidget *parent) : QWidget(parent)
{

    this->setAttribute(Qt::WA_TranslucentBackground);//设置窗口背景透明
    this->setWindowFlags(Qt::FramelessWindowHint); //设置无边框窗口


}
void dm90_lightRGBimage::createUI(){

    QLabel *back_image = new QLabel(this);
    back_image->move(0,0);
    back_image->resize(this->width(),this->height());
    back_image->setPixmap(QPixmap(":/image/images/DM90/dm90_lightRGB.png"));
    back_image->setScaledContents(true);
}

void dm90_lightRGBimage::mousePressEvent(QMouseEvent *qevent){

    switch (qevent->button()) {
    case Qt::LeftButton:
    {
        //鼠标左键按下处理
        qDebug()<<"jiruuuuuuuuuuuuuuuuuuuu";

        //        int x = qevent->x();
        //        int y = qevent->y();
        int x = QCursor::pos().x();
        int y = QCursor::pos().y();

        //获取坐标像素点
        QScreen *screen = qApp->primaryScreen();
        QPixmap pixmap = screen->grabWindow(0, x, y, 2, 2);

        //获取像素点RGB
        int red, green, blue;
        QImage image = pixmap.toImage();
        QColor color = image.pixel(0,0);
        red = color.red();
        green = color.green();
        blue = color.blue();

        if((red != 17) & (green!=17) & (blue!=17)){
            emit custom_click(red,green,blue);

        }

    }
        break;
    case Qt::RightButton:     // 0x00000002
    {
        //鼠标右键按下处理
    }
        break;
    default:
        break;
    }


}
void dm90_lightRGBimage::mouseReleaseEvent(QMouseEvent *qevent){

}
void dm90_lightRGBimage::mouseMoveEvent(QMouseEvent *qevent){

}

#if defined(Q_OS_WIN32)
void dm90_lightRGBimage::enterEvent(QEvent *){
#else
void dm90_lightRGBimage::enterEvent(QEnterEvent *){
#endif



//    int x = QCursor::pos().x();
//    int y = QCursor::pos().y();

//    //获取坐标像素点
//    QScreen *screen = qApp->primaryScreen();
//    QPixmap pixmap = screen->grabWindow(0, x, y, 2, 2);

//    //获取像素点RGB
//    int red, green, blue;
//    QImage image = pixmap.toImage();
//    QColor color = image.pixel(0,0);
//    red = color.red();
//    green = color.green();
//    blue = color.blue();

//    if((red != 17) & (green!=17) & (blue!=17)){

        QPixmap pixmap(":/image/images/DM90/dm90_CursorImg.png");
        QCursor cursor(pixmap); //加载xpm生成的图标文件
        this->setCursor(cursor); //设置自定义的鼠标样式

       // this->setCursor(Qt::PointingHandCursor);
//    }


    emit custom_enter();

}
void dm90_lightRGBimage::leaveEvent(QEvent *){

    emit custom_leave();
    //qDebug()<<"learrrrrrrrrrrrrrrrrrrrrr";

}

void dm90_lightRGBimage::paintEvent(QPaintEvent *event){

    //    QPainter painter(this);
    //        painter.setRenderHint(QPainter::Antialiasing);	// 反锯齿;
    //        painter.setBrush(QBrush(Qt::red));
    //        painter.setPen(Qt::transparent);
    //        QRect rect = this->rect();
    //        rect.setWidth(rect.width() - 1);
    //        rect.setHeight(rect.height() - 1);
    //        painter.drawRoundedRect(rect, 200, 200);
    //也可用QPainterPath 绘制代替 painter.drawRoundedRect(rect, 15, 15);
    //        {
    //            QPainterPath painterPath;
    //            painterPath.addRoundedRect(rect, 15, 15);
    //            p.drawPath(painterPath);
    //        }
    //        QWidget::paintEvent(event);


}
