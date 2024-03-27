#include "widget.h"

int if_sum = 1;
HWND win;

MainWidget::MainWidget(QWidget *parent)
:QWidget(parent)
{
    this->setWindowTitle("动态壁纸");
    this->setWindowIcon(QIcon("../image/tubiao.ico"));
    setObjectName("Mainwindow");
    setMinimumSize(328,648);//这两个数字好像很耳熟
    setMaximumSize(GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN));
    ConfigFile = new QSettings("../Cfg/config.cfg",QSettings::IniFormat);
    system = new QSystemTrayIcon(this);
    gifButton = new QPushButton("载入图片",this);
    mp4Button = new QPushButton("载入视频",this);
    _show = new QAction("显示",this);
    _clos = new QAction("退出",this);
    BGMcheckBtn = new SwitchButton(this);
    AutoBootcheckBtn = new SwitchButton(this);
    WebOpenBtn = new QPushButton("获取壁纸",this);
    closeapp = new QPushButton("隐藏到任务栏",this);
    HistoryBox = new QGroupBox(this);
    HistoryBox->setTitle("历史记录");

    connect(gifButton,&QPushButton::clicked,this,&MainWidget::OnGifBtnClicked);
    connect(mp4Button,&QPushButton::clicked,this,&MainWidget::OnMP4BtnClicked);
    connect(WebOpenBtn,&QPushButton::clicked,this,&MainWidget::OnWebOpenBtnClicked);
    connect(_show,&QAction::triggered,this,[=](){
        this->show();
    });
    connect(_clos,&QAction::triggered,this,[=](){
        label->close();
        vid->close();
        this->close();
    });
    connect(closeapp,&QPushButton::clicked,this,&MainWidget::MinWidget);
    connect(system,&QSystemTrayIcon::activated,this,&MainWidget::sys);
    connect(BGMcheckBtn,&SwitchButton::statusChanged,this,[=](){
        aud->setMuted(!BGMcheckBtn->checked());
        ConfigFile->setValue("cfg/壁纸音乐",BGMcheckBtn->checked()?"开启":"关闭");
    });
    connect(AutoBootcheckBtn,&SwitchButton::statusChanged,this,[=](){
        setProcessAutoRun(QApplication::applicationFilePath(),AutoBootcheckBtn->checked());
        ConfigFile->setValue("cfg/开机自启动",AutoBootcheckBtn->checked()?"开启":"关闭");
    });
    //使用上一次关闭时的参数初始化程序
    BGMcheckBtn->setChecked(ConfigFile->value("cfg/壁纸音乐") == "开启");
    InitPaper();
    AutoBootcheckBtn->setChecked(ConfigFile->value("cfg/开机自启动")=="开启");
    InitWidget();
}

MainWidget::~MainWidget()
{
}

void MainWidget::InitWidget()
{
    MAinLayout = new QHBoxLayout(this);
    MAinLayout->setContentsMargins(0, 0, 0, 0);

    QWidget *LeftWidget = new QWidget;
    LeftWidget->setMaximumWidth(328);
    MainLeftLayout = new QVBoxLayout();
//    MainLeftLayout->setContentsMargins(0, 0, 0, 0);
    LeftWidget->setLayout(MainLeftLayout);
    QHBoxLayout* btnLayout = new QHBoxLayout();
    QHBoxLayout* checkLayout = new QHBoxLayout();
    setLayout(MAinLayout);
//    MAinLayout->addLayout(MainLeftLayout);
    MAinLayout->addWidget(LeftWidget);
    MainLeftLayout->addLayout(btnLayout);
    btnLayout->setSpacing(20);
    btnLayout ->addWidget(gifButton);
    btnLayout->addWidget(mp4Button);
    btnLayout->addWidget(closeapp);
    btnLayout->addWidget(WebOpenBtn);
    checkLayout->addWidget(new QLabel("BGM:"));
    checkLayout->addWidget(BGMcheckBtn);
    checkLayout->addSpacing(20);
    checkLayout->addWidget(new QLabel("开机自启动:"));
    checkLayout->addWidget(AutoBootcheckBtn);
    MainLeftLayout->addLayout(checkLayout);
//    BGMcheckBtn->setMinimumSize(50,20);
    MainLeftLayout->addWidget(HistoryBox);
    gridLayout = new QGridLayout;
    HistoryBox->setLayout(gridLayout);


    QFile qss("../Qss/MainWidgetStyle.qss");
    if (qss.open(QFile::ReadOnly)){
        setStyleSheet( qss.readAll());
        qss.close();
    }
    setHistoryBox();
}

