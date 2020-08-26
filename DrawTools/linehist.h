#ifndef LINEHIST_H
#define LINEHIST_H

#include "mshape.h"

//竖直线
class LineHist : public MShape
{
public:
    LineHist();
    void paint(QPainter &painter) override;
};

#endif // LINEHIST_H
