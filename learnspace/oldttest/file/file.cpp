# include<iostream>
# include<fstream> 
using namespace std;

// void test01(){
//     //写文件
//     ofstream ofs;
// 	ofs.open("test.txt", ios::out);
// 	ofs << "姓名： 张三" << endl;
// 	ofs << "性别： 男" << endl;
// 	ofs << "年龄： 18" << endl;
// 	ofs.close();
// }
// void test02()
// {
//     ifstream ifs;
// 	ifs.open("test.txt", ios::in);
	
// 	if (!ifs.is_open())
// 	{
// 		cout << "文件打开失败" << endl;
// 		return;
// 	}
// 	char buf[1024] = { 0 };

// 	while (ifs >> buf)
// 	{
// 		cout << buf << endl ;
// 	}

// 	/*while (ifs.getline(buf,sizeof(buf)))
// 	{
// 		cout << buf << endl;
// 	}*/
	
// 	ifs.close();
// }
class person
{
public:
	char m_name[64];
	int m_age;
};
void test03()
{
	ofstream ofs("person.txt", ios::out | ios::binary);

	person p = { "张三",18 };
	ofs.write((const char*)&p, sizeof(person));

	ofs.close();



}

int main()
{
   // test02();
    return 0;
}