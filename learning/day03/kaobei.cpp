#include<iostream>
#include<string>
using namespace std;


class phone
{
public:
    phone(){
        cout<<"phone 的构造调用"<<endl;
    }
     phone(string name)
    {
        _name=name;
    }
    ~phone(){
        cout<<"phone 的构造调用"<<endl;
    }
    string _name;
};
class Game
{

public:
    Game(/* arGames */) {
        cout<<"game的默认构造调用"<<endl;
    }
    Game(string name)
    {
        _name=name;
    }
    ~Game() {
        cout<<"game的构造调用"<<endl;
    }
    string _name;
};
class person
{
public:
    person(/* arGames */) {
        cout<<"person的默认构造参数调用"<<endl;
    }
    person(sting name,string p)
    ~person() {
        cout<<"person的析构函数调用"<<endl;
    }

    string _name;
    phone _phone;
    Game _Gameame;
};
void test01()
{
    person p;
    
     p._phone._name = "三星";
     p._Gameame._name= "斗地主";
     
}
int main()
{
    test01();

}