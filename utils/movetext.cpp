#include "movetext.h"

MoveText::MoveText(QObject *parent) : QObject(parent)
{
    _time = QDateTime::currentMSecsSinceEpoch();
    _str = QString("Author:https://github.com/ExcellentBear or QHtzs");

}

void MoveText::exit_work()
{
    _status = false;  //假设为原子性的
}

void MoveText::start_work()
{
    _status = true;
    int i=0, length = _str.length();
    while (_status) {
        qint64 cur = QDateTime::currentMSecsSinceEpoch();
        if(cur - _time > 500){
            _time = cur;
            if(i>length-2) i = 0;
            QString msg = _str.mid(i++);
            emit send_cur_tip(msg);
        }
    }
}
