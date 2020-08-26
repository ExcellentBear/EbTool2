#ifndef MOVETEXT_H
#define MOVETEXT_H

#include <QObject>
#include <QString>
#include <QDateTime>

class MoveText : public QObject
{
    Q_OBJECT
public:
    explicit MoveText(QObject *parent = nullptr);

public slots:
    void start_work();

    void exit_work();

signals:
    void send_cur_tip(QString msg);

private:
    qint64 _time;
    QString _str;
    bool  _status;
};

#endif // MOVETEXT_H
