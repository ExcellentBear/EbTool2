#ifndef CVCOMMON_H
#define CVCOMMON_H

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"

#include <vector>

//添加命名空间，限制namespace cv作用于全局

using std::vector;

class CvCommon
{
public:
    CvCommon();
    virtual ~CvCommon();

    enum AXIS{HIST, VERT};
    bool ResizeImage(cv::Mat &src, cv::Mat &dst,cv::Size size, int flags = cv::INTER_LINEAR);
    bool CutOutImage(cv::Mat &src,cv::Mat &dst, cv::Rect &roi) /*dst does not allocate new memmory*/;
    cv::Mat MLoadImage(const char* fileName,int flags);
    std::vector<int> CountNoZeroAlongAxis(cv::Mat &mat, CvCommon::AXIS axis) /*enum type OpencvCommon::HIST, OpencvCommon::VERT */;
    std::vector<int> CountZeroAlongAxis(cv::Mat &mat, CvCommon::AXIS axis) /*enum type OpencvCommon::HIST, OpencvCommon::VERT */;
    bool UMat2FMat(cv::Mat &src,cv::Mat &dst);
    cv::Mat MatToOneRow(cv::Mat &src);
    bool RotateMat(cv::Mat &src, cv::Mat &dst, double degree, cv::Scalar bordFill);
    void VectorTo2FMat(std::vector<cv::Mat> &mats, cv::Mat &dst);


private:
    bool _isin_range(cv::Mat &mat,cv::Rect &roi);
    bool _is_resizeable(cv::Mat &mat,cv::Size &size);

};



#endif // CVCOMMON_H
