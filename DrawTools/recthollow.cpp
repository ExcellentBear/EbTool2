#include "recthollow.h"

RectHollow::RectHollow()
{

}

void RectHollow::paint(QPainter &painter)
{
     int x1 = _start.x();
     int x2 = _end.x();
     int y1 = _start.y();
     int y2 = _end.y();
     QPoint p2 = QPoint(x1,y2);
     QPoint p4 = QPoint(x2,y1);
     painter.drawLine(_start,p2);
     painter.drawLine(p2,_end);
     painter.drawLine(_end,p4);
     painter.drawLine(p4,_start);
}
