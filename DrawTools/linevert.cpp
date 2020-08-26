#include "linevert.h"

LineVert::LineVert()
{

}

void LineVert::paint(QPainter &painter)
{
    int y = _start.y();
    int x = _end.x();
    _end = QPoint(x,y);
    painter.drawLine(_start, _end);
}
