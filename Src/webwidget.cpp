#include "webwidget.h"
#include "picwidget.h"

#include <QDesktopServices>

WebWidget::WebWidget(QWidget *parent) :
    QWidget(parent)
{
    setFixedWidth(450);
//    QNetworkAccessManager *manager = new QNetworkAccessManager(this);

    updateBtn = new QPushButton("刷新",this);
    //findBtn = new QPushButton("搜索",this);
    //TempText = new QLineEdit(this);
    updateBtn->setGeometry(5,10,50,20);
    //TempText->setGeometry(70,10,175,20);
    //findBtn->setGeometry(255,10,50,20);
    QWidget *tempw = new  QWidget(this);
    int height = parent->height();
    tempw->setGeometry(5,40,405,height-45);
    // 设置子布局的位置和大小
    //TempText->installEventFilter(this);
    page =1;
    connect(updateBtn,&QPushButton::clicked, this, [this](){
        //TempText->setText("");
        updateBtn->setEnabled(false);
        iniWebWallpaperData(QString("https://zhutix.com/animated/page/%1/").arg(QString::number(page)));

    });
    updateBtn->setEnabled(false);
    //connect(findBtn,&QPushButton::clicked, this, &WebWidget::searchPaperUpDate);

    firstpage = new QPushButton("首页");
    lastpage = new QPushButton("上一页");
    curPage = new QLabel(QString("第%1页").arg(QString::number(page)));
    curPage->setAlignment(Qt::AlignCenter);
    nextpage = new QPushButton("下一页");
    pageBtnLayout = new QHBoxLayout;
    pageBtnLayout->addWidget(firstpage);
    pageBtnLayout->addWidget(lastpage);
    pageBtnLayout->addWidget(curPage);
    pageBtnLayout->addWidget(nextpage);
    connect(firstpage,&QPushButton::clicked,this,[this](){
        firstpage->setEnabled(false);
        lastpage->setEnabled(false);
        nextpage->setEnabled(false);
        page = 1;
        searchPaperUpDate();
        QTimer::singleShot(1000, this, [=](){
            firstpage->setEnabled(true);
            lastpage->setEnabled(true);
            nextpage->setEnabled(true);
        });
    });
    connect(lastpage,&QPushButton::clicked,this,[this](){
        firstpage->setEnabled(false);
        lastpage->setEnabled(false);
        nextpage->setEnabled(false);
        page --;
        if(page<1){
            page =1;
        }
        searchPaperUpDate();
        QTimer::singleShot(1000, this, [=](){
            firstpage->setEnabled(true);
            lastpage->setEnabled(true);
            nextpage->setEnabled(true);
        });
    });
    connect(nextpage,&QPushButton::clicked,this,[this](){
        firstpage->setEnabled(false);
        lastpage->setEnabled(false);
        nextpage->setEnabled(false);
        page ++;
        searchPaperUpDate();
        QTimer::singleShot(1000, this, [=](){
            firstpage->setEnabled(true);
            lastpage->setEnabled(true);
            nextpage->setEnabled(true);
        });
    });

    MainLayout = new QVBoxLayout;
    tempw->setLayout(MainLayout);
    imglayout = new QVBoxLayout;
    MainLayout->addLayout(imglayout);
    MainLayout->addLayout(pageBtnLayout);
    m_HtmlData = "";
    m_reply = nullptr;
    iniWebWallpaperData("https://zhutix.com/animated/page/1/");
    manager = new QNetworkAccessManager;


}

WebWidget::~WebWidget()
{
    delete m_reply;
    delete manager;
}


