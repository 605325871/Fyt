#include<iostream>
#include<string>
using namespace std;
#include<vector>
# define max 10
void printvector(vector<int> &v1)
{
    for(vector<int>::iterator it=v1.begin();it!=v1.end();it++)
    {
        cout<<*it;
    }
    cout<<endl;
}
void test01()
{   
    //默认构造
    vector<int> v;
    v.push_back(1);
      v.push_back(2);
        v.push_back(3);
          v.push_back(4);
  //  printvector(v);
    v.insert(v.begin(),100);
    printvector(v);
    v.insert(v.begin(),2,100);
    printvector(v);
 //   v.erase(v.begin(),v.end());
    v.clear();
    printvector(v);
}
int main()
{   
    test01();
    return 0;
}