void MainWidget::InitPaper()
{
    if(ConfigFile->value("cfg/MP4") == "正在使用"){
        QString fine = ConfigFile->value("cfg/MP4视频路径").toString();
        winwallpaper_send();
        EnumWindows(enumwin,(LPARAM)NULL);
        ShowWindow(win,SW_HIDE);
        vid = new QVideoWidget;
        med = new QMediaPlayer;
        med->setVideoOutput(vid);
        vid->setWindowState(Qt::WindowMaximized);
        vid->setWindowFlags(Qt::FramelessWindowHint);
        vid->showFullScreen();
    //    ConfigFile->setValue("cfg/GIF","暂停使用");
        med->setSource(QUrl::fromLocalFile(fine));
        med->setLoops(QMediaPlayer::Infinite);
        aud->setMuted(true);
        med->setAudioOutput(aud);
        if(ConfigFile->value("cfg/壁纸音乐") == "开启"){
            aud->setMuted(false);
        }
        med->play();
        SetParent((HWND)vid->winId(),winwallpaper());
        vid->show();
    }
    if(ConfigFile->value("cfg/GIF") == "正在使用"){
        QString fine = ConfigFile->value("cfg/GIF图片路径").toString();
        winwallpaper_send();
        EnumWindows(enumwin,(LPARAM)NULL);
        ShowWindow(win,SW_HIDE);
        label = new QLabel;
        movie = new QMovie;
        if(fine.endsWith(".gif")){

            movie->setFileName(fine);
            label->setMovie(movie);
            movie->start();

        }else{
            QPixmap pixmap(fine);
            label->setPixmap(pixmap);
            // 获取屏幕的缩放比例
            int scaleFactor = qApp->screens().at(0)->logicalDotsPerInch() / 96;
            // 调整图片的大小和分辨率
            pixmap = pixmap.scaled(pixmap.width() * scaleFactor, pixmap.height() * scaleFactor, /*Qt::IgnoreAspectRatio*/Qt::KeepAspectRatio, Qt::SmoothTransformation);
        }
        label->setWindowState(Qt::WindowMaximized);
        label->setWindowFlags(Qt::FramelessWindowHint);
        label->setScaledContents(true);

        SetParent((HWND)label->winId(),winwallpaper());
        label->show();
    }
}

/*
 * 下方三个方法都是实现壁纸的关键
 * 1.获取桌面最顶层窗口句柄
 * 2.发送信号至桌面最顶层窗口
 * 3.遍历WorkerW窗口
 */

HWND MainWidget::winwallpaper()
{
    HWND hwnd = ::FindWindow(L"Progman",NULL);
    return hwnd;
}

HWND MainWidget::winwallpaper_send()
{
    int result;

    HWND hwnd = ::FindWindow(L"Progman",NULL);
    SendMessageTimeout(hwnd,0x52c,0,0,SMTO_NORMAL,0x3e8,(PDWORD_PTR)&result);

    return hwnd;
}

WINBOOL MainWidget::enumwin(HWND han, LPARAM lp)
{
    HWND def = ::FindWindowEx(han,0,L"SHELLDLL_DefView",NULL);
    if(def!= 0)
    {
        win = FindWindowEx(0,han,L"WorkerW",0);
    }
    return true;
}

