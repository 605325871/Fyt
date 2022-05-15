#include<iostream>
#include<string>
using namespace std;

template<class T>
void myswap(T &a, T &b)
{
    T temp = a;
     a = b;
     b = temp;
}
int main()
{
    int a=10;
    int b=20;
    cout<< "a = "<<a<<endl;
    cout<< "b = "<<b<<endl;

    // 1.自动类型推导myswap(a,b);
    myswap<int>(a,b);//显式类型转换 
    cout<< "a = "<<a<<endl;
    cout<< "b = "<<b<<endl;
}