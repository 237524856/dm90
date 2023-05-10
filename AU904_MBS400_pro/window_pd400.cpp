

#pragma once
#pragma execution_character_set("utf-8")
#include "window_pd400.h"
#include <QStandardPaths>
#include "../common.h"

window_pd400::window_pd400(QWidget *parent) : QMainWindow(parent)
{

    QFontDatabase::addApplicationFont(":/image/MBS_400/SourceHanSansCN Medium.ttf");
//    this->setMaximumSize(1023,737);  //设置窗体最大尺寸
//    this->setMinimumSize(1023,737);  //设置窗体最小尺寸
//    QDesktopWidget* desktopWidget = QApplication::desktop();
//    QRect deskRect = desktopWidget->availableGeometry();
//    double availableScreenX = deskRect.width();
//    double availableScreenY = deskRect.height();

//    Qt::WindowFlags flags = 0;
//    flags |= Qt::WindowMinMaxButtonsHint;
//    setWindowFlags(flags);
}
