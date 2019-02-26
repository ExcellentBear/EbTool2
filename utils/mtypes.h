#ifndef MTYPES_H
#define MTYPES_H

#include "opencv2/core.hpp"
#include <QString>
#include <vector>

struct ResultModel
{
    int row;
    int col;
    QString value;
    ResultModel(int _row, int _col,QString _value) {
        row = _row;
        col = _col;
        value = _value;
    }
};

struct IndexMat
{
    int row;
    int col;
    cv::Mat mtx;
    IndexMat(int _row,int _col,const cv::Mat& m ) {
        row = _row;
        col = _col;
        mtx = m.clone();
    }
};

struct IndexMatList
{
    int row;
    int col;
    std::vector<cv::Mat> mat_list;
    IndexMatList(int r,int c) {
        row = r;
        col = c;
    }
    ~IndexMatList(){
        mat_list.clear();
    }
};

typedef QList<ResultModel> ResultType;
typedef std::vector<IndexMat> DoubleCutsMats;
typedef std::vector<IndexMatList> TreeCutsMats;


#endif // MTYPES_H

