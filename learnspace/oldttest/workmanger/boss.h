#pragma once
# include "worker.h"
using namespace std;

class boss :public worker
{
public:
	boss(int id, string name, int did);
	virtual void showinfo() = 0;
	virtual string getdepname() = 0;
};