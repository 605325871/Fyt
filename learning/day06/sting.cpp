#include<iostream>
#include<string>
using namespace std;
void test01()
{
    string s;
    const char* str="hello world";
    string s2(str);
    cout<<s2<<endl;
}
void test02()
{
    string str1;
    str1="hello world";
    cout<<"str1 = "<<str1<<endl;

    string  str2;
    str2 =str1;
    cout<<"str2 = "<<str2<<endl;

    string str3;
    str3='a';
    cout<<"str3 = "<<str3<<endl;
    string str4;
    str4.assign("hello world");
    cout<<"str4 = "<<str4<<endl;
    string str5;
    str5.assign("hello world",5);
    cout<<"str5 = "<<str5<<endl;
    string str6;
    str6.assign(str5);
        cout<<"str6 = "<<str6<<endl;
}
int main()
{
    test02();
}