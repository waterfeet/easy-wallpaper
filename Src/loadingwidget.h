#ifndef LOADINGWIDGET_H
#define LOADINGWIDGET_H
#include <QWidget>
#include <QTimer>

class LoadingWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LoadingWidget(QWidget *parent = nullptr);

signals:

public slots:

private:
    QTimer *m_timer;
    int m_angle;
    int m_radius;
    int m_ballSize;
    int m_centerX;
    int m_centerY;

protected:
    void paintEvent(QPaintEvent *event) override;
};


#endif // LOADINGWIDGET_H
