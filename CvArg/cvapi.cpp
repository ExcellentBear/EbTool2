#include "cvapi.h"

static void Append(std::vector<cv::Mat>& v1,std::vector<cv::Mat>& v2){
    for(int i=0; i < v2.size();i++) v1.push_back(v2.at(i));
}

CvApi::CvApi():_algor(OpencvAlgor())
{

}

CvApi::~CvApi(){

}


void CvApi::HistSplitMat(cv::Mat &src, std::vector<cv::Mat> &dst, int limitWhite)
{
    auto cuts = _algor.HistRflect(src,limitWhite,2);
    int height = src.rows;
    for(size_t i=0; i < cuts.size();i++){
        int width = cuts.at(i).second - cuts.at(i).first;
        cv::Rect rect(cuts.at(i).first,0,width,height);
        cv::Mat ROI;
        if(_algor.CutOutImage(src,ROI,rect)){
            cv::Mat m = ROI.clone();
            dst.push_back(m);
        }
    }
}

void CvApi::SplitSubMats(DoubleCutsMats &dmats, TreeCutsMats &tmats)
{
    std::vector<int> str_length;
    for(size_t i=0; i < dmats.size();i++){
        std::vector<cv::Mat> res;
        cv::Mat mt = dmats.at(i).mtx;
        _algor.EasySmooth(mt);
        HistSplitMat(mt,res,1);
        foreach (cv::Mat _mt, res) {
            str_length.push_back(_mt.rows);
        }
        IndexMatList indexMatList(dmats.at(i).row,dmats.at(i).col);
        Append(indexMatList.mat_list,res);
        tmats.push_back(indexMatList);
    }
    if(str_length.size()==0) return;
    std::sort(str_length.begin(),str_length.end());
    int meandian=0;
    if( str_length.size() & 1 ) meandian = str_length.at( (str_length.size()+1)/2 - 1);
    else meandian = (str_length.at(str_length.size()/2 -1) + str_length.at( str_length.size()/2 ) );
    // filter result
    for(int i=0;i<tmats.size();i++){
        IndexMatList ele = tmats.at(i);
        IndexMatList tmp(ele.row,ele.col);
        foreach (cv::Mat mt, ele.mat_list) {
            if(mt.cols >= meandian ){
                std::vector<cv::Mat> _splits = RecuDropFall(mt,meandian);
                Append(tmp.mat_list, _splits);
            }else{
                tmp.mat_list.push_back(mt);
            }
        }
        tmats[i] = tmp;
    }
}

int CvApi::_df_find_fpoint(cv::Mat &mat)
{
    int depth = 0,index = mat.cols/2;
    for(int i=mat.cols/3;i<mat.cols*3/4;i++){
       int dep=0;
       for(int j=0;j<mat.rows;j++){
            if(mat.ptr<uchar>(j)[i]==255) dep++;
            else break;
       }
       if(depth<dep){
           depth = dep;
           index = i;
        }
   }
       return index;
}

cv::Point CvApi::_get_point(std::vector<cv::Point> &trace, int &row)
{
    cv::Point point;
    for(size_t i=0; i < trace.size(); i++){
        if(trace.at(i).y==row) point = trace.at(i);
        if(trace.at(i).y > row) break;
    }
    return point;
}

std::pair<int,int> CvApi::_minmax_column_value(std::vector<cv::Point> &trace)
{
    int _min = std::numeric_limits<int>::max();
    int _max = std::numeric_limits<int>::min();
    for(std::vector<cv::Point>::iterator ite = trace.begin();ite != trace.end(); ite++){
        _min = std::min(_min,ite->x);
        _max = std::max(_max,ite->x);
    }
    return std::make_pair(_min,_max);
}

