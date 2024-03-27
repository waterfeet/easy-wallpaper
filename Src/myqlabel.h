#ifndef MYQLABEL_H
#define MYQLABEL_H
#include<QLabel>
#include<QMovie>
class myQLabel :public QLabel
{
public:
    myQLabel();
    void paintEvent(QPaintEvent *);
};

#endif // MYQLABEL_H
