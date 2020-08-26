#ifndef FIGUREWIDGET_H
#define FIGUREWIDGET_H


#include "DrawTools/shapedeco.h"
#include <QWidget>
#include <QMetaType>
#include <QSharedPointer>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QMutex>

typedef QSharedPointer<ShapeDeco> SharePtrShape;

//画布上作图
class FigureWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FigureWidget(ShapeFactory::MenuType type, QWidget *parent = nullptr);
    virtual ~FigureWidget();

    const QList<SharePtrShape>& getShapeList();
    void addAutoGenShape(ShapeDeco*); /*add new shape*/
    void addAutoGenShape(ShapeFactory::MenuType menuType,QPoint p1,QPoint p2); /*add new shape*/

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    void _remove_unknow_shape();
    void _reset_points(QPoint &p1, QPoint &p2); /*standard points of rect*/
    bool _isin_erase_area(QPoint lp1,QPoint lp2,QPoint &up_left,QPoint &down_right); /*ensure line is in rect*/
    void _actions_factory(); /*factory of shape usage and action*/

    ShapeFactory::MenuType _current_menu;
    bool _perm;
    SharePtrShape _shape;
    QList<SharePtrShape> _shape_list;
    QMutex _mutex;

signals:
    void send_subim_rect(QPoint p1,QPoint p2);
    void send_tessim_rect(QPoint p1,QPoint p2);

public slots:
    void setCurrentMenu(ShapeFactory::MenuType menuType); /*setting the current shape of object*/
    void clearShapeList(); /*remove all shape*/
    void removeLastDrew(); /*remove the shape last add/draw*/
    void removeAllAutoGen(); /*remove all shape that auto add*/
};

#endif // FIGUREWIDGET_H
