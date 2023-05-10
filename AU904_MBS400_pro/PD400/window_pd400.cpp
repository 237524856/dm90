#include "window_pd400.h"
#include <QApplication>
//#include "QDesktopWidget"
#include <QFontDatabase>
#include <QThread>
#include <QDebug>
#include <QTreeWidget>
#include <QButtonGroup>
#include <QCheckBox>
#include <QDesktopServices>
#include <QUrl>
#include "../HID/utils/CRC.h"
#include <QMessageBox>
#include <QStandardPaths>
#include "networking_class.h"
#include <ctype.h>

window_pd400::window_pd400(QWidget *parent) : basewindow(parent),
    is_open_status(false)
{
    QIcon appIcon(":/image/images/logo.ico");
    QApplication::setWindowIcon(appIcon);
    //QFontDatabase::addApplicationFont(":/image/MBS_400/SourceHanSansCN Medium.ttf");
    setFixedSize(1023*m_Magnification,737*m_Magnification);
    is_pd400needto_updatadsp=false;
    // setWindowFlags(Qt::CustomizeWindowHint | Qt::MSWindowsFixedSizeDialogHint);
    //        this->setMaximumSize(1023,737);  //设置窗体最大尺寸
    //        this->setMinimumSize(1023,737);  //设置窗体最小尺寸
    m_current_languge_index = 0;
    //create_pd400UI();
    m_basic_btn = new custom_button(this);
    m_basic_btn->hide();
    m_high_btn = new custom_button(this);
    m_high_btn->hide();
    m_setting_btn = new custom_checkbox(this);
    m_setting_btn->hide();
    m_product_label =new QLabel(this);

    connect(m_titleBar, SIGNAL(signalButtonCloseClicked()), this, SLOT(back_view_click()));

    m_disableTimer = new QTimer(this);
    connect(m_disableTimer, SIGNAL(timeout()), this, SLOT(disableHandleTimeout()));

    m_dfu_view = new dfu_0206_view();
    connect(m_dfu_view, SIGNAL(dsp_progress_value(int)), this, SLOT(dsp_kt0206_progress_val(int)));
    connect(m_dfu_view, SIGNAL(dsp_finish_progress(int)), this, SLOT(dsp_kt0206_finish(int)));

    m_starDspTimer = new QTimer(this);
    connect(m_starDspTimer, SIGNAL(timeout()), this, SLOT(starDspTimeout()));

    m_hearbeattimer = new QTimer(this);
    connect(m_hearbeattimer, SIGNAL(timeout()), this, SLOT(hearbeatTimeout()));
}

