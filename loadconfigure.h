#ifndef LOADCONFIGURE_H
#define LOADCONFIGURE_H

#include <QSharedPointer>
#include <QMap>
#include <QString>
#include <QFile>
#include <QIODevice>
#include <QStringList>

class LoadConfigure;
typedef QMap<QString,QString> ConfMap;
typedef ConfMap::iterator ConfIte;

typedef QSharedPointer<LoadConfigure> ShareConfInstance;

//配置文件载入单例
class LoadConfigure
{
public:
    LoadConfigure();
    ~LoadConfigure();
    LoadConfigure(const LoadConfigure& instance) = delete;
    LoadConfigure& operator = (const LoadConfigure& instance) = delete;
    static ShareConfInstance getConfInstance();
    static ConfMap& getConfMap();

private:
    ConfMap _conf;
    static ShareConfInstance _instance;

};

#endif // LOADCONFIGURE_H
