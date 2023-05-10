QT       += core gui charts multimedia
QT  += core gui svg

CONFIG += resources_big

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

QT += network

TARGET = "Maono Link"
# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS


# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    DM90/audio_recoder/playaudio.cpp \
    DM90/audio_recoder/recoder_wavelineview.cpp \
    DM90/audio_recoder/recoderaudio.cpp \
    DM90/customView/custom_item.cpp \
    DM90/customView/custom_itemlistview.cpp \
    DM90/customView/custom_lineeditview.cpp \
    DM90/customView/custom_listview.cpp \
    DM90/customView/custom_loadingview.cpp \
    DM90/customView/dm90_screenshotdelegate.cpp \
    DM90/customView/loadingwidget.cpp \
    DM90/dm90_custom_switch.cpp \
    DM90/dm90_devicedatasource.cpp \
    DM90/dm90_higheqview.cpp \
    DM90/dm90_highview.cpp \
    DM90/dm90_leftview.cpp \
    DM90/dm90_lightrgbimage.cpp \
    DM90/dm90_lightview.cpp \
    DM90/dm90_midview.cpp \
    DM90/dm90_receivemessage_thread.cpp \
    DM90/dm90_recoderview.cpp \
    DM90/dm90_rightview.cpp \
    DM90/signalProce/dm90_compressorview.cpp \
    DM90/signalProce/dm90_dentalsoundview.cpp \
    DM90/signalProce/dm90_detonatorview.cpp \
    DM90/signalProce/dm90_flangerview.cpp \
    DM90/signalProce/dm90_limiterview.cpp \
    DM90/signalProce/dm90_thresholdview.cpp \
    DM90/signalProce/dm90_volumeeqview.cpp \
    DM90/signalinstance.cpp \
    DM90/test.cpp \
    DM90/tone_environment/dm90_delayview.cpp \
    DM90/tone_environment/dm90_reverbview.cpp \
    DM90/tone_environment/dm90_toneview.cpp \
    DM90/window_dm90.cpp \
    DM90/yy_custombutton.cpp \
    HID/FirmwareUpgradeProtocol.cpp \
    HID/utils/CRC.cpp \
    HID/utils/CcmEasyEncrypt.cpp \
    HID/utils/HidBase.cpp \
    HID/utils/Protocol.cpp \
    HID/windows/hid.c \
    MBS_400/basewindow.cpp \
    MBS_400/mbs400_back_leftview.cpp \
    MBS_400/mbs400_back_midview.cpp \
    MBS_400/mbs400_back_rightview.cpp \
    MBS_400/mycustom_button.cpp \
    MBS_400/myslider.cpp \
    MBS_400/myspinbutton.cpp \
    MBS_400/mytitlebar.cpp \
    MBS_400/qhorslider.cpp \
    MBS_400/qverslider.cpp \
    MBS_400/switchcontrol.cpp \
    MBS_400/window_mbs400.cpp \
    PD400/childthread.cpp \
    PD400/dfu_thread.cpp \
    PD400/pd400_check_updateview.cpp \
    PD400/pd400_leftview.cpp \
    PD400/pd400_progress_view.cpp \
    PD400/pd400_rightview.cpp \
    PD400/pd400_setting_view.cpp \
    PD400/pd400_xianzhiqi_view.cpp \
    PD400/pd400_yasuoqi_view.cpp \
    PD400/sliderwidget.cpp \
    PD400/window_pd400.cpp \
    PM481/pm481_class/custommainbutton.cpp \
    PM481/pm481_common.cpp \
    PM481/pm481_devicedatasource.cpp \
    PM481/pm481_leftview.cpp \
    PM481/pm481_rightview.cpp \
    PM481/pm481sendreceiveclass.cpp \
    PM481/window_pm481.cpp \
    common.cpp \
    common_funtion.cpp \
    common_newwork_class.cpp \
    custom_button.cpp \
    custom_checkbox.cpp \
    customscrollview.cpp \
    devoperator.cpp \
    dfu_0206_view.cpp \
    header_class.cpp \
    main.cpp \
    mainsubview.cpp \
    mainwindow.cpp \
    msgbox.cpp \
    mypushbutton.cpp \
    networking_class.cpp \
    send_receive_massage_control.cpp \
    titleBar/mainbasewindow.cpp \
    titleBar/maintitlebar.cpp

