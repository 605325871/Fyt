#include <iostream>
#include <string.h>
#include <utility>
#include <vector>
template <typename T>
class unqie_ptr
{

public:
    constexpr unqie_ptr() = default;
    explicit unqie_ptr(T *ptr)
        : _ptr(ptr)
    {
    }
    unqie_ptr(unqie_ptr &rhs) = delete;
    unqie_ptr(unqie_ptr &&rhs) : _ptr(rhs.realse()){};

    unqie_ptr &operator=(std::nullptr_t)
    {
        realse();
        return *this;
    }
    unqie_ptr &operator=(const unqie_ptr &rhs) = delete;
    unqie_ptr &operator=(unqie_ptr &&rhs)
    {
        reset(rhs.realse());
        return *this;
    }
    explicit operator bool() { return static_cast<bool>(_ptr); };
    T *get() const noexcept //得到落指针；
    {
        return _ptr;
    }
    T *realse() //释放权限
    {
        return std::exchange(_ptr, nullptr);
    }
    void reset(T *ptr)
    {
        delete std::exchange(_ptr, ptr);
    }
    void swap(unqie_ptr &rhs)
    {
        std::swap(_ptr, rhs._ptr);
    }
    ~unqie_ptr()
    {
        delete _ptr;
    }

    T &operator*() //返回解引用的
    {
        return *_ptr;
    }
    T *operator->()
    {
        return _ptr; //返回裸指针
    };

private:
    T *_ptr{nullptr};
};

template <class T, typename... Args>
auto make_uequie(Args &&...args)
{
    return unqie_ptr<T>{new T(std::forward<Args>(args)...)};
}

int main()
{
    auto p = make_uequie<std::vector<int>>(3, 3);

    for (auto &a : *p)
    {
       std:: cout << a << " ";
    }
}