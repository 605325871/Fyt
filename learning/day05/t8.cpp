#include<iostream>
using namespace std;
class student
{
private:
    /* data */
public:
    int _age;
    char _name[64];
};

int main()
{
    student s{10,"zhangsan"};
    cout<<s._age<<endl;
    cout<<s._name<<endl;
    
}