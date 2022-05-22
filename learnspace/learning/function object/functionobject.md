 # 函数对象（仿函数）
>函数对象在使用时可以向普通函数那样调用，可以有参数，可以有返回值
 函数对象可以超出普通函数的概念，函数对象可以有自己的状态
 函数对象可以作为参数传递

 ```cpp
 class myadd
 {
public:
    int operator()(int v1,int v2) //对函数调用符重载
    {
        return v1+v2;
    }
 };
 ```
 * 函数对象在使用时，可以向普通函数一样调用
```cpp
void test01()
{
    myadd myadd;
    cout<<myadd(10,10)<<endl;
}
```
* 函数对象可以拥有自己的状态
```cpp
class myprint
{
public:
    myprint()
    {
        this->count=0;
    }
    void operator()(string test)
    {
        cout << test << endl;
        this->count++;
    }
    int count = 0;
};
void test01()
{
    myprint myprint;
    myprint("hello");
    myprint("hello");
    myprint("hello");
    myprint("hello");
    myprint("hello");
    cout<<" count = "<<myprint.count<<endl;

}
```
>与普通函数不同，仿函数在调用时不会像普通函数一样，随着调用的结束被销毁，可以有自己内部的记录

* 函数对象可以作为参数传递
```cpp
class myprint
{
public:
    myprint()
    {
        this->count = 0;
    }
    void operator()(string test)
    {
        cout << test << endl;
        this->count++;
    }
    int count = 0;
};
void doprint(myprint &mp, string test)
{
    mp(test);
}
void test01()
{
    myprint myp;
    doprint(myp, "aaaaaa");
}
```
# 谓词
>返回bool类型的仿函数称为谓词
如果opertor（）接受一个参数，则为一元谓词
两个参数，则为二元谓词

## 一元谓词
```cpp
class overfive
{
public:
    bool operator()(int val)
    {
        return val > 5;
    }
};
void test01()
{
    vector<int> v;
    for (int i = 0; i < 10; i++)
    {
        v.push_back(i + 1);
    }
    vector<int>::iterator it = find_if(v.begin(), v.end(), overfive());
    find_if(v.begin(), v.end(), overfive()); //传入匿名的函数对象
    //返回值为迭代器位置，找到一个大于5的数就终止
}
```
## 二元谓词
```cpp
class mycomper
{
public:
    bool operator()(int v1, int v2)
    {
        return v1 > v2;
    }
};
void test01()
{
    vector<int> v;
    v.push_back(10);
    v.push_back(45);
    v.push_back(21);
    v.push_back(35);
    v.push_back(12);
    sort(v.begin(), v.end(),mycomper());
}
```