HEADERS += \
    DM90/audio_recoder/playaudio.h \
    DM90/audio_recoder/recoder_wavelineview.h \
    DM90/audio_recoder/recoderaudio.h \
    DM90/customView/custom_item.h \
    DM90/customView/custom_itemlistview.h \
    DM90/customView/custom_lineeditview.h \
    DM90/customView/custom_listview.h \
    DM90/customView/custom_loadingview.h \
    DM90/customView/dm90_screenshotdelegate.h \
    DM90/customView/loadingwidget.h \
    DM90/dm90_custom_switch.h \
    DM90/dm90_default_datas.hpp \
    DM90/dm90_devicedatasource.h \
    DM90/dm90_higheqview.h \
    DM90/dm90_highview.h \
    DM90/dm90_leftview.h \
    DM90/dm90_lightrgbimage.h \
    DM90/dm90_lightview.h \
    DM90/dm90_midview.h \
    DM90/dm90_receivemessage_thread.h \
    DM90/dm90_recoderview.h \
    DM90/dm90_rightview.h \
    DM90/loadingwidget.h \
    DM90/signalProce/dm90_compressorview.h \
    DM90/signalProce/dm90_dentalsoundview.h \
    DM90/signalProce/dm90_detonatorview.h \
    DM90/signalProce/dm90_flangerview.h \
    DM90/signalProce/dm90_limiterview.h \
    DM90/signalProce/dm90_thresholdview.h \
    DM90/signalProce/dm90_volumeeqview.h \
    DM90/signalinstance.h \
    DM90/test.h \
    DM90/tone_environment/dm90_delayview.h \
    DM90/tone_environment/dm90_reverbview.h \
    DM90/tone_environment/dm90_toneview.h \
    DM90/window_dm90.h \
    DM90/yy_custombutton.h \
    HID/FirmwareUpgradeProtocol.h \
    HID/firemware_data.hpp \
    HID/hidapi.h \
    HID/utils/CRC.h \
    HID/utils/CcmEasyEncrypt.h \
    HID/utils/CommunicateInterface.h \
    HID/utils/DeviceManager.h \
    HID/utils/HidBase.h \
    HID/utils/Protocol.h \
    MBS_400/basewindow.h \
    MBS_400/mbs400_back_leftview.h \
    MBS_400/mbs400_back_midview.h \
    MBS_400/mbs400_back_rightview.h \
    MBS_400/mycustom_button.h \
    MBS_400/myslider.h \
    MBS_400/myspinbutton.h \
    MBS_400/mytitlebar.h \
    MBS_400/qhorslider.h \
    MBS_400/qverslider.h \
    MBS_400/switchcontrol.h \
    MBS_400/window_mbs400.h \
    PD400/childthread.h \
    PD400/dfu_thread.h \
    PD400/pd400_check_updateview.h \
    PD400/pd400_leftview.h \
    PD400/pd400_progress_view.h \
    PD400/pd400_rightview.h \
    PD400/pd400_setting_view.h \
    PD400/pd400_xianzhiqi_view.h \
    PD400/pd400_yasuoqi_view.h \
    PD400/sliderwidget.h \
    PD400/window_pd400.h \
    PM481/pm481_class/custommainbutton.h \
    PM481/pm481_common.h \
    PM481/pm481_devicedatasource.h \
    PM481/pm481_leftview.h \
    PM481/pm481_rightview.h \
    PM481/pm481sendreceiveclass.h \
    PM481/window_pm481.h \
    common.h \
    common_funtion.h \
    common_newwork_class.h \
    custom_button.h \
    custom_checkbox.h \
    customscrollview.h \
    devoperator.h \
    dfu_0206_view.h \
    header_class.h \
    mainsubview.h \
    mainwindow.h \
    msgbox.h \
    mypushbutton.h \
    networking_class.h \
    send_receive_massage_control.h \
    titleBar/mainbasewindow.h \
    titleBar/maintitlebar.h

FORMS += \
    mainwindow.ui


TRANSLATIONS += pd400_lang/pd400_zh.ts \
               pd400_lang/pd400_en.ts
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    image.qrc \
    languages.qrc \
    screen.qrc

DISTFILES +=

RC_FILE=logo.rc

#LIBS += -framework Cocoa


