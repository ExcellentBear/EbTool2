#ifndef MAINWINDOW_H
#define MAINWINDOW_H

/*
 采用键槽机制

 当采用postEvent/sendEvent + eventFilter/event机制时，在子widget或者没有初始化的
 QObject中，受时空限制，无法发送信号到指定的reciever（无法reciever获取实例指针（其未实例化））。
 信号/槽相对较灵活

*/


#include "Widgets/ImageDeal/drawwindow.h"
#include "Widgets/DataEdit/dataeditwindow.h"
#include <QMainWindow>
#include <QCloseEvent>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event);
    bool eventFilter(QObject *watched, QEvent *event);

private:
    Ui::MainWindow *ui;
    DrawWindow* ui2;
    DataEditWindow* ui3;
    QAction* _action;
    QMutex _mutex;

    QTimer  m_timer;
    uint8_t  m_index_;

private slots:
    void _init_draw_win();
};

#endif // MAINWINDOW_H
