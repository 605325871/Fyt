#include <iostream>
#include <memory>
#include <vector>
#include<initializer_list>
class strblob
{
    typedef std::vector<std::string>::size_type size_type;

private:
    std::shared_ptr<std::vector<std::string>> data;
    void check(size_type i, const std::string &msg) const; //如果data[i]不合法，抛出一个异常

public:
    strblob();
    strblob(std::initializer_list<std::string> i1);
    size_type size() const { return data->empty(); }
    //添加和删除元素
    void push_back(const std::string &t) { data->push_back(t); }
    void pop_back();
    //元素访问
    std::string &front();
    std::string &back();
    ~strblob() {}
};


