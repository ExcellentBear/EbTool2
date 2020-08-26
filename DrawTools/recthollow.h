#ifndef RECTHOLLOW_H
#define RECTHOLLOW_H

#include "mshape.h"

//空心矩阵，实质是4条边
class RectHollow:public MShape
{
public:
    RectHollow();
    void paint(QPainter &painter) override;
};

#endif // RECTHOLLOW_H