void window_pd400::starConnectHid(){
    //收发数据，连接是否成功
    if(pm481_devoperator){
        pm481_devoperator->pd400_get_bgdata.clear();
        pm481_devoperator->m_starThread = true;
        connect(this,SIGNAL(sendMsgSignals()),pm481_devoperator,SLOT(pd400_startSendMsg()));
        //开启线程
        emit sendMsgSignals();
        bool is_connect = pm481_devoperator->starConnectDevice(0x0100);
        if(is_connect){//USB连接成功
            is_modelconnect = true;
            //is_usb_connect =true;
            currentProPid = 0x0100;
            qDebug()<<"USB连接成功";
            //先获取MODELCOLDE
            m_commin_fun.getModelCode();
            //开始获取背景参数
            //m_commin_fun.bg_datasource();
        }else{
            qDebug()<<"USB连接失败";
        }
        //页面之间通讯的信号槽
        connect(pm481_devoperator, SIGNAL(connectDevice()), this, SLOT(OnConnectDevice()));
        connect(pm481_devoperator, SIGNAL(disconnectDevice()), this, SLOT(OnDisConnectDevice()));
        connect(pm481_devoperator,SIGNAL(pd400_refresh_main_signal()),this,SLOT(refresh_main_UI()));
        connect(pm481_devoperator,SIGNAL(pd400_sysreceive_datasignal()),this,SLOT(receive_sys_data()));
        connect(pm481_devoperator,SIGNAL(pd400_progress_current_value(int)),this,SLOT(progress_value(int)));
        connect(pm481_devoperator,SIGNAL(pd400_break_fail_mess(int)),this,SLOT(sendMessage_fail_hand(int)));
        //判断是否要连接
        connect(pm481_devoperator,SIGNAL(blockModelCodel()),this,SLOT(maingetModelCodel()));
        connect(pm481_devoperator,SIGNAL(blockDfuStatus(int)),this,SLOT(isDfuStatus(int)));
    }

    m_pTimer =NULL;
    if(!m_pTimer){
        m_pTimer = new QTimer(this);
        connect(m_pTimer, SIGNAL(timeout()), this, SLOT(handleTimeout()));
        m_pTimer->start(200);
    }
    //判断是否是升级模式
    //    m_isdfu_timer = new QTimer(this);
    //    connect(m_isdfu_timer, SIGNAL(timeout()), this, SLOT(isdfu_handleTimeout()));
    //    m_isdfu_timer->start(200);
    m_dfu_thread = NULL;
    if(!m_dfu_thread){
        m_dfu_thread = new DFU_thread();
        m_dfu_thread->isReceiveData = true;
        m_dfu_thread->m_ret = true;
        m_dfu_thread->m_dfu_mess = pm481_devoperator;
        m_dfu_thread->start();
    }
}
void window_pd400::clearDataSource(){
    m_mute_arrs.clear();
    m_synmicvolume_Slider_arrs.clear();
    m_pd400SynSlider_arrs.clear();
    pd400ComLimGainSliders.clear();
}
void window_pd400::create_pd400UI(){

    m_setting_btn->show();
    m_basic_btn->show();
    m_high_btn->show();
    clearDataSource();

    QButtonGroup *m_buttonGroup = new QButtonGroup();
    m_buttonGroup->setExclusive(true);

    //m_basic_btn = new QPushButton(this);
    m_basic_btn->resize(80*m_Magnification,26*m_Magnification);
    m_basic_btn->move(15*m_Magnification,67*m_Magnification);
    m_basic_btn->setText(tr("基础"));
    QString basic_btn_stys=QString("QPushButton{border-image:url(:/image/images/pd400_ba_hi_btn.png);font-family:'Source Han Sans CN Medium';color:white;font-size:%1px;}"
                                   "QPushButton:hover{border-image:url(:/image/images/pd400_ba_hi_btn.png);font-family:'Source Han Sans CN Medium';color:rgb(54,54,54);font-size:%1px;}"
                                   "QPushButton:pressed{border-image:url(:/image/images/pd400_ba_hi_lig_btn.png);}"
                                   "QPushButton:checked{border-image:url(:/image/images/pd400_ba_hi_lig_btn.png);}"
                                   "QPushButton:focus{outline: none;}").arg(15*m_Magnification);
    m_basic_btn->setStyleSheet(basic_btn_stys);
    //m_basic_btn->setCheckable(true);
    m_basic_btn->setCheckable(true);
    connect(m_basic_btn,SIGNAL(clicked()),this,SLOT(open_basic()));
    //    m_buttonGroup->addButton(m_basic_btn,0);

    //    m_high_btn = new QPushButton(this);
    m_high_btn->resize(80*m_Magnification,26*m_Magnification);
    m_high_btn->move(98*m_Magnification,67*m_Magnification);
    m_high_btn->setText(tr("高级"));
    m_high_btn->setStyleSheet(basic_btn_stys);
    m_high_btn->setCheckable(true);
    connect(m_high_btn,SIGNAL(clicked()),this,SLOT(open_high()));
    //    m_buttonGroup->addButton(m_high_btn,1);

    //m_buttonGroup->buttonClicked(m_high_btn);
    //connect(m_buttonGroup,SIGNAL(buttonClicked(int)),this,SLOT(change_status(int)));

    //基础高级禁点
    m_basic_btn->setDisabled(true);
    m_high_btn->setDisabled(true);

    //    m_setting_btn = new QCheckBox(this);
    m_setting_btn->resize(20*m_Magnification,20*m_Magnification);
    m_setting_btn->move(this->width()-40*m_Magnification,70*m_Magnification);

    QString setting_btn_stys=QString("QCheckBox::indicator{width: %1px;height: %1px;color: rgb(255, 0, 0);}"
                                     "QCheckBox::indicator:unchecked{border-image:url(:/image/images/pd400_setting_btn_default);}"
                                     "QCheckBox::indicator:unchecked:hover{border-image:url(:/image/images/pd400_setting_btn_hover);}"
                                     "QCheckBox::indicator:checked{border-image:url(:/image/images/pd400_setting_btn_press);}").arg(20*m_Magnification).arg(40*m_Magnification).arg(m_Magnification);
    m_setting_btn->setStyleSheet(setting_btn_stys);
    connect(m_setting_btn,SIGNAL(stateChanged(int)),this,SLOT(setting_change(int)));
    //左边的视图
    m_left_view = new pd400_leftview(this);
    m_left_view->move(15*m_Magnification,96*m_Magnification);
    m_left_view->resize(496*m_Magnification,604*m_Magnification);
    m_left_view->createUI();
    m_left_view->show();
    //右边的视图
    m_right_view = new pd400_rightview(this);
    m_right_view->move(518*m_Magnification,96*m_Magnification);
    m_right_view->resize(496*m_Magnification,604*m_Magnification);
    m_right_view->createUI();
    m_right_view->show();
    connect(m_right_view,SIGNAL(show_limitOrcomp_view(int)),this,SLOT(click_limit_comp_view(int)));
    connect(m_right_view,SIGNAL(DM30_timer_out()),this,SLOT(DM30_timer_outfun()));
    //右边的视图
    //    QPushButton *back_btn = new QPushButton(this);
    //    back_btn->setToolTip("提示");
    //    back_btn->resize(60,30);
    //    back_btn->move(10,180);
    //    back_btn->setText("返回");
    //    connect(back_btn,SIGNAL(clicked()),this,SLOT(back_view_click()));
    //    qDebug()<<this->width()<<this->height();

    m_product_label->resize(650*m_Magnification,40*m_Magnification);
    m_product_label->move(216*m_Magnification,this->height()-40*m_Magnification);
    m_product_label->setText(tr("请插入USB设备"));
    QString product_label_stys=QString("font-family:'Source Han Sans CN Medium';color:red;font-size:%1px;").arg(15*m_Magnification);
    m_product_label->setStyleSheet(product_label_stys);
    m_product_label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    QFont font;
    font.setFamily("Source Han Sans CN Medium");
    font.setPixelSize(13);

    QPalette pa;
    QColor niceBlue(255, 0, 0);
    pa.setColor(QPalette::WindowText,niceBlue);


    //设置的页面
    m_setting_view = new pd400_setting_view(this);
    m_setting_view->move(0,0);
    m_setting_view->resize(this->width(),this->height());
    m_setting_view->createUI();
    m_setting_view->hide();
    //    connect(m_setting_view,SIGNAL(change_languge(int)),this,SLOT(languge_click(int)));
    connect(m_setting_view,SIGNAL(check_version()),this,SLOT(check_version_click()));
    connect(m_setting_view,SIGNAL(setting_mouse_press()),this,SLOT(setting_mouse_click()));
    connect(m_setting_view,SIGNAL(reset_clickBlock()),this,SLOT(reset_action()));
    //设置按钮在最上层
    m_setting_btn->raise();

    m_show_xianfuView = new pd400_xianzhiqi_view(this);
    m_show_xianfuView->move(0,0);
    m_show_xianfuView->resize(this->width(),this->height());
    m_show_xianfuView->createUI();
    m_show_xianfuView->hide();
    connect(m_show_xianfuView,SIGNAL(close_xianfuqi_view()),this,SLOT(main_load_xianfuqi_data()));

    m_show_comp = new pd400_yasuoqi_view(this);
    m_show_comp->move(0,0);
    m_show_comp->resize(this->width(),this->height());
    m_show_comp->createUI();
    m_show_comp->hide();
    connect(m_show_comp,SIGNAL(close_yasuoqi_view()),this,SLOT(main_load_yasuoqi_data()));

    m_check_view = new pd400_check_updateView(this);
    m_check_view->move(0,0);
    m_check_view->resize(this->width(),this->height());
    m_check_view->createUI();
    m_check_view->hide();
    connect(m_check_view,SIGNAL(updata_DFU()),this,SLOT(DFU_updata()));
    connect(m_check_view,SIGNAL(finish_download_dspfile()),this,SLOT(alone_updata_dsp()));

    //进度条的页面
    m_progress_view = new pd400_progress_view(this);
    m_progress_view->move(0,0);
    m_progress_view->resize(this->width(),this->height());
    m_progress_view->createUI();
    m_progress_view->hide();
}
//设置页面的检查更新
void window_pd400::check_version_click(){

    m_setting_btn->setCheckState(Qt::Unchecked);
    m_setting_view->hide();
    //开始下载，根据下载的BIN文件获取不同的版本
    //m_check_view->set_diff_status(1);
    m_dm30downloaddfu_number = 0;
    m_check_view->m_model_codel = PD400;
    is_pd400needto_updatadsp = false;
    is_pd400updata_runing = false;

    //先判断mcu是否需要下载
    QString ver_str = networking_class::getInstance()->network_updateversion("http://159.75.118.131/upinfo_test.aspx?proj_name=PD400");
    QStringList ver_arrs = ver_str.split(".");
    if(ver_arrs.count()>=3){
        int net_dsp_version = ver_arrs.at(0).toInt()*1000+ver_arrs.at(1).toInt()*100+ver_arrs.at(2).toInt()-1000;
        int dsp_version = device_version_x*1000+device_version_y*100+device_version_z;
        if(net_dsp_version > dsp_version){//下载升级
            QString file = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+"/PD400_"+ver_str+".bin";
            m_check_view->set_bindfu_file(file);
            m_check_view->download_bin_file(QUrl("http://159.75.118.131/dnfile_test.aspx?f_sn=14"),file);
            is_pd400updata_runing =true;
            m_progress_type = PD400MCU;
        }else{//判断DSP固件的版本是否需要升级
            m_dm30downloaddfu_number = 100;
            QString ver_str = networking_class::getInstance()->network_updateversion("http://159.75.118.131/upinfo_test.aspx?proj_name=PD400_dsp");
            QStringList ver_arrs = ver_str.split(".");
            if(ver_arrs.count()>=3){
                int net_dsp_version = ver_arrs.at(0).toInt()*10000+ver_arrs.at(1).toInt()*1000+ver_arrs.at(2).toInt();
                int dsp_version = dsp_version_x*10000+dsp_version_y*1000+dsp_version_z;
                if(net_dsp_version > dsp_version){//下载DSP固件
                    m_hearbeattimer->stop();//升级DSP前关闭心跳包
                    m_PD400file_dsp = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+"/PD400_dsp"+ver_str+".bin";
                    //this->set_bindfu_file(file_dsp);
                    m_check_view->download_bin_file(QUrl("http://159.75.118.131/dnfile_test.aspx?f_sn=18"),m_PD400file_dsp);
                    //qDebug() <<"22222xxxxxxxxx"<<"下载完成";
                    is_pd400needto_updatadsp = true;
                    is_pd400updata_runing =true;
                }else{
                    m_check_view->set_diff_status(2);
                }
            }
        }
    }
//    QString file = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+"/PD400.bin";
//    m_check_view->set_bindfu_file(file);
//    m_check_view->download_bin_file(QUrl("http://159.75.118.131/dnfile_test.aspx?f_sn=14"),file);
    //为了适配除本机外其他电脑也能打开文件，暂无知道原因
    emit dfuReadFileData();
    //m_check_view->show();
}
//弹出限制器或者压缩器
void window_pd400::click_limit_comp_view(int status){

    if(status==1){//限制器
        m_show_xianfuView->show();
        m_show_xianfuView->changelanguage();
//        if(pm481_devoperator->pd400_get_bgdata.count()>=16){
            //           uint16_t limit = (pm481_devoperator->pd400_get_bgdata.at(9).param_value[1]<<8) | pm481_devoperator->pd400_get_bgdata.at(9).param_value[0];
            //           uint8_t limit_attack =  pm481_devoperator->pd400_get_bgdata.at(10).param_value[0];
            //           uint16_t limit_release = (pm481_devoperator->pd400_get_bgdata.at(11).param_value[1]<<8) | pm481_devoperator->pd400_get_bgdata.at(11).param_value[0];
            m_show_xianfuView->refresh_ui(xianfuqi_slider_values.at(0),xianfuqi_slider_values.at(1),xianfuqi_slider_values.at(2));
//        }else{
//            m_show_xianfuView->m_sliders.at(0)->setValue(xianfuqi_slider_values.at(0));
//        }

    }else{//压缩器
        m_show_comp->show();
        m_show_comp->changelanguage();
//        if(pm481_devoperator->pd400_get_bgdata.count()>=16){
            //            uint16_t comp = (pm481_devoperator->pd400_get_bgdata.at(4).param_value[1]<<8) | pm481_devoperator->pd400_get_bgdata.at(4).param_value[0];
            //            uint8_t comp_attack= pm481_devoperator->pd400_get_bgdata.at(5).param_value[0];
            //            uint16_t comp_release = (pm481_devoperator->pd400_get_bgdata.at(6).param_value[1]<<8) | pm481_devoperator->pd400_get_bgdata.at(6).param_value[0];
            //            uint8_t comp_rate= pm481_devoperator->pd400_get_bgdata.at(7).param_value[0];
            m_show_comp->refresh_ui(yaxianqi_slider_values.at(0),yaxianqi_slider_values.at(2),yaxianqi_slider_values.at(1),yaxianqi_slider_values.at(3),yaxianqi_slider_values.at(4));
//        }else{
//            m_show_comp->m_sliders.at(0)->setValue(yaxianqi_slider_values.at(0));
//        }
    }
}
void window_pd400::main_load_xianfuqi_data(){

    m_right_view->load_xianfuqi_data();
}
void window_pd400::main_load_yasuoqi_data(){

    m_right_view->load_yasuoqi_data();
}
void window_pd400:: setting_change(int status){

    if(status){
        m_setting_view->show();

    }else{
        m_setting_view->hide();
    }
    //qDebug()<<"xxxxkkkk"<<status;
}
//void window_pd400::change_status(int status){
//    if(status==0){//基础
//        m_left_view->m_mengban_image->hide();
//        m_right_view->m_mengban_image->show();
//    }else{//高级
//        m_left_view->m_mengban_image->show();
//        m_right_view->m_mengban_image->hide();
//    }
//    m_commin_fun.change_basic_high_Model(status);
//}

