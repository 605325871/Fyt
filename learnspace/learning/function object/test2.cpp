#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class mycomper
{
public:
    bool operator()(int v1, int v2)
    {
        return v1 > v2;
    }
};
void test01()
{
    vector<int> v;
    v.push_back(10);
    v.push_back(45);
    v.push_back(21);
    v.push_back(35);
    v.push_back(12);
    sort(v.begin(), v.end(),mycomper());
    for(auto i : v)
    cout<<i<<" ";
    cout<<endl;
}
int main()
{
    test01();
    return 0;
}