#include <iostream>
#include <string>
#include <atomic>
#include<vector>
class refcout
{
public:
    int use_count() { return _count; }
    void inc_count() { ++_count; }
    int dec_count() { --_count; }

private:
    std::atomic_int _count{1};
};

template <class T>
class sharedptr
{
public:
    sharedptr() = default;
    explicit sharedptr(T *ptr) : _ptr(ptr)
    {
        if (ptr)
        {
            _rep = new refcout{};
        }
    }
    sharedptr(const sharedptr &rhs) : _ptr(rhs._ptr), _rep(rhs._rep)
    {
        if (_rep)
        {
            _rep->inc_count();
        }
    }
    sharedptr(sharedptr &&rhs) : _ptr(rhs._ptr), _rep(rhs._rep)
    {
        rhs._ptr = nullptr;
        rhs._rep = nullptr;
    }
    ~sharedptr()
    {
        if (_rep && _rep->dec_count())
        {
            delete _ptr;
            delete _rep;
        }
    }
    sharedptr &operator=(const sharedptr &rhs)
    {
        if (&this != rhs)
            sharedptr{rhs}.swap(*this);
        return *this;
    }
    sharedptr &operator=(sharedptr &&rhs)
    {
        if (&this != rhs)
            sharedptr{move(rhs)}.swap(*this);
        return *this;
    }

    void swap(sharedptr &rhs)
    {
        std::swap(_ptr, rhs._ptr);
        std::swap(_rep, rhs._rep);
    }
    void reset(T *ptr = nullptr)
    {
        sharedptr{ptr}.swap(*this);
    }
    T *get()
    {
        return _ptr;
    }

    int us_count()
    {
        return _rep ? _rep->use_count() : 0;
    }

    T &operator*()
    {
        return *_ptr;
    }
    T *operator->()
    {
        return _ptr;
    }
    operator bool()
    {
        return static_cast<bool>(_ptr);
    }

private:
    T *_ptr{nullptr};
    refcout *_rep{nullptr};
};


template<class T,typename... Args>
auto make_shared(Args&&...args)
{
    return sharedptr<T>{new T(std::forward<Args>(args)...)};
}

int main()
{
  sharedptr<std::vector<int>> p =make_shared<std::vector<int>>(3);
}