#include<iostream>
#include<string.h>

using namespace std;

void test01()
{
    int a;
    int &b=a;

    int &&b=1; //右值引用，右值大多数都为将亡值，字面常量，函数返回值

    int o=10;

    // int &&k =o;  不能把左值赋值给右值
}

int main()
{

}