#include "ebevent.h"

EbEvent::EbEvent(EvenTData &eventData, EbEventType type):QEvent((QEvent::Type)USER_EVENT_TYPE)
{
    _type = type;
    _data = eventData; //deepcopy [EvenTData对拷贝做了重载，此处深拷贝]
}

EbEvent::~EbEvent()
{

}

EbEventType EbEvent::EbGetEventType()
{
    return _type;
}

EvenTData& EbEvent::EbGetEventData()
{
    return _data;
}
