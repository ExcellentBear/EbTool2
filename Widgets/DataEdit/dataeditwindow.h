#ifndef DATAEDITWINDOW_H
#define DATAEDITWINDOW_H

#include "utils/mtypes.h"
#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include <QMenu>
#include <QAction>
#include <QPoint>
#include <QClipboard>
#include <QFileDialog>
#include <QFile>
#include <QRegExp>
#include <QTextStream>
#include <QMetaType>

namespace Ui {
class DataEditWindow;
}

class DataEditWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit DataEditWindow(QWidget *parent = 0);
    ~DataEditWindow();

public slots:
    void RecieveResultData(ResultType resultData);
    void RecieveTessResult(QString tessStr);

private:
    void _init_grid();
    QMenu *_right_click_menu;
    QPoint _right_click_point;
    QAction* _add_head_row;
    QAction* _add_head_col;
    QAction* _add_col;
    QAction* _add_row;
    QAction* _del_col;
    QAction* _del_row;
    QAction* _copy_data;
    QAction* _paste_data;
    QAction* _copy_from_tesseract;
    void _init_table_widget_actions();

private slots:
    void _add_tablewidget_col();
    void _add_tablewidget_row();
    void _del_tablewidget_col();
    void _del_tablewidget_row();
    void _add_tablewidget_headcol();
    void _add_tablewidget_headrow();
    void _copy_tablewidget_data();
    void _paste_tablewidget_data();
    void _copy_tess_to_tablewidget();
    void show_tablewidget_menu(QPoint point);

    void on_Btn_CopyTessResult_clicked();

    void on_Btn_SaveResult_clicked();

private:
    Ui::DataEditWindow *ui;
};

#endif // DATAEDITWINDOW_H
