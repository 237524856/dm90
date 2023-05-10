#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include <QComboBox>
#include "PM481/pm481_class/custommainbutton.h"
#include <QStandardPaths>
#include "customscrollview.h"
#include <QFontDatabase>
#include <QDesktopServices>
#include "networking_class.h"
#include <QStandardPaths>
#include <QProcess>


#if defined(Q_OS_WIN32)

#pragma comment(lib,"User32.lib")
#pragma comment(lib,"Setupapi.lib")

#include <QGraphicsSvgItem>
#include <QtWidgets>
#include <QGraphicsScene>

#include <QSettings>
#define RUN_KEY "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\Maono Link.exe"

#else

#endif
MainWindow::MainWindow(QWidget *parent)
    : mainBasewindow(parent)
    //    , ui(new Ui::MainWindow)
{
    QIcon appIcon(":/image/images/logo.ico");
    QApplication::setWindowIcon(appIcon);
    QFontDatabase::addApplicationFont(":/image/MBS_400/SourceHanSansCN Medium.ttf");
    setFixedSize(1023*m_Magnification,737*m_Magnification);
    //this->setStyleSheet("background-color:white");
    int current_language = m_commClass.get_save_language_status();

#if defined(Q_OS_WIN32)
    //安装之后的语言
    QSettings *pReg = new QSettings(RUN_KEY, QSettings::NativeFormat);
    QVariant v = pReg->value("language"); //读取注册表值
    //当前的语言
    QString current_lang = v.toString();

    if(current_language == -1){

        if(current_lang ==QString::fromLocal8Bit("Chinese"))
        {
            current_language =0;
        }else{
            current_language =1;
        }
    }
#else

    if(current_language == -1){

        current_language = 1;
    }
#endif

    //current_language = 1;
    qDebug()<<"同步信息xxttteeeeeeeeeeee___mac"<<current_language;
    if(current_language==1){//英文版
        if (m_translator.load(":/pd400_lang/pd400_zh.qm")) {
            qApp->installTranslator(&m_translator);
        }
        language_index = current_language;
    }else if(current_language==0){//中文
        if (m_translator.load(":/pd400_lang/pd400_en.qm")) {
            qApp->installTranslator(&m_translator);
        }
        language_index = current_language;
    }

    //判断选中的数据是否存在，如果补存在就保存
    m_commClass.getDM90ListRowSelectStatus();

    //开始添加可以滑动的视图=====================
    m_mainSuperView = new QStackedWidget(this);
    m_mainSuperView->move(0,64*m_Magnification);
    m_mainSuperView->resize(this->width(),643*m_Magnification);

    m_mainView1 = new mainSubView(this);
    m_mainView1->resize(m_mainSuperView->width(),m_mainSuperView->height());
    //m_mainView1->setStyleSheet("background-color:rgb(100,0,0)");
    m_mainSuperView->addWidget(m_mainView1);
    m_mainView1->setObjectName("m_mainView1");

    m_mainView2 = new mainSubView(this);
    m_mainView2->resize(m_mainSuperView->width(),m_mainSuperView->height());
    //m_mainView2->setStyleSheet("background-color:rgb(100,0,0)");
    m_mainSuperView->addWidget(m_mainView2);
    m_mainView2->setObjectName("m_mainView2");


    m_main_bgimage = new QLabel(this);
    m_main_bgimage->move(0,64*m_Magnification);
    m_main_bgimage->resize(this->width(),643*m_Magnification);
    QString main_bgimage_stys=QString(":/image/images/Mainwindows/Main_backimage_%1.png").arg(m_Magnification);
    m_main_bgimage->setPixmap(QPixmap(":/image/images/Mainwindows/Main_backimage_3.png"));
    m_main_bgimage->setScaledContents(true);

    m_pm481_view = NULL;
    m_pd400_view = NULL;
    m_dm90_view = NULL;
    //    ui->setupUi(this);
    QVector<QString>title_strs = {"PD400X","DM30","DM90","PD200X","DM30RGB","DM70"};
    for (int i =0;i< title_strs.count();i++) {
        customMainButton *btn = new customMainButton(m_mainView1);
        QLabel *device_title = new QLabel(m_mainView1);
        if(i==0){
            btn->resize(213*m_Magnification,300*m_Magnification);
            QString bt_stys=QString("QPushButton{border-image:url(:/image/images/Mainwindows/PD400_big_3.png)}");
            btn->setStyleSheet(bt_stys);
        }else if(i==1){
            btn->resize(180*m_Magnification,335*m_Magnification);
            QString bt_stys=QString("QPushButton{border-image:url(:/image/images/Mainwindows/PM481_3.png)}");
            btn->setStyleSheet(bt_stys);
        }else if(i==2){
            btn->resize(156*m_Magnification,387*m_Magnification);
            QString bt_stys=QString("QPushButton{border-image:url(:/image/images/Mainwindows/DM90RGB_big.png)}");
            btn->setStyleSheet(bt_stys);
        }else if(i==3){
            btn->resize(256*m_Magnification,377*m_Magnification);
            QString bt_stys=QString("QPushButton{border-image:url(:/image/images/Mainwindows/PD200_big.png)}");
            btn->setStyleSheet(bt_stys);
        }else if(i==4){
            btn->resize(133*m_Magnification,301*m_Magnification);
            QString bt_stys=QString("QPushButton{border-image:url(:/image/images/Mainwindows/DM30RGB_big.png)}");
            btn->setStyleSheet(bt_stys);
        }else if(i==5){
            btn->resize(95*m_Magnification,266*m_Magnification);
            QString bt_stys=QString("QPushButton{border-image:url(:/image/images/Mainwindows/DM70_big.png)}");
            btn->setStyleSheet(bt_stys);
        }
        btn->move((this->width()-450*m_Magnification-335*m_Magnification)/2 + 450*m_Magnification*i,(200+64)*m_Magnification);

        if(i==4){
          device_title->resize(btn->width()+20*m_Magnification,50*m_Magnification);
        }else{
          device_title->resize(btn->width(),50*m_Magnification);
        }

        device_title->move(btn->x(),btn->y()-60*m_Magnification);
        device_title->setText(title_strs.at(i));
        QString device_title_stys=QString("background-color:transparent;color:rgb(158,158,158);text-align:center;font-size:%1px;font-family:'Source Han Sans CN Medium'").arg(32*m_Magnification);
        device_title->setStyleSheet(device_title_stys);
        device_title->setAlignment(Qt::AlignCenter);
        device_title->hide();
        m_deviceTitleArrs.push_back(device_title);

        //btn->setText(title_strs[i]);
        btn->setObjectName(title_strs.at(i));
        btn->hide();
        connect(btn,SIGNAL(custom_click()),this,SLOT(change_view()));
        connect(btn,SIGNAL(custom_enter(QString)),this,SLOT(device_enter_view(QString)));
        connect(btn,SIGNAL(custom_leave(QString)),this,SLOT(device_leave_view(QString)));
        m_pro_btns.push_back(btn);
    }

    //增加向左的按钮
    m_left_btn = new QPushButton(this);
    m_left_btn->resize(18*m_Magnification,30*m_Magnification);
    m_left_btn->move(20*m_Magnification,(m_mainSuperView->height()+m_left_btn->height())/2);
    m_left_btn->setStyleSheet(
                "QPushButton{border-image:url(:/image/images/Mainwindows/main_leftBtn_default.png);}"
                "QPushButton:hover{border-image:url(:/image/images/Mainwindows/main_leftBtn_press.png);}"
                "QPushButton:pressed{border-image:url(:/image/images/Mainwindows/main_leftBtn_press.png);}"
                "QPushButton:focus{outline: none;}"
                );
    connect(m_left_btn, SIGNAL(clicked()), this, SLOT(leftBtn_clicked()));
    m_left_btn->hide();

    //增加向右的按钮
    m_right_btn= new QPushButton(this);
    m_right_btn->resize(18*m_Magnification,30*m_Magnification);
    m_right_btn->move(this->width()-20*m_Magnification-m_right_btn->width(),(m_mainSuperView->height()+m_right_btn->height())/2);
    m_right_btn->setStyleSheet(
                "QPushButton{border-image:url(:/image/images/Mainwindows/main_rithtBtn_default.png);}"
                "QPushButton:hover{border-image:url(:/image/images/Mainwindows/main_rithtBtn_press.png);}"
                "QPushButton:pressed{border-image:url(:/image/images/Mainwindows/main_rithtBtn_press.png);}"
                "QPushButton:focus{outline: none;}"
                );
    connect(m_right_btn, SIGNAL(clicked()), this, SLOT(rightBtn_clicked()));
    m_right_btn->hide();

    mainCreateUI();
    //创建HID通讯的类
    pm481_devoperator = new DevOperator();

    m_childThread = new QThread();
    //多线程
    pm481_devoperator->moveToThread(m_childThread);
    m_childThread->start();
    //获取设备的个数
    pm481_devoperator->getDeviceArrs();
    bool is_connect = false;
    if(is_connect){//USB连接成功
        qDebug()<<"USB连接成功";
    }else{
        //qDebug()<<"USB连接失败";
    }
    m_MainTimer = new QTimer(this);
    connect(m_MainTimer, SIGNAL(timeout()), this, SLOT(handleTimeout()));
    m_MainTimer->start(230);

    m_animationTimer = new QTimer(this);
    connect(m_animationTimer, SIGNAL(timeout()), this, SLOT(animationTimeout()));

//    GET_PM481_DEVICE_DATA = new pc_PM481_device_dataSource();
//    memset(GET_PM481_DEVICE_DATA,0,sizeof(pc_PM481_device_dataSource));

}
void MainWindow::mainCreateUI(){

    //设置按钮
    m_setup_btn = new QPushButton(this);
#if defined(Q_OS_WIN32)
    m_setup_btn->move(this->width()-20*7*m_Magnification,22*m_Magnification);
#else
    m_setup_btn->move(this->width()-40*m_Magnification,22*m_Magnification);
#endif
    m_setup_btn->resize(20*m_Magnification,20*m_Magnification);
    m_setup_btn->setStyleSheet(
                "QPushButton{border-image:url(:/image/images/Mainwindows/Main_setup.png) 0 40 0 0;}"
                "QPushButton:hover{border-image:url(:/image/images/Mainwindows/Main_setup.png) 0 20 0 20;}"
                "QPushButton:pressed{border-image:url(:/image/images/Mainwindows/Main_setup.png) 0 0 0 40;}"
                "QPushButton:focus{outline: none;}"
                );
    connect(m_setup_btn, SIGNAL(clicked()), this, SLOT(setup_clicked()));

    //创建滚动视图
    customScrollView *scrollView = new customScrollView();
    scrollView->move(100,320);
    scrollView->resize(500,300);
    scrollView->setStyleSheet("background-color:red");
    scrollView->initChangeImageButton();

    QVector<QString> share_btn_names = { m_co_fun.add_image_forstatus(QString("url(:/image/images/pd400_sharedouyin_btn_%1.png)").arg(m_Magnification),17*m_Magnification),
                                         m_co_fun.add_image_forstatus("url(:/image/images/pd400_shareweibo_btn.png)",23),
                                         m_co_fun.add_image_forstatus("url(:/image/images/pd400_sharbi_btn.png)",42)};
    for (int i = 0;i< share_btn_names.count();i++) {
        QPushButton *share_buttom = new QPushButton(this);
        if(i==0){//抖音
            share_buttom->resize(17*m_Magnification,19*m_Magnification);
            share_buttom->move(20*m_Magnification + 48*i*m_Magnification,this->height()-30*m_Magnification);
            share_buttom->setObjectName("douyin");
        }else if(i==1){//微博
            share_buttom->resize(23*m_Magnification,19*m_Magnification);
            share_buttom->move(20*m_Magnification + 26*m_Magnification+17*m_Magnification,this->height()-30*m_Magnification);
            share_buttom->setObjectName("weibo");
        }else{//b站
            share_buttom->resize(42*m_Magnification,19*m_Magnification);
            share_buttom->move(20*m_Magnification + 26*2*m_Magnification+17*m_Magnification+23*m_Magnification,this->height()-30*m_Magnification);
            share_buttom->setObjectName("bizhan");
        }
        //        share_buttom->resize(22,22);
        //        share_buttom->move(20 + 48*i,this->height()-30);
        const QString str = share_btn_names.at(i);
        share_buttom->setStyleSheet(str);
        connect(share_buttom,SIGNAL(clicked()),this,SLOT(share_btn_click()));
        m_share_btns.push_back(share_buttom);
    }
    //没连接HID的是视图
    m_NoconnectView = new QLabel(this);
    m_NoconnectView->move(0,64*m_Magnification);
    m_NoconnectView->resize(this->width(),643*m_Magnification);
    m_NoconnectView->setText(tr("请连接MAONO设备"));
    QString noconnectView_stys=QString("background-color:transparent;color:rgb(158,158,158);text-align:center;font-size:%1px;font-family:'Source Han Sans CN Medium'").arg(32*m_Magnification);
    m_NoconnectView->setStyleSheet(noconnectView_stys);
    m_NoconnectView->setAlignment(Qt::AlignCenter);
    //m_NoconnectView->hide();

    //设置视图-------------------------------------------------------------------
    m_setup_view = new QWidget(this);
    m_setup_view->move(0,64*m_Magnification);
    m_setup_view->resize(this->width(),643*m_Magnification);
    m_setup_view->setAttribute(Qt::WA_StyledBackground);
    m_setup_view->setStyleSheet("background-color:rgb(0,0,0)");
    m_setup_view->hide();

    QLabel *setup_logo = new QLabel(m_setup_view);
    setup_logo->resize(241*m_Magnification,70*m_Magnification);
    setup_logo->move((this->width()-241*m_Magnification)/2,100*m_Magnification);
    QString setup_logo_stys=QString(":/image/images/Mainwindows/Main_setup_logo_%1.png").arg(m_Magnification);
    setup_logo->setPixmap(QPixmap(setup_logo_stys));

    //文字描述
    for (int i =0;i<5;i++) {
        QLabel *content_title = new QLabel(m_setup_view);
        content_title->move(0,200*m_Magnification+i*35*m_Magnification);
        content_title->resize(this->width(),30*m_Magnification);

        if(i==0){
            content_title->setText(tr("版权所有 @ 2022-2030 MAONO. 保留所有权利"));
        }else if(i==1){
            content_title->setText(tr("最终用户许可协议"));
        }else if(i==2){
            //m_version_str = "1.0.0";
            content_title->setText(tr("软件版本：") + m_appversion_str);
        }else if(i==3){
            m_updateLabel = new QLabel(content_title);
            m_updateLabel->resize(160*m_Magnification,content_title->height());
            m_updateLabel->move((content_title->width()-100*m_Magnification)/2,0);
            QString updateLabel_stys = QString("background-color:transparent;color:rgb(158,158,158);text-align:center;font-size:%1px;font-family:'Source Han Sans CN Medium'").arg((int)(15*m_Magnification));
            m_updateLabel->setStyleSheet(updateLabel_stys);
            m_updateLabel->setText(tr("软件更新："));
            m_updateLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

            content_title->setAlignment(Qt::AlignRight);

            m_update_btn = new QPushButton(content_title);
            m_update_btn->setText(tr("检查更新"));
            QString update_btn_stys = QString("QPushButton {background-color:transparent;color:rgb(158,158,158);text-align:left;font-size:%1px;font-family:'Source Han Sans CN Medium'}"
                                              "QPushButton:hover{background-color:transparent;color:rgb(54,54,54);text-align:left;font-size:%2px;font-family:'Source Han Sans CN Medium'}"
                                              "QPushButton:focus{outline: none;}").arg((int)(15*m_Magnification)).arg((int)(15*m_Magnification));
            m_update_btn->setStyleSheet(update_btn_stys);
            connect(m_update_btn,SIGNAL(clicked()),this,SLOT(updateVersion_click()));


        }else if(i==4){
            m_languageLabel = new QLabel(content_title);
            m_languageLabel->resize(80*m_Magnification,content_title->height());
            m_languageLabel->move((content_title->width()-160*m_Magnification)/2,0);
            QString languageLabel_stys = QString("background-color:transparent;color:rgb(158,158,158);text-align:center;font-size:%1px;font-family:'Source Han Sans CN Medium'").arg((int)(15*m_Magnification));
            m_languageLabel->setStyleSheet(languageLabel_stys);
            m_languageLabel->setText(tr("语言切换："));
            m_languageLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

            m_cbo_sex = new QComboBox(content_title);
            m_cbo_sex->move(m_languageLabel->x()+m_languageLabel->width()+10*m_Magnification,0);
            m_cbo_sex->resize(100*m_Magnification,content_title->height());
            m_cbo_sex->addItem(tr("简体中文"));
            m_cbo_sex->addItem(tr("English"));
            connect(m_cbo_sex, SIGNAL(currentIndexChanged(int)), this, SLOT(change_language(int)));
        }
        QString content_title_stys = QString("background-color:transparent;color:rgb(158,158,158);text-align:center;font-size:%1px;font-family:'Source Han Sans CN Medium'").arg((int)(15*m_Magnification));
        content_title->setStyleSheet(content_title_stys);
        content_title->setAlignment(Qt::AlignCenter);
        m_setupLabels.push_back(content_title);
    }

    //判断语言当前的状态处理不一样的文字显示
    if(language_index >0 ){//英文
        m_updateLabel->move((this->width()-320*m_Magnification)/2,0);
        m_update_btn->move(m_updateLabel->x()+m_updateLabel->width()+10*m_Magnification,-6);
        m_update_btn->resize(140*m_Magnification,35*m_Magnification);

        m_cbo_sex->move(m_languageLabel->x()+m_languageLabel->width()+10*m_Magnification,0);
        m_cbo_sex->blockSignals(true);
        m_cbo_sex->setCurrentIndex(1);
        m_cbo_sex->blockSignals(false);
    }else{
        m_updateLabel->move((this->width()-320*m_Magnification)/2,0);
        m_update_btn->move(m_updateLabel->x()+m_updateLabel->width(),-6);
        m_update_btn->resize(80*m_Magnification,35*m_Magnification);

        m_cbo_sex->move(m_languageLabel->x()+m_languageLabel->width(),0);
        m_cbo_sex->blockSignals(true);
        m_cbo_sex->setCurrentIndex(0);
        m_cbo_sex->blockSignals(false);
    }

    m_setup_back_btn = new QPushButton(this);
    m_setup_back_btn->move(30*m_Magnification,22*m_Magnification);
    m_setup_back_btn->resize(30*m_Magnification,30*m_Magnification);
    m_setup_back_btn->setStyleSheet(
                "QPushButton{border-image:url(:/image/images/Mainwindows/Main_setup_back.png) 0 40 0 0;}"
                "QPushButton:hover{border-image:url(:/image/images/Mainwindows/Main_setup_back.png) 0 20 0 20;}"
                "QPushButton:pressed{border-image:url(:/image/images/Mainwindows/Main_setup_back.png) 0 0 0 40;}"
                );
    m_setup_back_btn->hide();
    connect(m_setup_back_btn, SIGNAL(clicked()), this, SLOT(setup_back()));

    changelanguage_sharebtn();

    //升级上位机的视图
    m_newwork_update = new common_newwork_class(this);
    m_newwork_update->move(0,64*m_Magnification);
    m_newwork_update->resize(this->width(),this->height()-64*m_Magnification);
    m_newwork_update->createUI();
    m_newwork_update->hide();
    //m_newwork_update->network_updateversion();
}
void MainWindow::handleTimeout(){
    //第一时间全部隐藏按钮
    m_deviceArrs = pm481_devoperator->timeGetDeviceArrs();
    //每次获取时清空
    m_currentPro_btns.clear();
    m_currentDeviceTitleArrs.clear();

    if(m_deviceArrs.count()<=0){
        for (int i=0;i<m_pro_btns.count();i++) {
            m_pro_btns.at(i)->hide();
            m_deviceTitleArrs.at(i)->hide();
        }
    }else {

        for (int i=0;i<m_pro_btns.count();i++) {
            m_pro_btns.at(i)->hide();
            m_deviceTitleArrs.at(i)->hide();
        }

        for (int i=0;i<m_deviceArrs.count();i++) {

            if(m_deviceArrs[i]==PD400PID){
                m_currentPro_btns.push_back(m_pro_btns.at(0));
                m_currentDeviceTitleArrs.push_back(m_deviceTitleArrs.at(0));
            }else if (m_deviceArrs[i]==PM481PID){
                m_currentPro_btns.push_back(m_pro_btns.at(1));
                m_currentDeviceTitleArrs.push_back(m_deviceTitleArrs.at(1));
            }else if (m_deviceArrs[i]==DM90PID){
                m_currentPro_btns.push_back(m_pro_btns.at(2));
                m_currentDeviceTitleArrs.push_back(m_deviceTitleArrs.at(2));
            }else if (m_deviceArrs[i]==PD200PID){
                m_currentPro_btns.push_back(m_pro_btns.at(3));
                m_currentDeviceTitleArrs.push_back(m_deviceTitleArrs.at(3));
            }else if (m_deviceArrs[i]==DM30RGB){
                m_currentPro_btns.push_back(m_pro_btns.at(4));
                m_currentDeviceTitleArrs.push_back(m_deviceTitleArrs.at(4));
            }else if (m_deviceArrs[i]==DM70PID){
                m_currentPro_btns.push_back(m_pro_btns.at(5));
                m_currentDeviceTitleArrs.push_back(m_deviceTitleArrs.at(5));
            }
        }
    }

    //先判断是有几个设备
    if(m_deviceArrs.count()>=1 && m_deviceArrs.count()<=3){//少于或者等于三个设备的排列方式

        m_mainSuperView->setCurrentIndex(1);
        //先判断是那些设备
        int btns_width = 0;
        int space_btn = 0;

        for (int i=0;i<m_currentPro_btns.count();i++) {

            btns_width += m_currentPro_btns.at(i)->width();
        }
        space_btn = (this->width()-btns_width)/(m_currentPro_btns.count()+1);

        for (int i=0;i<m_deviceArrs.count();i++) {
            //显示设备按钮的方法
            m_currentPro_btns.at(i)->show();
            m_currentDeviceTitleArrs.at(i)->show();
            m_currentPro_btns.at(i)->setParent(m_mainView1);
            m_currentDeviceTitleArrs.at(i)->setParent(m_mainView1);
            if(i==0){
                m_currentPro_btns.at(i)->move(space_btn,(200-64)*m_Magnification);

            }else{
                m_currentPro_btns.at(i)->move(space_btn+m_currentPro_btns.at(i-1)->width()+m_currentPro_btns.at(i-1)->x(),(200-64)*m_Magnification);

            }
            //显示标题的位置，PD200X因为图片原因，显示

            m_currentDeviceTitleArrs.at(i)->move(m_currentPro_btns.at(i)->x(),m_currentPro_btns.at(i)->y()-60*m_Magnification);
             qDebug()<<"xxttteeeeeeeeeeeeeee___mac"<<m_currentDeviceTitleArrs.at(i)->x()<<m_currentDeviceTitleArrs.at(i)->width();

        }

        //m_pro_btns.at(4)->setParent(m_mainView1);
        //m_deviceTitleArrs.at(4)->setParent(m_mainView1);


        m_left_btn->hide();
        //m_right_btn->hide();
        m_mainSuperView->setCurrentIndex(0);

    }else if(m_deviceArrs.count()>3){//多于三个设备的排列方式，需要分页去完成

        if(m_mainSuperView->currentIndex() !=1)
        m_right_btn->show();
        //先判断是那些设备
        int btns_width = 0;
        int space_btn = 0;

        for (int i=0;i<m_currentPro_btns.count()-1;i++) {

            btns_width += m_currentPro_btns.at(i)->width();
        }
        space_btn = (this->width()-btns_width)/(m_currentPro_btns.count()-1+1);

        for (int i=0;i<3;i++) {
            //显示设备按钮的方法
            m_currentPro_btns.at(i)->show();
            m_currentDeviceTitleArrs.at(i)->show();
            if(i==0){
                m_currentPro_btns.at(i)->move(space_btn,(200-64)*m_Magnification);
            }else{
                m_currentPro_btns.at(i)->move(space_btn+m_currentPro_btns.at(i-1)->width()+m_currentPro_btns.at(i-1)->x(),(200-64)*m_Magnification);
            }
            //显示标题的位置，PD200X因为图片原因，显示
            m_currentDeviceTitleArrs.at(i)->move(m_currentPro_btns.at(i)->x(),m_currentPro_btns.at(i)->y()-60*m_Magnification);
        }

        //第二页
        m_currentPro_btns.at(3)->show();
        m_currentPro_btns.at(3)->move((this->width()-m_currentPro_btns.at(3)->width())/2,(200-64)*m_Magnification);
        m_currentDeviceTitleArrs.at(3)->show();
        m_currentDeviceTitleArrs.at(3)->move(m_currentPro_btns.at(3)->x(),m_currentPro_btns.at(3)->y()-60*m_Magnification);
        m_currentPro_btns.at(3)->setParent(m_mainView2);
        m_currentDeviceTitleArrs.at(3)->setParent(m_mainView2);

       // m_currentDeviceTitleArrs.at(4)->show();
    }
    //return ;

    //创建要显示对应的视图============================
    if(m_deviceArrs.count()>0){
        m_mainSuperView->show();
        m_main_bgimage ->hide();
        for (int i=0;i<m_deviceArrs.count();i++) {

            if(m_deviceArrs[i]==PD400PID){

                if(!m_pd400_view){
                    m_pd400_view = new window_pd400();
                    connect(m_pd400_view, SIGNAL(pd400BackClick()), this, SLOT(pd400_back()));
                }
            }else if(m_deviceArrs[i]==PM481PID){

                if(!m_pm481_view){
                     m_pm481_view = new window_pm481();
                     //m_pm481_view->pm481_createUI(1);
                    connect(m_pm481_view, SIGNAL(pm481BackClick()), this, SLOT(pm481_back()));
                }
            }else if(m_deviceArrs[i]==DM90PID){

                if(!m_dm90_view){
                    m_dm90_view = new window_dm90();
                }
                connect(m_dm90_view, SIGNAL(Dm90BackClick()), this, SLOT(dm90_back()));
            }else if(m_deviceArrs[i]==PD200PID){
                if(!m_pm481_view){
                    m_pm481_view = new window_pm481();
                   // m_pm481_view->pm481_createUI(2);
                    connect(m_pm481_view, SIGNAL(pm481BackClick()), this, SLOT(pm481_back()));
                }
            }else if(m_deviceArrs[i]==DM30RGB){
                if(!m_pm481_view){
                    m_pm481_view = new window_pm481();
                    //m_pm481_view->pm481_createUI(3);
                    connect(m_pm481_view, SIGNAL(pm481BackClick()), this, SLOT(pm481_back()));
                }
            }
        }
        m_NoconnectView->hide();
    }else {//没找到设备
        m_mainSuperView->hide();
        m_main_bgimage ->show();
        m_NoconnectView->show();
    }
    m_MainTimer->stop();
    //qDebug()<<"USB连接成功--------"<<deviceArrs;
}

