#include<iostream>
#include<fstream>
#include<string>
using namespace std;

/* 
文本文件 以文本的ASCII码形式存储在计算机中
二进制  二进制形式
*/
void test01()
{
    ofstream ofs;
    ofs.open("test.txt",ios::out); //写文件
    ofs<<"姓名： 张三"<<endl;
    ofs.close();
}
void test02()
{
    ifstream ifs;
    ifs.open("test.txt",ios::in);
    if(!ifs.is_open())
    {
        cout<<"文件打开失败"<<endl;
    }
    // 读数据  1.o
    // char buf[1024]={0};
    // while(ifs>>buf)
    // {
    //     cout<<buf<<endl;
    // }  
    
    // 2.0
    // char buf[1024]={0};
    // while(ifs.getline(buf,1024))
    // {
    //     cout<<buf<<endl;
    // }
    
    //3.0
    // string buf;
    // while(getline(ifs,buf))
    // {
    //     cout<<buf<<endl;
    // }
    
    //4,0
    char c;
    while((c=ifs.get())!=EOF)
    {
        cout<<c; //第四中不能加、endl
    }
    ifs.close();
}
int main()
{
    test02();
}