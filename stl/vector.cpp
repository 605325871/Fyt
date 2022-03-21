/*
 * @Date: 2022-03-20 17:58:49 * @Description: vector 数组的初步学习
 */
# include <iostream>
# include<vector>
#include<algorithm>
using namespace std;

// vector 容器存放内置数据类型
void myprintf(int val)
{
    cout<<val<<endl;
}
void test01()
{
    vector<int>y;

    //向容器中插入数据 
    y.push_back(10);
    y.push_back(20);
    y.push_back(30);
    y.push_back(40);

    //通过迭代器访问容器中的数据
    vector<int>::iterator itBegin=y.begin(); //起始迭代器 指向容器中第一个元素
    vector<int>::iterator itend=y.end();// 结束迭代器  指向容器中最后一个元素的下一个
// 一
 /*    for(vector<int>::iterator it=y.begin(); it != y.end();it++)
    {
        cout<<*it<<endl;
    }   */
    // 二 使用stl提供的遍历算法  需要包含算法头文件
    for_each(y.begin(),y.end(),myprintf);

}
int main()
{
    test01();
    return 0;
}
 

