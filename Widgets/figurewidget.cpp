#include "figurewidget.h"

FigureWidget::FigureWidget(ShapeFactory::MenuType type, QWidget *parent) : QWidget(parent)
{
    qRegisterMetaType<ShapeFactory::MenuType>("ShapeFactory::MenuType");
    qRegisterMetaType<QPoint>("QPoint");
    _current_menu = type;
    _perm = true;
    _shape = SharePtrShape(new ShapeDeco(type));
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setAutoFillBackground(true);
}

FigureWidget::~FigureWidget()
{
    _shape_list.clear();
}

void FigureWidget::_remove_unknow_shape()
{
    QList<int> indexes;
    for(int i=0; i< _shape_list.size();i++){
        if(_shape_list.at(i)->getMenuType() == ShapeFactory::UNKNOW){
            indexes.push_back(i);
        }
    }
    for(int j=indexes.size()-1; j>=0 ; j--){
        _shape_list.removeAt(indexes.at(j));
    }
}

void FigureWidget::_reset_points(QPoint &p1, QPoint &p2)
{
        int x1,x2,y1,y2;
        x1 = p1.x();
        x2 = p2.x();
        y1 = p1.y();
        y2 = p2.y();
        p1 = QPoint(std::min(x1,x2),std::min(y1,y2));
        p2 = QPoint(std::max(x1,x2),std::max(y1,y2));
}

bool FigureWidget::_isin_erase_area(QPoint lp1, QPoint lp2, QPoint &up_left, QPoint &down_right)
{
    int linePointX1 = lp1.x();
    int linePointY1 = lp1.y();
    int linePointX2 = lp2.x();
    int linePointY2 = lp2.y();
    int rectangleLeftTopX = up_left.x();
    int rectangleLeftTopY = up_left.y();
    int rectangleRightBottomX = down_right.x();
    int rectangleRightBottomY = down_right.y();
    int lineHeight = linePointY1 - linePointY2;
    int lineWidth = linePointX2 - linePointX1;
    int c = linePointX1 * linePointY2 - linePointX2 * linePointY1;
    if (    (lineHeight * rectangleLeftTopX + lineWidth * rectangleLeftTopY + c >= 0 && lineHeight * rectangleRightBottomX + lineWidth * rectangleRightBottomY + c <= 0)
         || (lineHeight * rectangleLeftTopX + lineWidth * rectangleLeftTopY + c <= 0 && lineHeight * rectangleRightBottomX + lineWidth * rectangleRightBottomY + c >= 0)
         || (lineHeight * rectangleLeftTopX + lineWidth * rectangleRightBottomY + c >= 0 && lineHeight * rectangleRightBottomX + lineWidth * rectangleLeftTopY + c <= 0)
         || (lineHeight * rectangleLeftTopX + lineWidth * rectangleRightBottomY + c <= 0 && lineHeight * rectangleRightBottomX + lineWidth * rectangleLeftTopY + c >= 0)
        ){
        if (rectangleLeftTopX > rectangleRightBottomX) qSwap(rectangleLeftTopX,rectangleRightBottomX);
        if (rectangleLeftTopY < rectangleRightBottomY) qSwap(rectangleLeftTopY,rectangleRightBottomY);
        if ((    linePointX1 < rectangleLeftTopX && linePointX2 < rectangleLeftTopX)
              || (linePointX1 > rectangleRightBottomX && linePointX2 > rectangleRightBottomX)
              || (linePointY1 > rectangleLeftTopY && linePointY2 > rectangleLeftTopY)
              || (linePointY1 < rectangleRightBottomY && linePointY2 < rectangleRightBottomY)
           ) {
               return false;
             } else {
                return true;
             }
       }else {
            return false;
       }
}

