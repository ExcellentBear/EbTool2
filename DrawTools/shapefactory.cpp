#include "shapefactory.h"

MShape* ShapeFactory::createShape(ShapeFactory::MenuType type)
{
    switch (type) {
    case ERASE_RECT:
    case PICK_OUT_RECT:
    case TESS_READ_RECT:
        return new RectHollow;

    case DRAW_H_LINE:
        return new LineHist;

    case DRAW_W_LINE:
        return new LineVert;

    case UNKNOW:
    default:
        return nullptr;
    }

}