///*
void WebWidget::iniWebWallpaperData(QString url)
{
    qDebug() << "开始获取数据";
    //https://zhutix.com/animated/
    //https://wallhaven.cc/toplist
    //http://steamworkshop.download/latest
    deleteItem(imglayout);//删掉原有布局内容

    if(loadingWidget==nullptr){
        loadingWidget = new LoadingWidget(this);
        m_progressBar = new QProgressBar(this);
        MainLayout->addWidget(loadingWidget);
        m_progressBar->setGeometry(5,630,290,15);
        m_progressBar->hide();
    }
    loadingWidget->raise();
    loadingWidget->show();
//    TempText->setText(searchkeyWord);

    QNetworkRequest mTestRequest((QUrl(url)));
    mTestRequest.setHeader(QNetworkRequest::UserAgentHeader, "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/115.0.0.0 Safari/537.36 Edg/115.0.1901.203");

    QNetworkAccessManager *mManager = new QNetworkAccessManager;

    m_HtmlData.clear();

    QNetworkReply * reply_ = mManager->get(mTestRequest);

    QEventLoop eventLoop2;
    connect(reply_, &QNetworkReply::finished, &eventLoop2, &QEventLoop::quit);
    eventLoop2.exec(QEventLoop::ExcludeUserInputEvents);

    m_HtmlData = reply_->readAll();
    //qDebug()<< m_HtmlData;
    readFinishedSlot();
    reply_->deleteLater();

}

bool WebWidget::eventFilter(QObject *obj, QEvent *event)
{
//     if (obj == TempText && event->type() == QEvent::KeyPress && TempText->text().count()>0)
//     {
//         QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
//         if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter)
//         {
//             page =1;
// //                TempText->setText(searchkeyWord);
//             keyEvent->ignore();
//             searchPaperUpDate();
//             qDebug() << "回车键被按下";
//             return true; // 返回true，表示已经处理了事件
//         }
//     }

    return QObject::eventFilter(obj, event);
}


void WebWidget::updateProgressBar(qint64 bytesReceived, qint64 bytesTotal)
{
    if (bytesTotal > 0) {
        int progress = static_cast<int>((bytesReceived * 100) / bytesTotal);
        m_progressBar->setValue(progress);
        if(progress == 100){
            TimeOut;
            m_progressBar->hide();
        }
    }
}

void WebWidget::searchPaperUpDate()
{
//    if(TempText->text().count()==0){
//        iniWebWallpaperData("https://wallhaven.cc/random");
//        return;
//    }
//    searchkeyWord = TempText->text();
//    QString content = "https://wallhaven.cc/search?q=%1&page=%2";
//    content = content.arg(TempText->text()).arg(page);
//    iniWebWallpaperData(content);
    iniWebWallpaperData(QString("https://zhutix.com/animated/page/%1/").arg(QString::number(page)));
}

