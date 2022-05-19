#include<iostream>
#include<vector>
#include<deque>
#include<algorithm>
#include<time.h>
using namespace std;

class person
{
public:
    string _name;
    int _marks;
public:
    person(/* args */) {}
    ~person() {}
    person(string name,int mark )
    :_name(name),_marks(mark)
    {}
};

void creatperson(vector<person>&v1)
{
    string nameseed="ABCDE";
    for(int i=0 ; i<5 ; i++)
    {
        string name="选手";
        name+=nameseed[i];
        int score=0;
        person p(name,score);
        v1.push_back(p);
    }
}

void getsocer(vector<person>&v1)
{
    for(vector<person>::iterator it=v1.begin() ; it!=v1.end();it++)
    {
        deque<int> d;
        for(int i=0;i<10;i++)
        {
            int score=rand()%41 + 60;
            d.push_back(score);
        }
        cout<<" 姓名 "<<it->_name<<"得分为 :"<<endl;
       for(deque<int>::iterator dit=d.begin() ; dit!=d.end();dit++)
        {
            cout<<*dit<<" ";
        }
        cout<<endl;
        sort(d.begin(),d.end());
        d.pop_back();
        d.pop_front();

        int sum=0;
        for(deque<int>::iterator dit=d.begin() ; dit!=d.end();dit++)
        {
            sum+= *dit;
        }
        int avg = sum/d.size();

        it->_marks=avg;
    }
}
void printvector(vector<person>&v1)
{
   
   for(auto item :v1)
    {
        cout<<item._name<<" "<<item._marks<<endl;
    }
}
int main()
{
    srand((unsigned int)time(NULL));
    vector<person> v;
    creatperson(v);
    getsocer(v);
    printvector(v);
    
}