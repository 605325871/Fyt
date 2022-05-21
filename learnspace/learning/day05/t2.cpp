#include<iostream>
using namespace std;


 
//virual 关键字可以让子类使用自己的函数
//动态多态满足条件
//1.有继承关系
//2.子类重写复类虚函数
// 动态多台使用
// 父类的  指针 或者 引用 执行子类对象
class anmial
{
private:
    /* data */
public:
    anmial(/* args */) {}
    ~anmial() {}
    virtual void dospeak() //virtual 可以实现地址晚绑定
    {
        cout<<"动物在说话"<<endl;
    }
};

class cat :public anmial
{
private:
    /* data */
public:
    cat(/* args */) {}
    ~cat() {}
    void dospeak()
    {
        cout<<"小猫在说话"<<endl;
    }
};
// 地址早绑定  在编译阶段确定函数地址
//猫说话则 让这个函数地址在运行阶段晚绑定
void dospeak(anmial &a)
{
    a.dospeak();
}
void test01()
{
  cat cat;
  dospeak(cat);
}
int main()
{
    test01();

}