#ifndef WEBWIDGET_H
#define WEBWIDGET_H

#include <QWidget>
#include <qnetworkrequest.h>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QRegularExpression>
#include <QLineEdit>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMovie>
#include <QTimer>
#include <QMessageBox>
#include <QScrollBar>
#include <QListWidget>
#include <QListWidgetItem>
#include <QFileDialog>
#include <QSystemTrayIcon>
#include <QProgressBar>
#include <QStatusBar>
#include <QKeyEvent>
#include "loadingwidget.h"
#include <QBuffer>

#define TimeOut 10000

struct WallpaperData {
    QString url;//源链接
    QString thumbnail;//缩略图
    QString size;//尺寸
    bool isPng;//是否为png
};


class WebWidget : public QWidget
{
    Q_OBJECT
public:
    explicit WebWidget(QWidget *parent = nullptr);
    ~WebWidget();
    void iniWebWallpaperData(QString url);
protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
signals:



private slots:
    void updateProgressBar(qint64 bytesReceived, qint64 bytesTotal);
    void searchPaperUpDate();

private:
    QStringList PicUrlList;
    QList<WallpaperData> wallpaperList;
    QPushButton *updateBtn;
    //QLineEdit *TempText;
    //QPushButton *findBtn;
    QVBoxLayout *MainLayout;
    QVBoxLayout *imglayout;
    QPushButton* firstpage ;
    QPushButton* lastpage;
    QPushButton* nextpage;
    QHBoxLayout *pageBtnLayout;
    LoadingWidget *loadingWidget = nullptr;
    QLabel *curPage;
    QNetworkAccessManager *manager;
    QNetworkReply *m_reply ;
    QProgressBar* m_progressBar;
    QString searchkeyWord = "";
    QString m_HtmlData;
    uint page = 1;
    void upDateWidget();
    void readDataSlot();
    void readFinishedSlot();
    QString url2imgurl(QString surl);
    void getPictureLinkFromHTML(QString htmlText);
    void getPicLinkFromHTML(QString htmlText);
    void deleteItem(QLayout *layout);
    void downLoadpic(QString url);
    bool downLoadLanzou(QString url);
    bool downLoad360(QString url);
    bool downLoadBaidu(QString url);
};

#endif // WEBWIDGET_H
