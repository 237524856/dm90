
#pragma once
#pragma execution_character_set("utf-8")
#include "customscrollview.h"
#include <QHBoxLayout>
#include <QPainter>
#include <QDebug>
#include <QButtonGroup>
#include <QDebug>

customScrollView::customScrollView(QWidget *parent) : QWidget(parent)
{
    m_currentDrawImageIndx=0;

    setAttribute(Qt::WA_StyledBackground);
    // 添加ImageOpacity属性;
    this->setProperty("ImageOpacity", 1.0);

    // 动画切换类;
    m_opacityAnimation = new QPropertyAnimation(this, "ImageOpacity");
    // 这里要设置的动画时间小于图片切换时间;
    m_opacityAnimation->setDuration(1500);

    // 设置ImageOpacity属性值的变化范围;
    m_opacityAnimation->setStartValue(1.0);
    m_opacityAnimation->setEndValue(0.0);
    // 透明度变化及时更新绘图;
    connect(m_opacityAnimation, SIGNAL(valueChanged(const QVariant&)), this, SLOT(update()));
    // 设置图片切换时钟槽函数;
    connect(&m_imageChangeTimer, SIGNAL(timeout()), this, SLOT(onImageChangeTimeout()));

}
void customScrollView::initChangeImageButton(){

    QButtonGroup* changeButtonGroup = new QButtonGroup;
    for (int i=0;i<2;i++) {
        QPushButton *mybtn = new QPushButton(this);
        mybtn->move(i*(this->width()-80),(this->height()-40)/2);
        mybtn->resize(80,40);
        mybtn->setStyleSheet("background-color:green");
        mybtn->setText("下一个");
        changeButtonGroup->addButton(mybtn, i);

    }
    connect(changeButtonGroup, SIGNAL(buttonClicked(int)), this, SLOT(onImageSwitchButtonClicked(int)));
}

void customScrollView::setImageList(QStringList imageFileNameList){

    m_imageFileNameList = imageFileNameList;
}
void customScrollView::addImage(QString imageFileName){

    m_imageFileNameList.append(imageFileName);

}
void customScrollView::startPlay(){

}

void customScrollView::onImageChangeTimeout(){

}
void customScrollView::paintEvent(QPaintEvent *event){

    QPainter painter(this);
        QRect imageRect = this->rect();

        // 如果图片列表为空，显示默认图片;
        if (m_imageFileNameList.isEmpty())
        {
            QPixmap backPixmap = QPixmap(":/Resources/CarouselImageBack.png");
            painter.drawPixmap(imageRect, backPixmap.scaled(imageRect.size()));
        }
        // 如果只有一张图片;
        else if (m_imageFileNameList.count() == 1)
        {
            QPixmap backPixmap = QPixmap(m_imageFileNameList.first());
            painter.drawPixmap(imageRect, backPixmap.scaled(imageRect.size()));
        }
        // 多张图片;
        else if (m_imageFileNameList.count() > 1)
        {
            float imageOpacity = this->property("ImageOpacity").toFloat();
            painter.setOpacity(1);
            painter.drawPixmap(imageRect, m_nextPixmap.scaled(imageRect.size()));
            painter.setOpacity(imageOpacity);
            painter.drawPixmap(imageRect, m_currentPixmap.scaled(imageRect.size()));
        }


}
void customScrollView::onImageSwitchButtonClicked(int buttonId)
{
    qDebug()<<"xxxxxx"<<buttonId;
    m_currentDrawImageIndx = buttonId - 1;
    if (m_currentDrawImageIndx == -1)
    {
        m_currentDrawImageIndx = m_imageFileNameList.count() - 1;
    }

    onImageChangeTimeout();
    m_imageChangeTimer.start(2000);
    update();
}
