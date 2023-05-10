#include "qverslider.h"
#include <QDebug>
#include <QMouseEvent>
#include "../common.h"

QVerSlider::QVerSlider(QWidget *parent) : QSlider(parent)
{

}
void QVerSlider::mousePressEvent(QMouseEvent *event)
{
    //获取当前点击位置,得到的这个鼠标坐标是相对于当前QSlider的坐标
    int currentX = event->pos().y();

    //获取当前点击的位置占整个Slider的百分比
    double per = currentX *1.0 /this->height();

    qDebug()<<"xxxxxx"<<per;
    //利用算得的百分比得到具体数字
    int value =this->maximum() - (per*(this->maximum() - this->minimum()) + this->minimum());

    //设定滑动条位置
    this->setValue(value);

    //滑动条移动事件等事件也用到了mousePressEvent,加这句话是为了不对其产生影响，是的Slider能正常相应其他鼠标事件
    QSlider::mousePressEvent(event);
}

void QVerSlider::mouseReleaseEvent(QMouseEvent *event)
{
    QSlider::mouseReleaseEvent(event);

    emit ValueChanged();
}

void QVerSlider::setSliderDisable(bool is_disabled){


    if(is_disabled){//不可点

        if(this->value()>120){
            QString sub_stys = QString("QSlider::groove:Vertical {background: rgba(255, 255, 0,0);width:%1px;border-image:url(:/image/images/mbs400_high_equp_disable.png);}"
                                       "QSlider::handle:Vertical {border-radius: 12px;width: 8px;height: 24px;margin-top: 0px;margin-left: -8px;margin-bottom: 0px;margin-right: -8px;background:white;}"
                                       "QSlider::sub-page:Vertical {background: rgba(171, 171, 171,0%);border-radius:2px;border-image:url(:/image/images/mbs400_high_eqback.png);}").arg(4*m_Magnification);
            this->setStyleSheet(sub_stys);
        }else{
            this->setStyleSheet(
                        "QSlider::groove:Vertical {background: rgba(255, 255, 0,0);width:8px;border-image:url(:/image/images/mbs400_high_eqdown_disable.png);}"
                        "QSlider::handle:Vertical {border-radius: 12px;width: 8px;height: 24px;margin-top: 0px;margin-left: -8px;margin-bottom: 0px;margin-right: -8px;background:white;}"
                        "QSlider::add-page:Vertical {background: rgba(171, 171, 171,0%);border-radius:2px;border-image:url(:/image/images/mbs400_high_eqback.png);}"
                        );
        }
        this->setDisabled(true);

    }else{//可点


        if(this->value()>120){
            QString sub_stys = QString("QSlider::groove:Vertical {background: rgba(255, 255, 0,0);width:%1px;border-image:url(:/image/images/mbs400_high_equp.png);}"
                                       "QSlider::handle:Vertical {border-radius: 12px;width: 8px;height: 24px;margin-top: 0px;margin-left: -8px;margin-bottom: 0px;margin-right: -8px;background:white;}"
                                       "QSlider::sub-page:Vertical {background: rgba(255, 171, 33,0%);border-radius:2px;border-image:url(:/image/images/mbs400_high_eqback.png);}").arg(4*m_Magnification);
            this->setStyleSheet(sub_stys);
        }else{//问题
            this->setStyleSheet(
                        "QSlider::groove:Vertical {background: rgba(255, 255, 0,0);width:8px;border-image:url(:/image/images/mbs400_high_eqdown.png);}"
                        "QSlider::handle:Vertical {border-radius: 12px;width: 8px;height: 24px;margin-top: 0px;margin-left: -8px;margin-bottom: 0px;margin-right: -8px;background:white;}"
                        "QSlider::add-page:Vertical {background: rgba(255, 171, 33,0%);border-radius:2px;border-image:url(:/image/images/mbs400_high_eqback.png);}"
                        );
        }
        this->setDisabled(false);
    }

}