void WebWidget::upDateWidget()
{

    // 遍历WallpaperData列表，依次显示缩略图
    // 删除所有的小部件
    QLayoutItem *child;
    while ((child = imglayout->takeAt(0)) != nullptr) {
        delete child->widget();
        delete child;
    }
    qDebug()<<QString::fromLocal8Bit("开始执行upDateWidget");
    // 创建一个QWidget，用于包含所有的图片小部件
    QListWidget *listWidget = new QListWidget;
    listWidget->setFixedWidth(400);
    listWidget->setStyleSheet("QListWidget{border:none; "
                              "background-color:rgba(0,0,0,0);}"
                              "QListWidget::Item:hover{background-color:rgba(47,46,46,0);border-radius:5px; }"
                              "QListWidget::item:selected{background-color:rgba(47,46,46,0);color:rgb(61,61,61);border:2px solid #FFC53D;border-radius:5px; }"
                                );
//    listWidget->verticalScrollBar()->setVisible(false);
    listWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    imglayout->addWidget(listWidget);

    for (int i = 0; i < wallpaperList.size(); ++i) {
        WallpaperData wallpaperData = wallpaperList.at(i);
        QString imageUrl = wallpaperData.thumbnail; // 这里是缩略图的URL
        QString sourceUrl = wallpaperData.url;
        // 创建一个PicWidget用于显示图片
        PicWidget *p_picwidget = new PicWidget(this);
        QListWidgetItem* item = new QListWidgetItem();
        item->setSizeHint(p_picwidget->size());
        listWidget->addItem(item);
        listWidget->setItemWidget(item, p_picwidget);


        // 创建一个QNetworkAccessManager，用于加载网络图片
        QNetworkAccessManager *networkManager = new QNetworkAccessManager(listWidget);
        connect(networkManager, &QNetworkAccessManager::finished, [=](QNetworkReply *reply) {

            if (reply->error() == QNetworkReply::NoError) {
                // 如果加载成功，将QPixmap设置到PicWidget上
                qDebug() << "获取: " << imageUrl << "成功";
                QByteArray data = reply->readAll();

                // 创建QMovie对象并设置数据
                QMovie *movie = new QMovie(this);
                movie->setCacheMode(QMovie::CacheAll); // 无限循环
                movie->setDevice(new QBuffer(movie)); // QBuffer作为QMovie的子对象
                static_cast<QBuffer*>(movie->device())->open(QIODevice::WriteOnly);
                static_cast<QBuffer*>(movie->device())->write(data);
                static_cast<QBuffer*>(movie->device())->close();

                movie->start(); // 开始播放动画
                p_picwidget->setMovie(movie);
                p_picwidget->setText(wallpaperData.size);
            } else {
                // 如果加载失败，显示错误信息
                if (p_picwidget) {
                    p_picwidget->setText("Error: " + reply->errorString());
                }
            }

            reply->deleteLater();
        });
        connect(p_picwidget,&PicWidget::picclicked,this,[=](){
            downLoadpic(sourceUrl);
        });
        // 发送一个GET请求，加载网络图片
        QNetworkRequest request((QUrl(imageUrl)));
        networkManager->get(request);
    }
    delete loadingWidget; //隐藏加载动画的标签
    loadingWidget = nullptr;
    //
    //
    curPage->setText(QString("第%1页").arg(QString::number(page)));
    //
//    imglayout->addLayout(templayout);
    imglayout->addWidget(listWidget);
    //imglayout->addLayout(pageBtnLayout);
//    imglayout->addWidget(m_progressBar);
    m_progressBar->hide();
    m_progressBar->setValue(0);
    QTimer::singleShot(1000, this, [=](){
        updateBtn->setEnabled(true);
    });
    //updateBtn->setEnabled(true);
}

void WebWidget::readDataSlot()
{
    qDebug()<<QString::fromLocal8Bit("开始执行readDataSlot");
    //sender()函数作用就是得到一个指针，这个指针是：当前槽函数所对应的信号是哪个对象发送的
    QNetworkReply *reply = (QNetworkReply *) sender();
    QByteArray data1 = reply->readAll();    //读取传回来的HTML数据
    QString strData = QString(data1);
    if(strData != "")
    {
        qDebug()<<"Get website page's data!";
        m_HtmlData.push_back(strData);
        //getPictureLinkFromHTML(strData);        //解析HTML，将图片链接存储在mDownloadList中
        //getPicLinkFromHTML(strData);              //新的网页
    }
    //    reply->deleteLater();                   //记得释放内存
}

void WebWidget::readFinishedSlot()
{
    if(m_HtmlData.length()>0){
        getPicLinkFromHTML(m_HtmlData);
    }
   // reply->deleteLater();       //记得释放内存
}

QString WebWidget::url2imgurl(QString surl)
{
    QString Filename="";
    QStringList tempstr = surl.split("/w/");
    if(tempstr.count()>1){
        Filename=tempstr[1];
    }
    surl.replace("wallhaven.cc","w.wallhaven.cc");
    surl.replace("/w/","/full/$$/wallhaven-");
    if(Filename.count()>2){
         surl.replace("$$",Filename.left(2));
    }
    qDebug() << surl;
    return surl;
}