void FigureWidget::_actions_factory()
{
    if(_shape->isEmpty()) return;
    ShapeFactory::MenuType menuType = _shape->getMenuType();
    switch (menuType) {
    case ShapeFactory::DRAW_H_LINE:
    case ShapeFactory::DRAW_W_LINE:
        break;
    case ShapeFactory::ERASE_RECT:
    {
        QPoint p1 = _shape->getStartPoint();
        QPoint p2 = _shape->getEndPoint();
        _reset_points(p1,p2);
        QList<int> indexs;
        for(int i=0;i<_shape_list.size();i++){
            SharePtrShape shape = _shape_list.at(i);
            if(shape->getMenuType()==ShapeFactory::DRAW_H_LINE ||shape->getMenuType()==ShapeFactory::DRAW_W_LINE){
               if(_isin_erase_area(shape->getStartPoint(),shape->getEndPoint(),p1,p2))  indexs.push_back(i);
            }else if(shape->getMenuType()==ShapeFactory::ERASE_RECT){
                indexs.push_back(i); /*remove Erase_rect after*/
            }
        }
        for(int j=indexs.size()-1;j>=0;j--) _shape_list.removeAt(indexs.at(j));
        break;
    }
    case ShapeFactory::PICK_OUT_RECT:
    {
        QPoint p1 = _shape->getStartPoint();
        QPoint p2 = _shape->getEndPoint();
        _reset_points(p1,p2);
        emit send_subim_rect(p1,p2);
        clearShapeList();
        break;
    }
    case ShapeFactory::TESS_READ_RECT:
    {
        QPoint p1 =_shape->getStartPoint();
        QPoint p2 = _shape->getEndPoint();
        _reset_points(p1,p2);
        emit send_tessim_rect(p1,p2);
        _shape->setMenuType(ShapeFactory::UNKNOW);
        break;
    }
    default:
        break;
    }
}

void FigureWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::RightButton){
        if(!_perm){
            _shape->setMenuType(ShapeFactory::UNKNOW);
            update();
            return;
        }
    }
    _perm = false;
    _shape = SharePtrShape(new ShapeDeco(_current_menu));
    _shape->setDrawType(false);
    _shape->setStartPoint(event->pos());
    _shape->setEndPoint(event->pos());
    _shape_list.push_back(_shape);

}

void FigureWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(_perm) return;
    _shape->setEndPoint(event->pos());
    update();
}

void FigureWidget::mouseReleaseEvent(QMouseEvent *event)
{
    _perm = true;
    _shape->setEndPoint(event->pos());
    _actions_factory();
    update();
}

void FigureWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setBrush(Qt::white);
    QPen pen(Qt::green, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    painter.setPen(pen);
    foreach (auto shape, _shape_list) {
        if( shape ){
            if(!shape->getDrawType())shape->paintShape(painter);
        }
    }
    pen = QPen(Qt::blue, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    painter.setPen(pen);
    foreach (auto shape, _shape_list) {
        if( shape ){
            if(shape->getDrawType())shape->paintShape(painter);
        }
    }
}

const QList<SharePtrShape>& FigureWidget::getShapeList()
{
    return _shape_list;
}

void FigureWidget::addAutoGenShape(ShapeDeco *shapeDeco)
{
    SharePtrShape shape = SharePtrShape(shapeDeco);
    shape->setDrawType(true);
    _shape_list.push_front(shape);
}

void FigureWidget::addAutoGenShape(ShapeFactory::MenuType menuType, QPoint p1, QPoint p2)
{
    ShapeDeco* shape = new ShapeDeco(menuType);
    shape->setStartPoint(p1);
    shape->setEndPoint(p2);
    addAutoGenShape(shape);
}

void FigureWidget::clearShapeList(){
    _shape_list.clear();
    update();
}

void FigureWidget::setCurrentMenu(ShapeFactory::MenuType menuType)
{
    _current_menu = menuType;
}

void FigureWidget::removeLastDrew()
{
    _mutex.lock();
    if(_shape_list.isEmpty()){
        _mutex.unlock();
        return;
    }
    _remove_unknow_shape();
    if(_shape_list.size()>0)_shape_list.pop_back();
    _mutex.unlock();
    update();
}

void FigureWidget::removeAllAutoGen()
{
    _mutex.lock();
    if(_shape_list.isEmpty()){
        _mutex.unlock();
        return;
      }
    QList<int> index;
    for(int i=0;i<_shape_list.size();i++){
         if(_shape_list.at(i)->getDrawType()) index.push_back(i);
      }
    for(int j=index.size()-1; j>=0; j--){
            _shape_list.removeAt(index.at(j));
      }
    _mutex.unlock();
    update();
}
