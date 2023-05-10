#pragma once
#pragma execution_character_set("utf-8")
#include <QDebug>
#include "dm90_lightview.h"

#include "dm90_custom_switch.h"
#include <QTableWidget>
#include <QListWidget>
#include <QComboBox>
#include "customView/dm90_screenshotdelegate.h"

dm90_lightView::dm90_lightView(QWidget *parent) : QWidget(parent)
{

    this->setAttribute(Qt::WA_StyledBackground,true);

    //创建计时器
    m_breatheTimer = new QTimer(this);
    connect(m_breatheTimer, SIGNAL(timeout()), this, SLOT(breatheTimeout()));
}
void dm90_lightView::createUI(){

    //左边的视图
    QWidget *leftView = new QWidget(this);
    leftView->move(10*m_Magnification,0*m_Magnification);
    leftView->resize(350*m_Magnification,600*m_Magnification);
    leftView->setAttribute(Qt::WA_StyledBackground,true);
    leftView->setStyleSheet("background:rgb(17,17,17);border-radius: 15px;");

    QLabel *outputLbl = new QLabel(leftView);
    outputLbl->move(20*m_Magnification,10*m_Magnification);
    outputLbl->resize(this->width(),20*m_Magnification);
    outputLbl->setText(tr("灯光调节"));
    QString updateLabel_stys = QString("background-color:transparent;color:rgb(255,255,255);font-size:%1px;font-family:'Source Han Sans CN Medium'").arg(16*m_Magnification);
    outputLbl->setStyleSheet(updateLabel_stys);

    dm90_lightRGBimage *rgb_image = new dm90_lightRGBimage(leftView);
    rgb_image->resize(200*m_Magnification,200*m_Magnification);
    rgb_image->move((leftView->width()-rgb_image->width())/2,34*m_Magnification);
    rgb_image->createUI();
    connect(rgb_image,SIGNAL(custom_click(int,int,int)),this,SLOT(rgb_click(int,int,int)));
    rgb_image->setStyleSheet("border-radius: 200px");

    m_cus_switch = new dm90_custom_switch(leftView);
    m_cus_switch->resize(60*m_Magnification,20*m_Magnification);
    m_cus_switch->move(leftView->width()-m_cus_switch->width()-10*m_Magnification,outputLbl->y());
    QString swith_style = QString("border:2px solid rgb(255,255,255);background-color:transparent;border-radius: %1px").arg(20*m_Magnification/2);
    m_cus_switch->setStyleSheet(swith_style);
    m_cus_switch->createUI();
    connect(m_cus_switch,SIGNAL(lightswitchClick(int)),this,SLOT(lightadjSwitchClick(int)));

    m_bgLab = new QLabel(this);
    m_bgLab->move(((this->width()-leftView->x()-leftView->width())-(211*m_Magnification)/2)+50*m_Magnification,(this->height()-(524*m_Magnification))/2+10*m_Magnification);
    m_bgLab->resize(110*m_Magnification,320*m_Magnification);
    m_bgLab->setStyleSheet("background-color:rgb(255,0,0)");

    //滚动视图
    QWidget *rgbListView = new QWidget(this);
    rgbListView->move(10*m_Magnification,260*m_Magnification);
    rgbListView->resize(350*m_Magnification,100*m_Magnification);
    rgbListView->setAttribute(Qt::WA_StyledBackground,true);

    m_RGBlistWidge = new QListWidget(rgbListView);
    m_RGBlistWidge->resize(rgbListView->width(),rgbListView->height());
    m_RGBlistWidge->move(0 ,0);
    QString listWidget_style = QString("QListWidget{background-color:rgb(11,11,11);border:0px solid red; color:rgb(27,27,27);font-size:%1px;}"

                                       "QScrollBar:vertical{width:8px;background-color:rgb(45,45,45);border-radius:4px;margin-top:20px;margin-bottom:20px;}"

                                       "QScrollBar::handle:vertical{background-color:rgb(245,179,53);width:8px;border-radius:4px;}"

                                       "QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical{background:rgba(45,45,45,100%);border-radius:4px;}"

                                       "QScrollBar::sub-line:vertical{height:0px;width:0px;subcontrol-position:top;}").arg(12*m_Magnification).arg(34*m_Magnification);
    m_RGBlistWidge->setStyleSheet(listWidget_style);
    m_RGBlistWidge->verticalScrollBar()->setContextMenuPolicy(Qt::NoContextMenu);
    m_RGBlistWidge->horizontalScrollBar()->setContextMenuPolicy(Qt::NoContextMenu);

    m_RGBlistWidge->setViewMode(QListWidget::IconMode);  // 设置内容为图片
    //m_RGBlistWidge->setIconSize(QSize(10, 10));          // 设置图片的大小
    m_RGBlistWidge->setSpacing(14*m_Magnification);      // 设置每个item之间的间隔大小
    m_RGBlistWidge->setMovement(QListWidget::Static);    // 设置每个item不可拖动
    m_RGBlistWidge->setMaximumHeight(rgbListView->height());
    m_RGBlistWidge->setResizeMode(QListView::Fixed);

    dm90_screenShotDelegate *delegate = new dm90_screenShotDelegate(this);
    m_RGBlistWidge->setItemDelegate(delegate);
    //设置默认的颜色
    listRefreshColor();

    connect(m_RGBlistWidge, SIGNAL(itemClicked(QListWidgetItem*)),
            this, SLOT(onListRGBItemClicked(QListWidgetItem*)));

    m_RGBlistWidge->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_RGBlistWidge,&QListWidget::customContextMenuRequested,[=](const QPoint& pos){//右击删除ITEM
        //分两种情况，一种是带“+”，一种是没有带的
        bool exitAddBtn = false;
        for (int i=0;i<m_RGBlistWidge->count();i++) {
            QListWidgetItem *item = m_RGBlistWidge->item(i);
            if(item->whatsThis()=="lastAddItem"){
                exitAddBtn = true;
            }else{

            }
        }
        int row = m_RGBlistWidge->row(m_RGBlistWidge->itemAt(pos));
        //DM70*******DM90******
        if(currentProPid==0x0102){

            if(exitAddBtn){
                if(m_RGBlistWidge->count()>=10){
                    if(row>7 && row< m_RGBlistWidge->count()-1){
                        QListWidgetItem *item = m_RGBlistWidge->takeItem(row);
                        delete item;
                    }
                }
            }else{

            }

        }else if(currentProPid==0x0103){//DM70*********

            if(exitAddBtn){
                if(m_RGBlistWidge->count()>=10){//只有一种情况
                    QListWidgetItem*item = m_RGBlistWidge->takeItem(m_RGBlistWidge->count()-1);
                    delete item;
                    QListWidgetItem *itemRow = m_RGBlistWidge->takeItem(row);
                    delete itemRow;
                    m_listHSV.removeLast();

                    QVector<int>addrs;
                    QVector<int>value;
                    if(m_mode_number==0){
                        addrs = {0x20da,0x20cb,0x20cc,0x20cd,0x20ce,0x20cf};
                        value = {0,1,0,0,0,100};
                    }else if(m_mode_number==2){
                        addrs = {0x20da,0x20cb,0x20cc,0x20d7,0x20d8,0x20d9};
                        value = {0,1,0,0,0,100};
                    }
                    int frame_len = 7 + 4 *value.count();
                    m_commin_fun.sendRandomMessageForDevice(frame_len,addrs,value);
                    GET_DEVICE_DATA->dm90_appendNumHSV = 0;
                    removeItemStatus(row);
                }
            }else{
                if(row>7){

                    if(GET_DEVICE_DATA->dm90_appendNumHSV==2){//有两种情况

                        QString color_str;
                        if(row==8){//添加后面的颜色,然后选中默认颜色为白色
                            QListWidgetItem*item = m_RGBlistWidge->item(9);
                            color_str = item->whatsThis();
                            m_listHSV.removeAt(m_listHSV.count()-2);


                        }else if(row==9){//添加前面的颜色
                            QListWidgetItem*item = m_RGBlistWidge->item(8);
                            color_str = item->whatsThis();
                            m_listHSV.removeLast();

                        }

                        if(color_str.length()>10){
                            int a = color_str.indexOf("b(");
                            int b = color_str.indexOf(");");

                            QStringList rgbList = color_str.mid(a+2,b-a-2).split(',');

                            //RGB转HSV
                            BGR2HSV(rgbList.at(0).toInt(),rgbList.at(1).toInt(),rgbList.at(2).toInt());
                            QVector<int>addrs;
                            QVector<int>value;
                            addrs = {0x20da,0x20db,0x20dc,0x20dd,0x20cb,0x20cc,0x20cd,0x20ce,0x20cf};
                            value = {1,m_H,(int)(m_S*100),(int)(m_V*100),1,0,0,0,100};
                            int frame_len = 7 + 4 *value.count();
                            m_commin_fun.sendRandomMessageForDevice(frame_len,addrs,value);
                        }

                        GET_DEVICE_DATA->dm90_appendNumHSV = 1;

                    }else if(GET_DEVICE_DATA->dm90_appendNumHSV==1){//只有一种颜色
                        QVector<int>addrs;
                        QVector<int>value;
                        if(m_mode_number==0){
                            addrs = {0x20da,0x20cb,0x20cc,0x20cd,0x20ce,0x20cf};
                            value = {0,1,0,0,0,100};
                        }else if(m_mode_number==2){
                            addrs = {0x20da,0x20cb,0x20cc,0x20d7,0x20d8,0x20d9};
                            value = {0,1,0,0,0,100};
                        }
                        int frame_len = 7 + 4 *value.count();
                        m_commin_fun.sendRandomMessageForDevice(frame_len,addrs,value);
                        GET_DEVICE_DATA->dm90_appendNumHSV = 0;
                        m_listHSV.removeLast();
                    }
                    removeItemStatus(row);
                    QListWidgetItem *item = m_RGBlistWidge->takeItem(row);
                    delete item;
                }
            }
        }

    });

    QWidget *brightness_unit = new QWidget(leftView);
    brightness_unit->setAttribute(Qt::WA_StyledBackground,true);
    brightness_unit->resize(leftView->width(),80*m_Magnification);
    brightness_unit->move(0,rgbListView->y()+rgbListView->height());

    QLabel *highLow_lab = new QLabel(brightness_unit);
    highLow_lab->move(20*m_Magnification,14*m_Magnification);
    highLow_lab->resize(100*m_Magnification,16*m_Magnification);
    highLow_lab->setText(tr("亮度"));
    QString highLow_lab_stys = QString("background-color:transparent;color:rgb(255,255,255);font-size:%1px;font-family:'Source Han Sans CN Medium'").arg(12*m_Magnification);
    highLow_lab->setStyleSheet(highLow_lab_stys);

    //亮度
    MySlider *brightness_slider = new MySlider(brightness_unit);
    brightness_slider->setOrientation(Qt::Horizontal);
    brightness_slider->setMinimum(0);
    brightness_slider->setMaximum(100);
    brightness_slider->setValue(xianfuqi_slider_values.at(0));
    brightness_slider->setGeometry(20*m_Magnification,highLow_lab->y()+highLow_lab->height()+10*m_Magnification,260*m_Magnification,12*m_Magnification);
    connect(brightness_slider,SIGNAL(valueChanged(int)),this,SLOT(brightness_change_value(int)));
    connect(brightness_slider,SIGNAL(sliderReleasedAt(int)),this,SLOT(brightness_release_value(int)));
    m_brightness_slider = brightness_slider;

    m_briCurrentValue = new QLabel(brightness_unit);
    m_briCurrentValue->move(brightness_unit->width()-60*m_Magnification,brightness_slider->y()-2*m_Magnification);
    m_briCurrentValue->resize(40*m_Magnification,14*m_Magnification);
    m_briCurrentValue->setText("-80dB");
    QString currentLabel_stys = QString("background-color:transparent;color:rgb(255,255,255);font-size:%1px;font-family:'Source Han Sans CN Medium'").arg(12*m_Magnification);
    m_briCurrentValue->setStyleSheet(currentLabel_stys);

    QLabel *left_value = new QLabel(brightness_unit);
    left_value->resize(40*m_Magnification,14*m_Magnification);
    left_value->move(brightness_slider->x(),brightness_slider->y()+brightness_slider->height()+5*m_Magnification);
    left_value->setText("0");
    left_value->setStyleSheet(currentLabel_stys);

    QLabel *right_value = new QLabel(brightness_unit);
    right_value->resize(40*m_Magnification,16*m_Magnification);
    right_value->move(brightness_slider->x()+brightness_slider->width()-40*m_Magnification,brightness_slider->y()+brightness_slider->height()+5*m_Magnification);
    right_value->setText("100");
    right_value->setStyleSheet(currentLabel_stys);
    right_value ->setAlignment(Qt::AlignRight);

    //效果
    QLabel *resultLbl = new QLabel(leftView);
    resultLbl->move(20*m_Magnification,brightness_unit->y()+brightness_unit->height());
    resultLbl->resize(this->width(),16*m_Magnification);
    resultLbl->setText(tr("效果"));
    //QString updateLabel_stys = QString("background-color:transparent;color:rgb(255,255,255);font-size:%1px;font-family:'Source Han Sans CN Medium'").arg(16*m_Magnification);
    resultLbl->setStyleSheet(updateLabel_stys);

    //下拉框
    m_LedCbo_sex = new QComboBox(leftView);
    m_LedCbo_sex->move(18*m_Magnification,resultLbl->y()+resultLbl->height()+10*m_Magnification);
    language_index==0?m_LedCbo_sex->resize(100*m_Magnification,24*m_Magnification):m_LedCbo_sex->resize(150*m_Magnification,24*m_Magnification);
    m_LedCbo_sex->addItem(tr("固定"));
    m_LedCbo_sex->addItem(tr("循环"));
    m_LedCbo_sex->addItem(tr("呼吸"));
    QString cbo_sex_stys = QString("QComboBox {background-color:transparent;color:rgb(255,255,255);font-size:%1px;font-family:'Source Han Sans CN Medium';border-radius: 0px;}"
                                   "QComboBox QAbstractItemView {color: white;background-color: transparent;selection-color: #3377FF;selection-background-color:#FFFFFF;border-radius: 0px;outline: 0px;}"
                                   "QComboBox QAbstractItemView::item {height:%2px;}"
                                   "QComboBox::drop-down {width: %3px;}"
                                   ).arg(18*m_Magnification).arg(30*m_Magnification).arg(22*m_Magnification);
    m_LedCbo_sex->setStyleSheet(cbo_sex_stys);
    m_LedCbo_sex->setView(new  QListView());
    connect(m_LedCbo_sex, SIGNAL(currentIndexChanged(int)), this, SLOT(change_result(int)));

    //速度
    m_speedview = new QWidget(leftView);
    m_speedview->setAttribute(Qt::WA_StyledBackground,true);
    m_speedview->resize(leftView->width(),80*m_Magnification);
    m_speedview->move(0,m_LedCbo_sex->y()+m_LedCbo_sex->height());
    //m_speedview->setStyleSheet("background:rgb(0,255,0)");

    QLabel *speed_lab = new QLabel(m_speedview);
    speed_lab->move(20*m_Magnification,14*m_Magnification);
    speed_lab->resize(100,14*m_Magnification);
    speed_lab->setText(tr("速度"));
    QString speed_lab_stys = QString("background-color:transparent;color:rgb(255,255,255);font-size:%1px;font-family:'Source Han Sans CN Medium'").arg(12*m_Magnification);
    speed_lab->setStyleSheet(speed_lab_stys);

    MySlider *speed_slider = new MySlider(m_speedview);
    speed_slider->setOrientation(Qt::Horizontal);
    speed_slider->setMinimum(1000);
    speed_slider->setMaximum(20000);
    speed_slider->setValue(xianfuqi_slider_values.at(0));
    speed_slider->setGeometry(20*m_Magnification,highLow_lab->y()+highLow_lab->height()+10*m_Magnification,260*m_Magnification,12*m_Magnification);
    connect(speed_slider,SIGNAL(valueChanged(int)),this,SLOT(speed_change_value(int)));
    connect(speed_slider,SIGNAL(sliderReleasedAt(int)),this,SLOT(speed_release_value(int)));
    m_speed_slider = speed_slider;

    m_speedCurrentValue = new QLabel(m_speedview);
    m_speedCurrentValue->move(brightness_unit->width()-60*m_Magnification,speed_slider->y()-2*m_Magnification);
    m_speedCurrentValue->resize(60*m_Magnification,14*m_Magnification);
    m_speedCurrentValue->setText("-80dB");
    //QString currentLabel_stys = QString("background-color:transparent;color:rgb(255,255,255);font-size:%1px;font-family:'Source Han Sans CN Medium'").arg(12*m_Magnification);
    m_speedCurrentValue->setStyleSheet(currentLabel_stys);

    left_value = new QLabel(m_speedview);
    left_value->resize(40*m_Magnification,14*m_Magnification);
    left_value->move(brightness_slider->x(),brightness_slider->y()+brightness_slider->height()+5*m_Magnification);
    left_value->setText("1000ms");
    left_value->setStyleSheet(currentLabel_stys);

    right_value = new QLabel(m_speedview);
    right_value->resize(60*m_Magnification,16*m_Magnification);
    right_value->move(brightness_slider->x()+brightness_slider->width()-40*m_Magnification,brightness_slider->y()+brightness_slider->height()+5*m_Magnification);
    right_value->setText("20000ms");
    right_value->setStyleSheet(currentLabel_stys);
    right_value ->setAlignment(Qt::AlignRight);

    //添加蒙版
    m_mask_view = new QWidget(this);
    m_mask_view->resize(leftView->width(),leftView->height());
    m_mask_view->move(leftView->x(),30*m_Magnification);
    m_mask_view->setAttribute(Qt::WA_StyledBackground,true);
    m_mask_view->setStyleSheet("background:rgba(3,3,3,200)");
    m_mask_view->hide();

    //加RGB的蒙版
    m_RGBmask_view = new QWidget(this);
    m_RGBmask_view->resize(leftView->width(),brightness_unit->y());
    m_RGBmask_view->move(0,30*m_Magnification);
    m_RGBmask_view->setAttribute(Qt::WA_StyledBackground,true);
    m_RGBmask_view->setStyleSheet("background:rgba(3,3,3,200)");
    m_RGBmask_view->hide();

    //把视图置顶
    outputLbl->raise();
    m_cus_switch->raise();

    //右边的视图
    m_devicebgImage = new QLabel(this);
    m_devicebgImage->resize(211*m_Magnification,524*m_Magnification);
    m_devicebgImage->move(((this->width()-leftView->x()-leftView->width())-m_devicebgImage->width()/2),(this->height()-m_devicebgImage->height())/2);
    m_devicebgImage->setPixmap(QPixmap(":/image/images/DM90/dm90_deviceImg.png"));
    m_devicebgImage->setScaledContents(true);

    //设置按钮
    custom_button *btn = new custom_button(this);
    btn->resize(60*m_Magnification,36*m_Magnification);
    btn->move(this->width()-btn->width()-20*m_Magnification,this->height()-btn->height()-20*m_Magnification);
    btn->setText(tr("重置"));
    QString btn_stys = QString("QPushButton{background-color:rgb(68,68,68) ;font-family:'Source Han Sans CN Medium';color:white;font-size:%1px;border:0px;border-radius: 6px;}"
                               "QPushButton:pressed{background-color:rgb(255,171,33) ;font-family:'Source Han Sans CN Medium';color:white;font-size:%1px;border:0px}"
                               "QPushButton:checked{background-color:rgb(255,171,33) ;font-family:'Source Han Sans CN Medium';color:white;font-size:%1px;border:0px}"
                               ).arg(16*m_Magnification);
    btn->setStyleSheet(btn_stys);
    connect(btn,SIGNAL(clicked()),this,SLOT(reset_click()));
}
void dm90_lightView::refresh(){

    if(currentProPid==0x0102){//DM90

        m_devicebgImage->resize(211*m_Magnification,524*m_Magnification);
        m_devicebgImage->setPixmap(QPixmap(":/image/images/DM90/dm90_deviceImg.png"));
        m_devicebgImage->move(((this->width()-10*m_Magnification-350*m_Magnification)-m_devicebgImage->width()/2),(this->height()-m_devicebgImage->height())/2);
        m_bgLab->move(((this->width()-10*m_Magnification-350*m_Magnification)-(211*m_Magnification)/2)+50*m_Magnification,(this->height()-(524*m_Magnification))/2+10*m_Magnification);

    }else if(currentProPid==0x0103){//DM70

        m_devicebgImage->resize(161*m_Magnification,447*m_Magnification);
        m_devicebgImage->setPixmap(QPixmap(":/image/images/DM90/dm70_deviceImg.png"));
        m_devicebgImage->move(((this->width()-10*m_Magnification-350*m_Magnification)-m_devicebgImage->width()/2),(this->height()-m_devicebgImage->height())/2);
        m_bgLab->move(((this->width()-10*m_Magnification-350*m_Magnification)-(211*m_Magnification)/2)+50*m_Magnification,(this->height()-(524*m_Magnification))/2+40*m_Magnification);
    }
    //开关的按钮
    m_cus_switch->refreshUI();

    int H=0, S=0, V=0;

    //是否显示灯光的蒙版
    if(GET_DEVICE_DATA->dm90_LED_OnOff==0){//关闭颜色
        m_mask_view->show();
        //关闭图片的颜色
        QString str1 = QString("background:rgb(%1,%2,%3)").arg(0).arg(0).arg(0);
        m_bgLab->setStyleSheet(str1);

    }else{
        m_mask_view->hide();
        //有三种情况，0固定，1循环，2呼吸
    }

    qDebug()<<"效果========"<<GET_DEVICE_DATA->dm90_LED_Mode<<GET_DEVICE_DATA->dm90_FIX_V;
    m_mode_number = GET_DEVICE_DATA->dm90_LED_Mode;

    if(GET_DEVICE_DATA->dm90_LED_Mode==0){

        //设置右边图片RGB的值
        H = GET_DEVICE_DATA->dm90_FIX_H;
        S = GET_DEVICE_DATA->dm90_FIX_S;
        V = GET_DEVICE_DATA->dm90_FIX_V;
        //转换为RGB
        HSVtoRGB(true,H/100,S,V);

    }else if(GET_DEVICE_DATA->dm90_LED_Mode==1){//循坏

        //设置右边图片RGB的值
        H = GET_DEVICE_DATA->dm90_LOOP_H;
        S = GET_DEVICE_DATA->dm90_LOOP_S;
        V = GET_DEVICE_DATA->dm90_LOOP_V;
        //转换为RGB
        //HSVtoRGB(H/100,S,V);
        m_RGBmask_view->show();
        m_breatheTimer->start(20);

    }else if(GET_DEVICE_DATA->dm90_LED_Mode==2){

        //设置右边图片RGB的值
        H = GET_DEVICE_DATA->dm90_breathe_H;
        S = GET_DEVICE_DATA->dm90_breathe_S;
        V = GET_DEVICE_DATA->dm90_breathe_V;
        //转换为RGB
        HSVtoRGB(true,H/100,S,V);
        m_breatheTimer->start(20);
    }
    //亮度
    m_brightness_slider->blockSignals(true);
    m_brightness_slider->setValue(V);
    m_briCurrentValue->setText(QString::number(V,10));
    m_brightness_slider->blockSignals(false);

    //呼吸的速度
    m_speed_slider->blockSignals(true);
    m_speed_slider->setValue(GET_DEVICE_DATA->dm90_breathe_Rate);
    m_speedCurrentValue->setText(QString::number(GET_DEVICE_DATA->dm90_breathe_Rate,10)+"ms");
    m_speed_slider->blockSignals(false);

    //下拉框
    m_LedCbo_sex->blockSignals(true);
    m_LedCbo_sex->setCurrentIndex(GET_DEVICE_DATA->dm90_LED_Mode);
    m_LedCbo_sex->blockSignals(false);
    QString str = QString("background:rgb(%1,%2,%3)").arg(m_R).arg(m_G).arg(m_B);
    if(GET_DEVICE_DATA->dm90_LED_OnOff!=0)
        m_bgLab->setStyleSheet(str);

    if(GET_DEVICE_DATA->dm90_LED_Mode==0){//打开

        // if(GET_DEVICE_DATA->dm90_LED_OnOff==1)
        //change_result(0);
        m_speedview->hide();

    }else if(GET_DEVICE_DATA->dm90_LED_Mode==1){
        m_speedview->hide();
    }else{

    }
    //根据NumHSV增加ITEM
    if(GET_DEVICE_DATA->dm90_appendNumHSV==0){//不需要增加
        listRefreshColor();
    }else if(GET_DEVICE_DATA->dm90_appendNumHSV==1){//增加一个

        uint32_t rgb = HSVtoRGB(false,GET_DEVICE_DATA->dm90_appendH1/100,GET_DEVICE_DATA->dm90_appendS1,GET_DEVICE_DATA->dm90_appendV1);
        QString str = QString("background:rgb(%1,%2,%3);border-radius: %4px;").arg((rgb>>8)&0xff).arg((rgb>>16)&0xff).arg(rgb&0xff).arg(10*m_Magnification);
        appendItem(str);

        QVector<int>hsvColor = {GET_DEVICE_DATA->dm90_appendH1,GET_DEVICE_DATA->dm90_appendS1,GET_DEVICE_DATA->dm90_appendV1};
        m_listHSV.push_back(hsvColor);

    }else if(GET_DEVICE_DATA->dm90_appendNumHSV==2){//增加2个

        uint32_t rgb = HSVtoRGB(false,GET_DEVICE_DATA->dm90_appendH1/100,GET_DEVICE_DATA->dm90_appendS1,GET_DEVICE_DATA->dm90_appendV1);
        QString str = QString("background:rgb(%1,%2,%3);border-radius: %4px;").arg((rgb>>8)&0xff).arg((rgb>>16)&0xff).arg(rgb&0xff).arg(10*m_Magnification);
        appendItem(str);
        QVector<int>hsvColor = {GET_DEVICE_DATA->dm90_appendH1,GET_DEVICE_DATA->dm90_appendS1,GET_DEVICE_DATA->dm90_appendV1};
        m_listHSV.push_back(hsvColor);

        uint32_t rgb2 = HSVtoRGB(false,GET_DEVICE_DATA->dm90_appendH2/100,GET_DEVICE_DATA->dm90_appendS2,GET_DEVICE_DATA->dm90_appendV2);
        str = QString("background:rgb(%1,%2,%3);border-radius: %4px;").arg((rgb2>>8)&0xff).arg((rgb2>>16)&0xff).arg(rgb2&0xff).arg(10*m_Magnification);
        appendItem(str);
        QVector<int>hsvColor2 = {GET_DEVICE_DATA->dm90_appendH2,GET_DEVICE_DATA->dm90_appendS2,GET_DEVICE_DATA->dm90_appendV2};
        m_listHSV.push_back(hsvColor2);
    }
}

