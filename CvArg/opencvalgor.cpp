#include "opencvalgor.h"

OpencvAlgor::OpencvAlgor()
{

}

OpencvAlgor::~OpencvAlgor()
{

}

void OpencvAlgor::_auto_detect_threshold(std::vector<int> &vect, int &limitSpace, int &limitTrue)
{
    std::vector<int> has_true;
    for(int i=0;i< vect.size();i++) if(vect.at(i)>0) has_true.push_back(vect[i]);
    if(has_true.size() < 1){
        limitSpace = 3;
        limitTrue = 3;
        return;
    }
    else{
        std::sort(has_true.begin(),has_true.end());
        int step = 1, minx = has_true.at(0) , count = 1, sum =has_true.at(0);
        for(int i=0; i<has_true.size();i++){
            if(minx < has_true.at(i)){
                step ++ ;
             minx = has_true.at(i);
            }
            if(step > 2) break;
            count ++;
            sum += minx;
        }
        limitTrue = 1 + sum/count; //limit True
    }
    has_true.clear();
    std::vector<int> is_blank;
    for(int i=0;i<vect.size();i++) if(vect.at(i) < limitTrue) is_blank.push_back(i);
    int _end = (int)(is_blank.size() - 1 );
    for(; _end >0 ; _end -- ) if( is_blank.at(_end) - is_blank.at(_end-1) > 1 ) break; //get right boundray
    int _count =0 , _blank_size = 0;
    for(int i=1; i<_end;i++){
        int distance = is_blank.at(i)-is_blank.at(i-1);
        if(distance > 1){
            _count ++ ;
            _blank_size += distance;
        }
    }
    limitSpace = _count == 0 ? 2 : 2+_blank_size/_count;
}

std::vector<std::pair<int,int>> OpencvAlgor::_hist_or_vert_rflect(cv::Mat &mat, int limitSpace, int limitTrue, CvCommon::AXIS axis)
{
    std::vector<std::pair<int,int>> res;
    if(mat.empty()) return res;
    std::vector<int> no_blank;
    std::vector<int> hist = CountZeroAlongAxis(mat,axis);
    if(limitSpace== -1 || limitTrue == -1) _auto_detect_threshold(hist,limitSpace,limitTrue);
    //==================== add a limit ========================================
    if(axis==CvCommon::VERT) limitSpace = 3;
    //===================== ====================================================
    for(int i=0;i<hist.size();i++){
        if(hist.at(i) >= limitTrue ) no_blank.push_back(i);
    }
    if(no_blank.empty()) return res;
    no_blank.push_back(no_blank.back()+limitSpace+1);
    int begin = std::max(0,no_blank.at(0)-1);
    for(int i=0;i<no_blank.size()-1;i++){
        if(no_blank.at(i+1) - no_blank.at(i) > limitSpace){
            res.push_back(std::make_pair(begin,no_blank.at(i)+1));
            begin = std::max(0,no_blank.at(i+1)-1);
        }
    }
    return res;
}

QImage OpencvAlgor::_mat_to_qimage(const cv::Mat &mat)
{
    QImage im=QImage();
    if(mat.empty()) return im;
    if(mat.type()==CV_8UC1){
      im = QImage((const unsigned char*) mat.data,mat.cols, mat.rows, mat.cols,QImage::Format_Grayscale8);
    }
    return im;
}

std::vector<std::pair<int,int>> OpencvAlgor::HistRflect(cv::Mat &mat, int limitSpace, int limitTrue)
{
    return _hist_or_vert_rflect(mat,limitSpace,limitTrue,CvCommon::HIST);
}

std::vector<std::pair<int,int>> OpencvAlgor::VertRflect(cv::Mat &mat, int limitSpace, int limitTrue)
{
    return _hist_or_vert_rflect(mat,limitSpace,limitTrue,CvCommon::VERT);
}

bool OpencvAlgor::AdjustImageSize(cv::Mat &src, uint maxLen, double &ratio, MImage &imstruct)
{
    if(src.empty()) return false;
    if( src.type() != CV_8UC1) return false;
    cv::Mat dst = cv::Mat(maxLen,maxLen,CV_8UC1,cv::Scalar(255));
    int one = std::max(src.cols,src.rows);
    ratio = ((double)maxLen)/(double)one;
    ratio = std::min(10.0,ratio);
    imstruct.fg_height = (int) src.rows*ratio;
    imstruct.fg_width = (int) src.cols*ratio;
    if(ratio*src.cols <=0 || ratio*src.rows <= 0 || ratio <= 0) return false;
    cv::Size dsize((int) src.cols*ratio,(int) src.rows*ratio);
    cv::Mat mat;
    if(!ResizeImage(src,mat,dsize)) return false;
    cv::Rect rect = cv::Rect(0,0,mat.cols,mat.rows);
    cv::Mat ROI;
    if(!CutOutImage(dst,ROI,rect)) return false;
    mat.copyTo(ROI);
    imstruct.mat = dst.clone();
    return true;
}


void OpencvAlgor::_mat_memset(cv::Mat &src, int col_row, int begin, int end, uchar dvalue,int flags)
{
    assert(flags==0 || flags==1);
    if(flags==1){
        uchar* data = src.ptr<uchar>(col_row);
        for(;begin < end;begin++) data[begin] = dvalue;
    }else{
        for(;begin<end;begin++) src.ptr<uchar>(begin)[col_row] = dvalue;
    }

}

