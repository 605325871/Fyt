# 函数模板
一种实现泛型编程的方法

>template  < typename T >

函数的声明或定义

template --声明模板 typename--数据类型 可以用classs替换


在声明好的模板紧跟函数即可
```cpp
template<class T>
void myswap(T& a, T& b)
{
	T temp = a;
	a = b;
	b = temp;
}
```
使用方式为自动类型推导与显示指定类型
```cpp
myswap(a, b); //myswap<int>(a, b)
```

调用规则
> 1.自动类型推倒，必须推导出```一致的数据类型``` 才可以使用

```cpp
int main()
{
    int a=10;
    int b=20;
    char c ='c';
    myswap(a,b)//正确
    // myswap(a,c)//错误
}
```
>2.模板必须指定类型才可以使用
```cpp
template<class T>
void func()
{ cout<<"func调用<<endl" }
int main()
{
    func<int>(); // 不传的话则会报错 
}
```

例 一个实现排序各种类型的模板
```cpp
template<class T>
void myswap(T &a, T&b)
{
    T temp = a;
    a= b;
    b=temp;
}
template<class T>
void mysort(T arr[] ,int len)
{
    for(int i=0; i<len;i++)
    {
        int max = i;
        for( int j = i+1;j<len;j++)
        {
            if(arr[max]<arr[j])
            {
                max= j;
            }

        }
        if(max != i)
        {
            myswap(arr[max],arr[i]);
        }
    }
}
template<class T>
void printfarr(T arr[],int len)
{
    for(int i=0; i<len ;i++)
    {
        cout<<arr[i]<<" ";
    }
    cout<<endl;
}
int main()
{
    char arr[]="bfasbdsg";
    int arr[]={1,23,4,2,5,8};
    int num = sizeof(arr)/sizeof(arr[0]);
    printfarr(arr,num);
    mysort(arr,num);
    printfarr(arr,num);
}
```
>注：普通函数调用可以发生自动类型转换，

>如果利用```显示类型指定```时模板函数也可以发生隐式类型转换

# 普通函数和函数模板的调用规则
* 如果函数模板和普通函数都可以调用，优先调用普通函数
* 可以通过空模板参数列表 强制调用函数模板   myadd<>(a,b)
* 普通函数和函数模板是可以发生重载的 
* 如果函数模板可以产生更好的匹配(更少的发生类型转换)，优先调用函数模板

注： 当提供了函数模板，最好不要提供重名的普通函数，避免二义性
# 类模板
在template 后紧跟一个类称之为类模板
```cpp
template<class nametype,class datatype>
class person
{
public:
    person(nametype name,datatype age) {
        this->_age=age;
        this->_name=name;
    }
    ~person() {}
    void showperson()
    {
        cout<<"name :  "<<this->_name<<"  age: "<<this->_age<<endl;
    }
    nametype _name;
    datatype _age;
};
void test01()
{
    person<string,int> p("zhangsan",18); //在使用时要显示其类型
    p.showperson();
}
```
调用规则
> 1.类模板没有自动类型推倒，要向上面一样显示指定才行\
2.类模板的参数列表可以有默认参数
```cpp
typelate<class nametype,class agetype = int>
     只能在类模板中使用      默认参数类型为int
 ```
类模板中函数调用规则，
类模板中的成员函数只有在==调用==阶段才创建

当类模板做函数参数时三种方式
```cpp
void personprint1(person<string, int>&p1)//类模板对象做函数的参数

template<class t1,class t2>
void personprint1(person<t1, t2>& p1)//参数模板化

template<class T>
void personprint1(T &p)
```
第一种最为广泛常用，后面两种较难维护

## 类模板与继承

子类 在继承类模板时，必须知道父类的参数类型这样子才能知道继承的来自父类的参数内存大小
```cpp
template<class nametype,class datatype>
class person
{
public:
    person(nametype name,datatype age) {
        this->_age=age;
        this->_name=name;
    }
    ~person() {}
    nametype _name;
    datatype _age;
};

class son :public person<string,int>
{}
```
但是这样失去了模板的灵活性
```cpp
template<class T1,class T2>
class son :public person<T1,T2>
{}
```
如果想要灵活指定父类中类型，那么可以将子类变为模板
## 类模板函数类外实现
```cpp
template<class t1,class t2>
person<t1,t2>:: person(t1 name ,t2 age)
```

```cpp
template<class t1, class t2> //识别是一个模板
void person<t1,t2>：：showperson()  //加上类模板的参数列表
    { 
    cout<<"name :  "<<this->_name<<" age:"<<this->_age<<endl;
    }
```
# 