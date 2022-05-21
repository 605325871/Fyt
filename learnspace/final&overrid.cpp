#include<iostream>
using namespace std;

class base
{
private:
    /* data */
public:
    base(/* args */);
    ~base();
    virtual void test1() final // //final表明类不能被继承，且只能修饰虚函数，表明虚函数不能被重写
    {
        cout<<"this is test 1"<<endl;

    }
    virtual void test2()
    {
        cout<<"this is test 2"<<endl;
    }

};

class child :public base
{
public:
     void test2() override  //使用override可以保证重写的正确
    {
     cout<<"this is test 2"<<endl;
    }

};