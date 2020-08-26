#ifndef EBEVENT_H
#define EBEVENT_H

#define USER_EVENT_TYPE 5005

#include "ebeventtype.h"
#include "eventdata.h"
#include "Widgets/ImageDeal/drawwindow.h"
#include <QEvent>


//未使用
class EbEvent : public QEvent
{
public:
    explicit EbEvent(EvenTData& eventData, EbEventType type);
    virtual ~EbEvent();
    EbEventType EbGetEventType();
    EvenTData& EbGetEventData();

private:
    EbEventType _type;
    EvenTData _data;
};

#endif // EBEVENT_H