void window_pd400::setting_mouse_click(){

    m_setting_btn->setCheckState(Qt::Unchecked);
    m_setting_view->hide();
}
//打开基础
void window_pd400::open_basic(){

    qDebug()<<pd400_mic_value<<"xxhhhhhhhhhhh";
    m_high_btn->setChecked(false);
    m_basic_btn->setChecked(true);
    m_left_view->m_mengban_image->hide();
    m_right_view->m_mengban_image->show();

    if(m_pd400current_model ==1)return;
    m_pd400current_model = 1;
    m_commin_fun.sys_Software_tag(1);
    m_commin_fun.change_basic_high_Model(0);
    //麦克风增益调到最大,限制器，压缩器调到对应的位置
    QVector<int>sliderValues = {100,40,80};
    QVector<QString>currentValues = {"100","0dB","0dB"};
    //关闭显示器压缩器
//    QString ComLimGainSliders_stys=QString("QSlider::groove:horizontal{background: rgba(0, 0, 0,100);height:%1px;border: 0px solid #FFFFFF;border-image:url(:/image/images/pd400_right_slider_bgimg.png);}"
//                                           "QSlider::handle:horizontal{width:%2px;height:%2px;margin-top: -%1px;margin-left: 0px;margin-bottom: -%1px;margin-right: 0px;border-image:url(:/image/images/pd400_round__disabled.png);}"
//                                           "QSlider::sub-page:horizontal{background-color:rgba(0,0,0,100);border-radius: %1px;border-image:url(:/image/images/pd400_right_addslider_disabled.png);}").arg(4*m_Magnification).arg(12*m_Magnification);
    for (int i=0;i<m_right_view->enable_btns.count();i++) {
        m_right_view->select_btns.at(i)->setDisabled(true);
        m_right_view->enable_btns.at(i)->blockSignals(true);
        m_right_view->enable_btns.at(i)->setCheckState(Qt::Unchecked);        
//        pd400ComLimGainSliders.at(i+1)->setStyleSheet(ComLimGainSliders_stys);
        m_right_view->enable_btns.at(i)->blockSignals(false);
    }
    //判断当前的远中近是那个值
    if(m_left_view->m_dis_current_number =="100"){//近
        //发送对应的数据
        m_commin_fun.set_basic_near();
        //使用距离
        m_commin_fun.pd400_yuanzhongjin(0);

    }else if(m_left_view->m_dis_current_number =="101"){//中

        //发送对应的数据
        m_commin_fun.set_basic_near();
        //使用距离
        m_commin_fun.pd400_yuanzhongjin(1);

    }else if(m_left_view->m_dis_current_number =="102"){//远
        m_commin_fun.set_basic_far();
        //使用距离
        m_commin_fun.pd400_yuanzhongjin(2);
        sliderValues = {100,80,60};
        currentValues = {"100","0dB","-20dB"};
    }

    if(m_left_view->m_tone_current_number=="dichen"){
        m_commin_fun.pd400_yinse(0);
    }else if(m_left_view->m_tone_current_number=="ziran"){
        m_commin_fun.pd400_yinse(1);
    }else if(m_left_view->m_tone_current_number=="mingliang"){
        m_commin_fun.pd400_yinse(2);
    }else{
        m_commin_fun.pd400_yinse(3);
    }
    for (int i=0;i<pd400ComLimGainSliders.count();i++) {
        pd400ComLimGainSliders.at(i)->blockSignals(true);
        pd400ComLimGainSliders.at(i)->setValue(sliderValues.at(i));
        pd400ComLimGainSliders.at(i)->blockSignals(false);
        m_right_view->m_current_labels.at(i)->setText(currentValues.at(i));
    }

    m_high_btn->setDisabled(true);
    m_disableTimer->stop();
    m_disableTimer->start(600);
}
//打开高级
void window_pd400::open_high(){

    m_high_btn->setChecked(true);
    m_basic_btn->setChecked(false);
    m_left_view->m_mengban_image->show();
    m_right_view->m_mengban_image->hide();

    if(m_pd400current_model ==2)return;
    m_pd400current_model = 2;

    m_commin_fun.sys_Software_tag(1);
    m_commin_fun.change_basic_high_Model(1);
    //发送均衡器模式的命令
    int eq_current_number = m_right_view->m_current_btn_select.toInt()-200;
    m_commin_fun.pd400_mic(eq_current_number);
    qDebug()<<"测试EQ====================x------"<<eq_current_number;

    m_commin_fun.save_gain_adv();

    m_basic_btn->setDisabled(true);
    m_disableTimer->stop();
    m_disableTimer->start(600);
}
//usb插入
void window_pd400::OnConnectDevice(){

    if(!is_pd400updata_runing){
        QThread::msleep(1000);
        is_usb_connect =true;
        is_modelconnect = true;
        //QThread::msleep(8000);
        //获取背景参数之前先清除原有的
        pm481_devoperator->m_PM481Request_device_datas.clear();
        m_commin_fun.getModelCode();
    }
    qDebug()<<"usb插入------";
}
void window_pd400::device_disconnect_fun(){

    pm481_devoperator->m_starThread = false;
    m_dfu_thread->isReceiveData = false;
    m_hearbeattimer->stop();
    m_disableTimer->stop();
    is_usb_connect =false;
    is_modelconnect = false;
    pm481_devoperator->m_PM481Request_device_datas.clear();
    m_pd400_device_datas.clear();
    pd400_m_sendMsgs.clear();
    m_receive_pcParMsgs.clear();
    m_modelCodelName.clear();
    m_product_label->setText(tr("请插入USB设备"));
    QString product_label_stys=QString("font-family:'Source Han Sans CN Medium';color:red;font-size:%1px;").arg(15*m_Magnification);
    m_product_label->setStyleSheet(product_label_stys);

    //蒙版开启
    m_left_view->m_mengban_image->show();
    m_right_view->m_mengban_image->show();

    m_basic_btn->setDisabled(true);
    m_basic_btn->setChecked(false);
    m_high_btn->setDisabled(true);
    m_high_btn->setChecked(false);
    setSliderDisabled(true);

    pm481_devoperator->m_isreceiveSem.release();
}
//usb拔出
void window_pd400::OnDisConnectDevice(){

    qDebug()<<"usb拔出------";
    if(!is_pd400updata_runing){
        device_disconnect_fun();
        emit pd400BackClick();
        this->close();
        delete this;
    }
}
void window_pd400::back_view_click(){

    pm481_devoperator->pd400_get_bgdata.clear();
    //m_dfu_thread->pause.lock();

#if defined(Q_OS_WIN32)

    device_disconnect_fun();
    emit pd400BackClick();
    this->close();
    delete this;
    pm481_devoperator->disconnect();

#else
    pm481_devoperator->disconnect();

#endif


}
//刷新各种UI，刷新的问题一般都在这里
void window_pd400::refresh_main_UI(){

    //先去掉相同得部分
    if(pm481_devoperator->m_PM481Request_device_datas.count()>=3){

        for(int i=0;i<pm481_devoperator->m_PM481Request_device_datas.size();i++){
            for(int j=i+1;j<pm481_devoperator->m_PM481Request_device_datas.size();j++){
                QByteArray array_data_front;
                array_data_front.append((char*)&pm481_devoperator->m_PM481Request_device_datas[j], sizeof(pm481_devoperator->m_PM481Request_device_datas[j]));
                QByteArray array_data_back;
                array_data_back.append((char*)&pm481_devoperator->m_PM481Request_device_datas[i], sizeof(pm481_devoperator->m_PM481Request_device_datas[i]));
                if(array_data_front==array_data_back){
                    pm481_devoperator->m_PM481Request_device_datas.remove(j);
                    j--;
                }
            }
        }
    }

    //更新数据到对象中
    for (int i=0;i<pm481_devoperator->m_PM481Request_device_datas.count();i++) {
        //去偷去尾
        QByteArray device_datas_pack;
        device_datas_pack.append((char*)&pm481_devoperator->m_PM481Request_device_datas.at(i), sizeof(pm481_devoperator->m_PM481Request_device_datas.at(i)));

        //最后一包只有21个数据
        device_datas_pack = device_datas_pack.mid(10,52);

        m_pd400_device_datas.append(device_datas_pack);
    }

    GET_PM481_DEVICE_DATA = new pc_PM481_device_dataSource();
    memset(GET_PM481_DEVICE_DATA,0,sizeof(pc_PM481_device_dataSource));
    //赋值给结构体
    GET_PM481_DEVICE_DATA = (pc_PM481_device_dataSource*)m_pd400_device_datas.data();

    //获取设备的名字    
    QString device_name  = tr("已连接");
    device_name += " ";
    QString setupdevice_name;
    for (int i =0;i<3; i++) {
        device_name += QChar((GET_PM481_DEVICE_DATA->PM481_DeviceName[i])&0xff);
        device_name += QChar((GET_PM481_DEVICE_DATA->PM481_DeviceName[i]>>8)&0xff);

        setupdevice_name += QChar((GET_PM481_DEVICE_DATA->PM481_DeviceName[i])&0xff);
        setupdevice_name += QChar((GET_PM481_DEVICE_DATA->PM481_DeviceName[i]>>8)&0xff);
    }
    device_name += tr("设备");
    m_setting_view->m_device_name_label->setText(tr("产品型号: ")+setupdevice_name);
    m_product_label->setText(device_name);
    QString product_label_stys=QString("font-family:'Source Han Sans CN Medium';color:white;font-size:%1px;").arg(15*m_Magnification);
    m_product_label->setStyleSheet(product_label_stys);

    //开启各种东西
    m_basic_btn->setDisabled(false);
    m_high_btn->setDisabled(false);
    setSliderDisabled(false);
    //获取版本号
    uint16_t version_x=device_version_x= GET_PM481_DEVICE_DATA->PM481_FirmwareRev[0];
    uint16_t version_y=device_version_y = GET_PM481_DEVICE_DATA->PM481_FirmwareRev[1];
    uint16_t version_z=device_version_z = GET_PM481_DEVICE_DATA->PM481_FirmwareRev[2];
    version_str =QString("V")+QString::number(version_x,10) +"."+QString::number(version_y,10)+"."+QString::number(version_z,10);
    m_setting_view->m_version_label->setText(tr("固件版本: ")+version_str);

    //DSP的版本号
    uint16_t DSP_x=dsp_version_x  = GET_PM481_DEVICE_DATA->PM481_DSP1Rev[0];
    uint16_t DSP_y=dsp_version_y  = GET_PM481_DEVICE_DATA->PM481_DSP1Rev[1];
    uint16_t DSP_z=dsp_version_z  = GET_PM481_DEVICE_DATA->PM481_DSP1Rev[2];
    QString dsp_str = QString("V")+QString::number(DSP_x,10) +"."+QString::number(DSP_y,10)+"."+QString::number(DSP_z,10);
    m_setting_view->m_DSP_label->setText(tr("DSP版本: ")+dsp_str);

    //设备SN号
    QString SN_str_name="";
    for (int i=0;i<25;i++) {
        if(isascii((GET_PM481_DEVICE_DATA->PM481_Device_SN[i])&0xff))
        SN_str_name +=  QChar((GET_PM481_DEVICE_DATA->PM481_Device_SN[i])&0xff);
        if(isascii((GET_PM481_DEVICE_DATA->PM481_Device_SN[i]>>8)&0xff))
        SN_str_name += QChar((GET_PM481_DEVICE_DATA->PM481_Device_SN[i]>>8)&0xff);
    }
    m_setting_view->m_SN_label->setText(tr("设备SN: ")+SN_str_name);

    //静音
    uint8_t mute =  GET_PM481_DEVICE_DATA->PM481_micMute;

    uint16_t mix = GET_PM481_DEVICE_DATA->PM481_mix;
    //耳机音量
    uint8_t mic_volume = GET_PM481_DEVICE_DATA->PM481_HPGain;

    uint8_t yinse = GET_PM481_DEVICE_DATA->PM481_Tone;
    uint8_t micjuli =  GET_PM481_DEVICE_DATA->PM481_Dis;

    //均衡器
    uint8_t eq_value = GET_PM481_DEVICE_DATA->PM481_micMode;

    uint8_t gain = GET_PM481_DEVICE_DATA->PM481_micBaseGain;


    uint16_t limit = GET_PM481_DEVICE_DATA->PM481_limitThreshold;

    uint16_t comp = GET_PM481_DEVICE_DATA->PM481_compThreshold;

    //2个使能
    uint8_t limit_enable = GET_PM481_DEVICE_DATA->PM481_limitEnable;
    uint8_t comp_enable = GET_PM481_DEVICE_DATA->PM481_compEnable;

    m_left_view->refresh_ui(mute,mix,yinse,micjuli,mic_volume);
    m_right_view->refresh_ui(mute,mix,eq_value,gain,limit,comp,limit_enable,comp_enable,mic_volume);

    //限制器的数据
    uint16_t limit_view = GET_PM481_DEVICE_DATA->PM481_limitThreshold;
    int change_limit = (limit_view-2000) /10 +40;
    uint8_t limit_attack_view =  GET_PM481_DEVICE_DATA->PM481_limitAttack;

    uint16_t limit_release_view = GET_PM481_DEVICE_DATA->PM481_limitRelease;
    xianfuqi_slider_values.replace(0,change_limit);
    xianfuqi_slider_values.replace(1,limit_attack_view);
    xianfuqi_slider_values.replace(2,limit_release_view);

    //压缩器界面的数据
    uint16_t comp_view = GET_PM481_DEVICE_DATA->PM481_compThreshold;
    int change_comp = (comp_view-2000) /10 +80;


    uint16_t comp_maxgain_view = GET_PM481_DEVICE_DATA->PM481_compMaxGain;
    int change_maxgain_comp = (comp_maxgain_view-2000) /10;

    uint8_t comp_attack_view = GET_PM481_DEVICE_DATA->PM481_compAttack;

    uint16_t comp_release_view = GET_PM481_DEVICE_DATA->PM481_compRelease;

    uint8_t comp_rate_view = GET_PM481_DEVICE_DATA->PM481_compRate;

    yaxianqi_slider_values.replace(0,change_comp);
    yaxianqi_slider_values.replace(1,comp_attack_view);
    yaxianqi_slider_values.replace(2,comp_release_view);
    yaxianqi_slider_values.replace(3,comp_rate_view);
    yaxianqi_slider_values.replace(4,change_maxgain_comp);

    uint8_t user_model =  GET_PM481_DEVICE_DATA->PM481_UserMode;
    m_pd400current_model = user_model + 1;
    if(user_model==0){//基础模式
        m_basic_btn->blockSignals(true);
        //m_basic_btn->setCheckable(true);
        m_basic_btn->setChecked(true);
        m_basic_btn->blockSignals(false);
        //显示蒙版
        m_left_view ->m_mengban_image->hide();
        m_right_view->m_mengban_image->show();

    }else{//高级模式
        m_high_btn->blockSignals(true);
        //m_high_btn->setCheckable(true);
        m_high_btn->setChecked(true);
        m_high_btn->blockSignals(false);
        //显示蒙版
        m_left_view ->m_mengban_image->show();
        m_right_view->m_mengban_image->hide();
    }

    //发送同步标志
    m_commin_fun.sys_Software_tag(0);

#if defined(Q_OS_WIN32)
    //开启心跳
    //m_hearbeattimer->start(700);
#else

#endif
    is_resetFaocClick = false;

}
//同步信息的方法
void window_pd400::receive_sys_data(){

    QByteArray array_data;
    array_data.append((char*)&pm481_devoperator->m_sys_receive_data, sizeof(pm481_devoperator->m_sys_receive_data));
    uint8_t addr = pm481_devoperator->m_sys_receive_data.param_addr[0];
    uint16_t value =(((pm481_devoperator->m_sys_receive_data.param_value[1] & 0xff)<<8) | (pm481_devoperator->m_sys_receive_data.param_value[0] & 0xff)) &0xffff;

    if(addr !=(PARAM_MIC_SIG_LEVEL & 0xff) && pm481_devoperator->m_sys_receive_data.command == 0x03){
        //pd400_send_value = value;
        //send_radom_data.command = 0x04;
    }
    if(pm481_devoperator->m_sys_receive_data.command == 0x03){
        m_left_view->sys_receive_data(addr,value);
        m_right_view->sys_receive_data(addr,value);
    }
    //判断出有没需要读取保存的gain
    if(addr ==0x24 && pm481_devoperator->m_sys_receive_data.command == 0x04){

        int gain_value = pm481_devoperator->m_sys_receive_data.param_value[0];
        m_commin_fun.set_high(gain_value);
        QVector<int>sliderValues = {gain_value,40,80};
        QVector<QString>currentValues = {QString::number(gain_value,10),"0dB","0dB"};
        for (int i=0;i<pd400ComLimGainSliders.count();i++) {
            pd400ComLimGainSliders.at(i)->blockSignals(true);
            pd400ComLimGainSliders.at(i)->setValue(sliderValues.at(i));
            pd400ComLimGainSliders.at(i)->blockSignals(false);
            m_right_view->m_current_labels.at(i)->setText(currentValues.at(i));
        }
        //关闭禁点的按钮
        m_right_view->select_btns.at(0)->setDisabled(true);
        m_right_view->sliders.at(1)->setSliderDisable(true);
        m_right_view->select_btns.at(1)->setDisabled(true);
        m_right_view->sliders.at(2)->setSliderDisable(true);
        //重新刷新各种压缩器，控制器
        xianfuqi_slider_values.replace(0,40);
        xianfuqi_slider_values.replace(1,100);
        xianfuqi_slider_values.replace(2,200);
        m_show_xianfuView->refresh_ui(xianfuqi_slider_values.at(0),xianfuqi_slider_values.at(1),xianfuqi_slider_values.at(2));
        yaxianqi_slider_values.replace(0,80);
        yaxianqi_slider_values.replace(1,100);
        yaxianqi_slider_values.replace(2,200);
        yaxianqi_slider_values.replace(3,1);
        yaxianqi_slider_values.replace(4,1);
        m_show_comp->refresh_ui(yaxianqi_slider_values.at(0),yaxianqi_slider_values.at(2),yaxianqi_slider_values.at(1),
                                yaxianqi_slider_values.at(3),yaxianqi_slider_values.at(4));
        //qDebug()<<"xxxxxxx-------xxx-------------酷酷酷酷酷";
    }
    // qDebug()<<"pcrect-------------------receive_data------"<<array_data<<"------xx"<<QString::number(addr,16);
    //qDebug()<<"xxxxxxx--------------------酷酷酷酷酷";
}
//切换语言
void window_pd400::languge_click(){

    qDebug()<<"yuyuuuuuuuuu------"<<m_current_languge_index;
    m_basic_btn->setText(tr("基础"));
    m_high_btn->setText(tr("高级"));
    if(is_usb_connect){
        QPalette pa;
        pa.setColor(QPalette::WindowText,QColor(255, 255, 255));
        m_product_label->setPalette(pa);
        m_product_label->setText(tr("已连接PD400设备"));
    }else{
        QPalette pa;
        pa.setColor(QPalette::WindowText,QColor(255, 0, 0));
        m_product_label->setPalette(pa);
        m_product_label->setText(tr("请插入USB设备"));
    }
    m_left_view->change_language();
    m_right_view->change_language();
}