int CvApi::DropFall(cv::Mat &src, cv::Mat &left, cv::Mat &right, int x)
{
    if (src.channels() != 1) return -1;
         int nl = src.rows;
         int nc = src.cols;
         bool leftRightFlg = false;
         std::vector<cv::Point> flow_trace;
         int col = x,row = 0;
         flow_trace.push_back(cv::Point(col,row));
         while( col< nc-1 && row < nl-1 ){
             uchar left_point = src.ptr<uchar>(row)[col-1];
             uchar right_point = src.ptr<uchar>(row)[col+1];
             uchar down_point = src.ptr<uchar>(row+1)[col];
             uchar down_left = src.ptr<uchar>(row+1)[col-1];
             uchar down_right = src.ptr<uchar>(row+1)[col+1];
             if (
                  (   (0 == left_point) && (0  == right_point) && (0  == down_point) && (0  == down_left) && (0  == down_right) )// all black
               || (   (255 == left_point) && (255  == right_point) && (255  == down_point) && (255  == down_left) && (255  == down_right) )//all white
               || (   (0 == down_left) && (255 == down_point))
               )
               {
                 row++;
                 leftRightFlg = false;
                 flow_trace.push_back(cv::Point(col,row));
              }
             else if ( (0==left_point) && (0== right_point) && (0==down_point) && (255== down_left) &&(0== down_right))
             {
                 row++;
                 col--;
                 flow_trace.push_back(cv::Point(col,row));
                 leftRightFlg = false;
             }
             else if( (0 == down_point) && (0==down_left) && ( 255 == down_right)){
                 row++;
                 col++;
                 flow_trace.push_back(cv::Point(col,row));
                 leftRightFlg = false;
             }
             else if ((255 == right_point) && (0 == down_point) && (0 == down_left) && (0 == down_right))
             {
                                if (!leftRightFlg)
                                {
                                    col++;
                                    leftRightFlg = true;
                                }
                                else
                                {
                                    row++;
                                    leftRightFlg = false;
                                }
                                 flow_trace.push_back(cv::Point(col,row));
             }
             else if ((255 == left_point) && (0 == down_point) && (0 == down_left) && (0 == down_right))
             {
                                if (!leftRightFlg)
                                {
                                    col--;
                                    leftRightFlg = true;
                                }
                                else
                                {
                                    row++;
                                    leftRightFlg = false;
                                }
                                flow_trace.push_back(cv::Point(col,row));
            }
             else
             {
                row++;
                flow_trace.push_back(cv::Point(col,row));
              }

         }
         auto minmax = _minmax_column_value(flow_trace);
         int minx = minmax.first;
         int maxx = minmax.second;
         if(minx<=0 || maxx<=0) return 1;
         left = cv::Mat().ones(nl,maxx+1,CV_8UC1);
         right = cv::Mat().ones(nl,nc-minx,CV_8UC1);
         for(int j=0;j<nl;j++){
             uchar* data = src.ptr<uchar>(j);
             uchar* ldata = left.ptr<uchar>(j);
             cv::Point pt = _get_point(flow_trace,j);
             for(int i=0;i<left.cols;i++){
                 if(i<pt.x) ldata[i] = data[i];
                 else ldata[i] = 255;
             }
         }
         for(int j=0;j<nl;j++){
             uchar* data = src.ptr<uchar>(j);
             uchar* rdata = right.ptr<uchar>(j);
             cv::Point pt = _get_point(flow_trace,j);
             for(int i=0;i<nc-minx+1;i++){
                 if(i>=pt.x) rdata[i] = data[i+minx];
                 else rdata[i] = 255;
             }
         }
         if(right.cols==nc && right.rows==nl) return 1;
         return 0;
}

std::vector<cv::Mat> CvApi::RecuDropFall(cv::Mat &mat, int thres)
{
    std::vector<cv::Mat> result;
    if(mat.cols < thres) return result;
    cv::Mat left,right;
    _algor.RemoveSpace(mat);
    int index = _df_find_fpoint(mat);
    int status = DropFall(mat,left,right,index);
    switch (status) {
    case -1:
    {
        break;
    }
    case 1:
    {
        result.push_back(mat.clone());
    }
    case 0:
    {
        if(left.cols < thres) result.push_back(left);
        else if(left.cols >= thres) {
            auto lres = RecuDropFall(left,thres);
            Append(result,lres);
        }
        //-----
        if(right.cols < thres) result.push_back(right);
        else if(right.cols >= thres){
            auto rres = RecuDropFall(right,thres);
            Append(result,rres);
        }
    }
    default:
    {
        break;
    }
    }
    return result;
}

void CvApi::_get_avg_length(std::vector<IndexMatList> &imtx, int &avgWidth, int &avgHeight)
{
    int num = 0;
    avgHeight = 0 ; avgWidth = 0;
    foreach (IndexMatList indexMatList, imtx) {
        foreach (cv::Mat mat, indexMatList.mat_list) {
            num ++;
            avgHeight += mat.rows;
            avgWidth += mat.cols;
        }
    }
    if(num==0) return;
    avgHeight /= num ;
    avgWidth /= num;
}

