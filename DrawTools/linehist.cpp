#include "linehist.h"

LineHist::LineHist()
{

}

void LineHist::paint(QPainter &painter)
{
    int x = _start.x();
    int y = _end.y();
    _end = QPoint(x,y);
    painter.drawLine(_start,_end);
}
