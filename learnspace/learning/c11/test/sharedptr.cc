#include <iostream>
#include <memory>
#include <vector>
#include <initializer_list>

using namespace std;
// class A : public ::enable_shared_from_this<A>
// {
// public:
//     A()
//     {
//         cout << " 构造" << endl;
//     }
//     shared_ptr<A> GetSelf()
//     {
//         // return shared_ptr<A>(this); // 不要这么做，此处会返回一个独立的智能指针
//         return shared_from_this();
//     }
//     ~A()
//     {
//         cout << "Deconstruction A" << endl;
//     }
// };

class A;
class B;

class A
{
public:
    std::shared_ptr<B> bptr;
    ~A()
    {
        cout << "A is deleted" << endl;
    }
};

class B
{
public:
    std::shared_ptr<A> aptr;
    ~B()
    {
        cout << "B is deleted" << endl; // 析构函数后，才去释放成员变量
    }
};

int main()
{
    {
        std::shared_ptr<A> ap(new A);
        std::shared_ptr<B> bp(new B);
        cout << "ap count = " << ap.use_count() << endl;
        cout << "bp count = " << bp.use_count() << endl;
        cout << "bptr count = " << ap->bptr.use_count() << endl;
        cout << "aptr count = " << bp->aptr.use_count() << endl;
        ap->bptr = bp;
        bp->aptr = ap;
        cout << "bptr count = " << ap->bptr.use_count() << endl;
        cout << "aptr count = " << bp->aptr.use_count() << endl;
        cout << "ap count = " << ap.use_count() << endl;
        cout << "bp count = " << bp.use_count() << endl;
    }

    cout << "main leave" << endl; // 循环引用导致ap bp退出了作用域都没有析构
    return 0;
}
