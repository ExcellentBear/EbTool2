#include "shapedeco.h"


ShapeDeco::ShapeDeco()
{
    _is_auto_draw = false;
    _menu_type = ShapeFactory::UNKNOW;
    _cur_shape = ShapeFactory::createShape(_menu_type);
}

ShapeDeco::ShapeDeco(ShapeFactory::MenuType menuType)
{
    _is_auto_draw = false;
    _menu_type = menuType;
    _cur_shape = ShapeFactory::createShape(menuType);
}


ShapeDeco::~ShapeDeco()
{
    delete _cur_shape;
}

bool ShapeDeco::isEmpty() const
{
    return _menu_type == ShapeFactory::UNKNOW;
}

void ShapeDeco::paintShape(QPainter &painter)
{
    if(!isEmpty()) _cur_shape->paint(painter);
}

void ShapeDeco::setMenuType(ShapeFactory::MenuType menuType)
{
    _menu_type = menuType;
}

void ShapeDeco::setStartPoint(QPoint start)
{
    if(!isEmpty()) _cur_shape->setStart(start);

}

void ShapeDeco::setEndPoint(QPoint end)
{
    if(!isEmpty()) _cur_shape->setEnd(end);
}

void ShapeDeco::setDrawType(bool isAutoGen)
{
    _is_auto_draw = isAutoGen;
}

ShapeFactory::MenuType ShapeDeco::getMenuType() const
{
    return _menu_type;
}

QPoint ShapeDeco::getStartPoint() const
{
    return _cur_shape->getStart();
}

QPoint ShapeDeco::getEndPoint() const
{
    return _cur_shape->getEnd();
}

bool ShapeDeco::getDrawType() const
{
    return _is_auto_draw;
}


