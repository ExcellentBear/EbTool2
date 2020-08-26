#ifndef APIPROXY_H
#define APIPROXY_H


#include "ml/directapi.h"
#include "ml/sockapi.h"
#include "ml/mlapi.h"


class ApiProxy:public MlApi
{
public:
    ApiProxy();
    virtual ~ApiProxy();
    QString getPredictResult(cv::Mat &mat) override;
    ResultType getPredictResult(DoubleCutsMats &mats) override;
    ResultType getPredictResult(TreeCutsMats &mats) override;

private:
    MlApi* _api;
};

#endif // APIPROXY_H
