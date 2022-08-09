#include "strblob.hh"
using namespace std;

strblob::strblob() : data(make_shared<vector<string>>()) {}

strblob::strblob(initializer_list<string> i1) : data(make_shared<vector<string>>(i1)) {}

void strblob::check(size_type i, const string &msg) const
{
    if (i >= data->size())
        throw out_of_range(msg);
}
string &strblob::front()
{
    check(0, "back on empty strblob");
    
    return data->front();
}

string &strblob::back()
{
    check(0, "back on empty strblob");
    return data->back();
}
void strblob::pop_back()
{
    check(0, "back on empty strblob");
    data->pop_back();
}

int main()
{
    auto p =make_shared<int>(42);
    
}