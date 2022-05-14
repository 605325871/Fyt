#include<iostream>
using namespace std;

class buliding
{
    friend class goodgay;
private:
    string bedroom;
public:
    buliding(/* args */);
    ~buliding();
    string _settingroom;

};

buliding::buliding()
{
    this->_settingroom="客厅";
    this->bedroom="卧室";
}
buliding::~buliding()
{
}

class goodgay
{
private:
    /* data */
public:
    goodgay(/* args */);
    ~goodgay();
    void visit();
    buliding* bd;
};

goodgay::goodgay(/* args */)
{
    bd =new buliding;
}

goodgay::~goodgay()
{
}
void goodgay::visit()
{
    cout<<"好有 正在访问客厅"<<bd->_settingroom<<endl;
    cout<<"好有 正在访问客厅"<<bd->bedroom<<endl;
}
void test01()
{
    goodgay gg;
    gg.visit();
}

int main()
{
    test01();
}