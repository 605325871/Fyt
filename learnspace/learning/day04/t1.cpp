#include<iostream>
using namespace std;

class t1
{
public:
    t1(int age) {
        _age=age;
    }
    t1 addone(t1 &t)
    {
        this->_age+=t._age;
        return *this;
    }
    t1& addtwo(t1 &t)
    {
        this->_age+=t._age;
        return *this;
    }
    ~t1() {}

    int _age;
};

void test01()
{
    t1  t(10);
    t1 t2(10);
   
    t2.addone(t).addone(t).addone(t);  //值传递返回则会重新调用一个新的构造函数
    cout<<"addone 下年龄为"<<t2._age<<endl;  

}
void test02()
{
    t1  t(10);
    t1 t2(10);
  
    t2.addtwo(t).addtwo(t).addtwo(t);  //引用传递返回 是地址本身
    cout<<"addone 下年龄为"<<t2._age<<endl;  
}
int main()
{
    test01();
    test02();
    return 0;
  
}
