#include "drawwindow.h"
#include "ui_drawwindow.h"


DrawWindow::DrawWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DrawWindow)
{
    ui->setupUi(this);

    setStyleSheet("QLabel{color:blue}"
                  "QLineEdit{color:green; border-radius: 5px 5px; }"
                  "QPushButton{color:green; border: none}"
                  "QToolBar::item{backgound-color: rgba(255, 255, 255, 0)}"
                  );

    _ml_api = new ApiProxy();

    _thread = new QThread(0);
    _work = new MoveText(0);
    _work->moveToThread(_thread);

    connect(_thread, SIGNAL(started()), _work, SLOT(start_work()),Qt::QueuedConnection);
    connect(_work,SIGNAL(send_cur_tip(QString)),this, SLOT(_update_win_name(QString)));
    _thread->start();

    _cv_api=CvApi();
    _tess_api= new TessApi();
    _m_image = MImage();
    _im_size_length = 1000;

    ui->LbThresV->setText(QString::number(ui->ThresBar->value()));

    _fig_widget = new FigureWidget(ShapeFactory::DRAW_H_LINE, ui->scrollArea);
    _fig_widget->setGeometry(0,0,_im_size_length,_im_size_length);

    ui->scrollArea->setWidget(_fig_widget);
    ui->scrollArea->setWidgetResizable(false);
    ui->scrollArea->setAutoFillBackground(true);

    _threshold = ui->ThresBar->value();

    _form1_grid_setting();
    _form2_grid_setting();
    _self_gird_setting();
    _menubar_setting();

    connect(bar, SIGNAL(actionTriggered(QAction*)), this, SLOT(_menu_action_response(QAction*)));
    connect(bar->actions().last(), SIGNAL(triggered()), this, SLOT(_load_image()));
    connect(_fig_widget,SIGNAL(send_subim_rect(QPoint,QPoint)),this, SLOT(set_sub_im(QPoint,QPoint)));
    connect(_fig_widget,SIGNAL(send_tessim_rect(QPoint,QPoint)),this,SLOT(set_tess_im(QPoint,QPoint)));
}

DrawWindow::~DrawWindow()
{
    //父类指向该类的对象均会自动释放
    _work->exit_work();
    _thread->quit();
    _thread->wait();
    delete _thread;
    delete _work;
    delete _ml_api;
    delete ui;
}

void DrawWindow::_form1_grid_setting()
{
    ui->form1GridLayout -> setColumnStretch(0, 6);
    ui->form1GridLayout -> setColumnStretch(1, 1);
    ui->form1GridLayout -> setColumnStretch(1, 1);
}

void DrawWindow::_form2_grid_setting()
{
    ui->gridLayout->setColumnStretch(0,5);
    ui->gridLayout->setColumnStretch(1,2);
    ui->gridLayout->setColumnStretch(2,1);
    ui->gridLayout->setColumnStretch(3,2);
    ui->gridLayout->setColumnStretch(4,2);
    ui->gridLayout->setColumnStretch(5,2);
    ui->gridLayout->setColumnStretch(6,1);
    ui->gridLayout->setColumnStretch(7,2);
    ui->gridLayout->setColumnStretch(8,5);
}

void DrawWindow::_self_gird_setting()
{
    _lb_tips = new QLabel(this);
    _lb_tips->setStyleSheet("font-size:20px;color:red");
    ui->statusbar->addWidget(_lb_tips);
    ui->gridLayout_2->setRowStretch(0,2);
    ui->gridLayout_2->setRowStretch(1,20);
    ui->gridLayout_2->setRowStretch(2,2);
}

void DrawWindow::_menubar_setting()
{
    bar = new QToolBar(this); //无需delete 会随着父类消亡
    QActionGroup* _group = new QActionGroup(this);
    _group->setExclusive(true); // only one can be selected at the same time

    QAction *hline = new QAction("hline",this);
    hline->setIcon(QIcon(":/icons/histline.png"));
    hline->setCheckable(true);
    hline->setChecked(true);
    bar->addAction(_group->addAction(hline));

    QAction *wline = new QAction("wline", this);
    wline->setIcon(QIcon(":/icons/vertline.png"));
    wline->setCheckable(true);
    bar->addAction(_group->addAction(wline));

    QAction *select = new QAction("select", this);
    select->setIcon(QIcon(":/icons/rect.png"));
    select->setCheckable(true);
    bar->addAction(_group->addAction(select));

    QAction *tess = new QAction("tess",this);
    tess->setIcon(QIcon(":/icons/tess.png"));
    tess->setCheckable(true);
    bar->addAction(_group->addAction(tess));

    QAction *erase = new QAction("erase", this);
    erase->setIcon(QIcon(":/icons/erase.png"));
    erase->setCheckable(true);
    bar->addAction(_group->addAction(erase));

    QAction *undraw = new QAction("undraw", this);
    undraw->setIcon(QIcon(":/icons/back.png")); 
    bar->addAction(undraw);

    QAction *cleara = new QAction("cleara",this);
    cleara->setIcon(QIcon(":/icons/clearall.png")); 
    bar->addAction(cleara);

    QAction *clearb = new QAction("clearb", this);
    clearb->setIcon(QIcon(":/icons/clearpart.png"));
    bar->addAction(clearb);

    QAction *loadfile = new QAction("loadfile", this);
    loadfile->setIcon(QIcon(":/icons/file.png"));
    bar->addAction(loadfile);
    this->addToolBar(bar);
}

