#ifndef OPENCVALGOR_H
#define OPENCVALGOR_H

#include "cvcommon.h"

#include <set>
#include <QImage>

struct MImage {
    cv::Mat mat;
    int fg_height;
    int fg_width;
    MImage(){
        mat = cv::Mat();
        fg_height = 0;
        fg_width = 0;
    }
};

struct IndexInfoForAlter
{
    int flags;
    int index;
    int begin;
    int end;
    IndexInfoForAlter(int _index,int _begin, int _end,int _flags) {
        flags = _flags;
        index = _index;
        begin = _begin;
        end = _end;
    }
    bool operator < (const IndexInfoForAlter& right) const {
        if(flags==right.flags && index == right.index && begin == right.begin && end == right.end ) return false; // to avoid all true
        return (flags < right.flags || index < right.index|| begin < right.begin|| end < right.end );
    }
};


class OpencvAlgor : public CvCommon
{
public:
    OpencvAlgor();
    virtual ~OpencvAlgor();

/*algors*/
public:
     std::vector<std::pair<int,int>> HistRflect(cv::Mat &mat,int limitSpace,int limitTrue);
     std::vector<std::pair<int,int>> VertRflect(cv::Mat &mat,int limitSpace,int limitTrue);
     bool AdjustImageSize(cv::Mat &src, uint maxLen, double &ratio , MImage &imstruct);
     QImage MImageToBinQImage(MImage &imstruct, uint8_t thresHold);
     void RemoveSpace(cv::Mat &mat);
     void FilterIslandPoints(cv::Mat &mat, uchar limitPoints);
     void EasySmooth(cv::Mat &mat);

private:
     std::vector<std::pair<int,int>> _hist_or_vert_rflect(cv::Mat &mat, int limitSpace,int limitTrue,CvCommon::AXIS axis);
     void _auto_detect_threshold(std::vector<int>& vect,int &limitSpace,int &limitTrue);
     QImage _mat_to_qimage(const cv::Mat& mat);
     void _imstruct_mat2bin(MImage &imstruct,uint8_t thresHold);
     void _mat_memset(cv::Mat &src,int col_row,int begin,int end, uchar dvalue,int flags) /*flags 0:hist and col_row as col, 1:vert and col_row as row*/;
     bool _is_all_black(cv::Mat &mat);
};

#endif // OPENCVALGOR_H
