#include "apiproxy.h"

ApiProxy::ApiProxy()
{
    if(LoadConfigure::getConfMap().value("mlapi", "default")=="default"){
        _api = new DirectApi;
    }else{
        _api = new SockApi;
    }
}

ApiProxy::~ApiProxy()
{
    delete _api;
}

QString ApiProxy::getPredictResult(cv::Mat &mat){
    return _api->getPredictResult(mat);
}


ResultType ApiProxy::getPredictResult(DoubleCutsMats &mats){
   return _api->getPredictResult(mats);
}

ResultType ApiProxy::getPredictResult(TreeCutsMats &mats)
{
    return _api->getPredictResult(mats);
}
