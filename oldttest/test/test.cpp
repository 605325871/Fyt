# include <iostream>
using namespace std;

// void swap(int*&a,int *&b)
// {
//     int* t;
//     t=a;
//     a=b;
//     b=t;
// }

// int main()
// {
//     int a=10,b=20;
//     cout<<"a="<<a;
//     cout<<"b="<<b<<endl;

//     swap(a,b);
//      cout<<"a="<<a<<endl;
//     cout<<"b="<<b<<endl;


// }   

int & sort()
{
    int a=20;
    return a;
}

int main()
{
    int &n=sort();
    int a;
  cin>>a;
    cout<<"a="<<a<<endl;
}