//显示设备的鼠标将要进入
void MainWindow::device_enter_view(QString btn_object){


    //m_MainTimer->stop();
    //m_MainTimer->blockSignals(true);
    QString deviceTitle_stys=QString("background-color:transparent;color:rgb(158,158,158);text-align:center;font-size:%1px;font-family:'Source Han Sans CN Medium'").arg(40*m_Magnification);
    customMainButton *btn = dynamic_cast<customMainButton *>(sender());

    int btn_index = 0;
    if(btn->objectName()=="PD400X"){
        btn->resize(263*m_Magnification,350*m_Magnification);
        btn_index = 0;

    }else if(btn->objectName()=="DM30"){

        btn->resize(230*m_Magnification,385*m_Magnification);
        btn_index = 1;
    }else if(btn->objectName()=="DM90"){
        btn->resize(166*m_Magnification,397*m_Magnification);
        btn_index = 2;
    }else if(btn->objectName()=="PD200X"){
        btn->resize(306*m_Magnification,427*m_Magnification);
        btn_index = 3;
    }else if(btn->objectName()=="DM30RGB"){
        btn->resize(153*m_Magnification,321*m_Magnification);
        btn_index = 4;
    }else if(btn->objectName()=="DM70"){
        btn->resize(105*m_Magnification,276*m_Magnification);
        btn_index = 5;
    }

    m_deviceTitleArrs.at(btn_index)->move(btn->x(),btn->y()-60*m_Magnification);
    if(btn->objectName()=="DM30RGB"){
      m_deviceTitleArrs.at(btn_index)->resize(btn->width()+40*m_Magnification,50*m_Magnification);
    }else{
      m_deviceTitleArrs.at(btn_index)->resize(btn->width(),50*m_Magnification);
    }
    m_deviceTitleArrs.at(btn_index)->setStyleSheet(deviceTitle_stys);

}

