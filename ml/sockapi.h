#ifndef SOCKAPI_H
#define SOCKAPI_H

#include "ml/mlapi.h"
#include <QTcpSocket>

class SockApi : public MlApi
{
public:
    SockApi();
    QString getPredictResult(cv::Mat &mat) override;
    ResultType getPredictResult(DoubleCutsMats &mats) override;
    ResultType getPredictResult(TreeCutsMats &mats) override;
};

#endif // SOCKAPI_H
