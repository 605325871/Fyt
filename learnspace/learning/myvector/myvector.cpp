#include <iostream>
#include <string>

namespace tt
{

    template <class T>
    class myvector
    {
    private:
        iterator _start;        //指向数据块的起始位置
        iterator _finish;       //指向有效元素的尾巴
        iterator _endofstorage; //指向存储容量的尾

    public:
        typedef T *iterator;
        typedef const T *const_iterator;
        iterator begin()
        {
            return _start;
        }
        iterator end()
        {
            return _finish;
        }
        const_iterator begin() const
        {
            return _start;
        }
        const_iterator end()
        {
            return _finish;
        }
        myvector()
        : _start(nullptr)
        , _finish(nullptr)
        , _endofstorage(nullptr)
        {}
        myvector(int n,,const T& value = T())
        void ~myvector();
    };

} // namespace tt
