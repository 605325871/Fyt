# include <iostream>
# include <vector>
# include <iterator>
using namespace std;

void printvector(vector<int>&v)
{
    for(vector<int>::iterator it=v.begin(); it!=v.end();it++)
    {
            cout<<*it<<" ";
    }
    cout<<endl;
}
void test01()
{
    vector<int>v;
    for(int i=0;i<10;i++)
    {
        v.push_back(i);
    }
    printvector(v);
    //尾删
    v.pop_back() ; 
    printvector(v);

    // 插入 第一个参数是迭代器
    v.insert(v.begin(),100);
    printvector(v);

    v.insert(v.begin()+3,2,100);
    printvector(v);
    //删除    
    v.erase(v.begin());
    printvector(v);
    
    cout<<"----------------------"<<endl;

    cout<<v.at(1)<<endl;


}
int main()
{
    test01();
    return 0;
}