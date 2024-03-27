#include "loadingwidget.h"
#include <QPainter>
#include <QLabel>
LoadingWidget::LoadingWidget(QWidget *parent) : QWidget(parent)
{
    setWindowTitle("Loading...");
    setFixedSize(parent->width(), 300); // 增加高度以容纳标签

    m_angle = 0;
    m_radius = 50;
    m_ballSize = 10;
    m_centerX = width() / 2;
    m_centerY = height() / 2;

    m_timer = new QTimer(this);
    m_timer->setInterval(20);
    connect(m_timer, &QTimer::timeout, [this]() {
        m_angle += 10;
        if (m_angle >= 360) {
            m_angle = 0;
        }
        update();
    });
    m_timer->start();

    QLabel *label = new QLabel("只要等的时间长，东西总会出来的。\n实在不行点刷新，再不行就重启吧。", this);
    label->setAlignment(Qt::AlignCenter);
    label->setWordWrap(true); // 启用自动换行
    label->setGeometry(0, m_centerY + m_radius + m_ballSize + 10, width(), 40); // 增加高度以容纳两行文字
}

void LoadingWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);

    for (int i = 0; i < 6; i++) {
        QColor color;
        switch (i) {
        case 0:
            color = QColor("#C6FFDD");
            break;
        case 1:
            color = QColor("#FBD786");
            break;
        case 2:
            color = QColor("#f7797d");
            break;
        case 3:
            color = QColor("#C6FFDD");
            break;
        case 4:
            color = QColor("#FBD786");
            break;
        case 5:
            color = QColor("#f7797d");
            break;
        }

        painter.setPen(Qt::NoPen);
        painter.setBrush(QBrush(color));
        painter.save();
        painter.translate(m_centerX, m_centerY);
        painter.rotate(m_angle + i * 60);
        painter.drawEllipse(QPoint(m_radius, 0), m_ballSize, m_ballSize);
        painter.restore();
    }
}
