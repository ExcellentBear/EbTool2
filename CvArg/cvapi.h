#ifndef CVAPI_H
#define CVAPI_H

#include "opencvalgor.h"
#include "utils/mtypes.h"


class CvApi
{
public:
    CvApi();
    ~CvApi();

public:
    void HistSplitMat(cv::Mat &src,std::vector<cv::Mat> &dst, int limitWhite);

    void SplitSubMats(DoubleCutsMats &dmats, TreeCutsMats &tmats);

    int DropFall(cv::Mat &src, cv::Mat &left, cv::Mat &right, int x);

    std::vector<cv::Mat> RecuDropFall(cv::Mat &mat,int thres);

    void CutOutAllDigit(std::vector<int> &xhls,std::vector<int> &ywls,cv::Mat &mat,int &avgHeight,int &avgWidth,std::vector<IndexMatList> &out);

    void StandardPics(TreeCutsMats &treeCutsMats, int avgWidth, int avgHeight);

    cv::Mat MatTo32X32(cv::Mat &src, int flags = cv::INTER_LINEAR);

    cv::Mat ExtendWidthOrHeight(cv::Mat &src);

    bool IsIgnoreAble(cv::Mat &mat,int avgHight, int avgWidth);

    cv::Mat loadImage(QString& file, int flag);

    bool AdjustImageSize(cv::Mat& mat,int length, double& ratio, MImage& im);

    QImage ToQimage(MImage& im, int& thres);

    bool CutOutImage(cv::Mat& src,cv::Mat& dst,cv::Rect rect);

    std::vector<std::pair<int,int>> HistRflect(cv::Mat &mat, int x, int y);
    std::vector<std::pair<int,int>> VertRflect(cv::Mat &mat, int x, int y);

private:
    int _df_find_fpoint(cv::Mat &mat);
    cv::Point _get_point(std::vector<cv::Point> &trace, int &row);
    std::pair<int,int> _minmax_column_value(std::vector<cv::Point> &trace);
    void _get_avg_length(std::vector<IndexMatList> &imtx, int &avgWidth, int &avgHeight);

    OpencvAlgor _algor;
};

#endif // CVAPI_H