void DrawWindow::_menu_action_response(QAction *action)
{
    QString name = action->text();
    if(name=="hline"){
        _fig_widget->setCurrentMenu(ShapeFactory::DRAW_H_LINE);
    }
    else if(name=="wline"){
        _fig_widget->setCurrentMenu(ShapeFactory::DRAW_W_LINE);
    }
    else if (name=="select"){
        _fig_widget->setCurrentMenu(ShapeFactory::PICK_OUT_RECT);
    }
    else if (name=="tess"){
        _fig_widget->setCurrentMenu(ShapeFactory::TESS_READ_RECT);
    }
    else if(name=="erase"){
        _fig_widget->setCurrentMenu(ShapeFactory::ERASE_RECT);
    }
    else if(name=="undraw"){
        _fig_widget->removeLastDrew();
    }
    else if(name=="cleara"){
        _fig_widget->clearShapeList();
    }
    else if(name=="clearb"){
        _fig_widget->removeAllAutoGen();
    }

}

void DrawWindow::show_message(QString msg)
{
    _lb_tips->setText(msg);
}

void DrawWindow::on_ThresBar_valueChanged(int value)
{
    if(abs(_threshold - value)>=5) {
       _threshold = value;
      ui->LbThresV->setText(QString::number(_threshold));
      if(_cv_api.AdjustImageSize(_src_roi,_im_size_length,_ratio,_m_image)){
                  int x=0,y=0;
                  bool status = _get_scrolarea_values(x,y);
                  _update_backgroud();
                  if(status) _set_scrolarea_values(x,y,_im_size_length,_im_size_length);
              }
    }
}

void DrawWindow::_load_image()
{
    QFileDialog *fileDialog = new QFileDialog(this);
    fileDialog->setWindowTitle("Load Image");
    fileDialog->setFileMode(QFileDialog::ExistingFile);
    fileDialog->setViewMode(QFileDialog::Detail);
    fileDialog->setNameFilter("tr(Image (*.bpm *.jpg *.png *.BPM *.JPG *.PNG)");
    QString filename("");
    QStringList filenames;
    if(fileDialog->exec()){
        filenames = fileDialog->selectedFiles();
        if(filenames.size() != 1){
             _lb_tips->setText(tr("no file is selected or more than one file are selected"));
             fileDialog->deleteLater();
             return;
            }
        }else{
            _lb_tips->setText(tr("please import a image file"));
            fileDialog->deleteLater();
            return;
        }
        filename = filenames.first();
        _fig_widget->clearShapeList();
        _src_im = _cv_api.loadImage(filename,0);
        _src_roi = _src_im.clone();
        _im_size_length = 1000;
        if(_cv_api.AdjustImageSize(_src_roi,_im_size_length, _ratio, _m_image)){
            _update_backgroud();
            int x=0, y=0;
            _set_scrolarea_values(x,y,_im_size_length,_im_size_length);
        }
}

void DrawWindow::_update_backgroud()
{
    _fig_widget->resize(_im_size_length,_im_size_length);
    _fig_widget->setAutoFillBackground(true);
    _lb_tips->setText("");
    _fig_widget->clearShapeList();
    if(_m_image.mat.empty()) return;
    QImage im = _cv_api.ToQimage(_m_image, _threshold);
    QPalette palette;
    palette.setBrush(QPalette::Background,QBrush(im));
    _fig_widget->setPalette(palette);
    update();
}

void DrawWindow::_update_win_name(QString msg)
{
    QString name = QString("EbTool----");
    name += msg;
    this->setWindowTitle(name);
}
//使得画面扩大缩小之后，滚动条相对图片位置(随比例变化)不变
void DrawWindow::_set_scrolarea_values(int &x, int &y, int &srcValue, int &curValue)
{
    QScrollBar *hbar = ui->scrollArea->horizontalScrollBar();
    QScrollBar *wbar = ui->scrollArea->verticalScrollBar();
    if(NULL == hbar || NULL == wbar) return ;
    if(srcValue == curValue){
        hbar->setValue(x);
        wbar->setValue(y);
    }else{
        double ratio =  (double) curValue/(double) srcValue;
        x *= ratio;  //int
        y *= ratio;  //int
        hbar->setValue(x);
        wbar->setValue(y);
    }
}

bool DrawWindow::_get_scrolarea_values(int &x, int &y)
{
    QScrollBar *hbar = ui->scrollArea->horizontalScrollBar();
    QScrollBar *wbar = ui->scrollArea->verticalScrollBar();
    if(NULL == hbar || NULL == wbar) return false;
    x = hbar->value();
    y = wbar->value();
    return true;
}

