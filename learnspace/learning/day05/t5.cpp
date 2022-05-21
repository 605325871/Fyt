#include<iostream>
#include<fstream>
#include<string>
using namespace std;
/*
    二进制形式 写文件 可以操作自定义数据类型
*/
class person
{
private:
    /* data */
public:
    char _name[64];
    int _age;
};
void test01()
{
    ofstream ofs("test.txt",ios::out|ios::binary);//以二进制形式写
    //ofs.open("test.txt",ios::out|ios::binary);
    person p ={"张三",18};
    ofs.write((const char*)&p,sizeof(person));
    ofs.close();

}
void test02()
{
    ifstream ifs("test.txt",ios::in|ios::binary);
    if(!ifs.is_open())
    {
        cout<<"打开失败"<<endl;
        return;
    }
    person p;
    ifs.read((char*)&p,sizeof(p));
    cout<<p._age << p._name<<endl;
    ifs.close();
}
int main()
{
    test02();
}