void OpencvAlgor::_imstruct_mat2bin(MImage &imstruct, uint8_t thresHold)
{
    cv::Mat memKeeper = imstruct.mat.clone();
    if(memKeeper.empty()) return;
    int r = imstruct.fg_height, c = imstruct.fg_width ;
    int nc = memKeeper.cols;
    int nr = memKeeper.rows;
    if(r ==0 || c==0 || r >nr ||c > nc) return;
    for(int j=0; j < nr ;j++ ){
        uchar *rowData = memKeeper.ptr<uchar>(j);
        for(int i=0; i < nc; i++) rowData[i] = rowData[i] > thresHold ? 255 :0;
    }
    //continue line
    std::set<IndexInfoForAlter> trContains;
    //row check
    for(int j=0; j < r; j++){
        uchar *rowData = memKeeper.ptr<uchar>(j);
        int begin = 0,end = 0;
        for(int i=0;i < c ; i++) {
            if(rowData[i] == 0 && i < c-1) {
                end++;
            }else{
                if(c > 256 &&(end-begin > 600 || end-begin > 0.67*c)){
                    trContains.insert(IndexInfoForAlter(j,begin,end,1));
                    if( j>0 && j < r-1){
                      trContains.insert(IndexInfoForAlter(j-1,begin,end,1));
                      trContains.insert(IndexInfoForAlter(j+1,begin,end,1));
                    }
                }
                begin = i;
                end = i;
            }
        }
    }
    // column check
    for(int i=0 ; i < c; i++){
        int begin = 0,end = 0;
        for(int j=0; j < r; j++){
            if(memKeeper.ptr<uchar>(j)[i] == 0 && j < r-1){
                end ++;
            }else{
                if(r > 256 &&(end-begin > 600 || end-begin >0.67*r)){
                    trContains.insert(IndexInfoForAlter(i,begin,end,0));
                    if( i > 0 && i < c-1){
                       trContains.insert(IndexInfoForAlter(i+1,begin,end,0));
                       trContains.insert(IndexInfoForAlter(i-1,begin,end,0));
                    }
                }
                begin = j;
                end = j;
            }
        }
    }
    foreach (IndexInfoForAlter ifal, trContains) {
        _mat_memset(memKeeper,ifal.index,ifal.begin,ifal.end,200,ifal.flags);
    }
    imstruct.mat = memKeeper.clone();
}

QImage OpencvAlgor::MImageToBinQImage(MImage &imstruct,  uint8_t thresHold)
{
    _imstruct_mat2bin(imstruct,thresHold);
    QImage im = _mat_to_qimage(imstruct.mat);
    return im;
}

bool OpencvAlgor::_is_all_black(cv::Mat &mat)
{
    int sum = 0;
    for(int j=0;j<mat.rows;j++){
        uchar* rowData = mat.ptr<uchar>(j);
        for(int i=0;i<mat.cols;i++){
            if(rowData[i]==0) sum ++;
        }
    }
    return sum == mat.rows*mat.cols;
}

void OpencvAlgor::RemoveSpace(cv::Mat &mat)
{
    std::vector<int> hist = CountZeroAlongAxis(mat,CvCommon::HIST);
    std::vector<int> vert = CountZeroAlongAxis(mat,CvCommon::VERT);
    int hb=0 , he = (int)hist.size()-1, vb = 0, ve = (int)vert.size()-1;
    while(hb < he){
        if(hist.at(hb)>0) break;
        hb++;
    }
    while(he > hb){
        if(hist.at(he)>0) break;
        he--;
    }
    while(vb < ve){
        if(vert.at(vb) >0) break;
        vb++;
    }
    while(ve > vb){
        if(vert.at(ve)>0) break;
        ve--;
    }
    cv::Mat dst;
    cv::Rect rect(hb,vb,he-hb,ve-vb);
    if(CutOutImage(mat,dst,rect)){
        if(_is_all_black(dst)){
            rect = cv::Rect(hb,0,he-hb,ve);
            CutOutImage(mat,dst,rect);
        }
        mat = dst.clone();
    }

}

void OpencvAlgor::FilterIslandPoints(cv::Mat &mat, uchar limitPoints)
{
    std::vector<std::pair<int,int>> tmp;
    for(int i=1;i<mat.rows-1;i++){
        uchar *row_0 = mat.ptr<uchar>(i-1);
        uchar *row_1 = mat.ptr<uchar>(i);
        uchar *row_2 = mat.ptr<uchar>(i+1);
        for(int j=1;i<mat.cols-1;j++){
                if(row_1[j]== 255) continue;
                if((row_0[j-1]+row_0[j]+row_0[j+1] + row_1[j-1]+row_1[j+1] + row_2[j-1]+row_2[j]+row_2[j+1]) > 255*(8-limitPoints) ) tmp.push_back(std::make_pair(i,j));
        }
     }
     for(size_t k=0;k<tmp.size();k++){
            mat.ptr<uchar>(tmp.at(k).first)[tmp.at(k).second] = 255;
      }
}

void OpencvAlgor::EasySmooth(cv::Mat &mat)
{
    if(mat.dims <= 2 && mat.step[0] > 0) return;
    if(mat.rows < 2 || mat.cols < 2) return;
    cv::Mat kernel =  cv::getStructuringElement(cv::MORPH_RECT,cv::Size(2,1));
    cv::Mat out;
    cv::dilate(mat,out,kernel);
    kernel = cv::getStructuringElement(cv::MORPH_RECT,cv::Size(1,2));
    cv::erode(out,mat,kernel);
}
