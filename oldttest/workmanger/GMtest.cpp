# include "GM.h"
# include "student.h"
# include "boss.h"
# include "teacher.h"
using namespace std;

//
void test02()
{
	worker* wok= new Student(1, "����", 1);
	wok->showinfo();
	delete wok;
	
	wok = new teacher(2, "����", 2);
	wok->showinfo();
	delete wok;
	wok = NULL;

}
void test01()
{

	GM t;
	welcome();
	t.show();
	while (true)
	{
	    cout << "����������ѡ��" ;
		int chioce = 0;
		cin >> chioce;
		switch (chioce)
		{
		case 1:  //����
			break;
		case 2:  //��ʾ
			break;
		case 3:  //ɾ�� 
			break;
		case 4:  //�޸�
			break;
		case 5:  //����
			break;
		case 6:  //����
			break;
		case 7:  //���
			break;
		case 0:  //�˳�
			t.exitos();
			break;
		default:
			cout << "���������룺"<<endl ;
			break;
		}

	}
}

int main()
{
	//test01();
	test02();
	return 0;
}