void dm90_lightView::refreshColorItem(const int onoff,const int model,const int H,const int S,const int V){

    GET_DEVICE_DATA->dm90_LED_OnOff = onoff;
    m_cus_switch->refreshUI();
    if(onoff==0){//关闭
        m_mask_view->show();
        QString str1 = QString("background:rgb(%1,%2,%3)").arg(0).arg(0).arg(0);
        m_bgLab->setStyleSheet(str1);
    }else{//打开
        m_mask_view->hide();
        uint32_t rgb = HSVtoRGB(false,(int)H/100,S,V);
        QString backStr = QString("background:rgb(%1,%2,%3);").arg((rgb>>8)&0xff).arg((rgb>>16)&0xff).arg(rgb&0xff);
        m_bgLab->setStyleSheet(backStr);
        //同步到列表中
        int deviceColorIndex =-1;
        for (int i=0;i<m_listHSV.count();i++) {
            QListWidgetItem*item = m_RGBlistWidge->item(i);
            QString colorStr = item->whatsThis();
            QList<QLabel*> labelList = m_RGBlistWidge->itemWidget(item)->findChildren<QLabel*>();
            if(labelList.count()>0)labelList.at(0)->hide();

                QVector<int>hsvColor = m_listHSV.at(i);
                if(hsvColor.count()<3)return;
                if ((hsvColor.at(0)==H&&hsvColor.at(1)*100==S&&hsvColor.at(2)*100==V) || (hsvColor.at(0)==H&&hsvColor.at(1)==S&&hsvColor.at(2)==V) ) {//包含了
                    deviceColorIndex  = i;
                }
        }
        if(deviceColorIndex ==-1)return;
        QList<QLabel*> labelList = m_RGBlistWidge->itemWidget(m_RGBlistWidge->item(deviceColorIndex))->findChildren<QLabel*>();
        if(labelList.count()>0)labelList.at(0)->show();
        //qDebug()<<"gggggggggg"<<deviceColorIndex<<backStr<<H<<S<<V<<m_listHSV;
    }

    m_LedCbo_sex->blockSignals(true);
    m_LedCbo_sex->setCurrentIndex(model);
    m_LedCbo_sex->blockSignals(false);
    change_mode(false,model);

}

