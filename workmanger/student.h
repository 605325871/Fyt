# include "worker.h"
using namespace std;

class Student :public worker
{
public:

	Student(int id,string name,int did);

	 void showinfo();

    string getdepname();

};
