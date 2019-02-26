#ifndef EVENTMIDDLEENGINE_H
#define EVENTMIDDLEENGINE_H

#include "events/ebevent.h"
#include "Widgets/figurewidget.h"
#include <QObject>
#include <QEvent>

//未使用
//event事件发送
class EventMiddleEngine : public QObject
{
    Q_OBJECT
public:
    explicit EventMiddleEngine(QObject *parent = nullptr);
    bool eventFilter(QObject *watched, QEvent *event) override;

signals:


public slots:
};

#endif // EVENTMIDDLEENGINE_H
