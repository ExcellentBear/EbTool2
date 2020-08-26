#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui3 = new DataEditWindow(this);
    ui->gridLayout->addWidget(ui3);

    _action = new QAction("OpenDrawWin", this);
    ui->mainToolBar->addAction(_action);

    ui2 = new DrawWindow(0);
    ui2->installEventFilter(this);
    ui2->show();

    connect(_action,SIGNAL(triggered()),this, SLOT(_init_draw_win()));
    connect(ui2,SIGNAL(send_ml_result(ResultType)),ui3,SLOT(RecieveResultData(ResultType)));
    connect(ui2,SIGNAL(send_tess_result(QString)),ui3, SLOT(RecieveTessResult(QString)));

}

MainWindow::~MainWindow()
{
    delete ui;
    delete ui2;
}

void MainWindow::_init_draw_win()
{
    _mutex.lock();
    if(ui2==nullptr){
        ui2 = new DrawWindow(0);
        ui2->installEventFilter(this);
        ui2->show();
    }
    _mutex.unlock();
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if(event->type()==QEvent::Close && watched==ui2){
        ui2->close();
        ui2->deleteLater();
        ui2 = nullptr;
        return true;
    }
    return false;
}

void MainWindow::closeEvent(QCloseEvent *event)
{

    if(ui2) ui2->close();
    event->accept();
}
