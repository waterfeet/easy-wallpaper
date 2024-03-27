#include "myqlabel.h"
#include <QPainter>
myQLabel::myQLabel()
{

}

void myQLabel::paintEvent(QPaintEvent *)
{
    if (this->movie() && this->movie()->isValid())
    {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing, true);
        QPixmap curr_pix = this->movie()->currentPixmap();
        if (this->hasScaledContents())
        {

            QPixmap pix = curr_pix.scaled(this->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
            painter.drawPixmap(QPoint(0, 0), pix);
        }
        else
        {
            painter.drawPixmap(QPoint(0, 0), curr_pix);
        }
    }
}
