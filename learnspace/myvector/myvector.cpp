#include <iostream>
#include <string>
#include <utility>
template <class T>
class myvector
{
public:
    myvector() noexcept = default;                           // noexpect 关键字不会去检查，但是抛出了异常仍然会中止掉
    explicit myvector(size_t n) : _cap{n}, _ptr{alloc(_cap)} // explicit 禁止隐式转换
    {
        for (; _len < n; ++_len)
        {
            construct(_ptr + _len);
        }
    }
    myvector(size_t n, const T &x) : _cap{n}, _ptr{alloc(_cap)}
    {
        for (; _len < n; ++_len)
        {
            construct(_ptr + _len, x);
        }
    }
    myvector(const myvector &rhs) : _cap(rhs.size()), _ptr{alloc(_cap)}
    {
        for (; _len; ++_len)
        {
            construct(_ptr + _len, rhs[_len]);
        }
    }
    myvector(myvector &&rhs) noexcept
    {
        _cap = std::exchange(this->_cap, 0);
        _len = std::exchange(this->_len, 0);
        _ptr = std::exchange(this->_ptr, nullptr);
    }
    myvector(std::initializer_list<T> il) : _cap{il.size()}, _ptr{alloc(_cap)}
    {
        for (auto &x : il)
        {
            construct(_ptr + _len, x);
            _len++;
        }
    }

    ~myvector()
    {
        clear();
        dealloc(_ptr);
    }

    void swap(myvector<T> &rhs)
    {
        using std::swap;
        swap(_cap, rhs._cap);
        swap(_len, rhs._len);
        swap(_ptr, rhs._ptr);
    }
    void clear()
    {
        for (; _len > 0; _len--)
        {
            destory(_ptr + _len - 1);
        }
    }
    myvector &operator=(const myvector &rhs)
    {
        if (this != &rhs)
            myvector{rhs}.swap(*this); //临时对象

        return *this;
    }
    myvector &operator=(myvector &&rhs)
    {
        if (this != &rhs)
            myvector{std::move(rhs)}.swap(*this);

        return *this;
    }
    myvector &operator=(std::initializer_list<T> il)
    {
        myvector{il}.swap(*this);
        return *this;
    }

    void push_back(const T &x)
    {
        emplace_back(x);
    }
    void push_back(T &&x)
    {
        emplace_back(std::move(x));
    }

    template <typename... Args> //万能引用
    void emplace_back(Args &&...arg)
    {
        if (_len == _cap)
        {
            auto new_cap = _cap ? _cap * 2 : 1;
            auto new_ptr = alloc(new_cap);
            for (auto new_len = 0; new_len < _len; ++new_len)
            {
                construct(new_ptr + new_len, std::move_if_noexcept(_ptr[new_len]));
            }
            _cap = new_cap;
            _ptr = new_ptr;
        }
        construct(_ptr + _len, std::forward<Args>(arg)...);
        ++_len;
    }
    void pop_back()
    {
        destory(_len + _ptr);
        --_len;
    }

    size_t size() const { return _len; }
    size_t capcity() const { return _cap; }

    bool empty() { return _len == 0; }

    T &operator[](size_t i)
    {
        return _ptr[i];
    }
    const T &operator[](size_t i) const;

    T *begin() { return _ptr; }
    T *end() { return _ptr + _len; }

    const T *begin() const { return _ptr; }
    const T *end() const { return _ptr + _len; }

private:
    T *alloc(size_t n)
    {
        return static_cast<T *>(::operator new(sizeof(T ) * n));
    }
    void dealloc(T *p)
    {
        ::operator delete(p);
    }
    template <typename... Args> //可变参数模板
    void construct(T *p, Args &&...arg)
    {
        ::new (p) T(std::forward<Args>(arg)...);
    }
    void destory(T *p)
    {
        p->~T();
    }
    size_t _cap{0}; //容量
    size_t _len{0}; //个数
    T *_ptr{nullptr};
};

void func(myvector<int> x);
int main()
{
    auto print = [](const myvector<std::string> &v)
    {
        for (const auto &x : v)
            std::cout << " " << x;
    };
    myvector<std::string> v{"1", "2", "3"};
    print(v);

    v.push_back("a");
    v.push_back("b");
    v.push_back("c");
    myvector<std::string> v1{"1", "2", "3"};
    print(v1);
}