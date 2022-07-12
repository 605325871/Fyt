#include <iostream>
#include <string>
#include<errno.h>
#include<string.h>
#include<unistdio.h>
using namespace std;
double Div(int a, int b)
{
	if (b == 0)
		throw "Div by zero!";//抛出错误
	else
		return a*1.0 / b;
}
void FileRead()
{
	string filename;
	cin >> filename;
	FILE* fp = fopen(filename.c_str(), "r");
	if (fp == nullptr)
	{
		string message = "fopen error:";
		message += filename;
		message += "->";
		message += strerror(errno);
		throw message;
	}
	char ch;
	while ((ch = fgetc(fp)) != EOF)
	{
		cout << ch;
	}
	cout << endl;
	fclose(fp);
}
void Func()
{
	int a, b;
	cin >> a >> b;
	try {
		Div(a, b);
	}
	catch (const int errno)
	{
		cout << "errno" << errno << endl;
	}
	FileRead();
}
int main()
{
	while (1)
	{
		try
		{
			Func();
		}
		catch (const char* message)
		{
			cout << "1->" << message << endl;
		}
		catch (const string message)
		{
			cout << "2->" << message << endl;
		}
		catch (...)
		{
			cout << "Unknow exception..." << endl;
		}
		cout << "3" << endl;
    
	}
	return 0;
}
