#include<iostream>
#include<string>
using namespace std;

class mystring
{

public:
    mystring(string name) 
    :_name(name)
    {
        cout<<_name<<"构造函数调用"<<endl;
    }
    ~mystring() {
        cout<<"析沟函数调用"<<endl;

    }
    mystring(const mystring &obj)
    {
    cout<<"拷贝构造函数的调用"<<endl;
    }
    mystring & operator=(const mystring &obj)
    {
        cout<<"operator =  函数调用"<<endl;
        return *this;
    }

    string _name;    
};

mystring test01()
{
    mystring  obj("aaa");

    return obj;
}

int main()
{
    mystring oo=test01();


}