#include "mainbasewindow.h"
#include <QApplication>
#include <QPainter>
#include <QFile>
#include <QDebug>
#include <QPainterPath>


mainBasewindow::mainBasewindow(QWidget *parent) : QMainWindow(parent)
{
#if defined(Q_OS_WIN32)
      this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);
#else

#endif
      // 设置窗口背景透明;
      setAttribute(Qt::WA_TranslucentBackground);

      // 注意！！！如果是主窗口不要设置WA_DeleteOnClose属性;
      // 关闭窗口时释放资源;
      setAttribute(Qt::WA_DeleteOnClose);
      // 初始化标题栏;
      initTitleBar();
}
void mainBasewindow::initTitleBar()
{
    m_titleBar = new mainTitleBar(this);
    m_titleBar->move(0, 0);

    connect(m_titleBar, SIGNAL(signalButtonMinClicked()), this, SLOT(onButtonMinClicked()));
    connect(m_titleBar, SIGNAL(signalButtonRestoreClicked()), this, SLOT(onButtonRestoreClicked()));
    //connect(m_titleBar, SIGNAL(signalButtonMaxClicked()), this, SLOT(onButtonMaxClicked()));
    connect(m_titleBar, SIGNAL(signalButtonCloseClicked()), this, SLOT(onButtonCloseClicked()));

}
void mainBasewindow::paintEvent(QPaintEvent* event)
{
    //设置背景色;
    QPainter painter(this);
    QPainterPath pathBack;
    pathBack.setFillRule(Qt::WindingFill);
    pathBack.addRoundedRect(QRect(0, 0, this->width(), this->height()), 3, 3);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
    painter.fillPath(pathBack, QBrush(QColor(0, 0, 0)));

    return QWidget::paintEvent(event);
}

void mainBasewindow::loadStyleSheet(const QString &sheetName)
{
    QFile file(":/Resources/" + sheetName + ".css");
    file.open(QFile::ReadOnly);
    if (file.isOpen())
    {
        QString styleSheet = this->styleSheet();
        styleSheet += QLatin1String(file.readAll());
        this->setStyleSheet(styleSheet);
        file.close();
    }
}

void mainBasewindow::onButtonMinClicked()
{
    qDebug()<<"zuixianhua------------------";
    showMinimized();
}

void mainBasewindow::onButtonRestoreClicked()
{
    QPoint windowPos;
    QSize windowSize;
    m_titleBar->getRestoreInfo(windowPos, windowSize);
    this->setGeometry(QRect(windowPos, windowSize));
}

void mainBasewindow::onButtonMaxClicked()
{
    m_titleBar->saveRestoreInfo(this->pos(), QSize(this->width(), this->height()));
    //QRect desktopRect = QApplication::desktop()->availableGeometry();
    //QRect FactRect = QRect(desktopRect.x() - 3, desktopRect.y() - 3, desktopRect.width() + 6, desktopRect.height() + 6);
    //setGeometry(FactRect);
}

void mainBasewindow::onButtonCloseClicked()
{
    delete m_titleBar;
    QApplication* app;
    app->exit(0);
    //close();
}
