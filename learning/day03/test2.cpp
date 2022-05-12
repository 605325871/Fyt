#include<iostream>
#include<string>
using namespace std;
class stack
{
private:
	int* _a;
	int _size;
	int _capcity;
public:
	stack(int capcity = 4) 
	:_a(new int[capcity])
	,_size(0)
	,_capcity(capcity)
	{
		cout<<" stack (int cpacity = 4)"<<endl;
	}
	~stack() 
	{
		delete [] _a;
		_size=_capcity = 0;
		cout<<"~stack()"<<endl;
	}
};

int main()
{
	stack st;

	// stack*ps =new stack;
	// delete ps;
}