void CvApi::CutOutAllDigit(std::vector<int> &xhls, std::vector<int> &ywls, cv::Mat &mat, int &avgHeight, int &avgWidth, std::vector<IndexMatList> &out)
{
    if(xhls.size()==0|| ywls.size()==0) return;
    DoubleCutsMats indexMats;
    std::sort(xhls.begin(),xhls.end());
    std::sort(ywls.begin(),ywls.end());
    xhls[0] = std::max(0,xhls.at(0));
    ywls[0] = std::max(0,ywls.at(0));
    for(size_t i=0; i < xhls.size()-1; i+=2){
        for(size_t j=0; j < ywls.size()-1; j+=2){
            cv::Rect rect(xhls.at(i),ywls.at(j),xhls.at(i+1)-xhls.at(i),ywls.at(i+1)-ywls.at(i));
            cv::Mat ROI;
            if( _algor.CutOutImage(mat,ROI,rect) ){
                IndexMat ele((int)j/2,(int)i/2,ROI);
                indexMats.push_back(ele);
            }
        }
    }
    SplitSubMats(indexMats,out);
    _get_avg_length(out,avgWidth,avgHeight);
}

cv::Mat CvApi::MatTo32X32(cv::Mat &src, int flags)
{
    cv::Mat mat;
    cv::Size size;
    size.width = 32;
    size.height = 32;
    _algor.ResizeImage(src,mat,size,flags);
    return mat;
}

bool CvApi::IsIgnoreAble(cv::Mat &mat, int avgHight, int avgWidth)
{
    int zeroNum = 0;
    for(int j = 0;j < mat.rows; j++){
        uchar* data = mat.ptr<uchar>(j);
        for(int i=0; i< mat.cols; i++){
            if(data[i]==0) zeroNum ++ ;
        }
    }
    if(zeroNum < 4) return true;
    if(zeroNum < avgHight/10 || zeroNum < avgWidth/10) return true;
    if(zeroNum < avgHight*avgWidth/300) return true;
    return false;
}

cv::Mat CvApi::ExtendWidthOrHeight(cv::Mat &src)
{
    cv::Mat result;
    int rows = src.rows, cols = src.cols;
    if(rows > cols){
        result = cv::Mat(rows,rows,CV_8UC1,cv::Scalar(255));
        int x = (rows - cols)/2;
        cv::Rect rect(x,0,cols,rows);
        src.copyTo(result(rect));
    }else if( rows==cols){
        result = src.clone();
    }else{
        result = cv::Mat(cols,cols,CV_8UC1,cv::Scalar(255));
        int y = (cols - rows)/2;
        cv::Rect rect(0,y,cols,rows);
        src.copyTo(result(rect));
    }
    return result;
}

void CvApi::StandardPics(TreeCutsMats &treeCutsMats, int avgWidth, int avgHeight)
{
    avgWidth = avgWidth;
    for(size_t i=0; i< treeCutsMats.size();i++){
        for(size_t j=0 ; j < treeCutsMats.at(i).mat_list.size(); j++){
            cv::Mat tmp = treeCutsMats.at(i).mat_list.at(j).clone();
            if(tmp.empty()) continue;
            _algor.RemoveSpace(tmp);
            if(tmp.rows < avgHeight/2 ){
                if(IsIgnoreAble(tmp,avgHeight,avgWidth)) treeCutsMats[i].mat_list[j] = cv::Mat();
                else{
                    cv::Mat extendMat = ExtendWidthOrHeight(treeCutsMats.at(i).mat_list.at(j));
                    treeCutsMats[i].mat_list[j] = MatTo32X32(extendMat);
                }
            }else{
                treeCutsMats[i].mat_list[j] = MatTo32X32(tmp);
            }
        }
    }
}

cv::Mat CvApi::loadImage(QString &file, int flag)
{
    return _algor.MLoadImage(file.toLocal8Bit().data(), flag);
}

bool CvApi::AdjustImageSize(cv::Mat &mat, int length, double& ratio, MImage& im)
{
    return _algor.AdjustImageSize(mat,(uint)length,ratio,im);
}

QImage CvApi::ToQimage(MImage& im, int& thres)
{
    uint8_t tmp = (uint8_t)thres;
    thres = (int) tmp;
    return _algor.MImageToBinQImage(im, tmp);
}

bool CvApi::CutOutImage(cv::Mat &src, cv::Mat &dst, cv::Rect rect)
{
    return _algor.CutOutImage(src,dst,rect);
}

std::vector<std::pair<int,int>> CvApi::HistRflect(cv::Mat &mat, int x, int y){
      return _algor.HistRflect(mat,x,y);
}

std::vector<std::pair<int,int>> CvApi::VertRflect(cv::Mat &mat, int x, int y){
     return _algor.VertRflect(mat,x,y);
}
