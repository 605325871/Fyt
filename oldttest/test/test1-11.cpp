// # include <iostream>

// using namespace std;

// class person
// {
// public:
//      string m_name;

// protected:
//     string m_car;
// private:
//    int m_id;

// public:
//     void func()
//     {   m_name="张三";
//         m_car="拖拉机";
//         m_id=1;
//     }

// };

// int main()
// {
//     person p1;

//     p1.m_name="李四";
//     // m_car 为保护权限不可访问
    

//     return 0;

// }

# include <iostream>

using namespace std;

class person
{

public:
    person() 
    {
        cout<<"person的无参构造"<<endl;
    }
    person(int age)
    {
        m_age=age;
       cout<<"person的有参构造"<<endl;
    }
    person(const person &p)
    {
        cout<<"person的拷贝构造"<<endl;
        m_age=p.m_age;
    }
    ~person()
     {
         cout<<"person的析造函数调用"<<endl;
     }
  int m_age;
};

void test01()
{
    person p1(20);
    person p2(p1);
}

int main()
{
    test01();
    return 0;
}