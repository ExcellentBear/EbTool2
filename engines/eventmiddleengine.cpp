#include "eventmiddleengine.h"


EventMiddleEngine::EventMiddleEngine(QObject *parent) : QObject(parent)
{

}

bool EventMiddleEngine::eventFilter(QObject *watched, QEvent *event)
{
    if(event->type() !=(QEvent::Type)USER_EVENT_TYPE){
        return false; //传给其它event处理
    }
    EbEvent* mEvent = static_cast<EbEvent*>(event);
    EbEventType type = mEvent->EbGetEventType();
    EvenTData data = mEvent->EbGetEventData();
    return true;
}
