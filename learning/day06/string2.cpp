#include<iostream>
#include<string>
using namespace std;
void test01()
{
    string str1="I ";
    str1.append("am");
    cout<<str1<<endl;

    str1.append("abcdefg");
    cout<<str1<<endl;

    str1.append(str1,0,3);// 拼接字符串 第一个是从那个位置开始，第二是截取多少个
    cout<<str1<<endl;

    int pos = str1.find("am");

    cout<<pos<<endl;
    int pos2 =str1.rfind("a");
    cout<<pos2<<endl;

    string str3;
    str3.append(str1);
    str3.replace(1,3,"1111"); //从1号位置起3个字符串 替换成1111
    cout<<str3<<endl;

    int key=str3.compare(str1);
    cout<<key<<endl;

    //可以通过 【】 和at 访问字符串中字符
    str3[0]='x';
    string str4("hello");
    //insert he
    str4.insert(0,"123");
    cout<<str4<<endl;

    str4.erase(0,3);
    cout<<str4<<endl;
    //从某个位置起截取多少个字符串
    string email="zhangsan@qq.com";
    int pos6 = email.find("@");
    string usernam=email.substr(0,pos6);
    cout<<usernam<<endl;

}
int main()
{
    test01();
    return 0;
}