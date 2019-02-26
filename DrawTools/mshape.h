#ifndef MSHAPE_H
#define MSHAPE_H

#include <QPoint>
#include <QPainter>

class MShape
{
public:

    virtual void setStart(QPoint start);
    virtual void setEnd(QPoint end);
    virtual QPoint getStart();
    virtual QPoint getEnd();

    virtual void paint(QPainter& painter) = 0;

protected:
    QPoint _start;
    QPoint _end;

};

#endif // MSHAPE_H
