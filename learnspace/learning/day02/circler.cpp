#include<iostream>
# define pi  3.14
using namespace std;

class circler
{
public:
    circler() {}
    ~circler() {}
    double calculate()
    {
        return 2*pi*m_R;
    }
    int m_R;
};
void test01()
{
    circler c1;
    c1.m_R=10;
    cout<<"c1的周长为"<<c1.calculate()<<endl;
}
int main()
{   
    test01();
    return 0;
}