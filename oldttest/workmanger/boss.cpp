# include "boss.h"
using namespace std;
boss::boss(int id, string name, int did)
{
	this->m_id = id;
	this->name = name;
	this->m_deptid = did;
}

void boss::showinfo()
{
	cout << "boss编号" << this->m_id
		<< "\tbos姓名" << this->name
		<< "\t岗位：" << this->getdepname()
		<< "\t岗位职责：boss" << endl;
}

string boss::getdepname()
{
	return "boss";
}