void WebWidget::getPictureLinkFromHTML(QString htmlText)
{
    qDebug()<<QString::fromLocal8Bit("开始执行getPictureLinkFromHTML");
    QRegularExpression figureRegex("<figure.*?</figure>");
    QRegularExpressionMatchIterator figureMatches = figureRegex.globalMatch(htmlText);

    wallpaperList.clear();

    while (figureMatches.hasNext()) {
        QRegularExpressionMatch figureMatch = figureMatches.next();
        QString figureText = figureMatch.captured(0);

        QRegularExpression urlRegex("href=\"(https://wallhaven.cc/w/[\\w]+)\"");
        QRegularExpression thumbnailRegex("data-src=\"([^\"]+)\"");
        QRegularExpression sizeRegex("<span class=\"wall-res\">(\\d+ x \\d+)</span>");
        QRegularExpression pngRegex("<span>PNG</span>");

        QRegularExpressionMatch urlMatch = urlRegex.match(figureText);
        QRegularExpressionMatch thumbnailMatch = thumbnailRegex.match(figureText);
        QRegularExpressionMatch sizeMatch = sizeRegex.match(figureText);
        bool isPng = pngRegex.match(figureText).hasMatch();

        if (urlMatch.hasMatch() && thumbnailMatch.hasMatch() && sizeMatch.hasMatch()) {
            qDebug()<<urlMatch.captured(1);
            //这里要通过urlMatch.captured(1)获取文件大小
            QString url = url2imgurl(urlMatch.captured(1));
            QString thumbnail = thumbnailMatch.captured(1);
            QString size = sizeMatch.captured(1);
            if(isPng){
                url+=".png";
            }else{
                url+=".jpg";
            }
//            qint64 imgsize = getimgsize(url);
            WallpaperData wallpaperData = { url, thumbnail, size, isPng};

            wallpaperList.append(wallpaperData);
        }
    }
    qDebug()<<wallpaperList.count();
    if(wallpaperList.count()==0){
        searchPaperUpDate();
        return;
    }
    upDateWidget();
}

void WebWidget::getPicLinkFromHTML(QString htmlText)
{
    QStringList slist = htmlText.split("\"b2_gap ");
    htmlText = slist.last();
    QStringList slist2 = htmlText.split("pagination shu");
    htmlText = slist2.first();

    htmlText.remove(QRegularExpression("\\s"));
    wallpaperList.clear();
    QMap<QString, QString> wallpaperInfoMap;

    static QRegularExpression divPattern("<li(.*?)/li>");
    QRegularExpressionMatchIterator matchIter = divPattern.globalMatch(htmlText);


    while (matchIter.hasNext()){
        QRegularExpressionMatch match = matchIter.next();
        QString divContent = match.captured(1);
        qDebug().noquote()<<"divContent"<< divContent <<Qt::endl;

        static QRegularExpression titlePattern("<divclass=\"shu\">(.*?)</div>", QRegularExpression::CaseInsensitiveOption);
        QRegularExpressionMatch titleMatch = titlePattern.match(divContent);
        QString title = titleMatch.captured(1);
        static QRegularExpression linkPattern("</a><ahref=\"(.*?)\"class=\"imglist-charblock\"", QRegularExpression::CaseInsensitiveOption);
        QRegularExpressionMatch linkMatch = linkPattern.match(divContent);
        QString link = linkMatch.captured(1);
        static QRegularExpression sizePattern("\"b2fontb2-download-cloud-line\"></i>(.*?)<iclass=\"b2fontb2-funds-box-line\">", QRegularExpression::CaseInsensitiveOption);
        QRegularExpressionMatch sizeMatch = sizePattern.match(divContent);
        QString size = sizeMatch.captured(1);
        static QRegularExpression thumbnailPattern("data-src=\"(.*?)\"src=", QRegularExpression::CaseInsensitiveOption);
        QRegularExpressionMatch thumbnailMatch = thumbnailPattern.match(divContent);
        QString thumbnail = thumbnailMatch.captured(1);
//        qDebug().noquote()<<"title"<<title<<Qt::endl;
//        qDebug().noquote()<<"link"<<link<<Qt::endl;
//        qDebug().noquote()<<"size"<<size<<Qt::endl;
//        qDebug().noquote()<<"thumbnail"<<thumbnail<<Qt::endl;
        WallpaperData wallpaperData = {link, thumbnail, size, false};

        wallpaperList.append(wallpaperData);
    }

    qDebug()<<wallpaperList.count();
    if(wallpaperList.count()==0){
        //searchPaperUpDate();
        return;
    }
    upDateWidget();

}

