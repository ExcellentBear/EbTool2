#include "examplemodel.h"

ExampleModel::ExampleModel(vector<int> layers)
{
    _laber_number = layers.back();
    model = cv::ml::ANN_MLP::create();
    cv::Mat layerSize = cv::Mat_<int>(1, static_cast<int>(layers.size()));
    for(size_t i=0;i<layers.size();i++){
            layerSize.at<int>(0, static_cast<int>(i)) = layers.at(i);
        }
    model->setLayerSizes(layerSize);
    model->setTermCriteria(cv::TermCriteria(cv::TermCriteria::MAX_ITER+cv::TermCriteria::EPS,300, FLT_EPSILON));
    model->setActivationFunction(cv::ml::ANN_MLP::SIGMOID_SYM);
    model->setTrainMethod(cv::ml::ANN_MLP::BACKPROP, 0.001);
}

void ExampleModel::LoadModel(std::string fileName, std::string labelfName)
{
    model = model->load(fileName.c_str());
    std::ifstream infile(labelfName.c_str());
    if(!infile.is_open()) return;
    _enum_map.clear();
    string line;
    std::getline(infile,line);
    for(string::size_type t=0;t<line.length();t++){
            _enum_map.push_back(line.at(t));
     }
     assert(_enum_map.size()==_laber_number);
}

void ExampleModel::SaveModel(std::string fileName, std::string labelfName)
{
    model->save(fileName.c_str());
    string line("");
    for(vector<char>::size_type t=0;t<_enum_map.size();t++) line += _enum_map.at(t);
    std::ofstream ofile(labelfName.c_str());
    if(ofile.is_open()) ofile << line;
}

cv::Mat ExampleModel::OneHot(vector<char> &labels)
{
    set<char> tmp;
    for(size_t t=0;t<labels.size();t++) tmp.insert(labels.at(t));
    _set_enum_map(tmp);
    cv::Mat result= cv::Mat().zeros((int)labels.size(),(int)tmp.size(),CV_32FC1);
    for(size_t i=0;i<labels.size();i++){
        int j = _get_index(labels.at(i));
        result.ptr<float>(i)[j] = 1.0f;
    }
    return result;
}

string ExampleModel::Predict(cv::Mat &mat)
{
    string result("");
    if(mat.rows<1) return result;
    cv::Mat resp;
    model->predict(mat,resp);
    if(resp.cols==0){
        return "feed mat error";
    }
    for(int i=0;i<resp.rows;i++){
        float* data = resp.ptr<float>(i);
        int index = _arg_max(data,resp.cols);
        if(index==-1) result += "predict error";
        else result += _enum_map.at(index);
    }
    return result;
}

void ExampleModel::TrainningModel(cv::Mat &trainData, cv::Mat &labelData)
{
     assert(!trainData.empty());
     assert(!labelData.empty());
     assert(_laber_number==labelData.cols); //length == onehot array length
     assert(trainData.rows==labelData.rows); // ROW_SAMPLE
     cv::Ptr<cv::ml::TrainData> trainDatas = cv::ml::TrainData::create(trainData,cv::ml::ROW_SAMPLE,labelData);
     model->train(trainDatas);
}

void ExampleModel::PrintLabels()
{
    std::cout << "labels contain  " ;
    for(size_t i=0;i<_enum_map.size();i++) std::cout << _enum_map.at(i) << ", ";
    std::cout << std::endl;
}

int ExampleModel::_get_index(char ch)
{
    int res = -1;
    for(vector<char>::size_type t=0;t<_enum_map.size();t++){
        if(_enum_map.at(t)==ch){
            res = (int) t;
            break;
        }
    }
    return res;
}

int ExampleModel::_arg_max(float *data, int size)
{
    if(size==0) return -1;
    if(size==1) return 0;
    int result = 0;
    float _m = data[0];
    for(int i=1;i<size;i++){
        if(_m < data[i]){
            _m = data[i];
            result = i;
        }
    }
    return result;
}

void ExampleModel::_set_enum_map(set<char> &labels)
{
    _enum_map.clear();
    for(set<char>::iterator ite =labels.begin();ite != labels.end();ite++){
        _enum_map.push_back(*ite);
    }
}

