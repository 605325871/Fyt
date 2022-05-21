# include "teacher.h"

teacher::teacher(int id, string name, int did)
{
	this->m_id = id;
	this->name = name;
	this->m_deptid = did;
}

void teacher::showinfo()
{
	cout << "老师编号" << this->m_id
		<< "\t老师姓名" << this->name
		<< "\t岗位老师：" << this->getdepname()
		<< "\t岗位职责：老师" << endl;
 }
string teacher::getdepname()
{
	return "老师";
}
