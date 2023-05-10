#include "mainwindow.h"

#include <QApplication>

#include "MBS_400/window_mbs400.h"
#include "PD400/window_pd400.h"
#include "PM481/window_pm481.h"

#include "HID/hidapi.h"

#include <QSharedMemory>
#include "common.h"
//#include "windows.h"

#include <QScreen>


//#pragma comment(lib,"C:/VLD/Visual Leak Detector/lib/Win32/vld.lib")
//#ifdef _DEBUG
//#include "C:/VLD/Visual Leak Detector/include/vld.h"
//#endif


//HINSTANCE hUser32 = LoadLibrary(L"User32.dll");

int main(int argc, char *argv[])
{
    //单开
    QSharedMemory shared("maonolink");//随便填个名字就行
    if (shared.attach())
    {
        return 0;
    }
    shared.create(1);
    //屏幕自适应的办法
    //    if(QT_VERSION>=QT_VERSION_CHECK(5,6,0))
    //             QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    //    if (hUser32)
    //     {
    //            typedef BOOL (WINAPI* LPSetProcessDPIAware)(void);
    //            LPSetProcessDPIAware pSetProcessDPIAware = (LPSetProcessDPIAware)GetProcAddress(hUser32, "SetProcessDPIAware");
    //            if (pSetProcessDPIAware)
    //            {
    //                pSetProcessDPIAware();
    //            }
    //            FreeLibrary(hUser32);
    //    }

    QApplication a(argc, argv);


    QList<QScreen *> list_screen = QGuiApplication::screens();
    // 通过循环可以遍历每个显示器
    //    for (int i = 0; i < list_screen.size(); i++)
    //    {
    QRect rect = list_screen.at(0)->geometry();
    int desktop_width = rect.width();
    int desktop_height = rect.height();



#if defined(Q_OS_WIN32)

    if(desktop_height>=1080 && desktop_height<1536){
        m_Magnification =1.0;
    }else if(desktop_height>1536){
        m_Magnification =2.0;
    }else if(desktop_height<1080){
        m_Magnification =0.8;
    }else{
        m_Magnification =1.5;
    }
#else
    qreal dpiVal1 = list_screen.at(0)->logicalDotsPerInch();

            if(dpiVal1 ==96){//100%

                m_Magnification =1.0;
            }else if(dpiVal1 ==120){//125%

                m_Magnification =1.25;
            }else if(dpiVal1 ==144){//150%
                m_Magnification =1.5;

            }else if(dpiVal1 ==168){//175%

                m_Magnification =1.75;
            }else if(dpiVal1 ==192){//200%

                m_Magnification =2.0;
            }else if(dpiVal1 ==216){//225%
                m_Magnification =2.25;

            }else if(dpiVal1 ==240){//250%
                m_Magnification =2.5;

            }else if(dpiVal1 ==288){//300%
                m_Magnification =3.0;

            }
    // 打印屏幕分辨率
    qDebug() << desktop_width <<desktop_height<<dpiVal1;
#endif



    //    }

    MainWindow *w;
    w = new MainWindow();
    //w->setWindowTitle("M Link");
    w->show();
    return a.exec();
}