void WebWidget::deleteItem(QLayout *layout)
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

void WebWidget::downLoadpic(QString url)
{
    if(false){//这个是下载图片的，暂时屏蔽不用
        // 弹出对话框选择下载路径
        QString filePath = QFileDialog::getSaveFileName(this, tr("Save Image"), "../wallpaperdownload/pic", tr("Images (*.png *.xpm *.jpg)"));
        if(filePath == ""){
            return;
        }
        m_reply = manager->get(QNetworkRequest(QUrl(url)));
        m_progressBar->setRange(0, 100);
        m_progressBar->show();
        connect(m_reply, &QNetworkReply::downloadProgress, this, &WebWidget::updateProgressBar);
        connect(m_reply, &QNetworkReply::finished, this, [this]() {
            if (m_reply->error() != QNetworkReply::NoError) {
                qDebug() << "Download error:" << m_reply->errorString();
                // 处理错误情况
            }
        });


        m_reply->ignoreSslErrors();
        // 等待下载完成
        QEventLoop loop;
        connect(m_reply, SIGNAL(finished()), &loop, SLOT(quit()));
        loop.exec();
        // 处理下载失败
        if (m_reply->error() != QNetworkReply::NoError)
        {
            qDebug() << "Download failed:" << m_reply->errorString();
            return;
        }
        // 处理选择路径对话框被取消的情况
        if (filePath.isEmpty())
        {
            qDebug() << "Save path not selected.";
            return;
        }

        // 将下载的图片保存到本地文件系统
        QFile file(filePath);
        if (file.open(QIODevice::WriteOnly))
        {
            file.write(m_reply->readAll());
            file.close();
            // 显示系统托盘提示
            QSystemTrayIcon* trayIcon = new QSystemTrayIcon(this);
            trayIcon->setIcon(QIcon("../image/tubiao.ico"));
            trayIcon->show();
            QString message = QString("Image saved to %1").arg(filePath);
            trayIcon->showMessage(tr("Download Complete"), message, QSystemTrayIcon::Information, 5000);

        }
    }

    //接下来是下载视频的，此时输入的是https://zhutix.com/animated/makima-cemetery-xft/这样的网页
    //需要先爬到蓝奏云链接
    qDebug()<<"开始执行视频下载";

    if(downLoadLanzou(url)){
        qDebug()<<"下载完成";
    }
    else{

        if(downLoad360(url)){

        }
        else{
            downLoadBaidu(url);
        }
    }
    //https://s-11.lanzog.com:446/03271500155937021bb/2024/01/03/b0f0c0259af97b1fb0bae62a4fb9c572.zip?st=44pu74m5b6WWl4njJ3PZpA&e=1711527532&b=U0wMbVI5UT0AOFNkA34GQQQ0DTwBZQd1CD4Id10jUX5RN1kSV3lWKVFtUnQ_c&fi=155937021&pid=115-220-211-208&up=2&mp=0&co=0
}

