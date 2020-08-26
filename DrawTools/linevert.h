#ifndef LINEVERT_H
#define LINEVERT_H

#include "mshape.h"

//水平线
class LineVert : public MShape
{
public:
    LineVert();
    void paint(QPainter &painter) override;
};

#endif // LINEVERT_H
