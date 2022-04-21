# include "student.h"

Student::Student(int id, string name, int did)
{
	this->m_id = id;
	this->name = name;
	this->m_deptid = did;
}

void Student::showinfo()
{
	cout << "学生学号" << this->m_id
		<< "\t学生姓名" << this->name
		<< "\t岗位：" << this->getdepname()
		<<"\t岗位职责：学习"<<endl;
}

string Student::getdepname()
{
	return "学生"; 
 }
