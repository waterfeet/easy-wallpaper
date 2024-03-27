#include "picwidget.h"
#include "qmovie.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>

PicWidget::PicWidget(QWidget *parent)
    : QWidget(parent)
{
//    setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
    // 设置布局
    setFixedSize(400,250);
    stackedWidget = new QStackedWidget(this);

    //QVBoxLayout *mainLayout = new QVBoxLayout(this);
//    mainLayout->setContentsMargins(0,0,0,0);
    m_imageLabel = new QLabel(this);
//    m_imageLabel->setScaledContents(true);
   // m_imageLabel->setFixedSize(400,225);
    m_imageLabel->setFixedSize(400,225);
    m_imageLabel->setText("movie");
    stackedWidget->addWidget(m_imageLabel);

    m_button = new QPushButton("点击下载");
    m_button->setObjectName("picbtn");
    m_button->setFixedSize(400,225);
    stackedWidget->addWidget(m_button);

    setStyleSheet("#picbtn {"
        "background-color: rgba(252,252,252,20%);"
//        "background-clip: content;"
        "border-radius: 0px;"
        "color: rgba(9,9,9,100%);"
        "border:none;"
        "font-size: 50px;"
        "font-family: 微软雅黑;"
        "text-align: center;"
        "}");

    m_textLabel = new QLabel(this);
    m_textLabel->setAlignment(Qt::AlignCenter);
    m_textLabel->setStyleSheet("background-color: transparent;");
    //stackedWidget->addWidget(m_textLabel);

    // 将堆栈窗口添加到主布局中
    //mainLayout->addWidget(stackedWidget);
    stackedWidget->setFixedSize(400,225);
    m_textLabel->setVisible(false);
    m_button->setVisible(false);
    m_textLabel->setGeometry(stackedWidget->x(), stackedWidget->y()+200,400,15);
    //mainLayout->addWidget(m_textLabel);
    //

    // 设置信号和槽
    connect(m_button, &QPushButton::clicked, this, &PicWidget::picclicked);

}

PicWidget::~PicWidget()
{
    // 当你需要释放资源时，可以这样做：
    if (m_imageLabel->movie()->state() == QMovie::Running) {
        m_imageLabel->movie()->stop(); // 停止电影播放
    }
    // 删除QMovie对象
    delete m_imageLabel->movie();
    // 从QLabel上移除QMovie
    m_imageLabel->setMovie(nullptr);


}

void PicWidget::setImage(const QPixmap &pixmap)
{
    m_imageLabel->setPixmap(pixmap);
}

void PicWidget::setMovie(QMovie *movie)
{
    //movie->start();
    m_imageLabel->setMovie(movie);


}

void PicWidget::setText(const QString &text)
{
    m_text = text;
    m_textLabel->setText(m_text);
}

void PicWidget::enterEvent(QEnterEvent *event)
{

    Q_UNUSED(event);
    if (!underMouse()) {
        return;
    }

    if(m_imageLabel->movie()){

        //m_imageLabel->movie()->setPaused(false);

    }
    m_button->setVisible(true);
    m_textLabel->setVisible(true);
    m_button->raise();
    m_textLabel->raise();
}

void PicWidget::leaveEvent(QEvent *event)
{
    Q_UNUSED(event);

    if(m_imageLabel->movie()){
        //m_imageLabel->movie()->setPaused(true);
        //m_imageLabel->movie()->stop();
    }
    m_button->setVisible(false);
    m_textLabel->setVisible(false);
}