void DrawWindow::on_Btn_ReloadIm_clicked()
{
    _im_size_length = 1000;
    _src_roi = _src_im.clone();
    if(_cv_api.AdjustImageSize(_src_roi,_im_size_length,_ratio,_m_image)){
            int x=0,y=0;
            _update_backgroud();
            _set_scrolarea_values(x,y,_im_size_length,_im_size_length);
        }
}

void DrawWindow::on_Btn_Up_clicked()
{
        int srcValue = _im_size_length;
        if(srcValue > 10000) return;
        _im_size_length += 200;
        if(_cv_api.AdjustImageSize(_src_roi,_im_size_length,_ratio,_m_image)){
            int x=0,y=0;
            bool status = _get_scrolarea_values(x,y);
            _update_backgroud();
            if(status) _set_scrolarea_values(x,y,srcValue,_im_size_length);
        }
}

void DrawWindow::on_Btn_Down_clicked()
{
    int srcValue = _im_size_length;
    if(srcValue < 200) return;
    _im_size_length -= 200;
    if(_cv_api.AdjustImageSize(_src_roi,_im_size_length,_ratio,_m_image)){
        int x=0,y=0;
        bool status = _get_scrolarea_values(x,y);
        _update_backgroud();
        if(status) _set_scrolarea_values(x,y,srcValue,_im_size_length);
    }
}

void DrawWindow::set_sub_im(QPoint p1, QPoint p2)
{
    if(_src_roi.empty()) return;
    if(p1.x() >= p2.x() || p1.y() >= p2.y() ) return;
    cv::Rect rect = cv::Rect(p1.x()/_ratio,p1.y()/_ratio,(p2.x()-p1.x())/_ratio,(p2.y()-p1.y())/_ratio);
    cv::Mat dst;
    if(_cv_api.CutOutImage(_src_roi, dst, rect)){
        _src_roi = dst.clone();
        if(_cv_api.AdjustImageSize(_src_roi,_im_size_length,_ratio,_m_image)){
            _update_backgroud();
        }
    }
}

void DrawWindow::set_tess_im(QPoint p1, QPoint p2)
{
    if(_src_roi.empty()) return;
    if(p1.x() >= p2.x() || p1.y() >= p2.y() ) return;
    cv::Rect rect = cv::Rect(p1.x()/_ratio,p1.y()/_ratio,(p2.x()-p1.x())/_ratio,(p2.y()-p1.y())/_ratio);
    cv::Mat dst;
    if(_cv_api.CutOutImage(_src_roi,dst,rect)){
        QString res = _tess_api->getOcrResult(dst);
        if(res.endsWith('\n')) res = res.mid(0,res.length()-1);
         emit send_tess_result(res);
        }
}

void DrawWindow::on_Btn_Cut_clicked()
{
    if(_m_image.mat.empty()) return;
    if(_m_image.mat.rows < 4 || _m_image.mat.cols < 4) return;
    std::vector<std::pair<int,int>> hist =_cv_api.HistRflect(_m_image.mat,4,-1);
    std::vector<std::pair<int,int>> vert =_cv_api.VertRflect(_m_image.mat,-1,-1);
    for(size_t i=0; i < hist.size(); i++){
        _fig_widget->addAutoGenShape(ShapeFactory::DRAW_H_LINE,QPoint(hist.at(i).first,0),QPoint(hist.at(i).first,_m_image.mat.rows-1));
        _fig_widget->addAutoGenShape(ShapeFactory::DRAW_H_LINE,QPoint(hist.at(i).second,0),QPoint(hist.at(i).second,_m_image.mat.rows-1));
    }
    for(size_t i=0; i <vert.size();i++){
       _fig_widget->addAutoGenShape(ShapeFactory::DRAW_W_LINE,QPoint(0,vert.at(i).first),QPoint(_m_image.mat.cols-1,vert.at(i).first));
       _fig_widget->addAutoGenShape(ShapeFactory::DRAW_W_LINE,QPoint(0,vert.at(i).second),QPoint(_m_image.mat.cols-1,vert.at(i).second));
    }
    _fig_widget->update();
}

void DrawWindow::on_Btn_Recn_clicked()
{
   QList<SharePtrShape> shapeList =  _fig_widget->getShapeList();
   if(shapeList.empty()) return;
   TreeCutsMats out;
   std::vector<int> xhls,ywls;
   int avgWidth=0, avgHeight = 0;
   for(int i = 0; i < shapeList.size(); i++){
      if(shapeList.at(i)->getMenuType() == ShapeFactory::DRAW_W_LINE){
                ywls.push_back(shapeList.at(i)->getStartPoint().y());
      }
      else if(shapeList.at(i)->getMenuType()==ShapeFactory::DRAW_H_LINE){
                xhls.push_back(shapeList.at(i)->getStartPoint().x());
      }
   }
   if(xhls.size()<2 || (xhls.size()&1) || ywls.size()<2 || (ywls.size() & 1) ) return;
   _cv_api.CutOutAllDigit(xhls,ywls,_m_image.mat,avgHeight,avgWidth,out);
  _cv_api.StandardPics(out,avgWidth,avgHeight);
  ResultType result = _ml_api->getPredictResult(out);
  emit send_ml_result(result);
}
