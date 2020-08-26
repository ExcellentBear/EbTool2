#ifndef TESSAPI_H
#define TESSAPI_H

#include "opencv2/core.hpp"
#include "allheaders.h"
#include "baseapi.h"
#include <QString>

class TessApi  //no deep copy, 使用时请用new指针，而不直接创建实例.
{
public:
    TessApi();
    ~TessApi();
    QString getOcrResult(cv::Mat& mat);

private:
    tesseract::TessBaseAPI* _api;
};

#endif // TESSAPI_H
