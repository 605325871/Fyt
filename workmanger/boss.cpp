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
	cout << "boss���" << this->m_id
		<< "\tbos����" << this->name
		<< "\t��λ��" << this->getdepname()
		<< "\t��λְ��boss" << endl;
}

string boss::getdepname()
{
	return "boss";
}