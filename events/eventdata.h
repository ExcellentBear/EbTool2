#ifndef EVENTDATA_H
#define EVENTDATA_H

//未使用
class EvenTData
{
public:
   EvenTData();
   explicit EvenTData(int size, void* instance);
   EvenTData(EvenTData& instance);
   EvenTData& operator=(EvenTData& instance);
    ~EvenTData();
   char* getData()/*char* 在对象析构前使用，析构后指针会是野指针，暂时不用智能指针*/;

private:
    int _size;
    char* _data;
};

#endif // EVENTDATA_H