//按钮重置的方法
void dm90_lightView::reset_click(){

    m_mask_view->show();
    //开关按钮关闭状态
    m_cus_switch->setStyleBtn(0);

    //亮度设置为50
    m_brightness_slider->blockSignals(true);
    m_brightness_slider->setValue(100);
    m_briCurrentValue->setText("100");
    m_brightness_slider->blockSignals(false);

    HSVtoRGB(true,0,0,100);

    //速度设置为3000
    m_speed_slider->blockSignals(true);
    m_speed_slider->setValue(3000);
    m_speedCurrentValue->setText("3000ms");
    m_speed_slider->blockSignals(false);
    //效果设置为固定
    m_LedCbo_sex->setCurrentIndex(0);

    QString str = QString("background:rgb(%1,%2,%3)").arg(0).arg(0).arg(0);
    m_bgLab->setStyleSheet(str);
    //发送数据
    QVector<int>addrs;
    QVector<int>value={0,0,3000,50,100,0,0};
    for (int i=0;i<7;i++) {
        addrs.push_back(0x20CB+i);
    }

    if(currentProPid==0x0103){//DM70

        //QVector<int>addrs= {0x20da};
        addrs.push_back(0x20da);
        value.push_back(0);
        int frame_len = 7 + 4 *value.count();
        m_commin_fun.sendRandomMessageForDevice(frame_len,addrs,value);
        listRefreshColor();
        GET_DEVICE_DATA->dm90_appendNumHSV = 0;

    }else if(currentProPid==0x0102){//DM90

        m_commin_fun.sendRandomMessageForDevice(0x23,addrs,value);
    }

}
//吸取RGB颜色*****************************************
void dm90_lightView::rgb_click(int r,int g,int b){

    //取消列表的选中
    cancelListItemSelectStatus();
    m_R = r;
    m_G = g;
    m_B = b;

    BGR2HSV(r,g,b);

    //发送信息
    QVector<int>addrs;
    QVector<int>value;
    if(currentProPid==0x0102){

        if(m_mode_number==0){//固定
            addrs = {0x20cd,0x20ce,0x20cf};
            value = {m_H,(int)(m_S*100),GET_DEVICE_DATA->dm90_FIX_V};
        }else if(m_mode_number==1){
            addrs = {0x20d2,0x20d3,0x20d4};
            value = {m_H,(int)(m_S*100),GET_DEVICE_DATA->dm90_LOOP_V};
        }else if(m_mode_number==2){
            addrs = {0x20d7,0x20d8,0x20d9};
            value = {m_H,(int)(m_S*100),GET_DEVICE_DATA->dm90_breathe_V};
        }
        m_commin_fun.sendRandomMessageForDevice(0x13,addrs,value);

    }else if(currentProPid==0x0103){

        if(m_mode_number==0){//固定
            addrs = {0x20cb,0x20cc,0x20cd,0x20ce,0x20cf};
            value = {1,0,m_H,(int)(m_S*100),GET_DEVICE_DATA->dm90_FIX_V};
        }else if(m_mode_number==1){
            addrs = {0x20cb,0x20cc,0x20d2,0x20d3,0x20d4};
            value = {1,1,m_H,(int)(m_S*100),GET_DEVICE_DATA->dm90_LOOP_V};
        }else if(m_mode_number==2){
            addrs = {0x20cb,0x20cc,0x20d7,0x20d8,0x20d9};
            value = {1,2,m_H,(int)(m_S*100),GET_DEVICE_DATA->dm90_breathe_V};
        }
        int frame_len = 7 + 4 *value.count();
        qDebug()<<"kk========="<<r<<g<<b<<value;
        m_commin_fun.sendRandomMessageForDevice(frame_len,addrs,value);
    }


    if(GET_DEVICE_DATA->dm90_LED_Mode==0){//固定

        GET_DEVICE_DATA->dm90_FIX_H = m_H;
        GET_DEVICE_DATA->dm90_FIX_S = m_S*100;
        HSVtoRGB(true,GET_DEVICE_DATA->dm90_FIX_H/100,GET_DEVICE_DATA->dm90_FIX_S,GET_DEVICE_DATA->dm90_FIX_V);

    }else if(GET_DEVICE_DATA->dm90_LED_Mode==1){//打开

        GET_DEVICE_DATA->dm90_LOOP_H = m_H;
        GET_DEVICE_DATA->dm90_LOOP_S = m_S*100;
        HSVtoRGB(true,GET_DEVICE_DATA->dm90_LOOP_H/100,GET_DEVICE_DATA->dm90_LOOP_S,GET_DEVICE_DATA->dm90_LOOP_V);

    }else if(GET_DEVICE_DATA->dm90_LED_Mode==2){

        GET_DEVICE_DATA->dm90_breathe_H = m_H;
        GET_DEVICE_DATA->dm90_breathe_S = m_S*100;
        HSVtoRGB(true,GET_DEVICE_DATA->dm90_breathe_H/100,GET_DEVICE_DATA->dm90_breathe_S,GET_DEVICE_DATA->dm90_breathe_V);
    }

    QString str = QString("background:rgb(%1,%2,%3)").arg(m_R).arg(m_G).arg(m_B);
    m_bgLab->setStyleSheet(str);
    m_breatheR=m_R,m_breatheG=m_G, m_breatheB=m_B;

    //对比列表是否有相同的颜色，如果没有则增加“+”按钮
    if(currentProPid == 0x0102){
        exitSamecolorAndappendItem();
    }else if(currentProPid == 0x0103){
        DM70exitSamecolorAndappendItem();
    }

}
//item的点击事件****************************
void dm90_lightView::onListRGBItemClicked(QListWidgetItem*item){

    if(item->whatsThis()=="lastAddItem"){//增加颜色的方法
        QString rgb_style;
        if(GET_DEVICE_DATA->dm90_LED_Mode==2){
            rgb_style = QString("background:rgb(%1,%2,%3);border-radius: %4px;").arg(m_breatheR).arg(m_breatheG).arg(m_breatheB).arg(10*m_Magnification);
        }else{
            rgb_style = QString("background:rgb(%1,%2,%3);border-radius: %4px;").arg(m_R).arg(m_G).arg(m_B).arg(10*m_Magnification);
        }
        QListWidgetItem* item = new QListWidgetItem;
        item->setSizeHint(QSize(30*m_Magnification, 30*m_Magnification));
        item->setWhatsThis(rgb_style);
        m_RGBlistWidge->insertItem(m_RGBlistWidge->count()-1,item);

        qDebug()<<"hsv222----"<<GET_DEVICE_DATA->dm90_FIX_V<<GET_DEVICE_DATA->dm90_LOOP_V<<GET_DEVICE_DATA->dm90_appendNumHSV<<rgb_style;
        //增加ITEM前"勾"去掉
        cancelListItemSelectStatus();

        QLabel *rgbitemView = new QLabel();
        rgbitemView->resize(30*m_Magnification,30*m_Magnification);
        rgbitemView->setAttribute(Qt::WA_StyledBackground,true);
        rgbitemView->setStyleSheet(rgb_style);

        QLabel *selectLal = new QLabel(rgbitemView);
        selectLal->resize(11*m_Magnification,8*m_Magnification);
        selectLal->move(20*m_Magnification,0*m_Magnification);
        selectLal->setPixmap(QPixmap(":/image/images/DM90/item_select.png"));
        selectLal->setStyleSheet("background:transparent");
        selectLal->setScaledContents(true);
        selectLal->show();
        m_RGBlistWidge->setItemWidget(item,rgbitemView);       //增加item

        //如果是DM70设备需要往设备写入数据，否则不用
        if(currentProPid == 0x0103){ //DM70

            if(GET_DEVICE_DATA->dm90_appendNumHSV==0){
                QVector<int>addrs;
                QVector<int>value;
                addrs = {0x20da,0x20db,0x20dc,0x20dd};
                QVector<int>hsvColor;
                if(m_mode_number==0){
                   value = {1,GET_DEVICE_DATA->dm90_FIX_H,GET_DEVICE_DATA->dm90_FIX_S,GET_DEVICE_DATA->dm90_FIX_V};
                   hsvColor = {GET_DEVICE_DATA->dm90_FIX_H,GET_DEVICE_DATA->dm90_FIX_S,GET_DEVICE_DATA->dm90_FIX_V};
                }else if(m_mode_number==2){
                   value = {1,GET_DEVICE_DATA->dm90_breathe_H,GET_DEVICE_DATA->dm90_breathe_S,GET_DEVICE_DATA->dm90_breathe_V};
                   hsvColor = {GET_DEVICE_DATA->dm90_breathe_H,GET_DEVICE_DATA->dm90_breathe_S,GET_DEVICE_DATA->dm90_breathe_V};
                }
                int frame_len = 7 + 4 *value.count();
                m_commin_fun.sendRandomMessageForDevice(frame_len,addrs,value);
                m_listHSV.push_back(hsvColor);
                GET_DEVICE_DATA->dm90_appendNumHSV = 1;
            }else if(GET_DEVICE_DATA->dm90_appendNumHSV==1){
                QVector<int>addrs;
                QVector<int>value;
                addrs = {0x20da,0x20de,0x20df,0x20e0};
                QVector<int>hsvColor;
                if(m_mode_number==0){
                   value = {2,GET_DEVICE_DATA->dm90_FIX_H,GET_DEVICE_DATA->dm90_FIX_S,GET_DEVICE_DATA->dm90_FIX_V};
                   hsvColor = {GET_DEVICE_DATA->dm90_FIX_H,GET_DEVICE_DATA->dm90_FIX_S,GET_DEVICE_DATA->dm90_FIX_V};
                }else if(m_mode_number==2){
                   value = {2,GET_DEVICE_DATA->dm90_breathe_H,GET_DEVICE_DATA->dm90_breathe_S,GET_DEVICE_DATA->dm90_breathe_V};
                   hsvColor = {GET_DEVICE_DATA->dm90_breathe_H,GET_DEVICE_DATA->dm90_breathe_S,GET_DEVICE_DATA->dm90_breathe_V};
                }
                qDebug()<<"gggggggggg2222222"<<value;
                int frame_len = 7 + 4 *value.count();
                m_commin_fun.sendRandomMessageForDevice(frame_len,addrs,value);
                m_listHSV.push_back(hsvColor);
                GET_DEVICE_DATA->dm90_appendNumHSV = 2;
            }
        }

        for (int i=0;i<m_RGBlistWidge->count();i++) {
            QListWidgetItem *item = m_RGBlistWidge->item(i);
            if(item->whatsThis()=="lastAddItem"){
                //隐藏最后的“+”按钮
                QListWidgetItem*item = m_RGBlistWidge->takeItem(m_RGBlistWidge->count()-1);
                delete item;
            }
        }
    }else{

        m_currentSelectItemIndex = m_RGBlistWidge->row(item);
        QList<QLabel*> labelList = m_RGBlistWidge->itemWidget(item)->findChildren<QLabel*>();
        for (int i=0;i<m_RGBlistWidge->count();i++) {
            QListWidgetItem *item = m_RGBlistWidge->item(i);
            QList<QLabel*> labelList = m_RGBlistWidge->itemWidget(item)->findChildren<QLabel*>();
            if(labelList.count()>0)labelList.at(0)->hide();
            if(item->whatsThis()=="lastAddItem"){
                //隐藏最后的“+”按钮
                QListWidgetItem*item = m_RGBlistWidge->takeItem(m_RGBlistWidge->count()-1);
                delete item;
            }
        }
        labelList.at(0)->show();

        QString str = item->whatsThis();
        if(str.length()>10){

            int a = str.indexOf("b(");
            int b = str.indexOf(");");

            QStringList rgbList = str.mid(a+2,b-a-2).split(',');
            QVector<int>addrs;
            QVector<int>value;
            //RGB转HSV
            BGR2HSV(rgbList.at(0).toInt(),rgbList.at(1).toInt(),rgbList.at(2).toInt());
            m_R = rgbList.at(0).toInt(),m_G = rgbList.at(1).toInt(),m_B = rgbList.at(2).toInt();
            //两种情况，
            if(currentProPid==0x0102){
                if(m_mode_number==0){//固定
                    addrs = {0x20cd,0x20ce,0x20cf};
                    m_bgLab->setStyleSheet(str);
                    GET_DEVICE_DATA->dm90_FIX_H = m_H;
                    GET_DEVICE_DATA->dm90_FIX_S = m_S*100;
                    GET_DEVICE_DATA->dm90_FIX_V = m_V*100;

                }else if(m_mode_number==1){
                    addrs = {0x20d2,0x20d3,0x20d4};
                }else if(m_mode_number==2){
                    addrs = {0x20d7,0x20d8,0x20d9};
                    GET_DEVICE_DATA->dm90_breathe_H = m_H;
                    GET_DEVICE_DATA->dm90_breathe_S = m_S*100;
                    GET_DEVICE_DATA->dm90_breathe_V = m_V*100;
                }
                value = {m_H,(int)(m_S*100),(int)(m_V*100)};
                m_commin_fun.sendRandomMessageForDevice(0x13,addrs,value);
            }else if(currentProPid==0x0103){//DM70

                QVector<int>hsvColor = m_listHSV.at(m_currentSelectItemIndex);
                if(m_mode_number==0){//固定
                    addrs = {0x20cb,0x20cc,0x20cd,0x20ce,0x20cf};
                    m_bgLab->setStyleSheet(str);
                    GET_DEVICE_DATA->dm90_FIX_H = hsvColor.at(0);
                    GET_DEVICE_DATA->dm90_FIX_S = hsvColor.at(1);
                    GET_DEVICE_DATA->dm90_FIX_V = hsvColor.at(2);
                    value = {1,0,GET_DEVICE_DATA->dm90_FIX_H,GET_DEVICE_DATA->dm90_FIX_S,GET_DEVICE_DATA->dm90_FIX_V};

                }else if(m_mode_number==1){
                    addrs = {0x20cb,0x20cc,0x20d2,0x20d3,0x20d4};
                    value = {1,1,m_H,(int)(m_S*100),(int)(m_V*100)};
                }else if(m_mode_number==2){
                    addrs = {0x20cb,0x20cc,0x20d7,0x20d8,0x20d9};
                    GET_DEVICE_DATA->dm90_breathe_H = hsvColor.at(0);
                    GET_DEVICE_DATA->dm90_breathe_S = hsvColor.at(1);
                    GET_DEVICE_DATA->dm90_breathe_V = hsvColor.at(2);
                    value = {1,2,GET_DEVICE_DATA->dm90_breathe_H,GET_DEVICE_DATA->dm90_breathe_S,GET_DEVICE_DATA->dm90_breathe_V};
                }
                int frame_len = 7 + 4 *value.count();
                qDebug()<<"kkpppppp========="<<value<<m_V<<m_currentSelectItemIndex<<hsvColor;
                m_commin_fun.sendRandomMessageForDevice(frame_len,addrs,value);
            }
        }
    }
}

