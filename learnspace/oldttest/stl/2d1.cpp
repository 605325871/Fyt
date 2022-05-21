/*
 * @Author: your name
 * @Date: 2022-03-23 15:30:02
 * @LastEditTime: 2022-03-23 16:32:09
 * @LastEditors: Please set LastEditors
 * @Description: 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: /ccc/learning/stl/2d1.cpp
 */
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
    vector<int> v1;
    
    for(int i=0;i<10;i++)
    {
        v1.push_back(i);
    }
    printvector(v1);

    //通过区间的形式构造
 
    vector<int> v2(v1.begin(),v1.end());
    printvector(v2);
    // 通过n个elnm的方式副职 10  个 100
    vector<int> v3(10,100);
    printvector(v3);

    vector<int>v4;// assign前面为闭区间，后面为闭区间
    v4.assign(v1.begin(),v1.end());
    printvector(v4);

    v4.resize(15,0);//resize当超过所村数据容量默认用0 填空，或者（15， 1）用特定的元素填充
    printvector(v4);

}
int main()
{
    test01();
    return 0;
}