#include<iostream>
using namespace std;
class calculator
{
public:
    calculator(/* args */) {}
    ~calculator() {}
    virtual int getresult()
    {
        return 0;
    }
    int _num1;
    int _num2;
};

class add1:public calculator
{
private:
    /* data */
public:
    add1(/* args */) {}
    ~add1() {}
    virtual int getresult()
    {
        return _num1+_num2;
    }
};
class sub:public calculator
{
private:
    /* data */
public:
    sub(/* args */) {}
    ~sub() {}
    virtual int getresult()
    {
        return _num1-_num2;
    }
};
class mult:public calculator
{
private:
    /* data */
public:
    mult(/* args */) {}
    ~mult() {}
    virtual int getresult()
    {
        return _num1*_num2;
    }
};

void test01()
{
    //多肽使用条件 父类指针或者引用指向子类
    //
    calculator* abc =new add1;
    abc->_num1=10;
    abc->_num2=10;
    cout<< abc->_num1<<" + "<<abc->_num2<<abc->getresult()<<endl;
}
int main()
{
    test01();
}