bool WebWidget::downLoadLanzou(QString url)
{
    //获得蓝奏云链接，找到下载区域的链接
    QNetworkRequest mTestRequest((QUrl(url)));
    mTestRequest.setHeader(QNetworkRequest::UserAgentHeader, "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/115.0.0.0 Safari/537.36 Edg/115.0.1901.203");

    QNetworkAccessManager *mManager = new QNetworkAccessManager;
    QString t_HtmlData;
    QNetworkReply * reply_ = mManager->get(mTestRequest);
    QEventLoop eventLoop2;
    connect(reply_, &QNetworkReply::finished, &eventLoop2, &QEventLoop::quit);
    eventLoop2.exec(QEventLoop::ExcludeUserInputEvents);

    t_HtmlData = reply_->readAll();
    reply_->deleteLater();

    QStringList slist = t_HtmlData.split("class=\"xiaa-buttons\">\n<span>蓝奏云盘：<a href=\"");//\n<span>蓝奏云盘：<a href=\"
    t_HtmlData = slist.last();
    QStringList slist2 = t_HtmlData.split("\" target=");
    t_HtmlData = slist2.first();
    qDebug()<< t_HtmlData;
    QString lanzouURL = t_HtmlData;
    if(lanzouURL.isEmpty()){
        return false;
    }

    QString apiURL = "https://api.leafone.cn/api/lanzou?url="+lanzouURL;
    qDebug()<<"apiURL:" <<apiURL;

    QNetworkRequest mTestRequest2((QUrl(apiURL)));
    mTestRequest2.setHeader(QNetworkRequest::UserAgentHeader, "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/115.0.0.0 Safari/537.36 Edg/115.0.1901.203");
    QNetworkReply * reply2_ = mManager->get(mTestRequest2);
    QEventLoop eventLoop3;
    connect(reply2_, &QNetworkReply::finished, &eventLoop3, &QEventLoop::quit);
    eventLoop3.exec(QEventLoop::ExcludeUserInputEvents);
    t_HtmlData = reply2_->readAll();
    reply2_->deleteLater();
    QStringList slist3 = t_HtmlData.split("\"url\": \"");
    t_HtmlData = slist3.last();
    QStringList slist4 = t_HtmlData.split("\"\n");
    t_HtmlData = slist4.first();

    qDebug()<< t_HtmlData;
    QString getURL = t_HtmlData;
    if(getURL.isEmpty()){
        return false;
    }

    // 弹出对话框选择下载路径
    QString filePath = QFileDialog::getSaveFileName(this, tr("Save video"), "../wallpaperdownload/pic/", tr("zip (*.zip)"));
    if(filePath == ""){
        return false;
    }
    m_reply = manager->get(QNetworkRequest(QUrl(getURL)));
    m_progressBar->setRange(0, 100);
    m_progressBar->show();
    connect(m_reply, &QNetworkReply::downloadProgress, this, &WebWidget::updateProgressBar);
    connect(m_reply, &QNetworkReply::finished, this, [this]() {
        if (m_reply->error() != QNetworkReply::NoError) {
            qDebug() << "Download error:" << m_reply->errorString();
            // 处理错误情况
        }
    });


    m_reply->ignoreSslErrors();
    // 等待下载完成
    QEventLoop loop;
    connect(m_reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    // 处理下载失败
    if (m_reply->error() != QNetworkReply::NoError)
    {
        qDebug() << "Download failed:" << m_reply->errorString();
        return false;
    }
    // 处理选择路径对话框被取消的情况
    if (filePath.isEmpty())
    {
        qDebug() << "Save path not selected.";
        return false;
    }

    // 将下载的图片保存到本地文件系统
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly))
    {
        file.write(m_reply->readAll());
        file.close();
        // 显示系统托盘提示
        QSystemTrayIcon* trayIcon = new QSystemTrayIcon(this);
        trayIcon->setIcon(QIcon("../image/tubiao.ico"));
        trayIcon->show();
        QString message = QString("Image saved to %1").arg(filePath);
        trayIcon->showMessage(tr("Download Complete"), message, QSystemTrayIcon::Information, 5000);
        return true;
    }

        // ///////////////////////////////////////////////////////////////////////
    /*
    QNetworkRequest mTestRequest2((QUrl(lanzouURL)));
    mTestRequest2.setHeader(QNetworkRequest::UserAgentHeader, "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/115.0.0.0 Safari/537.36 Edg/115.0.1901.203");
    QNetworkReply * reply2_ = mManager->get(mTestRequest2);
    QEventLoop eventLoop3;
    connect(reply2_, &QNetworkReply::finished, &eventLoop3, &QEventLoop::quit);
    eventLoop3.exec(QEventLoop::ExcludeUserInputEvents);
    t_HtmlData = reply2_->readAll();
    reply2_->deleteLater();

    QStringList slist3 = t_HtmlData.split("<iframe class=\"ifr2\"");
    t_HtmlData = slist3.last();
    QStringList slist4 = t_HtmlData.split("\" frameborder");
    t_HtmlData = slist4.first();
    QStringList slist5 = t_HtmlData.split("src=\"");
    t_HtmlData = slist5.last();

    if(t_HtmlData.isEmpty()){
        return false;
    }

    QString downloadURL = "https://zhutix.lanzouw.com" + t_HtmlData;
    qDebug()<< downloadURL;
    */
    // ///////////////////////////////////////////////////////////////////////
    //获取下载区域链接，获取压缩包链接
    // QNetworkRequest mTestRequest3((QUrl(downloadURL)));
    // mTestRequest2.setHeader(QNetworkRequest::UserAgentHeader, "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/115.0.0.0 Safari/537.36 Edg/115.0.1901.203");
    // QNetworkReply * reply3_ = mManager->get(mTestRequest3);
    // QEventLoop eventLoop4;
    // connect(reply3_, &QNetworkReply::finished, &eventLoop4, &QEventLoop::quit);
    // eventLoop4.exec(QEventLoop::ExcludeUserInputEvents);
    // t_HtmlData = reply3_->readAll();
    // reply3_->deleteLater();
    // qDebug()<< t_HtmlData;
    // QStringList slist6 = t_HtmlData.split("<iframe class=\"ifr2\"");
    // t_HtmlData = slist6.last();
    // QStringList slist7 = t_HtmlData.split("\" frameborder");
    // t_HtmlData = slist7.first();

    // QString downloadzipURL = t_HtmlData;
    // qDebug()<< downloadzipURL;


    //QDesktopServices::openUrl(QUrl(downloadURL.toLatin1()));
    return true;
}