void MainWindow::device_leave_view(QString btn_object){


    if(!is_show_device){

        QString deviceTitle_stys=QString("background-color:transparent;color:rgb(158,158,158);text-align:center;font-size:%1px;font-family:'Source Han Sans CN Medium'").arg(32*m_Magnification);
        int btn_index = 0;
        customMainButton *btn = dynamic_cast<customMainButton *>(sender());
        if(btn->objectName()=="PD400X"){
            btn->resize(213*m_Magnification,300*m_Magnification);
            btn_index = 0;

        }else if(btn->objectName()=="DM30"){

            btn->resize(180*m_Magnification,335*m_Magnification);
            btn_index = 1;

        }else if(btn->objectName()=="DM90"){
            btn->resize(145*m_Magnification,360*m_Magnification);
            btn_index = 2;

        }else if(btn->objectName()=="PD200X"){
            btn->resize(256*m_Magnification,377*m_Magnification);
            btn_index = 3;
        }else if(btn->objectName()=="DM30RGB"){
            btn->resize(133*m_Magnification,301*m_Magnification);
            btn_index = 4;
        }else if(btn->objectName()=="DM70"){
            btn->resize(95*m_Magnification,266*m_Magnification);
            btn_index = 5;
        }

        qDebug()<<"xxxxxkkkkkkkpppppp"<<btn->x();
        m_deviceTitleArrs.at(btn_index)->move(btn->x(),btn->y()-60*m_Magnification);
        if(btn->objectName()=="DM30RGB"){
          m_deviceTitleArrs.at(btn_index)->resize(btn->width()+20*m_Magnification,50*m_Magnification);
        }else{
          m_deviceTitleArrs.at(btn_index)->resize(btn->width(),50*m_Magnification);
        }

        m_deviceTitleArrs.at(btn_index)->setStyleSheet(deviceTitle_stys);

        m_MainTimer->start(1000);

    }

}
void MainWindow::animationTimeout(){
    QPushButton *btn = m_pro_btns.at(2);
    QString deviceTitle_stys=QString("background-color:transparent;color:rgb(158,158,158);text-align:center;font-size:%1px;font-family:'Source Han Sans CN Medium'").arg((32+m_animationscale_label)*m_Magnification);
    m_animationscale += 0.01;
    m_animationscale_label ++;
    if(m_animationscale>=1.2)m_animationTimer->stop();
    m_pro_btns.at(2)->resize(145*m_Magnification*m_animationscale,360*m_Magnification*m_animationscale);
    m_deviceTitleArrs.at(2)->move(btn->x(),btn->y()-60*m_Magnification);
    m_deviceTitleArrs.at(2)->resize(btn->width(),50*m_Magnification);
    m_deviceTitleArrs.at(2)->setStyleSheet(deviceTitle_stys);

}
//进入不同的产品主界面------------------------
void MainWindow::change_view(){

    QString device_title_stys=QString("background-color:transparent;color:rgb(158,158,158);text-align:center;font-size:%1px;font-family:'Source Han Sans CN Medium'").arg(32*m_Magnification);
    //所有的标题都恢复原来的大小
    for (int i=0;i<m_currentDeviceTitleArrs.count();i++) {
        m_currentDeviceTitleArrs.at(i)->setStyleSheet(device_title_stys);
    }
    QPushButton *btn = static_cast<QPushButton *>(sender());
    qDebug()<<"USB连接成功--1111------"<<btn->objectName();
   //m_MainTimer->stop();
    if(btn->objectName() == "PD400X"){
        this->hide();

        if(m_pd400_view){
            m_pd400_view->show();
            m_pd400_view->starConnectHid();
            m_pd400_view->create_pd400UI();
            btn->resize(213*m_Magnification,300*m_Magnification);
        }
        m_pd400_view->setWindowTitle("Maono Link");
        // m_pd400_view->languge_click();

    }else if(btn->objectName() == "DM30" || btn->objectName() == "PD200X" || btn->objectName() == "DM30RGB"){//DM30
        //m_setup_btn->hide();
        this->hide();
        qDebug()<<"USB连接成功--222222------";
        //        }
        if(m_pm481_view){

            //设置PID MODELCODE
            if(btn->objectName() == "DM30"){
                currentProPid = 0x0101;
                m_currentModelCodel = "P0KT02";
                btn->resize(180*m_Magnification,335*m_Magnification);
                m_pm481_view->pm481_createUI(1);
            }else if(btn->objectName() == "PD200X"){
                currentProPid = 0x0104;
                m_currentModelCodel = "P0KT05";
                btn->resize(256*m_Magnification,377*m_Magnification);
                m_pm481_view->pm481_createUI(2);
            }else if(btn->objectName() == "DM30RGB"){
                currentProPid = 0x0106;
                m_currentModelCodel = "P0KT06";
                btn->resize(133*m_Magnification,301*m_Magnification);
                m_pm481_view->pm481_createUI(3);
            }
            m_pm481_view->show();
            m_pm481_view->starConnectHid();
            //m_pm481_view->pm481_createUI();

        }else{
          qDebug()<<"USB连接成功--222222----ppp--";
        }
        m_pm481_view->setWindowTitle("Maono Link");
    }else if(btn->objectName() == "DM90"){//DM90
        //测试一波--------------------------
        this->hide();
        m_dm90_view = new window_dm90();
        // dm90_view->setFixedSize(1023,737);
        m_dm90_view->show();
        m_dm90_view->starConnectHid(0x0102);
        btn->resize(156*m_Magnification,387*m_Magnification);
        connect(m_dm90_view, SIGNAL(Dm90BackClick()), this, SLOT(dm90_back()));

        //        QString path = qApp->applicationFilePath();
        //        //qDebug()<<"xxx"<<path;
        //        QStringList tempStringList =  path.split("/");
        //        tempStringList.removeLast();
        //        QString fileName = tempStringList.join("/");
        //        QString fileName1 = tempStringList.join("/");
        //        fileName += "/DM90 0.1.12.exe";
        //        //打开新的程序
        //        QString workpath =fileName;

        //        QProcess * process = new QProcess();
        //        QStringList list;
        //        if(language_index <= 0){//中文
        //            list<<"--lang=cn";
        //        }else{
        //          list<<"--lang=en";
        //        }

        //        process->start(workpath,list);

        //        connect(process,SIGNAL(started()),SLOT(YYstarted()));
        //        connect(process,SIGNAL(finished(int,QProcess::ExitStatus)),SLOT(YYfinished()));
        //        connect(process,SIGNAL(stateChanged(QProcess::ProcessState)),SLOT(YYstateChanged()));
        //        QApplication* app;
        //        this->hide();
    }else if(btn->objectName() == "DM70"){

        this->hide();
        m_dm90_view = new window_dm90();
        // dm90_view->setFixedSize(1023,737);
        m_dm90_view->show();
        m_dm90_view->starConnectHid(0x0103);
        btn->resize(95*m_Magnification,266*m_Magnification);
        connect(m_dm90_view, SIGNAL(Dm90BackClick()), this, SLOT(dm90_back()));
    }
    is_show_device = true;

    //位置
    int btns_width = 0;
    for (int i=0;i<m_currentPro_btns.count();i++) {

        btns_width += m_currentPro_btns.at(i)->width();
    }
    int space_btn = (this->width()-btns_width)/(m_currentPro_btns.count()+1);

    //先判断是有几个设备
    if(m_deviceArrs.count()>=1){//多于两个设备的排列方式

        for (int i=0;i<m_deviceArrs.count();i++) {
            //显示设备按钮的方法
            if(i==0){
                m_currentPro_btns.at(i)->move(space_btn,200*m_Magnification);
            }else{
                m_currentPro_btns.at(i)->move(space_btn+m_currentPro_btns.at(i-1)->width()+m_currentPro_btns.at(i-1)->x(),200*m_Magnification);
            }
            //显示标题的位置，PD200X因为图片原因，显示
            m_currentDeviceTitleArrs.at(i)->move(m_currentPro_btns.at(i)->x(),m_currentPro_btns.at(i)->y()-60*m_Magnification);
            if(btn->objectName() == "DM30RGB"){
               m_currentDeviceTitleArrs.at(i)->resize(m_currentPro_btns.at(i)->width()+30*m_Magnification,50*m_Magnification);
            }
            else{
               m_currentDeviceTitleArrs.at(i)->resize(m_currentPro_btns.at(i)->width(),50*m_Magnification);
            }

        }
    }

}
void MainWindow::pd400_back(){

    m_pd400_view = NULL;
    m_MainTimer->start(100);
    this->show();
    is_show_device = false;
#if defined(Q_OS_WIN32)

#else
   // pm481_devoperator->disconnect();
#endif

}
void MainWindow::pm481_back(){

    m_pm481_view =NULL;
    m_MainTimer->start(50);
    this->show();
    is_show_device = false;
#if defined(Q_OS_WIN32)

#else
    //pm481_devoperator->disconnect();
#endif
}
void MainWindow::dm90_back(){
    m_dm90_view =NULL;
    m_MainTimer->start(50);
    this->show();
    is_show_device = false;
#if defined(Q_OS_WIN32)

#else
    pm481_devoperator->disconnect();
#endif

}