//关键代码！将窗口至于桌面最顶层窗口之下，随后播放多媒体
void MainWidget::mp4(QString fine)
{
    winwallpaper_send();

    EnumWindows(enumwin,(LPARAM)NULL);
    ShowWindow(win,SW_HIDE);
   if(ConfigFile->value("cfg/GIF") == "正在使用"){
        label->close();
        delete label;
        delete movie;
    }
    else if(ConfigFile->value("cfg/MP4") == "正在使用"){
        vid->close();
        delete vid;
        delete med;

    }
    vid = new QVideoWidget;
    med = new QMediaPlayer;
    med->setVideoOutput(vid);
    vid->setWindowState(Qt::WindowMaximized);
    vid->setWindowFlags(Qt::FramelessWindowHint);
    vid->showFullScreen();
    med->setSource(QUrl::fromLocalFile(fine));
    ConfigFile->setValue("cfg/MP4","正在使用");
    ConfigFile->setValue("cfg/MP4视频路径",fine);
    ConfigFile->setValue("cfg/GIF","暂停使用");

    med->setLoops(QMediaPlayer::Infinite);
    aud->setMuted(true);
    med->setAudioOutput(aud);

    if(ConfigFile->value("cfg/壁纸音乐") == "开启"){
        aud->setMuted(false);
    }
    med->play();
    SetParent((HWND)vid->winId(),winwallpaper());
    vid->show();
}

//关键代码！将窗口至于桌面最顶层窗口之下，随后显示图片
void MainWidget::gif(QString fine)
{
    winwallpaper_send();
    EnumWindows(enumwin,(LPARAM)NULL);
    ShowWindow(win,SW_HIDE);
   if(ConfigFile->value("cfg/GIF") == "正在使用")
   {
       label->close();
       delete label;
       delete movie;
   }
   else if(ConfigFile->value("cfg/MP4") == "正在使用")
   {
       vid->close();
       delete vid;
       delete med;
   }
   //将信息写入配置文件，待下次启动时初始化程序
    label = new QLabel;
    movie = new QMovie;
    if(fine.endsWith(".gif")){

        movie->setFileName(fine);
        label->setMovie(movie);
        movie->start();

    }else{
        QPixmap pixmap(fine);
        label->setPixmap(pixmap);
        // 获取屏幕的缩放比例
        int scaleFactor = qApp->screens().at(0)->logicalDotsPerInch() / 96;

        // 调整图片的大小和分辨率
        pixmap = pixmap.scaled(pixmap.width() * scaleFactor, pixmap.height() * scaleFactor, /*Qt::IgnoreAspectRatio*/Qt::KeepAspectRatio, Qt::SmoothTransformation);

//        pixmap.setDevicePixelRatio(devicePixelRatioF());

    }
    ConfigFile->setValue("cfg/GIF","正在使用");
    ConfigFile->setValue("cfg/GIF图片路径",fine);
    ConfigFile->setValue("cfg/MP4","暂停使用");
//    label->showFullScreen();
    label->setWindowState(Qt::WindowMaximized);
    label->setWindowFlags(Qt::FramelessWindowHint);
        label->setScaledContents(true);
//    QPixmap pixmap = movie->currentPixmap().scaled(label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);


//    QPainter painter(this);
//    painter.setRenderHint(QPainter::Antialiasing, true);   // 设置抗锯齿
//    painter.drawPixmap(label->geometry(), pixmap);

    SetParent((HWND)label->winId(),winwallpaper());
    label->show();
}

void MainWidget::sys(QSystemTrayIcon::ActivationReason r)
{
    if(r == QSystemTrayIcon::DoubleClick)
    {
        this->show();
    }

}

void MainWidget::saveHistory(const QString path)
{
    QStringList historyPathList = loadHistory();
    if(historyPathList.contains(path)) {
        historyPathList.removeAll(path);
    }
    historyPathList.append(path);
    if(historyPathList.count()>10){
        historyPathList.removeFirst();
    }
    for(int i=0;i<historyPathList.count();++i){
            ConfigFile->setValue(QString("history/%1").arg(i),historyPathList[i]);
    }
    setHistoryBox();
}

