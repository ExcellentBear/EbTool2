#ifndef SHAPEDECO_H
#define SHAPEDECO_H

#include "DrawTools/shapefactory.h"

class ShapeDeco
{

public:
    ShapeDeco();
    ShapeDeco(ShapeFactory::MenuType menuType);
    ~ShapeDeco();
    void paintShape(QPainter& painter);


    void setMenuType(ShapeFactory::MenuType menuType);
    void setStartPoint(QPoint start);
    void setEndPoint(QPoint end);
    void setDrawType(bool isAutoGen);

    ShapeFactory::MenuType getMenuType() const;
    QPoint getStartPoint() const;
    QPoint getEndPoint() const;
    bool getDrawType() const;

    bool isEmpty() const;

private:
    MShape* _cur_shape;
    ShapeFactory::MenuType _menu_type;
    bool _is_auto_draw;
};

#endif // SHAPEDECO_H
