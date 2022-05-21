# include <iostream>

using namespace std;

class person
{
public:
     string m_name;

protected:
    string m_car;
private:
   int m_id;

public:
    void func()
    {   m_name="张三";
        m_car="拖拉机";
        m_id=1;
    }

};

int main()
{
    person p1;

    p1.m_name="李四";
    // m_car 为保护权限不可访问
    

    return 0;

}