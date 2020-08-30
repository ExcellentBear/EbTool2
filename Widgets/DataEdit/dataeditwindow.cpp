#include "dataeditwindow.h"
#include "ui_dataeditwindow.h"


DataEditWindow::DataEditWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DataEditWindow)
{
    qRegisterMetaType<ResultType>("ResultType");
    ui->setupUi(this);
//    m_index_ = 0;
//    m_timer.setSingleShot(false);
//    QString title("EBTool-Result");
//    connect(&m_timer, &QTimer::timeout, this, [&, title](){setWindowTitle(title.mid(m_index_++ % title.length()));});
//    m_timer.start(100);
    setStyleSheet("QLabel{color:blue} QLineEdit{color:green; border-radius: 5px 5px; } QPushButton{color:green; border: none}");

    _init_grid();
    _init_table_widget_actions();
}

DataEditWindow::~DataEditWindow()
{
    delete ui;
}

void DataEditWindow::_init_grid()
{
    ui->statusbar->addWidget(ui->Lb_Tips);
    ui->gridLayout_2->setRowStretch(0,5);
    ui->gridLayout_2->setRowStretch(1,1);
    ui->gridLayout->setRowStretch(0,1);
    ui->gridLayout->setRowStretch(1,1);
    ui->gridLayout->setColumnStretch(0,1);
    ui->gridLayout->setColumnStretch(1,2);
    ui->gridLayout->setColumnStretch(2,1);
}

void DataEditWindow::_init_table_widget_actions()
{
    ui->Tbw_Result->setContextMenuPolicy(Qt::CustomContextMenu);
    _right_click_menu = new QMenu(ui->Tbw_Result);
    _right_click_point = QPoint(0,0);
    _add_col = new QAction("add col",ui->Tbw_Result);
    _del_col = new QAction("del col",ui->Tbw_Result);
    _add_row = new QAction("add row",ui->Tbw_Result);
    _del_row = new QAction("del row",ui->Tbw_Result);
    _add_head_col = new QAction("add head col",ui->Tbw_Result);
    _add_head_row = new QAction("add head row",ui->Tbw_Result);
    _copy_data = new QAction("copy",ui->Tbw_Result);
    _paste_data = new QAction("paste",ui->Tbw_Result);
    _copy_from_tesseract = new QAction("copy from tess",ui->Tbw_Result);
    _right_click_menu->addAction(_add_col);
    _right_click_menu->addAction(_del_col);
    _right_click_menu->addAction(_add_row);
    _right_click_menu->addAction(_del_row);
    _right_click_menu->addAction(_add_head_col);
    _right_click_menu->addAction(_add_head_row);
    _right_click_menu->addAction(_copy_data);
    _right_click_menu->addAction(_paste_data);
    _right_click_menu->addAction(_copy_from_tesseract);
    connect(_add_col,SIGNAL(triggered()),this,SLOT(_add_tablewidget_col()));
    connect(_del_col,SIGNAL(triggered()),this,SLOT(_del_tablewidget_col()));
    connect(_add_row,SIGNAL(triggered()),this,SLOT(_add_tablewidget_row()));
    connect(_del_row,SIGNAL(triggered()),this,SLOT(_del_tablewidget_row()));
    connect(_add_head_row,SIGNAL(triggered()),this,SLOT(_add_tablewidget_headrow()));
    connect(_add_head_col,SIGNAL(triggered()),this,SLOT(_add_tablewidget_headcol()));
    connect(_copy_data,SIGNAL(triggered()),this,SLOT(_copy_tablewidget_data()));
    connect(_paste_data,SIGNAL(triggered()),this,SLOT(_paste_tablewidget_data()));
    connect(_copy_from_tesseract,SIGNAL(triggered()),this,SLOT(_copy_tess_to_tablewidget()));
    connect(ui->Tbw_Result,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(show_tablewidget_menu(QPoint)));
}

void DataEditWindow::show_tablewidget_menu(QPoint pos)
{
    _right_click_menu->move(cursor().pos());
    _right_click_menu->show();
    _right_click_point = pos;
}

void DataEditWindow::_add_tablewidget_headcol()
{
    ui->Tbw_Result->insertColumn(0);
}

void DataEditWindow::_add_tablewidget_headrow()
{
    ui->Tbw_Result->insertRow(0);
}

void DataEditWindow::_add_tablewidget_col()
{
    if(_right_click_point.x()==0 && _right_click_point.y()==0) return;
    QModelIndex index = ui->Tbw_Result->indexAt(_right_click_point);
    int col = index.column();
    ui->Tbw_Result->insertColumn(col+1);
}

void DataEditWindow::_del_tablewidget_col()
{
    if(_right_click_point.x()==0 && _right_click_point.y()==0) return;
    QModelIndex index = ui->Tbw_Result->indexAt(_right_click_point);
    int col = index.column();
    ui->Tbw_Result->removeColumn(col);
}

void DataEditWindow::_add_tablewidget_row()
{
     if(_right_click_point.x()==0 && _right_click_point.y()==0) return;
     QModelIndex index = ui->Tbw_Result->indexAt(_right_click_point);
     int row = index.row();
     ui->Tbw_Result->insertRow(row+1);
}

