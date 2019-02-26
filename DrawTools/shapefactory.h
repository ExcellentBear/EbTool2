#ifndef SHAPEFACTORY_H
#define SHAPEFACTORY_H

#include "DrawTools/mshape.h"
#include "DrawTools/linehist.h"
#include "DrawTools/linevert.h"
#include "DrawTools/recthollow.h"
#include <QSharedPointer>


//简单MShape工厂
class ShapeFactory
{
public:
    enum MenuType{
        DRAW_H_LINE,  //histgram line
        DRAW_W_LINE,  //vert line
        TESS_READ_RECT, //rect for tesseract-ocr recongize
        PICK_OUT_RECT,  //rect to pick out
        ERASE_RECT,   //rect to erase
        UNKNOW, // unsured type
    };
    static MShape* createShape(ShapeFactory::MenuType type);

};

#endif // SHAPEFACTORY_H