void dm90_lightView::brightness_change_value(int currentValue){

    m_currentSliderNum = 1;
    m_briCurrentValue->setText(QString::number(currentValue,10));
    m_DM90delayTimer->stop();
    m_DM90delayTimer->start(20);
}

//延迟发送信息======================
void dm90_lightView::lightStarDelayTimerFun(){

    if(Qwdget_timer_index==3){

        if(m_currentSliderNum == 1){//亮度
            QVector<int>adders={0x20cf,0x20d4,0x20d9};
            m_commin_fun.sendMessageForDevice(adders.at(m_mode_number),1,m_brightness_slider->value());
            //修改上位机的亮度
            int H=0, S=0, V=0;
            if(GET_DEVICE_DATA->dm90_LED_Mode==0){
                //设置右边图片RGB的值
                H = GET_DEVICE_DATA->dm90_FIX_H;
                S = GET_DEVICE_DATA->dm90_FIX_S;
                V = m_brightness_slider->value();
                GET_DEVICE_DATA->dm90_FIX_V = m_brightness_slider->value();
                //转换为RGB
                HSVtoRGB(true,H/100,S,V);
                QString str = QString("background:rgb(%1,%2,%3)").arg(m_R).arg(m_G).arg(m_B);
                m_bgLab->setStyleSheet(str);

                //对比列表是否有相同的颜色，如果没有则增加“+”按钮
                if(currentProPid==0x0102){
                  exitSamecolorAndappendItem();
                }else if(currentProPid==0x0103){
                  DM70exitSamecolorAndappendItem();
                }


            }else if(GET_DEVICE_DATA->dm90_LED_Mode==1){
                GET_DEVICE_DATA->dm90_LOOP_V = m_brightness_slider->value();
            }else if(GET_DEVICE_DATA->dm90_LED_Mode==2){
                GET_DEVICE_DATA->dm90_breathe_V = m_brightness_slider->value();
            }
        }else if(m_currentSliderNum == 2){//速度
            m_commin_fun.sendMessageForDevice(0x20d6,1,m_speed_slider->value());
            GET_DEVICE_DATA->dm90_breathe_Rate = m_speed_slider->value();
            m_breatheTimer->start(GET_DEVICE_DATA->dm90_breathe_Rate/100);
        }
        //发完数据之后停止计时器
        m_DM90delayTimer->stop();
    }
}

