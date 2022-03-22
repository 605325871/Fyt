/*
 * @Date: 2022-03-20 17:58:49 * @Description: vector 数组的初步学习
 */
# include <iostream>
# include<vector>
#include<algorithm>
#include <string>
using namespace std;

// vector 容器存放内置数据类型
void myprintf(int val)
{
    cout<<val<<endl;
}
void test01()
{
    vector<int>y;

    //向容器中插入数据 
    y.push_back(10);
    y.push_back(20);
    y.push_back(30);
    y.push_back(40);

    //通过迭代器访问容器中的数据
    vector<int>::iterator itBegin=y.begin(); //起始迭代器 指向容器中第一个元素
    vector<int>::iterator itend=y.end();// 结束迭代器  指向容器中最后一个元素的下一个
// 一
 /*    for(vector<int>::iterator it=y.begin(); it != y.end();it++)
    {
        cout<<*it<<endl;
    }   */
    // 二 使用stl提供的遍历算法  需要包含算法头文件
    for_each(y.begin(),y.end(),myprintf);


}
void test02()
{
    vector < vector <int>> v;
    vector <int >v1;
    vector <int >v2;
    vector <int >v3;
    vector <int >v4;
    for(int i=0;i<4;i++)
    {
        v1.push_back(i+1);
        v2.push_back(i+1);
        v3.push_back(i+1);
        v4.push_back(i+1);
    }
    v.push_back(v1);
     v.push_back(v2);
      v.push_back(v3);
       v.push_back(v4);
       for(vector<vector<int>>:: iterator it=v.begin(); it != v.end();it++)
       {
           //*it 为 vector<int>
           for(vector<int >::iterator  vit =(*it).begin(); vit!= (*it).end();vit++)
           {
               cout<<*vit<<endl;
           }
       }



}
/*
 - string();  创建一个新的字符串 例如string str
 - sting(const char*s);使用字符串s初始化
 - string(const string&str);使用一个string对象初始化另一个string对象
 - string(int n, char c);使用n个字符c初始化
 */
void test03()
{
    //string是c++风格的的字符串 是一个类
    //特点  find copy delete replace insert

    //string 的构造函数

        string s1;
        const char* str = "hello world";
        string s2(str);
        cout<<s2<<endl;
        
    string s3(s2);
      cout<<"s3 = "<<s3<<endl;
    string s4(10,'a');
        cout<<"s4 = "<<s4<<endl;


}
void test04()
{
    string str1;
    str1="hello";
    cout<<"str1 = "<<str1<<endl;

    str1+="world";
    cout<<"str1 = "<<str1<<endl;

   /* string str2;
    str2.assign("hello c++");
    cout<<str2<<endl;

      string str3;
    str3.assign("hello c++",5);//只拿前5个字符
    cout<<"str3  = "<<str3<<endl;

    string str4;
    str4.assign(10,'w');
    cout<<str4<<endl;
    */

   str1+=';';
    cout<<"str1 = "<<str1<<endl;
    string s2 ="lol dnf";
    str1+=s2;

    str1.append("hahahhahahah");
    cout<<"str1 = "<<str1<<endl;
    int pos = str1.find("de"); //rfind是从右向左找
    str1.replace(1,3,"1111"); // replace 在替换时要制定从那个位置开始，多少个字符，要替换成什么字符串
        cout<<pos<<endl;

    cout<<"-------------------------"<<endl;
    cout<<str1<<endl;

  // 字符串比较
   string str4("hello");  //类似于string compare的比较
   if(str4.compare(str1)==0)
   {
       cout<<"一样" <<endl;
   }
   else
   {
       cout<<"不一样"<<endl;
   }
}


int main()
{
    test04();
    return 0;
}
 

