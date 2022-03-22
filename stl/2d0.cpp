/*
 * @Author: your name
 * @Date: 2022-03-22 20:59:51
 * @LastEditTime: 2022-03-22 21:22:56
 */ 
# include <iostream>
# include<vector>
#include<string>

using namespace std;
/*
void test01()
{
    string s1="tian xia wu shung";

    for (int i = 0; i < s1.size(); i++)
    {
        printf("%c",s1[i]); //【】也可以访问单个字符
    }
    printf("\n");

    for(int i=0; i<s1.size();i++)
    {
        cout<<s1.at(i);  //at 可以访问单个字符
    }
    cout<<endl;
    

}
*/
void test01()
{
    string s1;
    s1="ehhehehe";
    s1.insert(1,"555");  //从第一个字符开始插入
    cout<<s1<<endl;

    s1.erase(1,3); //从某个位置开始插入和删除

     cout<<s1<<endl;

}

void test02()
{
    string str2="131@qq.com";


    int pos= str2.find("@");
      string sb= str2.substr(0,pos);
    cout<<sb<<endl;
}

int main()
{
    test02();
}
