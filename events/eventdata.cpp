#include "eventdata.h"

EvenTData::EvenTData():_size(0),_data(nullptr)
{
}

EvenTData::EvenTData(int size, void *instance)
{
    _size = size;
    if(_size>0){
    _data = new char[_size];
    char* tmp = reinterpret_cast<char*>(instance);
    for(int i=0;i<size;i++){
        _data[i] = tmp[i];
    }
    }else{
        _size = 0;
        _data = nullptr;
    }
}

EvenTData::EvenTData(EvenTData &instance)
{
    if(_size>0) delete _data;
    _size = instance._size;
    if(_size>0){
        _data = new char[_size];
        for(int i=0; i< _size;  i++){
        _data[i] = instance._data[i];
        }
    }else{
        _size = 0;
        _data = nullptr;
        }
}

EvenTData& EvenTData::operator=(EvenTData& instance){
    if(this != &instance){
        if(_size>0)delete _data;
        _size = instance._size;
        _data = new char[_size];
        for(int i=0; i< _size;  i++){
            _data[i] = instance._data[i];
        }
    }
    return *this;
}

EvenTData::~EvenTData()
{
    delete _data;
}

char* EvenTData::getData()
{
    return _data;
}