void DataEditWindow::_del_tablewidget_row()
{
    if(_right_click_point.x()==0 && _right_click_point.y()==0) return;
    QModelIndex index = ui->Tbw_Result->indexAt(_right_click_point);
    int row = index.row();
    ui->Tbw_Result->removeRow(row);
}

void DataEditWindow::_copy_tablewidget_data()
{
    QClipboard *clipboard = QApplication::clipboard();
    QList<QTableWidgetSelectionRange> items = ui->Tbw_Result->selectedRanges();
    if(items.empty()) {
        clipboard->setText("");
        return;
    }
    QString res;
    for(int i=items.at(0).topRow();i<=items.at(0).bottomRow();i++){
        for(int j=items.at(0).leftColumn();j<=items.at(0).rightColumn();j++){
            if( ui->Tbw_Result->item(i,j)){
                    QString tmp = ui->Tbw_Result->item(i,j)->text();
                    tmp = tmp.replace(QChar('\n'),QChar(' ')).replace(QChar('\t'),QChar(' '));
                    res += tmp;
            }
            if(j<items.at(0).rightColumn()) res += "\t";
        }
        if(i<items.at(0).bottomRow()) res += "\r\n";
    }
    clipboard->setText(res);
}

void DataEditWindow::_copy_tess_to_tablewidget()
{
    if(_right_click_point.x()==0 && _right_click_point.y()==0) return;
        QString txt = ui->Edt_TessResult->text();
        if(txt==QString("")) return;
        for(int i=0;i<txt.length();i++){
            if( txt.at(i)==QChar('\t') || txt.at(i)==QChar('\n')) txt[i]=QChar(32);
        }
        QModelIndex index = ui->Tbw_Result->indexAt(_right_click_point);
        int row=index.row();
        int col = index.column();
        ui->Tbw_Result->setItem(row,col,new QTableWidgetItem(txt));
}

void DataEditWindow::_paste_tablewidget_data()
{
    QClipboard *clipboard = QApplication::clipboard();
    QString txt = clipboard->text();
    QModelIndex index = ui->Tbw_Result->indexAt(_right_click_point);
    int row=index.row(),col=index.column();
    QStringList lines = txt.split(QString("\n"));
    for(int i=0;i<lines.size();i++){
        QString line = lines.at(i);
        QStringList eles = line.split(QString("\t"));
        for(int j=0;j<eles.size();j++){
            QString ele = eles.at(j).trimmed();
            if((col+j) >= ui->Tbw_Result->columnCount()) ui->Tbw_Result->insertColumn(ui->Tbw_Result->columnCount());
            if((row+i) >= ui->Tbw_Result->rowCount()) ui->Tbw_Result->insertRow(ui->Tbw_Result->rowCount());
            ui->Tbw_Result->setItem(row+i,col+j,new QTableWidgetItem(ele));
        }
    }
}

void DataEditWindow::on_Btn_CopyTessResult_clicked()
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(ui->Edt_TessResult->text());
}

void DataEditWindow::on_Btn_SaveResult_clicked()
{
    int row = ui->Tbw_Result->rowCount();
    int col = ui->Tbw_Result->columnCount();
    if(row==1 && col==1){
        ui->Lb_Tips->setText("error:table only have one value");
        return;
    }
    if(row==0 || col==0){
        ui->Lb_Tips->setText("error:table is empty");
        return;
    }
    QString filename = ui->Edt_DefaultFileName->text();
    filename = filename.remove(QRegExp("\\s"));
    if(filename.length()==0){
        ui->Lb_Tips->setText("please input filename");
        return;
    }
    if(!filename.toLower().endsWith(".csv")){
        filename += ".csv";
    }
    QString full = QFileDialog::getSaveFileName(this,tr("Save File"),filename);
    QFile file(full);
    if(file.exists()){
        ui->Lb_Tips->setText("file exists,please refer another name or save in another dir");
        return;
    }
    if (!file.open(QIODevice::ReadWrite| QIODevice::Text)) {
        ui->Lb_Tips->setText("failed to create csv file");
        return;
    }
    QTextStream in(&file);
    for(int i=0;i<row;i++){
        for(int j=0;j<col;j++){
           QString val;
           if(!ui->Tbw_Result->item(i,j)) val="";
           else  val = ui->Tbw_Result->item(i,j)->text();
           for(int i=0;i<val.length();i++){
                if(val.at(i)==QChar('\t')||val.at(i)==QChar('\n')||val.at(i)==QChar(',')) val[i] = QChar(' ');
              }
            if(j != col-1) in<<val<<",";
            else in<<val<<"\n";
        }
    }
    file.close();
}

void DataEditWindow::RecieveResultData(ResultType resultData)
{
    if(resultData.empty()) return;
    ui->Tbw_Result->clear();
    ui->Tbw_Result->setRowCount(resultData.back().row+1);
    ui->Tbw_Result->setColumnCount(resultData.back().col+1);
    for(int i=0 ; i< resultData.size();i++){
        ResultModel item = resultData.at(i);
        ui->Tbw_Result->setItem(item.row,item.col,new QTableWidgetItem(item.value));
    }
}

void DataEditWindow::RecieveTessResult(QString tessStr)
{
    ui->Edt_TessResult->setText(tessStr);
}