void MainWindow::setup_clicked(){

    m_setup_view->show();
    m_titleBar->m_imageLogo->hide();
    m_setup_back_btn->show();
    m_setup_btn->hide();
    qDebug()<<"设置------";
}

void MainWindow::YYfinished(){
    this->show();
    m_MainTimer->start(100);
    qDebug()<<"YYfinished----";
}
void MainWindow::YYstarted(){
    qDebug()<<"YYstarted-0-0-0-";
}

void MainWindow::YYstateChanged(){

    qDebug()<<"YYstateChanged----";
}

void MainWindow::setup_back(){
    m_setup_view->hide();
    m_setup_back_btn->hide();
    m_setup_btn->show();
    m_titleBar->m_imageLogo->show();
}
void MainWindow::share_btn_click(){

    QPushButton *btn = dynamic_cast<QPushButton *>(sender());
    QUrl url;
    if(btn->objectName()=="douyin"){
        if(language_index <= 0){//中文
            url = QUrl("https://v.douyin.com/NGscDqk/");
        }else{
            url = QUrl("https://www.facebook.com/maonoglobal");
        }
        QDesktopServices::openUrl(url);

    }else if(btn->objectName()=="weibo"){

        if(language_index <= 0){//中文
            url = QUrl("https://weibo.com/7484526778/manage");
        }else{
            url = QUrl("https://twitter.com/Maono_Global");
        }
        QDesktopServices::openUrl(url);
    }else{
        if(language_index <= 0){//中文
            url = QUrl("https://space.bilibili.com/647359920?spm_id_from=333.337.0.0");
        }else{
            url = QUrl("https://www.instagram.com/maonoglobal/");
        }
        QDesktopServices::openUrl(url);
    }
}

