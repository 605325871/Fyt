#include<iostream>
#include<map>
#include<string>
#include<utility>
#include<vector>
using namespace std;

#define cehua 0;
#define meishu 1;
#define yanfa 2;
class worker
{
public:
    string _name;
    int _salary;
public:
    worker(/* args */) {}
    ~worker() {}
    worker(const worker&p1)
    {
        this->_name=p1._name;
        this->_salary=p1._salary;
    }
    worker(int salary,string name)
    :_salary(salary),_name(name)
    {}
    void creatvector(vector<worker>&v1)
    {
        string nameseed="ABCDEFGHIJ";
        for(int i=0;i<10;i++)
        {
            worker worker;
            worker._name=" 选手 ";
            worker._name+=nameseed[i];
            worker._salary=rand()%10000 + 10000;
            v1.push_back(worker);
        }
    }
    void setgroup(vector<worker> v , multimap<int,worker> &m)
    {
        for(vector<worker>::iterator it=v.begin();it!=v.end();it++)
        {
            int depid= rand()%3; // 0 1 2
            m.insert(make_pair(depid,*it));
        }
    
    }
    void showworker(multimap<int ,worker>&m)
    {
        cout<< "策划 "<<endl;
        printworker(m,0,1);
        cout<<"美术"<<endl;
        printworker(m,1,2);
        cout<<"研发"<<endl;

        multimap<int,worker> ::iterator pos = m.find(2);
        int size=0;
        int cout=(int)m.count(2);
        for(; pos!=m.end()&&size<cout;pos++,size++)
        {
           std:: cout<<"姓名 : "<<pos->second._name<<" 工资 "<<pos->second._salary<<endl; 
        }
    }
    void printworker(multimap<int ,worker>&m,int findvalue,int unfindvalue)
    {
        multimap<int,worker>::iterator pos=m.find(findvalue); 
        for( ; pos!=m.find(unfindvalue);pos++)
        {
            cout<<"姓名 : "<<pos->second._name<<" 工资 "<<pos->second._salary<<endl; 
        }
    }

};

int main()
{
    worker w;
    vector<worker> v;
    multimap<int ,worker>m;
    w.creatvector(v);
    w.setgroup(v,m);
    w.showworker(m);

}