void window_pd400::handleTimeout(){

    //qDebug()<<"------酷酷酷酷酷"<<pd400_send_value<<pd400_diffsend_value;
    QMap<QString,int>::iterator send_value_iter = PD400_send_value.find(QString::number(send_radom_data.param_addr[0],16));
    QMap<QString,int>::iterator different_value_iter = PD400_different_value.find(QString::number(send_radom_data.param_addr[0],16));

    //qDebug()<<"11111111111111xxxxxxx酷酷酷酷酷"<<send_value_iter.value()<<different_value_iter.value()<<QString::number(send_radom_data.param_addr[0]);
    if(different_value_iter.value() != send_value_iter.value()){
        if(is_usb_connect)
            //当数据为自己写的时候才发送
            //qDebug()<<"xxxxxxx酷酷酷酷酷"<<send_value_iter.value()<<different_value_iter.value();
            if(send_radom_data.command == 0x03){
                pd400_m_sendMsgs.push_back(send_radom_data);
                //qDebug()<<"xxxxxxx酷酷酷酷酷"<<pd400_send_value;
            }
    }
    PD400_different_value.insert(QString::number(send_radom_data.param_addr[0],16),send_value_iter.value());
}
void window_pd400::disableHandleTimeout(){

    m_basic_btn->setDisabled(false);
    m_high_btn->setDisabled(false);
    m_commin_fun.sys_Software_tag(0);
    m_disableTimer->stop();
}
void window_pd400::isdfu_handleTimeout(){

    send_radom_data = {};
    send_radom_data.report = 0x4b;
    send_radom_data.frame_header = 0xc4;
    send_radom_data.frame_len_L = 0x0A;
    send_radom_data.command = 0x0b;
    send_radom_data.param_addr[0] = 0x01;
    //send_radom_data.param_addr[1] = 0x20;
    send_radom_data.param_value[0] = 2;
    send_radom_data.param_value[1] = 0x24;
    send_radom_data.crc_L = 0xFF;
    //send_radom_data.crc_H = (pd400_crc >> 8) & 0xFF;
    // qDebug()<<"xxxxxxx酷酷酷酷酷";
    if(!is_usb_connect)return;
    // pd400_m_sendMsgs.push_back(send_radom_data);
    //qDebug()<<"xxxxxxx酷酷酷酷酷";
}
//升级固件的方法
void window_pd400::DFU_updata(){

    if(!is_usb_connect)return;

    m_hearbeattimer->stop();
    pm481_devoperator->dfu_send_packNumber = m_check_view->m_dataSourceList.count()-1;
    pm481_devoperator->dfu_send_current_packNumber = 0;
    m_file_datas.clear();

    qDebug()<< m_check_view->m_dataSourceList.count()<<"xxxllllll";
    //QList<QByteArray> my_list_data;
    // m_dfu_thread->m_dfu_dataList = m_check_view->m_dataSourceList;
    QByteArray byte_number;
    byte_number.append(63, char(0));
    byte_number.replace(0,11,m_check_view->m_dataSourceList.at(0));
    byte_number.insert(0,0X4b);
    m_file_datas.push_back(byte_number);

    for (int i=1;i<m_check_view->m_dataSourceList.count();i++) {

        QByteArray send_data = m_check_view->m_dataSourceList.at(i);
        //分成多包发送
        //是否需要补0
        int add_data = send_data.count()%63;
        if(add_data !=0){//后面补0

            for (int j =0;j<(63-add_data);j++) {
                send_data.append(static_cast<char>(0));
            }
        }
        for (int n=0;n<send_data.count()/63;n++) {
            QByteArray send_mess = send_data.mid(n*63,63);
            send_mess.insert(0,0X4b);

            //if(!pm481_devoperator->m_pc_isconnect)return;
            QString str;
            for (int k=0;k<64;k++) {
                uint8_t a = send_mess[k] & 0xff;
                str += " "+QString::number(a,16);
            }
            //qDebug()<<"send_message"<<str;
            m_file_datas.push_back(send_mess);
            //pm481_devoperator->send_updata_DFU_message((unsigned char*)send_mess.data());
        }
        //
    }
    pm481_devoperator->m_dfu_dataList.clear();
    pm481_devoperator->m_dfu_dataList = m_file_datas;

    //判断是否是DSP跟MCU一起升级
    if(is_pd400needto_updatadsp){
        m_progress_view->m_progress_view->setMaximum(m_file_datas.count()+105);
        m_progress_view->m_progress_view->setValue(0);
    }else{
        m_progress_view->m_progress_view->setMaximum(m_file_datas.count());
        m_progress_view->m_progress_view->setValue(0);
    }

    m_progress_view->show();
    //添加4b
}
void window_pd400::starDspTimeout(){

    if(pm481_devoperator->m_pid == currentProPid){
        m_dfu_view->m_pid = pm481_devoperator->m_pid;
        m_dfu_view->startNewExe(m_PD400file_dsp);
    }else{
        dsp_kt0206_finish(-1);
    }
    m_starDspTimer->stop();
}
//单独升级dsp，不升级MCU
void window_pd400::alone_updata_dsp(){

    if(is_pd400needto_updatadsp){//升级DSP
        QFile file(m_PD400file_dsp);
        if(!file.open(QIODevice::ReadOnly)){
            QMessageBox::information(NULL, "Title", "打开错误+"+file.errorString(),
                                     QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
            return;
        }
        m_progress_view->m_progress_view->setValue(0);
        m_progress_view->m_current_value->setText(QString::number(0,10)+"%");
        m_progress_type = PD400DSP;
        m_progress_view->show();
        m_dfu_view->m_pid = pm481_devoperator->m_pid;
        m_dfu_view->startNewExe(m_PD400file_dsp);
    }
}
void window_pd400::dsp_kt0206_progress_val(int value){
    if(m_progress_type == PD400MCU){
       progress_value(value + m_file_datas.count());
    }else{
      m_progress_view->m_progress_view->setMaximum(m_dfu_view->m_total_lengh);
      progress_value(value);
    }
}
void window_pd400::dsp_kt0206_finish(int status){

    m_progress_view->hide();

    //打开所有得蒙版
    device_disconnect_fun();
    m_product_label->setText(tr("请重新连接设备"));
    QString product_label_stys=QString("font-family:'Source Han Sans CN Medium';text-align:right;color:red;font-size:%1px;").arg(15*m_Magnification);
    m_product_label->setStyleSheet(product_label_stys);

    if(status ==1){//成功    
        QMessageBox msg;
        msg.setWindowTitle(tr("提示"));
        msg.setText(tr("升级成功,请重新连接设备"));
        QPushButton *noButton = msg.addButton(tr("是"),QMessageBox::ActionRole);
        msg.addButton(QMessageBox::No);
        msg.button(QMessageBox::No)->setHidden(true);
        msg.setDefaultButton(noButton);
        msg.exec();
    }else{//失败     
        QMessageBox msg;
        msg.setWindowTitle(tr("提示"));
        msg.setText(tr("升级失败"));
        QPushButton *noButton = msg.addButton(tr("是"),QMessageBox::ActionRole);
        msg.addButton(QMessageBox::No);
        msg.button(QMessageBox::No)->setHidden(true);
        msg.setDefaultButton(noButton);
        msg.exec();
    }
    //QThread::msleep(1000);
    is_pd400updata_runing =false;
    back_view_click();


}
void window_pd400::progress_value(int current_value){


    switch (m_progress_type) {
    case PD400MCU:{

        if(is_pd400needto_updatadsp){
            m_progress_view->m_progress_view->setValue(current_value);
            int value =static_cast<int>((static_cast<double>(current_value) / static_cast<double>(m_file_datas.count()+105))*100) ;
            m_progress_view->m_current_value->setText(QString::number(value,10)+"%");
        }else{
            m_progress_view->m_progress_view->setValue(current_value);
            int value =static_cast<int>((static_cast<double>(current_value) / static_cast<double>(m_file_datas.count()))*100) ;
            //qDebug()<<"sxxxxxxxxxxxxxxxend_message"<<value<<current_value<<m_file_datas.count();
            m_progress_view->m_current_value->setText(QString::number(value,10)+"%");
        }

    }
        break;
    case PD400DSP:{
        m_progress_view->m_progress_view->setValue(current_value);
        int value =static_cast<int>((static_cast<double>(current_value) / static_cast<double>(m_dfu_view->m_total_lengh))*100) ;
        qDebug()<<current_value <<m_dfu_view->m_total_lengh<<value;
        m_progress_view->m_current_value->setText(QString::number(value,10)+"%");
    }
        break;
    default:
        {

        }
    }
}
//固件升级失败了
void window_pd400::sendMessage_fail_hand(int status){

    m_hearbeattimer->stop();
    QString connect_str;
    if(status==1){
        m_progress_view->hide();
        is_pd400updata_runing =false;
        //打开所有得蒙版
        device_disconnect_fun();
        m_product_label->setText(tr("请重新连接设备"));
        QString product_label_stys=QString("font-family:'Source Han Sans CN Medium';color:red;font-size:%1px;").arg(15*m_Magnification);
        m_product_label->setStyleSheet(product_label_stys);
        QMessageBox::information(NULL, tr("提示"), tr("升级失败"),QMessageBox::Yes);
        back_view_click();

    }else if(status==2){
        dsp_updata();
    }else{

        m_progress_view->hide();
        is_pd400updata_runing =false;
        //打开所有得蒙版
        device_disconnect_fun();
        if(is_resetFaocClick){
           m_product_label->setText(tr("已恢复出厂设置，请重新连接设备"));
        }else{
          m_product_label->setText(tr("设备连接失败,请重新连接"));
        }

        QString product_label_stys=QString("font-family:'Source Han Sans CN Medium';color:red;font-size:%1px;").arg(15*m_Magnification);
        m_product_label->setStyleSheet(product_label_stys);
        //QMessageBox::information(NULL, tr("提示"), connect_str + QString::number(status,10),QMessageBox::Yes);
    }
}
void window_pd400::dsp_updata(){

    qDebug()<<"xkxkxkxkxk"<<m_PD400file_dsp<<is_pd400needto_updatadsp;
    //判断是否需要升级
    if(is_pd400needto_updatadsp){//升级DSP
        QFile file(m_PD400file_dsp);
        if(!file.open(QIODevice::ReadOnly)){
            QMessageBox::information(NULL, "Title", "打开错误+"+file.errorString(),
                                     QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
            return;
        }
        m_starDspTimer->start(2000);

    }else{//不需要升级DSP

        //打开所有得蒙版
        device_disconnect_fun();
        m_product_label->setText(tr("请重新连接设备"));
        QString product_label_stys=QString("font-family:'Source Han Sans CN Medium';color:red;font-size:%1px;").arg(15*m_Magnification);
        m_product_label->setStyleSheet(product_label_stys);
        m_progress_view->hide();

        QMessageBox msg;
        msg.setWindowTitle(tr("提示"));
        msg.setText(tr("升级成功"));
        QPushButton *noButton = msg.addButton(tr("是"),QMessageBox::ActionRole);
        msg.addButton(QMessageBox::No);
        msg.button(QMessageBox::No)->setHidden(true);
        msg.setDefaultButton(noButton);
        msg.exec();

        QThread::msleep(1000);
        is_pd400updata_runing =false;
    }
}
void window_pd400::maingetModelCodel(){

    //获取modelcode
    QString modelcodel_str = "";

    for (int i=0;i<6;i++) {
        modelcodel_str +=  QChar(m_modelCodelName.at(0).buff[10+i]);
    }

    if(modelcodel_str==PD400MODELCODE){//为当前的设备
        is_usb_connect =true;//连接成功
        //清空数组
        m_receive_pcParMsgs.clear();
        m_modelCodelName.clear();
        //判断DFU模式
        m_product_label->setText(tr("同步设备参数中..."));
        m_commin_fun.is_dfuStatus();
    }else{
        QMessageBox::information(NULL, tr("提示"), modelcodel_str + "获取model失败",QMessageBox::Yes);
    }
}
void window_pd400::isDfuStatus(int isDfu){
    if(isDfu){//Dfu模式
        m_product_label->setText(tr("设备为升级模式"));
        QString product_label_stys=QString("font-family:'Source Han Sans CN Medium';color:white;font-size:%1px;").arg(15*m_Magnification);
        m_product_label->setStyleSheet(product_label_stys);

    }else{//APP模式下
        //发送同步标志
        m_commin_fun.sys_Software_tag(1);
        is_usb_connect =true;
        pm481_devoperator->pd400_get_bgdata.clear();
        if(pm481_devoperator->pd400_get_bgdata.count()<=0)
            m_commin_fun.bg_datasource();
    }
}
void window_pd400::setSliderDisabled(bool status){

    for (int i=0;i<m_synmicvolume_Slider_arrs.count();i++) {

        MySlider *mic_volue_slider = m_synmicvolume_Slider_arrs.at(i);
        mic_volue_slider->setDisabled(status);
    }
    for (int i=0;i<m_pd400SynSlider_arrs.count();i++) {

        SliderWidget *mix_volue_slider = m_pd400SynSlider_arrs.at(i);
        mix_volue_slider->setDisabled(status);
    }
    for (int i=0;i<pd400ComLimGainSliders.count();i++) {
        MySlider *pd400_rightslider = pd400ComLimGainSliders.at(i);
        pd400_rightslider->setDisabled(status);
    }
}
//心跳包
void window_pd400::hearbeatTimeout(){

    //先判心跳包有没数据先
    if(m_receive_pcParMsgs.count()<=0)
    //发送数据
    m_commin_fun.hearbeat_fun();
}
//恢复出厂设置
void window_pd400::reset_action(){

    m_hearbeattimer->stop();
    m_setting_btn->setCheckState(Qt::Unchecked);
    m_setting_view->hide();

    //恢复出厂设置
    pm481_devoperator->resetFactorySetting();


    //切换提示语 提示重新插入设备
    is_resetFaocClick =true;

    //蒙版开启
    m_left_view->m_mengban_image->show();
    m_right_view->m_mengban_image->show();
    m_basic_btn->setDisabled(true);
    m_basic_btn->setChecked(false);
    m_high_btn->setDisabled(true);
    m_high_btn->setChecked(false);
    setSliderDisabled(true);

    m_product_label->setText(tr("已恢复出厂设置，请重新连接设备"));
    QString product_label_stys=QString("font-family:'Source Han Sans CN Medium';color:red;font-size:%1px;").arg(15*m_Magnification);
    m_product_label->setStyleSheet(product_label_stys);

    QThread::msleep(500);

    m_hearbeattimer->stop();
    m_disableTimer->stop();
    is_usb_connect =false;
    is_modelconnect = false;
    pm481_devoperator->m_PM481Request_device_datas.clear();
    m_pd400_device_datas.clear();
    pd400_m_sendMsgs.clear();
    m_receive_pcParMsgs.clear();
    m_modelCodelName.clear();
    m_product_label->setText(tr("请插入USB设备"));
    m_product_label->setStyleSheet(product_label_stys);

    //蒙版开启
    m_left_view->m_mengban_image->show();
    m_right_view->m_mengban_image->show();

    m_basic_btn->setDisabled(true);
    m_basic_btn->setChecked(false);
    m_high_btn->setDisabled(true);
    m_high_btn->setChecked(false);
    setSliderDisabled(true);

    pm481_devoperator->m_isreceiveSem.release();


    is_modelconnect = true;

    //获取背景参数之前先清除原有的
    m_commin_fun.getModelCode();



}
//不加换了EXE文件没法打开文件
void window_pd400::DM30_timer_outfun(){
    emit dfuReadFileData();
    qDebug()<<"xxpppppppppppppp";
}
window_pd400::~window_pd400(){

    delete m_show_xianfuView;
    delete m_show_comp;
    delete m_pTimer;
    m_pTimer = NULL;

    m_dfu_thread->m_ret = false;
    m_dfu_thread->quit();
    m_dfu_thread->wait();

}


