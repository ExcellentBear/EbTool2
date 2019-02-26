#ifndef MLAPI_H
#define MLAPI_H

#include "CvArg/cvapi.h"
#include "loadconfigure.h"
#include "utils/mtypes.h"
#include "ml/examplemodel.h"

#include <QString>

class MlApi
{
public:
    MlApi();
    virtual ~MlApi();
    virtual QString getPredictResult(cv::Mat &mat)=0;
    virtual ResultType getPredictResult(DoubleCutsMats &mats) = 0;
    virtual ResultType getPredictResult(TreeCutsMats &mat) = 0;
};

#endif // MLAPI_H
