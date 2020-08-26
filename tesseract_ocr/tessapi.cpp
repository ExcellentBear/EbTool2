#include "tessapi.h"

TessApi::TessApi()
{
    _api = new tesseract::TessBaseAPI();
    _api->Init("./traindata", "chi_sim");
}

TessApi::~TessApi()
{
    _api->End();
    delete _api;
}


QString TessApi::getOcrResult(cv::Mat &mat)
{
    if(mat.rows <=0 || mat.cols <= 0) return QString();
    cv::Mat tess_mat = mat.clone();
    _api->SetImage((uchar*) tess_mat.data,tess_mat.size().width,tess_mat.size().height,tess_mat.channels(),(int)tess_mat.step1());
    const char* out = _api->GetUTF8Text();
    QString result(out);
    delete[] out;
    return result;
}
