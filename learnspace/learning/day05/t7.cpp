#include<iostream>
#include<string>
using namespace std;
template<class T>
void myswap(T &a, T&b)
{
    T temp = a;
    a= b;
    b=temp;
}
template<class T>
void mysort(T arr[] ,int len)
{
    for(int i=0; i<len;i++)
    {
        int max = i;
        for( int j = i+1;j<len;j++)
        {
            if(arr[max]<arr[j])
            {
                max= j;
            }

        }
        if(max != i)
        {
            myswap(arr[max],arr[i]);
        }
    }
}
template<class T>
void printfarr(T arr[],int len)
{
    for(int i=0; i<len ;i++)
    {
        cout<<arr[i]<<" ";
    }
    cout<<endl;
}
int main()
{
    char arr[]="bfasbdsg";
    int arr[]={1,23,4,2,5,8};
    int num = sizeof(arr)/sizeof(arr[0]);
    printfarr(arr,num);
    mysort(arr,num);
    printfarr(arr,num);
}