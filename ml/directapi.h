#ifndef DIRECTAPI_H
#define DIRECTAPI_H

#include "ml/mlapi.h"

class DirectApi : public MlApi
{
public:
    DirectApi();
    virtual ~DirectApi();
    QString getPredictResult(cv::Mat &mat) override;
    ResultType getPredictResult(DoubleCutsMats &mats) override;
    ResultType getPredictResult(TreeCutsMats &mats) override;

private:
    ExampleModel* _model;
};

#endif // DIRECTAPI_H
