#include "DirectApi.h"

DirectApi::DirectApi()
{
    int otp = LoadConfigure::getConfMap().value("mlapi_labels").toInt();
    vector<int> layers = {32*32,215,36,otp};
    _model = new ExampleModel(layers);
    _model->LoadModel("model.xml","label.txt");
    //MLApi::DefaultMlModel->PrintLabels();
}

DirectApi::~DirectApi()
{
    delete _model;
}

QString DirectApi::getPredictResult(cv::Mat &mat)
{
   string res = _model->Predict(mat);
   return QString(res.c_str());
}

ResultType DirectApi::getPredictResult(DoubleCutsMats &mats)
{
    ResultType result;
    foreach (IndexMat indexMat, mats) {
        QString res = getPredictResult(indexMat.mtx);
        ResultModel model = ResultModel(indexMat.row,indexMat.col,res);
        result.push_back(model);
    }
    return result;
}

ResultType DirectApi::getPredictResult(TreeCutsMats &mats)
{
    OpencvAlgor opencvAlgor;
    DoubleCutsMats doubleCutsMats;
    for(size_t i=0; i<mats.size();i++){
      IndexMatList indexMatList = mats.at(i);
      cv::Mat mat;
        opencvAlgor.VectorTo2FMat(indexMatList.mat_list,mat);
        IndexMat indexMat = IndexMat(indexMatList.row,indexMatList.col,mat);
        doubleCutsMats.push_back(indexMat);
    }
    return getPredictResult(doubleCutsMats);
}


