#include "cvcommon.h"

CvCommon::CvCommon()
{

}

CvCommon::~CvCommon()
{

}

bool CvCommon::ResizeImage(cv::Mat &src, cv::Mat &dst, cv::Size size, int flags)
{
    if(!_is_resizeable(src,size)) return false;
    cv::resize(src,dst,size,flags);
    return true;
}

bool CvCommon::CutOutImage(cv::Mat &src, cv::Mat &dst, cv::Rect &roi)
{
    if(!_isin_range(src,roi)) return false;
    dst = src(roi);
    return true;
}

cv::Mat CvCommon::MatToOneRow(cv::Mat &src)
{
    return src.reshape(0,1);
}

bool CvCommon::UMat2FMat(cv::Mat &src, cv::Mat &dst)
{
    if(src.rows<=0||src.cols<=0) return false;
    dst = cv::Mat(src.rows,src.cols,CV_32FC1);
    for(int j=0;j<src.rows;j++){
        float* arr = dst.ptr<float>(j);
        for(int i=0;i<src.cols;i++){
            arr[i] = src.ptr<uchar>(j)[i] <126 ? -0.5f:0.5f;
        }
    }
    return true;
}

std::vector<int> CvCommon::CountNoZeroAlongAxis(cv::Mat &mat, CvCommon::AXIS axis)
{
    std::vector<int> result;
    int size;
    if(axis==CvCommon::HIST){
       size = mat.cols;
       int* hist = new int[size]();
       for(int j=0; j<mat.rows;j++){
              uchar *data = mat.ptr<uchar>(j);
              for(int i=0;i<size;i++){
                  if(data[i]-0>1e-5) hist[i] += 1;
              }
          }
       for(int k=0;k<size;k++) result.push_back(hist[k]);
       delete[] hist;
    }else{
       size = mat.rows;
       int* vert = new int[size]();
       for(int j=0;j<size;j++){
              uchar* data = mat.ptr<uchar>(j);
              for(int i=0;i<mat.cols;i++){
                  if(data[i]-0>1e-5) vert[j] += 1;
              }
          }
       for(int k=0;k<size;k++) result.push_back(vert[k]);
       delete[] vert;
    }
    return result;
}

std::vector<int> CvCommon::CountZeroAlongAxis(cv::Mat &mat, CvCommon::AXIS axis)
{
    std::vector<int> result = CountNoZeroAlongAxis(mat,axis);
    for(int i=0;i<result.size();i++){
        if(axis==CvCommon::HIST) result[i] = mat.rows - result.at(i);
        else result[i] = mat.cols - result.at(i);
    }
    return result;
}

cv::Mat CvCommon::MLoadImage(const char *fileName, int flags=CV_LOAD_IMAGE_GRAYSCALE)
{
    return cv::imread(fileName,flags);
}

bool CvCommon::RotateMat(cv::Mat &src, cv::Mat &dst, double degree, cv::Scalar bordFill)
{
    if(src.empty()) return false;
    int chnel = src.channels();
    if(!(chnel==1 || chnel==3)) return false;
    cv::Point2f center(src.cols/2.0f,src.rows/2.0f);
    float angle = 0.0f - (float)degree;
    cv::Mat rot = cv::getRotationMatrix2D(center,angle,1);
    cv::Rect bbox = cv::RotatedRect(center,src.size(),angle).boundingRect();
    rot.at<double>(0, 2) += bbox.width / 2.0 - center.x;
    rot.at<double>(1, 2) += bbox.height / 2.0 - center.y;
    cv::warpAffine(src,dst,rot,bbox.size(),1,0,bordFill);
    return true;
}

void CvCommon::VectorTo2FMat(std::vector<cv::Mat> &mats, cv::Mat &dst)
{
    if(mats.empty()) return;
    std::vector<cv::Mat>::iterator ite;
    for(ite = mats.begin();ite != mats.end();){
        if(ite->empty()) mats.erase(ite);
        else ite++;
    }
    int rows = (int)mats.size();
    int cols = mats.at(0).rows*mats.at(0).cols;
    dst = cv::Mat(rows,cols,CV_32FC1);
    for(int i=0;i<rows;i++){
        cv::Mat mat = MatToOneRow(mats.at(i));
        cv::Mat _mat;
        if(UMat2FMat(mat,_mat)){
            _mat.copyTo(dst.rowRange(i,i+1));
        }
    }
}

bool CvCommon::_isin_range(cv::Mat &mat, cv::Rect &roi)
{
    return (0 <= roi.x
                && 0 <= roi.width
                && roi.x + roi.width <= mat.cols
                && 0 <= roi.y
                && 0 <= roi.height
                && roi.y + roi.height <= mat.rows) ;
}

bool CvCommon::_is_resizeable(cv::Mat &mat, cv::Size &size)
{
    if(mat.rows <=0 || mat.cols <=0) return false;
    if(size.width <=0 || size.height <=0) return false;
    return true;
}
