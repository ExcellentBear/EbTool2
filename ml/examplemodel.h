#ifndef EXAMPLEMODEL_H
#define EXAMPLEMODEL_H

#include "opencv2/ml.hpp"
#include <vector>
#include <set>
#include <fstream>
#include <iostream>

using std::vector;
using std::string;
using std::set;

class ExampleModel
{
public:
    explicit ExampleModel(vector<int> layers);

    void LoadModel(string fileName,string labelfName);
    void SaveModel(string fileName,string labelfName);

    string Predict(cv::Mat &mat);
    cv::Mat OneHot(vector<char> &labels);

    void TrainningModel(cv::Mat &trainData, cv::Mat &labelData);
    void PrintLabels();

private:
    cv::Ptr <cv::ml::ANN_MLP> model;
    vector<char> _enum_map;
    int _laber_number;
    int _get_index(char ch);
    int _arg_max(float* data,int size);
    void _set_enum_map(set<char> &labels);
};

#endif // EXAMPLEMODEL_H
