# include "student.h"

Student::Student(int id, string name, int did)
{
	this->m_id = id;
	this->name = name;
	this->m_deptid = did;
}

void Student::showinfo()
{
	cout << "ѧ��ѧ��" << this->m_id
		<< "\tѧ������" << this->name
		<< "\t��λ��" << this->getdepname()
		<<"\t��λְ��ѧϰ"<<endl;
}

string Student::getdepname()
{
	return "ѧ��"; 
 }
