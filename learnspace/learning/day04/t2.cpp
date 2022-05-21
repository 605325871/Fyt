#include<iostream>
using namespace std;

class person
{
public:
    person(/* args */) {}
    ~person() {}

    void showcalssname()
    {
        cout<<"this is person class "<<endl;
    }
    void showcalssage()
    {
        if(this == nullptr)
        {
            return;
        }
        cout<<"person age= "<<this->_age<<endl;// err : 传入指针为nullptr
    }

    int _age;
};

void test01()
{
    person* p = nullptr;
    p->showcalssname();
    p->showcalssage();


}

int main()
{
    test01();

}