void dm90_lightView::brightness_release_value(int){

}


//速度的进度条
void dm90_lightView::speed_change_value(int currentValue){

    m_currentSliderNum = 2;
    m_speedCurrentValue->setText(QString::number(currentValue,10)+"ms");
    m_DM90delayTimer->stop();
    m_DM90delayTimer->start(20);
}

void dm90_lightView::speed_release_value(int){

}
//模式切换
void dm90_lightView::change_result(int result_num){

    change_mode(true,result_num);
}

void dm90_lightView::change_mode(bool sendMessage,int result_num){

    m_RGBmask_view->hide();
    m_mode_number = result_num;
    m_breatheTimer->stop();

    int H=0, S=0, V=0;

    switch (result_num) {
    case result_fix:{//固定
        m_speedview->hide();
        //设置右边图片RGB的值
        H = GET_DEVICE_DATA->dm90_FIX_H;
        S = GET_DEVICE_DATA->dm90_FIX_S;
        V = GET_DEVICE_DATA->dm90_FIX_V;
        //转换为RGB
        HSVtoRGB(true,H/100,S,V);

    }
        break;
    case result_loop:{//循环------不能选颜色
        m_speedview->hide();
        m_RGBmask_view->show();
        //设置右边图片RGB的值
        H = GET_DEVICE_DATA->dm90_LOOP_H;
        S = GET_DEVICE_DATA->dm90_LOOP_S;
        V = GET_DEVICE_DATA->dm90_LOOP_V;
        //转换为RGB
        HSVtoRGB(true,H/100,S,V);
        m_breatheTimer->start(20);
    }
        break;
    case result_breathing:{//呼吸
        m_speedview->show();
        //设置右边图片RGB的值
        H = GET_DEVICE_DATA->dm90_breathe_H;
        S = GET_DEVICE_DATA->dm90_breathe_S;
        V = GET_DEVICE_DATA->dm90_breathe_V;
        //转换为RGB
        HSVtoRGB(true,H/100,S,V);
        m_breatheTimer->start(GET_DEVICE_DATA->dm90_breathe_Rate/100);
    }
        break;
    default:
        break;
    }
    //亮度
    m_brightness_slider->blockSignals(true);
    m_brightness_slider->setValue(V);
    m_briCurrentValue->setText(QString::number(V,10));
    m_brightness_slider->blockSignals(false);
    //更换图片颜色
    QString str = QString("background:rgb(%1,%2,%3)").arg(m_R).arg(m_G).arg(m_B);
    m_bgLab->setStyleSheet(str);
    //发送数据
    if(sendMessage)
    m_commin_fun.sendMessageForDevice(DM90PARAM_RGB_SHOW_START+1,1,result_num);
    GET_DEVICE_DATA->dm90_LED_Mode = result_num;
}

