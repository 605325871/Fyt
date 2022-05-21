# include "worker.h"
using namespace std;

class teacher :public worker
{
public:
	teacher(int id, string name, int did);
	virtual void showinfo();
	virtual string getdepname();

};