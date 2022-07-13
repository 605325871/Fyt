#include <iostream>

template <typename T>
class ArrayStack
{
public:
    ArrayStack(int s = 10); //默认的栈容量为10
    ~ArrayStack();

public:
    T top();        //获取栈顶元素
    void push(T t); //压栈操作
    T pop();        //弹栈操作
    bool isEmpty(); //判空操作
    int size();     //求栈的大小

private:
    int _count;    //栈的元素数量
    int _capacity; //栈的容量
    T *_array;     //底层为数组
};

template <class T>
ArrayStack<T>::ArrayStack(int s)
{
    _count = 0;
    _capacity = s;
    _array = new T[_capacity];
}

template<class T>
ArrayStack<T>::~ArrayStack()
{
    _count=
}