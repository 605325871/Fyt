#include<iostream>
#include<string>
using namespace std;

//单例模式，为了创建类中对象，且保证只有一个对象实例

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