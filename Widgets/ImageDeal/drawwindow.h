#ifndef DRAWWINDOW_H
#define DRAWWINDOW_H

/*
 * 优化
1.优化了信号槽，部分不需要信号槽方式的非耗时操作，采用了直接调用 槽函数/成员方法 形式
2.固定形式布局，改成栅格形式布局，窗体方法缩小美观程度不受影响
*/

#include "CvArg/cvapi.h"
#include "DrawTools/shapedeco.h"
#include "Widgets/figurewidget.h"
#include "tesseract_ocr/tessapi.h"
#include "utils/movetext.h"
#include "ml/apiproxy.h"

#include <QMainWindow>
#include <QWidget>
#include <QAction>
#include <QToolBar>
#include <QButtonGroup>
#include <QFileDialog>
#include <QPalette>
#include <QScrollBar>
#include <QActionGroup>
#include <QLabel>
#include <QThread>


namespace Ui {
class DrawWindow;
}

class DrawWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit DrawWindow(QWidget *parent = 0);
    ~DrawWindow();

signals:
    void send_ml_result(ResultType);

    void send_tess_result(QString);

public slots:
    void show_message(QString msg);

    void set_sub_im(QPoint p1, QPoint p2);

    void set_tess_im(QPoint p1, QPoint p2);

private slots:
    void _menu_action_response(QAction* action);

    void on_ThresBar_valueChanged(int value);

    void _load_image();

    void _update_backgroud();

    void _set_scrolarea_values(int &x, int &y, int &srcValue, int &curValue);

    void on_Btn_ReloadIm_clicked();

    void on_Btn_Up_clicked();

    void on_Btn_Down_clicked();

    void _update_win_name(QString msg);

    void on_Btn_Cut_clicked();

    void on_Btn_Recn_clicked();

private:
    Ui::DrawWindow *ui;
    FigureWidget* _fig_widget;
    QToolBar* bar;
    QLabel* _lb_tips;

    QThread* _thread;
    MoveText* _work;

    CvApi _cv_api;
    TessApi* _tess_api;
    ApiProxy* _ml_api;

    int _threshold;
    cv::Mat _src_im;
    cv::Mat _src_roi;
    MImage _m_image;
    int _im_size_length = 1000; // c++ 11
    double _ratio = 1.0;

    void _form1_grid_setting();
    void _form2_grid_setting();
    void _self_gird_setting();
    void _menubar_setting();

    bool _get_scrolarea_values(int &x,int &y);

};

#endif // DRAWWINDOW_H
