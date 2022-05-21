#pragma once
# include<iostream>
# include <stdlib.h>
#include <windows.h>
# include<string>
using namespace std;
class worker
{
public:
	virtual void showinfo()=0;
	virtual string getdepname() = 0;
	//±‡∫≈
	int m_id;
	//–’√˚
	string name;
	//≤ø√≈±‡∫≈
	int m_deptid;
	
	
};