//中英文切换的事件
void MainWindow::change_language(int index){

    //本地保存中英文的当前选择
    m_commClass.set_save_language_status(index);

    if(language_index==0){//中文
        if (m_translator.load(":/pd400_lang/pd400_zh.qm")) {
            bool result = qApp->installTranslator(&m_translator);
            qDebug()<<"kkkkxxxx"<<language_index<<result;
        }
        m_updateLabel->move((this->width()-320*m_Magnification)/2,0);
        m_update_btn->move(m_updateLabel->x()+m_updateLabel->width()+10*m_Magnification,-6);
        m_update_btn->resize(140*m_Magnification,35*m_Magnification);

        m_cbo_sex->move(m_languageLabel->x()+m_languageLabel->width()+10*m_Magnification,0);
    }else{//英文
        if (m_translator.load(":/pd400_lang/pd400_en.qm")) {
            bool result = qApp->installTranslator(&m_translator);
            qDebug()<<"kkkkxxxx"<<language_index<<result;
        }
        m_updateLabel->move((this->width()-320*m_Magnification)/2,0);
        m_update_btn->move(m_updateLabel->x()+m_updateLabel->width(),-6);
        m_update_btn->resize(80*m_Magnification,35*m_Magnification);

        m_cbo_sex->move(m_languageLabel->x()+m_languageLabel->width(),0);
    }
    language_index = index;

    m_NoconnectView->setText(tr("请连接MAONO设备"));
    //刷新各种ui的中英文
    for (int i=0;i<m_setupLabels.count();i++) {
        if(i==0){
            m_setupLabels.at(i)->setText(tr("版权所有 @ 2022-2030 MAONO. 保留所有权利"));
        }else if(i==1){
            m_setupLabels.at(i)->setText(tr("最终用户许可协议"));
        }else if(i==2){
            m_setupLabels.at(i)->setText(tr("软件版本：") + m_appversion_str);
        }
    }
    m_updateLabel->setText(tr("软件更新："));
    m_update_btn->setText(tr("检查更新"));
    m_languageLabel->setText(tr("语言切换："));

    changelanguage_sharebtn();

}
void MainWindow::changelanguage_sharebtn(){

    if(language_index >0 ){//英文
        QVector<QString> share_btn_names = { m_co_fun.add_image_forstatus("url(:/image/images/mbs400_fb_btn.png)",22),
                                             m_co_fun.add_image_forstatus("url(:/image/images/mbs400_tt_btn.png)",22),
                                             m_co_fun.add_image_forstatus("url(:/image/images/mbs400_ins_btn.png)",22)};
        for (int i = 0;i< share_btn_names.count();i++) {

            QPushButton *share_buttom = m_share_btns.at(i);
            share_buttom->resize(22*m_Magnification,22*m_Magnification);
            share_buttom->move(20*m_Magnification + 48*i*m_Magnification,this->height()-30*m_Magnification);
            const QString str = share_btn_names.at(i);
            share_buttom->setStyleSheet(str);
        }
    }else{
        QVector<QString> share_btn_names = { m_co_fun.add_image_forstatus(QString("url(:/image/images/pd400_sharedouyin_btn.png)").arg(m_Magnification),17),
                                             m_co_fun.add_image_forstatus("url(:/image/images/pd400_shareweibo_btn.png)",23),
                                             m_co_fun.add_image_forstatus("url(:/image/images/pd400_sharbi_btn.png)",42)};
        for (int i = 0;i< share_btn_names.count();i++) {
            QPushButton *share_buttom = m_share_btns.at(i);
            if(i==0){//抖音
                share_buttom->resize(17*m_Magnification,19*m_Magnification);
                share_buttom->move(20*m_Magnification + 48*i*m_Magnification,this->height()-30*m_Magnification);
            }else if(i==1){//微博
                share_buttom->resize(23*m_Magnification,19*m_Magnification);
                share_buttom->move((20 + 26+17)*m_Magnification,this->height()-30*m_Magnification);
            }else{//b站
                share_buttom->resize(42*m_Magnification,19*m_Magnification);
                share_buttom->move(20*m_Magnification + 26*2*m_Magnification+17*m_Magnification+23*m_Magnification,this->height()-30*m_Magnification);
            }
            const QString str = share_btn_names.at(i);
            share_buttom->setStyleSheet(str);
        }
    }
}
void MainWindow::updateVersion_click(){


    m_newwork_update->network_updateversion();
    qDebug()<<"点击了更新上位机------"<<m_appversion_str;

    //    QString ver_str = networking_class::getInstance()->network_updateversion("http://159.75.118.131/upinfo_test.aspx?proj_name=Maono Link");
    //    QStringList ver_arrs = ver_str.split(".");
    //    QStringList currentVer_arrs = m_version_str.split(".");
    //    if(ver_arrs.count()>=3){
    //        int net_soft_version = ver_arrs.at(0).toInt()*1000+ver_arrs.at(1).toInt()*100+ver_arrs.at(2).toInt();
    //        int current_soft_version = currentVer_arrs.at(0).toInt()*1000+currentVer_arrs.at(1).toInt()*100+currentVer_arrs.at(2).toInt();
    //        //判断网络请求的版本跟当前的版本号对比
    //        if(net_soft_version>current_soft_version){
    //            //从网络下载
    //            m_newwork_update->network_download();
    //            qDebug()<<"点击了更新上位机------"<<ver_str<<m_version_str;
    //        }

    //    }

    //网络请求对比版本号
}
//下一页的方法
void MainWindow::leftBtn_clicked(){

    m_mainSuperView->setCurrentIndex(0);
    m_right_btn->show();
    m_left_btn->hide();
}
//上一页的方法
void MainWindow::rightBtn_clicked(){

    m_mainSuperView->setCurrentIndex(1);
    m_left_btn->show();
    m_right_btn->hide();
}

MainWindow::~MainWindow()
{
    qDebug()<<"析构函数清空所有指针------";

    //delete m_pTimer;
    //    delete ui;
}
