# include "teacher.h"

teacher::teacher(int id, string name, int did)
{
	this->m_id = id;
	this->name = name;
	this->m_deptid = did;
}

void teacher::showinfo()
{
	cout << "��ʦ���" << this->m_id
		<< "\t��ʦ����" << this->name
		<< "\t��λ��ʦ��" << this->getdepname()
		<< "\t��λְ����ʦ" << endl;
 }
string teacher::getdepname()
{
	return "��ʦ";
}