void dm90_lightView::breatheTimeout(){

    if(m_mode_number==2){//呼吸

        if(m_isbreathColorFront){
            m_breathTimeStar-=1;
            if(m_breathTimeStar<=10){
                m_isbreathColorFront = false;
            }
        }else{
            m_breathTimeStar+=1;
            if(m_breathTimeStar>GET_DEVICE_DATA->dm90_breathe_V){
                //m_breathTimeStar = 0;
                m_isbreathColorFront = true;
            }
        }
        int H = GET_DEVICE_DATA->dm90_breathe_H;
        int S = GET_DEVICE_DATA->dm90_breathe_S;
        int V = m_breathTimeStar;
        uint32_t rgb = HSVtoRGB(true,H/100,S,V);
        QString str = QString("background:rgb(%1,%2,%3)").arg((rgb>>8)&0xff).arg((rgb>>16)&0xff).arg(rgb&0xff);
        m_bgLab->setStyleSheet(str);
    }else if(m_mode_number==1){//循坏

        if(m_isloopColorFront){
            m_loopTimeStar-=1;
            if(m_loopTimeStar<=0){
                m_isloopColorFront = false;
            }
        }else{
            m_loopTimeStar+=1;
            if(m_loopTimeStar>=350){
                //m_breathTimeStar = 0;
                m_isloopColorFront = true;
            }
        }

        int H = m_loopTimeStar;
        int S = 100;
        int V = GET_DEVICE_DATA->dm90_LOOP_V;
        uint32_t rgb = HSVtoRGB(true,H,S,V);
        int rgb_r = (rgb>>8)&0xff;
        int rgb_g = (rgb>>8)&0xff;
        int rgb_b = rgb&0xff;
        QString str = QString("background:rgb(%1,%2,%3)").arg(rgb_r).arg(rgb_g).arg(rgb_b);
        //qDebug()<<"xxxxx"<<rgb_r<<rgb_g<<rgb_b<<m_loopTimeStar<<S<<V;
        m_bgLab->setStyleSheet(str);
    }
}
//灯光调节的开关/**********/
void dm90_lightView::lightadjSwitchClick(int swich_number){

    BGR2HSV(255,100,0);
    if(swich_number==0){//关闭

        m_mask_view->show();
        m_commin_fun.sendMessageForDevice(DM90PARAM_RGB_SHOW_START,1,0);
        int R = 0;
        int G = 0;
        int B = 0;
        QString str = QString("background:rgb(%1,%2,%3)").arg(R).arg(G).arg(B);
        m_DM90delayTimer->stop();
        m_breatheTimer->stop();
        m_bgLab->setStyleSheet(str);

    }else{//打开

        m_mask_view->hide();
        m_commin_fun.sendMessageForDevice(DM90PARAM_RGB_SHOW_START,1,1);

        int R = m_R;
        int G = m_G;
        int B = m_B;
        QString str = QString("background:rgb(%1,%2,%3)").arg(R).arg(G).arg(B);
        m_bgLab->setStyleSheet(str);
        //开启计时器
        //        if(GET_DEVICE_DATA->dm90_LED_Mode==1){//循环
        //           m_DM90delayTimer->start(20);
        //        }else if(GET_DEVICE_DATA->dm90_LED_Mode==2){//呼吸
        m_breatheTimer->start(20);
        //        }

    }
}
uint32_t dm90_lightView::HSVtoRGB(const bool&isgetrgb,uint16_t h, uint16_t s, uint16_t v)
{
    if(h >= 360) h = 360;

    if(s >= 100) s = 100;

    if(v >= 100) v = 100;

    int i;
    uint8_t R_Color = 0;
    uint8_t B_Color = 0;
    uint8_t G_Color = 0;
    uint32_t Temp_Color = 0;
    i = h / 60;
    int difs = h % 60;
    float RGB_max = v * 2.55f;
    float RGB_min = RGB_max * (100 - s) / 100.0f;
    float RGB_Adj = (RGB_max - RGB_min) * difs / 60.0f;
    switch(i)
    {
    case 0:
        R_Color = RGB_max;
        G_Color = RGB_min + RGB_Adj;
        B_Color = RGB_min;
        break;

    case 1:
        R_Color = RGB_max - RGB_Adj;
        G_Color = RGB_max;
        B_Color = RGB_min;
        break;

    case 2:
        R_Color = RGB_min;
        G_Color = RGB_max;
        B_Color = RGB_min + RGB_Adj;
        break;

    case 3:
        R_Color = RGB_min;
        G_Color = RGB_max - RGB_Adj;
        B_Color = RGB_max;
        break;

    case 4:
        R_Color = RGB_min + RGB_Adj;
        G_Color = RGB_min;
        B_Color = RGB_max;
        break;

    default:
        R_Color = RGB_max;
        G_Color = RGB_min;
        B_Color = RGB_max - RGB_Adj;
        break;
    }
    //qDebug()<<"99999xxxxx"<<R_Color<<G_Color<<B_Color;
    if(isgetrgb)
        m_R = R_Color;m_G = G_Color;m_B = B_Color;
    Temp_Color = ((uint32_t)R_Color << 8) | ((uint32_t)G_Color << 16) | ((uint32_t)B_Color << 0);
    return Temp_Color;
}

