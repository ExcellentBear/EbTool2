#include "sockapi.h"

SockApi::SockApi()
{

}

QString SockApi::getPredictResult(cv::Mat &mat) {
    QString host = LoadConfigure::getConfMap().value("tcp_server_host", "127.0.0.1");
    uint16_t port = LoadConfigure::getConfMap().value("tcp_server_port","1234").toUShort();
    QTcpSocket tmpSock;
    tmpSock.connectToHost(host,port);
    QString result("");
    for(int i=0;i<mat.rows;i++){
        char toSend[32*32] = {'\0'};
        float* rowData = mat.ptr<float>(i);
        for(int j=0;j<mat.cols;j++){
            toSend[j] = rowData[j] < 0.0f ? 48+1: 48+2; // '1','2'
        }
        tmpSock.write(toSend,32*32);
        if(tmpSock.waitForReadyRead()){
            char recv[2] = {'\0'};
            tmpSock.read(recv,1);
            result += QString(recv);
        }else{
            result = "tcp connect error";
            break;
        }
    }
    tmpSock.close();
    return result;
}

ResultType SockApi::getPredictResult(DoubleCutsMats &mats) {
    QString host = LoadConfigure::getConfMap().value("tcp_server_host", "127.0.0.1");
    uint16_t port = LoadConfigure::getConfMap().value("tcp_server_port","1234").toUShort();
    QTcpSocket tmpSock;
    tmpSock.connectToHost(host,port);
    ResultType results;
    for(IndexMat indexMat: mats) {
        cv::Mat mat = indexMat.mtx;
        int row = indexMat.row;
        int col = indexMat.col;
        QString result("");
        for(int i=0;i<mat.rows;i++){
            char toSend[32*32] = {'\0'};
            float* rowData = mat.ptr<float>(i);
            for(int j=0;j<mat.cols;j++){
                toSend[j] = rowData[j] < 0.0f ? 48+1: 48+2; // '1','2'
            }
            tmpSock.write(toSend,32*32);
            if(tmpSock.waitForReadyRead()){
                char recv[2] = {'\0'};
                tmpSock.read(recv,1);
                result += QString(recv);
            }else{
                result = "tcp connect error";
                break;
            }
        }
        ResultModel model = ResultModel(row,col,result);
        results.push_back(model);
    }
    tmpSock.close();
    return results;
}

ResultType SockApi::getPredictResult(TreeCutsMats &mats)
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
