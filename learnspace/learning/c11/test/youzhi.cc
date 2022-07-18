#include <iostream>
#include <string.h>
using namespace std;

class mystring
{
public:
    mystring(char *tmp = "abc")
    {
        len = strlen(tmp);
        str = new char[len + 1];
        strcpy(str, tmp);

        cout << "普通构造函数调用" << endl;
    }
    void print()
    {
        cout << str << endl;
    }
    mystring(const mystring &tmp)
    {
        len = tmp.len;
        str = new char[len + 1];
        strcpy(str, tmp.str);
        cout << "拷贝构造函数调用" << endl;
    }

    //移动拷贝构造函数调用
    mystring(mystring &&t)
    {
        str = t.str; //拷贝地址，没有重新申请内存
        len = t.len;

        t.str = NULL; //原来的指针指空
        cout << "移动拷贝构造" << endl;
    }
    //移动赋值函数调用
    mystring &operator=(mystring &&tmp)
    {
        if (&tmp == this)
        {
            return *this;
        }

        len = 0;
        delete[] str;

        len = tmp.len;
        str = tmp.str;
        tmp.str = nullptr;

        return *this;
    }
    mystring &operator=(const mystring &tmp)
    {
        if (&tmp == this)
        {
            return *this;
        }
        len = 0;
        if (str != nullptr)
            delete[] str;

        // 重新申请内存
        len = tmp.len;
        str = new char[len + 1];
        strcpy(str, tmp.str);

        cout << "   = 构造函数调用" << endl;
        return *this;
    }
    ~mystring()
    {
        if (str != nullptr)
            delete[] str;
        cout << " 析构函数调用" << endl;
    }

private:
    char *str = nullptr;
    int len;
};

mystring func()
{
    mystring obj("midke");
    return obj;
}

void test01()
{
    mystring tmp = func();
}
int main()
{
    test01();
}