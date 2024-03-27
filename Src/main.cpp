#include "widget.h"
#include <QApplication>
#include <QMainWindow>
//using namespace std;
#include <windows.h>
#include <QDebug>
#include <QScreen>
#include <WinUser.h>
#include <iostream>
#include <qpixmap.h>

double calculateScaleFactor()
{
    auto activeWindow = GetActiveWindow();
    HMONITOR monitor = MonitorFromWindow(activeWindow, MONITOR_DEFAULTTONEAREST);

    // Get the logical width and height of the monitor
    MONITORINFOEX monitorInfoEx;
    monitorInfoEx.cbSize = sizeof(monitorInfoEx);
    GetMonitorInfo(monitor, &monitorInfoEx);
    auto cxLogical = monitorInfoEx.rcMonitor.right - monitorInfoEx.rcMonitor.left;
    auto cyLogical = monitorInfoEx.rcMonitor.bottom - monitorInfoEx.rcMonitor.top;

    // Get the physical width and height of the monitor
    DEVMODE devMode;
    devMode.dmSize = sizeof(devMode);
    devMode.dmDriverExtra = 0;
    EnumDisplaySettings(monitorInfoEx.szDevice, ENUM_CURRENT_SETTINGS, &devMode);
    auto cxPhysical = devMode.dmPelsWidth;
    auto cyPhysical = devMode.dmPelsHeight;

    // Calculate the scaling factor
    double horizontalScale = ((double)cxPhysical / (double)cxLogical);
    double verticalScale = ((double)cyPhysical / (double)cyLogical);

    std::cout << "Horizonzal scaling: " << horizontalScale << "\n";
    std::cout << "Vertical scaling: " << verticalScale;

    return horizontalScale > verticalScale ? verticalScale : horizontalScale;
}


int main(int argc, char *argv[])
{
//    double scaleFactor = calculateScaleFactor();
    // enable QIcon produces highdpi pixmap
//    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

//    qputenv("QT_SCALE_FACTOR", QByteArray::number(scaleFactor));

    QApplication a(argc, argv);
    QDir::setCurrent(QCoreApplication::applicationDirPath());
    qDebug()<<QCoreApplication::applicationDirPath();

    MainWidget w;
    w.show();

    return a.exec();
}




//static void HighDpiAdapt()
//{
//    // 获取当前显示器的数目
//    int numbers = GetSystemMetrics(SM_CMONITORS);
//    string scale_name = "";
//    for (int i = 0; i < numbers; ++i)
//    {
//        DISPLAY_DEVICEW device;
//        device.cb = sizeof(device);
//        BOOL result = EnumDisplayDevicesW(NULL, i, &device, 0);
//        DEVMODEW device_mode;
//        device_mode.dmSize = sizeof(device_mode);
//        device_mode.dmDriverExtra = 0;
//        result = EnumDisplaySettingsExW(device.DeviceName, ENUM_CURRENT_SETTINGS, &device_mode, 0);
//        string screen_name = WStringToAnsiString(device.DeviceName);
//        scale_name += screen_name;
//        scale_name += (device_mode.dmPelsWidth > 1920) ? "=1.5;" : "=1;";  //设置不同的缩放比例系数
//    }
//    scale_name[scale_name.size() - 1] = '\0';
//    qputenv("QT_SCREEN_SCALE_FACTORS", scale_name.c_str());
//    //处理图像模糊问题
//    QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
//    //禁止高缩放支持
//    //QGuiApplication::setAttribute(Qt::AA_DisableHighDpiScaling);   //AA_EnableHighDpiScaling
//    //程序保持默认的尺寸，不缩放
//    QGuiApplication::setAttribute(Qt::AA_Use96Dpi);
//}