bool dm90_lightView::IsEquals(double val1, double val2)
{
    return fabs(val1 - val2) < 0.001;
}
// 将RGB格式转换为HSV格式
void dm90_lightView::BGR2HSV(uint16_t R, uint16_t G, uint16_t B)
{


            double r, g, b;
            double h, s, v;
            double min, max;
            int  MIN, MAX;           // 用于比较，整数做比较更好


            r = (double)R / 255;
            g = (double)G / 255;
            b = (double)B / 255;

                                     { MIN = R; min = r; }
            if (G < MIN) { MIN = G; min = g; }
            if (B < MIN) { MIN = B; min = b; }

                                     { MAX = R; max = r; }
            if (G > MAX) { MAX = G; max = g; }
            if (B > MAX) { MAX = B; max = b; }

            v = max;

            if (MAX == 0.0f)  s = 0.0f;
            else              s = 1.0f - min / max;

            if (MAX == MIN)
            {
                    h = 0.0f;
            }
            else if (MAX == R)
            {
                    if (G >= B)  h = 60.0f * (g - b) / (max - min) + 0.0f;
                    else         h = 60.0f * (g - b) / (max - min) + 360.0f;
            }
            else if (MAX == G)
            {
                    h = 60.0f * (b - r) / (max - min) + 120.0f;
            }
            else if (MAX == B)
            {
                    h = 60.0f * (r - g) / (max - min) + 240.0f;
            }
            else
            {
                    h = 0.0f;
            }

    m_H=h*100;m_S=s;m_V=v;

    //qDebug()<<"ppppppppp"<<m_H<<m_S<<m_V<<h<<s<<v;

//    double b, g, r;
//    int h=0;
//    double s, v;
//    double min, max;
//    double delta;

//    b = B / 255.0;
//    g = G / 255.0;
//    r = R / 255.0;

//    if (r > g)
//    {
//        r>b?max=r:max=b;
//        //max = MAX(r, b);
//        g<b?min=g:min=b;
//        //min = MIN(g, b);
//    }
//    else
//    {
//        g>b?max=g:max=b;
//        //max = MAX(g, b);
//        r<b?min=r:min=b;
//        //min = MIN(r, b);
//    }

//    v = max;
//    delta = max - min;

//    if (IsEquals(max, 0))
//        s = 0.0;
//    else
//        s = delta / max;

//    if (max == min)
//        h = 0.0;
//    else
//    {
//        if (IsEquals(r, max) && g >= b)
//        {
//            h = 60 * (g - b) / delta + 0;
//        }
//        else if (IsEquals(r, max) && g < b)
//        {
//            h = 60 * (g - b) / delta + 360;
//        }
//        else if (IsEquals(g, max))
//        {
//            h = 60 * (b - r) / delta + 120;
//        }
//        else if (IsEquals(b, max))
//        {
//            h = 60 * (r - g) / delta + 240;
//        }
//    }

//    double m_h;
//    double m_s;
//    double m_v;

//    m_h = (int)(h + 0.5);
//    m_h = (m_h > 359) ? (m_h - 360) : m_h;
//    m_h = (m_h < 0) ? (m_h + 360) : m_h;
//    m_s = s;
//    m_v = v;

//    m_H=m_h;m_S=m_s;m_V=m_v;
    //qDebug()<<"ppppppppp"<<m_h<<m_s<<m_v;
}
void dm90_lightView::listRefreshColor(){

    while (m_RGBlistWidge->count() > 0) {
        QListWidgetItem *item = m_RGBlistWidge->takeItem(0);
        delete item;
    }
    m_listHSV.clear();

    QVector<QString>colors = { QString("background:rgb(255,255,255);border-radius: %1px;").arg(15*m_Magnification),
                               QString("background:rgb(255,0,0);border-radius: %1px;").arg(15*m_Magnification),
                               QString("background:rgb(255,123,0);border-radius: %1px;").arg(15*m_Magnification),
                               QString("background:rgb(255,255,0);border-radius: %1px;").arg(15*m_Magnification),
                               QString("background:rgb(0,255,0);border-radius: %1px;").arg(15*m_Magnification),
                               QString("background:rgb(0,255,255);border-radius: %1px;").arg(15*m_Magnification),
                               QString("background:rgb(0,0,255);border-radius: %1px;").arg(15*m_Magnification),
                               QString("background:rgb(255,0,255);border-radius: %1px;").arg(15*m_Magnification),
                             };
    for (int i = 0; i < 8; i++) {
        QListWidgetItem* item = new QListWidgetItem;
        item->setSizeHint(QSize(30*m_Magnification, 30*m_Magnification));
        item->setWhatsThis(QString::number(i,10));
        m_RGBlistWidge->addItem(item);

        if(i ==8){
            QLabel *rgbitemView = new QLabel();
            rgbitemView->resize(30*m_Magnification,30*m_Magnification);
            //rgbitemView->setStyleSheet("background:rgb(0,255,0);border-radius: 30px;");
            rgbitemView->setPixmap(QPixmap(":/image/images/DM90/dm90_add_btn.png"));
            rgbitemView->setScaledContents(true);
            item->setWhatsThis("lastAddItem");
            m_RGBlistWidge->setItemWidget(item,rgbitemView);       // 增加item
        }else{
            QLabel *rgbitemView = new QLabel();
            rgbitemView->setFixedSize(QSize(30*m_Magnification,30*m_Magnification));
            rgbitemView->setAttribute(Qt::WA_StyledBackground,true);
            rgbitemView->setStyleSheet(colors.at(i));
            item->setWhatsThis(colors.at(i));
            QLabel *selectLal = new QLabel(rgbitemView);
            selectLal->resize(11*m_Magnification,8*m_Magnification);
            selectLal->move(20*m_Magnification,0*m_Magnification);
            selectLal->setPixmap(QPixmap(":/image/images/DM90/item_select.png"));
            selectLal->setStyleSheet("background:transparent");
            selectLal->setScaledContents(true);
            selectLal->hide();
            m_RGBlistWidge->setItemWidget(item,rgbitemView);       // 增加item

            //转换一下
            int a = colors.at(i).indexOf("b(");
            int b = colors.at(i).indexOf(");");

            QStringList rgbList = colors.at(i).mid(a+2,b-a-2).split(',');

            //RGB转HSV
            BGR2HSV(rgbList.at(0).toInt(),rgbList.at(1).toInt(),rgbList.at(2).toInt());
            QVector<int>hsvColor = {m_H,(int)m_S*100,(int)m_V*100};
            m_listHSV.push_back(hsvColor);
        }
    }

}
void dm90_lightView::cancelListItemSelectStatus(){

    for (int i=0;i<m_RGBlistWidge->count();i++) {
        QListWidgetItem *item = m_RGBlistWidge->item(i);
        QList<QLabel*> labelList = m_RGBlistWidge->itemWidget(item)->findChildren<QLabel*>();
        if(labelList.count()>0)labelList.at(0)->hide();
    }
}
void dm90_lightView::exitSamecolorAndappendItem(){
    //对比列表是否有相同的颜色，如果没有则增加“+”按钮
    bool exitAddBtn = false;
    bool exitRGBcolor = false;
    for (int i=0;i<m_RGBlistWidge->count();i++) {
        QListWidgetItem *item = m_RGBlistWidge->item(i);
        if(item->whatsThis()=="lastAddItem"){
            exitAddBtn = true;
        }else{
            QString str = item->whatsThis();
            if(str.length()>10){
                int a = str.indexOf("b(");
                int b = str.indexOf(");");
                QStringList rgbList = str.mid(a+2,b-a-2).split(',');
                int R = rgbList.at(0).toInt();
                int G = rgbList.at(1).toInt();
                int B = rgbList.at(2).toInt();
                if(m_R==R&&m_G==G&&m_B==B)exitRGBcolor=true;
            }
        }
    }

    if(exitAddBtn){//存在，删除“+”
        if(exitRGBcolor){
            //隐藏最后的“+”按钮
            QListWidgetItem*item = m_RGBlistWidge->takeItem(m_RGBlistWidge->count()-1);
            delete item;
        }
    }else{
        if(!exitRGBcolor){//不存在，添加“+”
            QListWidgetItem* item = new QListWidgetItem;
            item->setSizeHint(QSize(30*m_Magnification, 30*m_Magnification));
            item->setWhatsThis("lastAddItem");
            m_RGBlistWidge->addItem(item);
            QLabel *rgbitemView = new QLabel();
            rgbitemView->resize(30*m_Magnification,30*m_Magnification);
            rgbitemView->setPixmap(QPixmap(":/image/images/DM90/dm90_add_btn.png"));
            rgbitemView->setScaledContents(true);
            m_RGBlistWidge->setItemWidget(item,rgbitemView);       // 增加item
        }
    }
}

