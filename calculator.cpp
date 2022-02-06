# include <iostream>
using namespace std;

// class calculator
// {
// public:
//     int getresult(string oper)
//     {
//         if(oper == "+")
//         {
//             return  num_1 + num_2;
//         }
//         else if(oper == "-")
//         {
//             return  num_1 - num_2;
//         }
//     }
//     int num_1;
//     int num_2;
// };

class abscalculator
{
public:
    virtual int sort()
    {
        return 0;
    }
    
    int m_num1;
    int m_num2;

};

//加法计算
class addcalculator:public abscalculator
{
public:
     int sort()
    {
        return m_num1 + m_num2;
    }

};
//减法
class subcalculator:public abscalculator
{
public:
     int sort()
    {
        return m_num1 - m_num2;
    }

};
//乘法
class multcalculator:public abscalculator
{
public:
     int sort()
    {
        return m_num1 * m_num2;
    }

};
//除法
class addcalculator:public abscalculator
{
public:
    int sort()
    {
        return m_num1 / m_num2;
    }

};
void test01()
{
    abscalculator *abc = new addcalculator;
    abc->m_num1=10;
    abc->m_num2=10;

    cout<<abc->m_num1<<"+"<<abc->m_num2<<"="<<abc->sort()<<endl;
    delete abc;
    
}


int main()
{
    test01();
    return 0;
}