#include<iostream>
using namespace std;
class person
{
public:
    person() {}
    ~person() {}
    //this指针的本质是常量指针 指针指向 不可以改变  person * const this
    // 加入const修饰后函数为常函数 本质是  将this指针改为  const person * const this
    void showperson()const
    {
        b=10 ;// 加上mutable后则可以进行修改
    }
    int _age;
    mutable int b;
};
void test02()
{
    const person p;

    p.b=100; // b可以改变

    p.showperson() ;//常对象只能调用常函数
}