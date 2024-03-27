#ifndef PICWIDGET_H
#define PICWIDGET_H


#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QGraphicsOpacityEffect>
#include <QStackedWidget>

class PicWidget : public QWidget
{
    Q_OBJECT

public:
    PicWidget(QWidget *parent = nullptr);
    ~PicWidget();

    void setImage(const QPixmap &pixmap);
    void setMovie(QMovie *movie);
    void setText(const QString &text);

signals:
    void picclicked();

protected:
    void enterEvent(QEnterEvent *event);
    void leaveEvent(QEvent *event);

private:
    QLabel *m_imageLabel;
    QLabel *m_textLabel;
    QPushButton *m_button;
    QString m_text;
    QStackedWidget *stackedWidget;
};



#endif // PICWIDGET_H