bool WebWidget::downLoad360(QString url)
{    QNetworkRequest mTestRequest((QUrl(url)));
    mTestRequest.setHeader(QNetworkRequest::UserAgentHeader, "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/115.0.0.0 Safari/537.36 Edg/115.0.1901.203");

    QNetworkAccessManager *mManager = new QNetworkAccessManager;
    QString t_HtmlData;
    QNetworkReply * reply_ = mManager->get(mTestRequest);
    QEventLoop eventLoop2;
    connect(reply_, &QNetworkReply::finished, &eventLoop2, &QEventLoop::quit);
    eventLoop2.exec(QEventLoop::ExcludeUserInputEvents);

    t_HtmlData = reply_->readAll();
    reply_->deleteLater();

    QStringList slist = t_HtmlData.split("class=\"xiaa-buttons\">\n<span>360云盘：<a href=\"");//\n<span>蓝奏云盘：<a href=\"
    t_HtmlData = slist.last();
    QStringList slist2 = t_HtmlData.split("\" target=");
    t_HtmlData = slist2.first();
    qDebug()<< t_HtmlData;
    QString _360URL = t_HtmlData;
    if(_360URL.isEmpty()){
        return false;
    }
    QDesktopServices::openUrl(QUrl(_360URL.toLatin1()));
    return true;
}

bool WebWidget::downLoadBaidu(QString url)
{

}


