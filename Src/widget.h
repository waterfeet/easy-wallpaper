#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

#include <QMenu>
#include <QMovie>
#include <QTimer>
#include <qlabel.h>
#include <QSettings>
#include <windows.h>
#include <QFileDialog>
#include <QVideoWidget>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QSystemTrayIcon>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QFileInfo>
#include <QDir>
#include <QApplication>
#include <Qdesktopservices>
#include "SwitchButton.h"
#include "webwidget.h"


#define  columnCount   2
#define  rowCount   5
#define AUTO_RUN "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run"

class MainWidget : public QWidget
{
    Q_OBJECT
public:
    MainWidget(QWidget *parent = nullptr);
    ~MainWidget();

    void InitWidget();
    void InitPaper();
    HWND winwallpaper();
    HWND winwallpaper_send();
    static inline BOOL CALLBACK enumwin(HWND han,LPARAM lp);
    void mp4(QString fine); //显示视频
    void gif(QString fine); //显示图片
    //最小化托盘信号处理
    void sys(QSystemTrayIcon::ActivationReason r);

    QSystemTrayIcon* system;
    QLabel* label;
    QVideoWidget* vid;
    QMediaPlayer* med;
    QAudioOutput* aud = new QAudioOutput;

    //使用此类记录使用状态
    QSettings* ConfigFile;

//    void f(QGridLayout* layout);
private:
    void saveHistory(const QString path);
    void setHistoryBox();
    void setProcessAutoRun(const QString &appPath,bool flag);
    bool isAutoRun(const QString &appPath);
    void loadhistorypaper(int index);
    void deleteItem(QLayout *layout);
    QStringList loadHistory();
    QPushButton *gifButton;
    QPushButton *mp4Button;
    QPushButton *closeapp;
    QPushButton *WebOpenBtn;
    SwitchButton *BGMcheckBtn;
    SwitchButton *AutoBootcheckBtn;
    QHBoxLayout *MAinLayout;
    QVBoxLayout *MainLeftLayout;
    QAction* _show;
    QAction* _clos;
    QGroupBox* HistoryBox;
    QGridLayout* gridLayout;
    QMovie* movie;
    WebWidget* m_webwidget;
public slots:
    void OnGifBtnClicked();
    void OnMP4BtnClicked();
    void OnWebOpenBtnClicked();
    void MinWidget();
};
#endif // WIDGET_H