void dm90_lightView::DM70exitSamecolorAndappendItem(){

    //对比列表是否有相同的颜色，如果没有则增加“+”按钮
    bool exitAddBtn = false;
    bool exitRGBcolor = false;
    for (int i=0;i<m_RGBlistWidge->count();i++) {
        QListWidgetItem *item = m_RGBlistWidge->item(i);
        if(item->whatsThis()=="lastAddItem"){
            exitAddBtn = true;
        }else{
            QString str = item->whatsThis();
            if(str.length()>10){
                int a = str.indexOf("b(");
                int b = str.indexOf(");");
                QStringList rgbList = str.mid(a+2,b-a-2).split(',');
                int R = rgbList.at(0).toInt();
                int G = rgbList.at(1).toInt();
                int B = rgbList.at(2).toInt();
                if(m_R==R&&m_G==G&&m_B==B)exitRGBcolor=true;
            }
        }
    }

    if(exitAddBtn){//存在，删除“+”
        if(exitRGBcolor){
            //隐藏最后的“+”按钮
            QListWidgetItem*item = m_RGBlistWidge->takeItem(m_RGBlistWidge->count()-1);
            delete item;
        }
    }else{
        if(!exitRGBcolor && m_RGBlistWidge->count()<=9){//不存在，添加“+”
            QListWidgetItem* item = new QListWidgetItem;
            item->setSizeHint(QSize(30*m_Magnification, 30*m_Magnification));
            item->setWhatsThis("lastAddItem");
            m_RGBlistWidge->addItem(item);
            QLabel *rgbitemView = new QLabel();
            rgbitemView->resize(30*m_Magnification,30*m_Magnification);
            rgbitemView->setPixmap(QPixmap(":/image/images/DM90/dm90_add_btn.png"));
            rgbitemView->setScaledContents(true);
            m_RGBlistWidge->setItemWidget(item,rgbitemView);       // 增加item
        }
    }

}
void dm90_lightView::appendItem(QString colorStr){

    QListWidgetItem* item = new QListWidgetItem;
    item->setSizeHint(QSize(30*m_Magnification, 30*m_Magnification));
    item->setWhatsThis(QString::number(9,10));
    m_RGBlistWidge->addItem(item);
    QLabel *rgbitemView = new QLabel();
    rgbitemView->setFixedSize(QSize(30*m_Magnification,30*m_Magnification));
    rgbitemView->setAttribute(Qt::WA_StyledBackground,true);
    rgbitemView->setStyleSheet(colorStr);
    item->setWhatsThis(colorStr);
    QLabel *selectLal = new QLabel(rgbitemView);
    selectLal->resize(11*m_Magnification,8*m_Magnification);
    selectLal->move(20*m_Magnification,0*m_Magnification);
    selectLal->setPixmap(QPixmap(":/image/images/DM90/item_select.png"));
    selectLal->setStyleSheet("background:transparent");
    selectLal->setScaledContents(true);
    selectLal->hide();
    m_RGBlistWidge->setItemWidget(item,rgbitemView);       // 增加item
}
void dm90_lightView::removeItemStatus(int itemIndex){

    if(itemIndex == m_currentSelectItemIndex){

        qDebug()<<"pppppp"<<m_currentSelectItemIndex;
        m_RGBlistWidge->blockSignals(true);
        m_RGBlistWidge->setCurrentRow(0);
        m_RGBlistWidge->blockSignals(false);
        QString str = QString("background:rgb(%1,%2,%3)").arg(255).arg(255).arg(255);
        m_bgLab->setStyleSheet(str);
        //选中白色
    }



}
