# 单例设计
目地： 是为了让类只有一个实类化

在某些情况下只用一个类,构造函数和拷贝构造私有化
在类内维护一个指针

```c++
class chairman
{
private:
    //构造函数私有化
    chairman() {
        cout<<"创建对象"<<endl;
    }
    //私有化的拷贝构造
    chairman(const chairman&c)
    {}
public:
        static chairman* getinsert()
        {
            return singleman;
        }
private:
    static chairman* singleman;
};

chairman* chairman ::singleman = new chairman;
void test01()
{
    // chairman::singleman;
   chairman* a =chairman:: getinsert(); //使singalman不会成为空指针
   chairman* b =chairman:: getinsert();

   if(a == b)
       cout<<"相同"<<endl;
    else
      cout<<"不相同"<<endl;
//    chairman* cm3= new chairman(*a);
//     if(cm3 == a)
//        cout<<"相同"<<endl;
//     else
//        cout<<"不相同"<<endl;

}

int main()
{
  //  cout<<"main方法diaoyong";  //对象先于main方法调用
    test01();
    return 0;

}
```