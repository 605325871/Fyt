# include "GM.h"
# include "student.h"
# include "boss.h"
# include "teacher.h"
using namespace std;

//
void test02()
{
	worker* wok= new Student(1, "张三", 1);
	wok->showinfo();
	delete wok;
	
	wok = new teacher(2, "李四", 2);
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
	    cout << "请输入您的选择：" ;
		int chioce = 0;
		cin >> chioce;
		switch (chioce)
		{
		case 1:  //增加
			break;
		case 2:  //显示
			break;
		case 3:  //删除 
			break;
		case 4:  //修改
			break;
		case 5:  //查找
			break;
		case 6:  //排序
			break;
		case 7:  //清空
			break;
		case 0:  //退出
			t.exitos();
			break;
		default:
			cout << "请重新输入："<<endl ;
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