void MainWidget::setHistoryBox()
{
    QStringList historyPathList = loadHistory();
    deleteItem(gridLayout);
//    f(gridLayout);
    int cnt = 0;
    QString imgpath = "/";
    QString imgText;
    for (int i = 0; i < rowCount; i++) {
        for (int j = 0; j < columnCount; j++) {
            // 创建缩略图 QLabel，并设置图片

            QVBoxLayout* tmpLayout = new QVBoxLayout;

            QPushButton* thumbnailBtn = new QPushButton;

            if(cnt<historyPathList.count()){
                imgpath = historyPathList[cnt];
                imgText = QFileInfo(historyPathList[cnt]).completeBaseName();
                connect(thumbnailBtn,&QPushButton::clicked,this,[=](){
                    loadhistorypaper(cnt);
                });
            }else{
                imgpath ="../image/mountains.png";
                imgText = "";
            }
            if(imgpath.endsWith(".mp4")){
                imgpath ="../image/mountains.png";
            }
            thumbnailBtn->setObjectName("label_img");
            thumbnailBtn->installEventFilter(this);
            QPixmap thumbnailPixmap(imgpath);
            QSize picSize(GetSystemMetrics(SM_CXSCREEN)/10, GetSystemMetrics(SM_CYSCREEN)/20);
            thumbnailPixmap = thumbnailPixmap.scaled(picSize, Qt::KeepAspectRatio,Qt::SmoothTransformation);
            thumbnailBtn->setIcon(QIcon(thumbnailPixmap));
            thumbnailBtn->setIconSize(picSize);
            thumbnailBtn->setMaximumWidth(125);

            // 创建文件名 QLabel，设置文本并设置自动换行和隐藏超出边界的部分
            QLabel* nameLabel = new QLabel;
            nameLabel->setObjectName("label_name");
            nameLabel->setText(imgText);
            nameLabel->setWordWrap(true);
            nameLabel->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
            nameLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
            nameLabel->setMaximumWidth(125);
            // 将缩略图和文件名添加到布局中
            tmpLayout->addWidget(thumbnailBtn);
            tmpLayout->addWidget(nameLabel);
            gridLayout->addLayout(tmpLayout, i, j);
            cnt++;
        }
    }

    // 设置网格布局的外边距和间距
    gridLayout->setContentsMargins(10, 10, 10, 10);
    gridLayout->setSpacing(10);
}

void MainWidget::setProcessAutoRun(const QString &appPath, bool flag)
{
    QSettings settings(AUTO_RUN, QSettings::NativeFormat);

    //以程序名称作为注册表中的键,根据键获取对应的值（程序路径）
    QFileInfo fInfo(appPath);
    QString name = fInfo.baseName();    //键-名称

    //如果注册表中的路径和当前程序路径不一样，则表示没有设置自启动或本自启动程序已经更换了路径
    QString oldPath = settings.value(name).toString(); //获取目前的值-绝对路劲
    QString newPath = QDir::toNativeSeparators(appPath);    //toNativeSeparators函数将"/"替换为"\"
    if(flag)
    {
        if (oldPath != newPath)
            settings.setValue(name, newPath);
    }
    else
        settings.remove(name);
}

bool MainWidget::isAutoRun(const QString &appPath)
{
    QSettings settings(AUTO_RUN, QSettings::NativeFormat);
    QFileInfo fInfo(appPath);
    QString name = fInfo.baseName();
    QString oldPath = settings.value(name).toString();
    QString newPath = QDir::toNativeSeparators(appPath);
    return (settings.contains(name) && newPath == oldPath);
}

