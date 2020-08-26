#include "loadconfigure.h"

//不采用宏指定绝对路径，使配置文件默认为exe目录下 "config.ini"
LoadConfigure::LoadConfigure()
{
    QFile file("config.ini");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;
    while(!file.atEnd()){ //just simple parse
       QByteArray lineByte = file.readLine();
       QString line(lineByte);
       line = line.trimmed();
       if(line.startsWith("#")) continue; //简单的配置文件解释器
       int index=-1;
       for(int i=1;i<line.length();i++){
               if(line.at(i)==QChar('=')){
                   index = i;
                   break;
               }
        }
        if(index < 0) continue;
        QString key = line.mid(0,index).toLower();
        QString value = line.mid(index+1).toLower();
        _conf[key] = value;
    }
}

LoadConfigure::~LoadConfigure()
{
    _conf.clear();
}

ShareConfInstance LoadConfigure::_instance(new LoadConfigure);

ShareConfInstance LoadConfigure::getConfInstance()
{
    return _instance;
}

ConfMap& LoadConfigure::getConfMap()
{
    return _instance->_conf;
}
