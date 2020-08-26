#include "mshape.h"


void MShape::setStart(QPoint start)
{
    _start = start;
}

void MShape::setEnd(QPoint end)
{
    _end = end;
}

QPoint MShape::getStart()
{
    return _start;
}

QPoint MShape::getEnd()
{
    return _end;
}