void MainWidget::loadhistorypaper(int index)
{
    QStringList historyPathList = loadHistory();
    QString tmppath = historyPathList[index];

    if(tmppath.endsWith(".mp4")){
        if(ConfigFile->value("cfg/MP4视频路径")!=tmppath||ConfigFile->value("cfg/MP4")=="暂停使用"){
            mp4(tmppath);
        }
    }
    if(tmppath.endsWith(".jpg")||
    tmppath.endsWith(".png")||
    tmppath.endsWith(".jpeg")||
    tmppath.endsWith(".gif")){
        if(ConfigFile->value("cfg/GIF图片路径")!=tmppath||ConfigFile->value("cfg/GIF")=="暂停使用"){
            gif(tmppath);
        }
    }
    saveHistory(tmppath);
}


QStringList MainWidget::loadHistory()
{
    QStringList historyPathList;
    for(int i=0;i<10;++i){
        if(!ConfigFile->contains(QString("history/%1").arg(i))){
            break;
        }
        historyPathList.append(ConfigFile->value(QString("history/%1").arg(i)).toString());
    }
    qDebug()<<historyPathList;
    return historyPathList;
}

void MainWidget::OnGifBtnClicked()
{
       QString path = "/";
        if(QFile(ConfigFile->value("cfg/GIF图片路径").toString()).exists()){
            path = ConfigFile->value("cfg/GIF图片路径").toString();
        }
       //获取文件路径与文件类型过滤器，返回文件路径
       QString gifFile = QFileDialog::getOpenFileName(this,"打开动态图片",path,"image(*.png *.jpg *.jpeg *.gif)");
       //先关掉上一个窗口，不然无法在退出程序时关闭窗口
       if(gifFile.size() == 0){return;}

       //将文件路径传递给自定义图片显示函数
       gif(gifFile);
       saveHistory(gifFile);
}

void MainWidget::OnMP4BtnClicked()
{
       QString path = "/";
        if(QFile(ConfigFile->value("cfg/MP4视频路径").toString()).exists()){
            path = ConfigFile->value("cfg/MP4视频路径").toString();
        }
       QString mp4File = QFileDialog::getOpenFileName(this,"打开视频文件",path,"MV(*.mp4 *.wav *.mp3)");
        if(mp4File.size() == 0){return;}

        mp4(mp4File);
        saveHistory(mp4File);
}

void MainWidget::OnWebOpenBtnClicked()
{
    qDebug()<<"获取壁纸";

    if(MAinLayout->count()==1){
        m_webwidget = new WebWidget(this);
        setFixedWidth(780);
        MAinLayout->addWidget(m_webwidget);
        WebOpenBtn->setText("结束获取");
    }else{
        if(m_webwidget->isHidden()){
            // height = this->height();
            setFixedWidth(780);
            m_webwidget->show();
            WebOpenBtn->setText("结束获取");

        }else{
            // height = this->height();
            m_webwidget->hide();
            WebOpenBtn->setText("获取壁纸");
            setFixedWidth(328);
        }
    }
    return;
}

void MainWidget::MinWidget()
{
        this->hide();
        QMenu* menu = new QMenu(this);
        menu->addAction(_show);
        menu->addAction(_clos);
        system->setContextMenu(menu);
        system->setToolTip("动态壁纸");
        system->setIcon(QIcon("../image/tubiao.ico"));
        system->show();
        if(if_sum)
        {
           system->showMessage("动态壁纸","已隐藏到最小化托盘");
           if_sum = 0;
        }
}

void MainWidget::deleteItem(QLayout *layout)
{
        QLayoutItem *child;
        while ((child = layout->takeAt(0)) != nullptr)
        {
           //setParent为NULL，防止删除之后界面不消失
           if(child->widget())
           {
            layout->removeItem(child);
            delete child->widget();
            delete child;
            child = nullptr;
           }
           else if (child->layout())
           {
            this->deleteItem(child->layout());
            child->layout()->deleteLater();
           }
           delete child;
           child = nullptr;
        }
}










